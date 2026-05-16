//
//
#ifndef VOLCANO_GRAPHICS_PIPELINE_H
#define VOLCANO_GRAPHICS_PIPELINE_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Shader.h>
#include <Volcano/Graphics/Program.h>

VOLCANO_GRAPHICS_BEGIN

class Pipeline {
public:
    enum class CullMode {
        None,
        Front,
        Back
    };

    enum class FrontFace {
        Clockwise,
        CounterClockwise
    };

    enum class CompareOp {
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        NotEqual,
        GreaterEqual,
        Always
    };

    enum class BlendFactor {
        Zero,
        One,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha
    };

    enum class BlendOp {
        Add,
        Subtract,
        ReverseSubtract,
        Min,
        Max
    };

    enum class VertexFormat {
        Float2,
        Float3,
        Float4,
        UInt4,
    };

    struct VertexAttributeDesc {
        uint32_t location = 0;
        uint32_t binding = 0;
        VertexFormat format;
        uint32_t offset = 0;
    };

    struct VertexBufferLayoutDesc {
        uint32_t binding = 0;
        uint32_t stride = 0;
        bool perInstance = false;
    };

    struct VertexInputState {
        std::vector<VertexBufferLayoutDesc> buffers;
        std::vector<VertexAttributeDesc> attributes;
    };

public:
	Pipeline() = default;
	virtual ~Pipeline() = default;

public:

private:
    CullMode cull_mode_;
	FrontFace front_face_;
	CompareOp compare_op_;
	BlendFactor blend_src_factor_;
	BlendFactor blend_dst_factor_;
	BlendOp blend_op_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_PIPELINE_H
