//
//
#include <Volcano/Graphics/Frame.h>
#include <Volcano/Graphics/Mesh.h>

VOLCANO_GRAPHICS_BEGIN

Frame::Frame() {
    reset();
}

void Frame::reset() noexcept {
    get(Bool::Clear) = true;
    get(Bool::AmbientLight) = true;
    get(Vector3f::AmbientLight) = { 0.05f, 0.05f, 0.05f };
    get(Vector3f::CameraPosition) = { 0.0f, 0.0f, 0.0f };
    get(Vector4f::ClearColor) = { 0.0f, 0.0f, 0.0f, 255.0f };
    get(Affine3f::View) = Eigen::Affine3f::Identity();
    get(Affine3f::Model) = Eigen::Affine3f::Identity();
    get(Matrix4f::Projection) = Eigen::Matrix4f::Identity();
    items_.clear();
    lights_.clear();
}

void Frame::add(const Transform& transform) noexcept {
    get(Affine3f::Model) = transform.toAffine();
}

void Frame::add(const Transform& transform, const Mesh& mesh, const Material* material) {
    Item item;
    item.mesh = &mesh;
    item.material = material;
    item.transform = transform;
    add(item);
}

void Frame::add(const Item& item) {
    if (item.mesh != nullptr && !item.mesh->empty()) {
        items_.push_back(item);
    }
}

void Frame::add(const Light& light) {
    lights_.push_back(light);
}

void Frame::commit(int x, int y, int width, int height) const noexcept {
    glViewport(x, y, width, height);

    if (get(Bool::Clear)) {
        auto& color = get(Vector4f::ClearColor);
        glClearColor(color.x() / 255.0f, color.y() / 255.0f, color.z() / 255.0f, color.w() / 255.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

VOLCANO_GRAPHICS_END
