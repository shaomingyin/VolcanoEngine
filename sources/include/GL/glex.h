/*
 *
 */
#ifndef GLEX_H
#define GLEX_H

#include <GL/gl3w.h>

#define GLEX_DEBUG

#ifndef GLEX_API
#	define GLEX_API
#endif

#ifndef GLEX_ASSERT
#	ifdef GLEX_DEBUG
#		include <assert.h>
#		define GLEX_ASSERT(expr) assert(expr)
#	else
#		define GLEX_ASSERT(expr) do { } while (0)
#	endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	GLEX_LOG_LEVEL_FATAL = 0,
	GLEX_LOG_LEVEL_ERROR,
	GLEX_LOG_LEVEL_WARNING,
	GLEX_LOG_LEVEL_INFO,
	GLEX_LOG_LEVEL_DEBUG,
	GLEX_LOG_LEVEL_MAX
} GLEXLogLevel;

typedef enum {
	GLEX_HEAP_TYPE_STATIC_DRAW = 0,
	GLEX_HEAP_TYPE_DYNAMIC_DRAW,
	GLEX_HEAP_TYPE_MAX
} GLEXHeapType;

typedef enum {
	GLEX_LIGHT_TYPE_DIRECTIONAL = 0,
	GLEX_LIGHT_TYPE_POINT,
	GLEX_LIGHT_TYPE_SPOT,
	GLEX_LIGHT_TYPE_MAX,
	GLEX_LIGHT_TYPE_AMBIENT /* For internal. */
} GLEXLightType;

typedef enum {
	GLEX_LIGHT_SPOT_SHAPE_CIRCLE = 0,
	GLEX_LIGHT_SPOT_SHAPE_RECTANGLE,
	GLEX_LIGHT_SPOT_SHAPE_TRIANGLE,
	GLEX_LIGHT_SPOT_SHAPE_MAX
} GLEXLightSpotShape;

typedef enum {
	GLEX_BUILTIN_MATERIAL_EMPTY = 0,
	GLEX_BUILTIN_MATERIAL_MAX
} GLEXBuiltinMaterial;

typedef struct {
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat texCoord[2];
	GLfloat __pad;
} GLEXVertex;

typedef GLuint GLEXVertexIndex;

typedef struct GLEXContext_ GLEXContext;
typedef struct GLEXLight_ GLEXLight;
typedef struct GLEXMaterial_ GLEXMaterial;
typedef struct GLEXMesh_ GLEXMesh;

GLEX_API GLEXContext *glexCreateContext(void *userData);
GLEX_API void glexDeleteContext(GLEXContext *context);
GLEX_API GLEXContext *glexCurrentContext(void);
GLEX_API void glexMakeCurrent(GLEXContext *context);

GLEX_API void *glexUserData(void);

GLEX_API void glexEnableLog(void);
GLEX_API void glexDisableLog(void);
GLEX_API GLboolean glexIsLogEnabled(void);
GLEX_API void glexLogPrefix(const char *prefix);
GLEX_API void glexLogLevel(GLEXLogLevel logLevel);
GLEX_API GLEXLogLevel glexGetLogLevel(void);

GLEX_API void glexViewport(int x, int y, int width, int height);

GLEX_API void glexEnableClear(void);
GLEX_API void glexDisableClear(void);
GLEX_API GLboolean glexIsClearEnabled(void);
GLEX_API void glexClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

GLEX_API void glexBeginFrame(void);
GLEX_API void glexEndFrame(void);

GLEX_API void glexResetTransform(void);
GLEX_API void glexPushTransform(void);
GLEX_API void glexPopTransform(void);
GLEX_API void glexTranslate(GLfloat x, GLfloat y, GLfloat z);
GLEX_API void glexScale(GLfloat x, GLfloat y, GLfloat z);
GLEX_API void glexRotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

GLEX_API void glexLookAt(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ,
	GLfloat dirX, GLfloat dirY, GLfloat dirZ, GLfloat upX, GLfloat upY, GLfloat upZ);

GLEX_API void glexPerpective(GLfloat fov, GLfloat ratio, GLfloat zNear, GLfloat zFar);

GLEX_API GLEXLight *glexCreateLight(GLEXLightType lightType);
GLEX_API GLEXLight *glexAmbientLight(void);
GLEX_API void glexDeleteLight(GLEXLight *light);
GLEX_API void glexBindLight(GLEXLight *light);
GLEX_API void glexAddLight(void);
GLEX_API void glexLightStrength(GLfloat strength);
GLEX_API void glexLightColor(GLfloat red, GLfloat green, GLfloat blue);
GLEX_API void glexLightDirection(GLfloat x, GLfloat y, GLfloat z);
GLEX_API void glexLightSpotShape(GLEXLightSpotShape lightSpotShape);
GLEX_API void glexLightRange(GLfloat range);

GLEX_API GLEXMaterial *glexCreateMaterial(void);
GLEX_API GLEXMaterial *glexGetBuiltinMaterial(GLEXBuiltinMaterial builtinMaterial);
GLEX_API void glexDeleteMaterial(GLEXMaterial *material);
GLEX_API void glexBindMaterial(GLEXMaterial *material);

GLEX_API GLEXMesh *glexCreateMesh(GLEXHeapType heapType, GLsizeiptr vertexCount, GLsizeiptr vertexIndexCount);
GLEX_API void glexDeleteMesh(GLEXMesh *mesh);
GLEX_API void glexBindMesh(GLEXMesh *mesh);
GLEX_API void glexAddMesh(void);
GLEX_API void glexMeshVertexData(const GLEXVertex *p, GLsizeiptr count, GLintptr offset);
GLEX_API void glexMeshVertexIndexData(const GLEXVertexIndex *p, GLsizeiptr count, GLintptr offset);
GLEX_API GLEXVertex *glexMapMeshVertexBuffer(GLintptr offset, GLsizeiptr count);
GLEX_API void glexUnmapMeshVertexBuffer(void);
GLEX_API GLEXVertexIndex *glexMapMeshVertexIndexBuffer(GLintptr offset, GLsizeiptr count);
GLEX_API void glexUnmapMeshVertexIndexBuffer(void);

#ifdef __cplusplus
}
#endif

#endif /* GLEX_H */
