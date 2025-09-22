//
//
#ifndef VOLCANO_GRAPHICS_PIPELINE_H
#define VOLCANO_GRAPHICS_PIPELINE_H

#include <array>
#include <bitset>

#include <QColor>
#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/View.h>

VOLCANO_GRAPHICS_BEGIN

class Pipeline {
public:
    enum class BoolAttribute {
        Clear = 0,
        Max
    };

    enum class Vector3Attribute {
        Max
    };

    enum class Vector4Attribute {
        Max
    };

    enum class Matrix4x4Attribute {
        Max
    };

    enum class ColorAttribute {
        ClearColor = 0,
        Max
    };

public:
    Pipeline(QOpenGLContext* gl_context = nullptr);
    virtual ~Pipeline();

public:
    void reset();
    void apply();
    void render(const View& view);

    bool isEnabled(BoolAttribute i) const {
        return bool_attributes_[static_cast<int>(i)];
    }

    void enable(BoolAttribute i) {
        bool_attributes_.set(static_cast<int>(i));
    }

    void disable(BoolAttribute i) {
        bool_attributes_.reset(static_cast<int>(i));
    }

    const QVector3D& attr(Vector3Attribute i) const {
        return vector3_attributes_[static_cast<int>(i)];
    }

    QVector3D& attr(Vector3Attribute i) {
        return vector3_attributes_[static_cast<int>(i)];
    }

    const QVector4D& attr(Vector4Attribute i) const {
        return vector4_attributes_[static_cast<int>(i)];
    }

    QVector4D& attr(Vector4Attribute i) {
        return vector4_attributes_[static_cast<int>(i)];
    }

    const QMatrix4x4& attr(Matrix4x4Attribute i) const {
        return matrix4x4_attributes_[static_cast<int>(i)];
    }

    QMatrix4x4& attr(Matrix4x4Attribute i) {
        return matrix4x4_attributes_[static_cast<int>(i)];
    }

    const QColor& attr(ColorAttribute i) const {
        return color_attributes_[static_cast<int>(i)];
    }

    QColor& attr(ColorAttribute i) {
        return color_attributes_[static_cast<int>(i)];
    }

    const QOpenGLShaderProgram& program() const {
        return program_;
    }

    QOpenGLShaderProgram& program() {
        return program_;
    }

private:
    std::bitset<static_cast<size_t>(BoolAttribute::Max)> bool_attributes_;
    std::array<QVector3D, static_cast<size_t>(Vector4Attribute::Max)> vector3_attributes_;
    std::array<QVector4D, static_cast<size_t>(Vector3Attribute::Max)> vector4_attributes_;
    std::array<QMatrix4x4, static_cast<size_t>(Matrix4x4Attribute::Max)> matrix4x4_attributes_;
    std::array<QColor, static_cast<size_t>(ColorAttribute::Max)> color_attributes_;
    QOpenGLShaderProgram program_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_PIPELINE_H
