#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <string>
#include <utility>

#include <nlohmann/json.hpp>
#include "Format.h"

#include <Volcano/Schema/Model.h>

VOLCANO_SCHEMA_BEGIN

void Model::validate() const {
    for (const auto& part : parts) {
        detail::validatePath(part.mesh, true);
        detail::validatePath(part.material, true);
        if (!part.transform.matrix().allFinite() ||
            part.transform.matrix().row(3) != Eigen::RowVector4f(0, 0, 0, 1)) {
            throw std::invalid_argument("Model part transform must be a finite affine matrix");
        }
    }
}

Model Model::read(Stream& stream) {
    const auto bytes = stream.readAll();
    try {
        const auto document = detail::Json::parse(bytes.begin(), bytes.end());
        detail::requireJsonHeader(document, "model");
        const auto& entries = document.at("parts");
        if (!entries.is_array()) {
            throw std::invalid_argument("Model parts must be an array");
        }

        Model result;
        result.parts.reserve(entries.size());
        for (const auto& entry : entries) {
            Part part;
            part.mesh = detail::readPath(entry.at("mesh"));
            part.material = detail::readPath(entry.at("material"));
            const auto& matrix = entry.at("transform");
            if (!matrix.is_array() || matrix.size() != 16) {
                throw std::invalid_argument("Model transform requires 16 row-major numbers");
            }
            for (std::size_t index = 0; index < 16; ++index) {
                part.transform.matrix()(index / 4, index % 4) = matrix.at(index).get<float>();
            }

            result.parts.push_back(std::move(part));
        }
        result.validate();
        return result;
    } catch (const detail::Json::exception& error) {
        throw std::invalid_argument(std::string("Invalid Model JSON: ") + error.what());
    }
}

void Model::write(Stream& stream) const {
    validate();
    detail::Json entries = detail::Json::array();
    for (const auto& part : parts) {
        detail::Json matrix = detail::Json::array();
        for (std::size_t index = 0; index < 16; ++index) {
            matrix.push_back(part.transform.matrix()(index / 4, index % 4));
        }
        entries.push_back({
            {"mesh", detail::pathString(part.mesh)},
            {"material", detail::pathString(part.material)},
            {"transform", std::move(matrix)}
        });
    }

    const detail::Json document = {{"type", "model"}, {"version", 1}, {"parts", std::move(entries)}};
    const auto text = document.dump(2) + '\n';
    stream.writeAll(std::span(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()));
}

VOLCANO_SCHEMA_END
