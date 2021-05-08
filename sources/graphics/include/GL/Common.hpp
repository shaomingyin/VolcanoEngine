//
//
#ifndef GL_COMMON_HPP
#define GL_COMMON_HPP

#include <GL/gl3w.h>

#include <Eigen/Geometry>

#define GL_DEBUG_ENABLED 1

#ifndef GL_ASSERT
#   if GL_DEBUG_ENABLED
#       include <cassert>
#       define GL_ASSERT(expr) assert(expr)
#   else
#       define GL_ASSERT(expr) do { } while (0)
#   endif
#endif

#ifndef GL_INLINE
#   define GL_INLINE inline
#endif

#ifndef GL_API
#   define GL_API
#endif

#ifndef GL_NAMESPACE
#   define GL_NAMESPACE GL
#endif

#define GL_BEGIN namespace GL_NAMESPACE {
#define GL_END }

GL_BEGIN

GL_END

#endif // GL_COMMON_HPP
