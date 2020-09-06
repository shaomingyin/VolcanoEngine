

target_sources(Volcano
    PUBLIC
        Volcano.hpp
    PRIVATE
        fontstash.h
        miniz.h
        miniz_export.h
        miniz_common.h
        miniz_tdef.h
        miniz_tinfl.h
        miniz_zip.h
        nanovg.h
        nanovg_gl.h
        nanovg_gl_utils.h
        stb_image.h
        stb_truetype.h
    )

add_subdirectory(Volcano Volcano)
