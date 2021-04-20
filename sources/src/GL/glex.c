/*
 *
 */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <cx/list.h>
#include <HandmadeMath.h>
#include <GL/glex.h>

typedef struct {
	hmm_vec3 translate;
	hmm_vec3 scale;
	hmm_quaternion rotation;
} GLEXTransform;

#define GLEX_TRANSFORM_STACK_DEPTH 64

#define GLEX_HEAP_SIZE_SHIFT 26
#define GLEX_HEAP_SIZE (1 << GLEX_HEAP_SIZE_SHIFT)
#define GLEX_HEAP_MASK (GLEX_HEAP_SIZE - 1)

typedef struct {
	cx_list_t bufferList;
	GLsizeiptr freeSize;
	GLenum target;
	GLenum usage;
	GLuint bufferObject;
	cx_list_node_t node;
} GLEXHeap;

typedef struct {
	GLEXHeap *heap;
	GLintptr offset;
	GLsizeiptr size;
	cx_list_node_t node;
} GLEXBuffer;

struct GLEXLight_ {
	GLEXLightType type;
	GLfloat strength;
	GLfloat range;
	hmm_vec3 color;
	hmm_vec3 direction;
	cx_list_node_t node;
};

struct GLEXMaterial_ {
	GLboolean builtin;
	GLuint program;
	cx_list_node_t node;
};

struct GLEXMesh_ {
	GLEXHeapType heapType;
	GLEXBuffer vertexBuffer;
	void *vertexMap;
	GLEXBuffer vertexIndexBuffer;
	void *vertexIndexMap;
	cx_list_node_t node;
};

typedef struct {
	GLEXLight *light;
	GLEXTransform transform;
} GLEXFrameLight;

typedef struct {
	GLEXMesh *mesh;
	GLEXMaterial *material;
	GLEXTransform transform;
} GLEXFrameMesh;

#define GLEX_FRAME_LIGHT_MAX 256
#define GLEX_FRAME_MESH_MAX 1024

typedef struct {
	GLEXFrameLight lightArray[GLEX_FRAME_LIGHT_MAX];
	GLint lightCount;
	GLEXFrameMesh meshArray[GLEX_FRAME_MESH_MAX];
	GLint meshCount;
} GLEXFrameData;

struct GLEXContext_ {
	GLboolean logEnabled;
	GLEXLogLevel logLevel;
	GLchar *logPrefix;
	GLint logId;

	GLint viewport[4];

	GLboolean clearEnabled;
	hmm_vec4 clearColor;

	cx_list_t heapLists[GLEX_HEAP_TYPE_MAX];

	GLEXTransform transformStack[GLEX_TRANSFORM_STACK_DEPTH];
	GLint transformTop;

	hmm_vec3 viewPosition;
	hmm_vec3 viewDirection;
	hmm_vec3 viewUp;

	GLfloat viewFov;
	GLfloat viewRatio;
	GLfloat viewRange[2];

	GLEXLight *currentLight;
	GLEXLight ambientLight;
	cx_list_t lights[GLEX_LIGHT_TYPE_MAX];

	GLEXMaterial *currentMaterial;
	GLEXMaterial builtinMaterials[GLEX_BUILTIN_MATERIAL_MAX];

	GLEXMesh *currentMesh;
	cx_list_t meshes;

	GLboolean framing;
	GLEXFrameData frameData;

	void *userData;
};

static const hmm_vec3 glexAxis[3] = {
	{ 1.0f, 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f }
};

static CX_THREAD GLEXContext *glex = NULL;

static void glexWriteLog(GLEXLogLevel logLevel, const char *fmt, ...)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(0 <= logLevel && logLevel <= GLEX_LOG_LEVEL_MAX);

	if (!glex->logEnabled || logLevel > glex->logLevel)
		return;

	GLchar buf[256];
	GLint prefixLen;
	static const GLchar ind[5] = { 'F', 'E', 'W', 'I', 'D' };
	
	if (glex->logPrefix != NULL) {
		prefixLen = snprintf(buf, sizeof(buf),
			"[GLEX] %c %p: %s", ind[logLevel], glex, glex->logPrefix);
	} else {
		prefixLen = snprintf(buf, sizeof(buf),
			"[GLEX] %c %p: ", ind[logLevel], glex);
	}

	va_list ap;
	va_start(ap, fmt);
	int userLen = vsnprintf(buf + prefixLen, sizeof(buf) - prefixLen, fmt, ap);
	va_end(ap);

	GLenum type;
	GLenum severity;

	switch (logLevel) {
	case GLEX_LOG_LEVEL_FATAL:
	case GLEX_LOG_LEVEL_ERROR:
		type = GL_DEBUG_TYPE_ERROR;
		severity = GL_DEBUG_SEVERITY_HIGH;
		break;
	case GLEX_LOG_LEVEL_WARNING:
		type = GL_DEBUG_TYPE_ERROR;
		severity = GL_DEBUG_SEVERITY_MEDIUM;
		break;
	default:
		type = GL_DEBUG_TYPE_OTHER;
		severity = GL_DEBUG_SEVERITY_LOW;
		break;
	}

	glDebugMessageInsert(
		GL_DEBUG_SOURCE_APPLICATION, type, glex->logId++, severity, prefixLen + userLen, buf);
}

#define GLEX_LOGF(fmt, ...) glexWriteLog(GLEX_LOG_LEVEL_FATAL, fmt, __VA_ARGS__)
#define GLEX_LOGE(fmt, ...) glexWriteLog(GLEX_LOG_LEVEL_ERROR, fmt, __VA_ARGS__)
#define GLEX_LOGW(fmt, ...) glexWriteLog(GLEX_LOG_LEVEL_WARNING, fmt, __VA_ARGS__)
#define GLEX_LOGI(fmt, ...) glexWriteLog(GLEX_LOG_LEVEL_INFO, fmt, __VA_ARGS__)
#define GLEX_LOGD(fmt, ...) glexWriteLog(GLEX_LOG_LEVEL_DEBUG, fmt, __VA_ARGS__)

static GLboolean glexAllocBufferFromHeap(GLEXHeap *heap, GLEXBuffer *out, GLsizeiptr size)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(out != NULL);

	if (size > heap->freeSize)
		return GL_FALSE;

	cx_list_node_t *node;
	GLEXBuffer *used = NULL;
	GLsizeiptr offset = 0;

	CX_LIST_FOREACH(node, &heap->bufferList) {
		used = CX_MEMBEROF(node, GLEXBuffer, node);
		if ((used->offset - offset) >= size)
			break;
		offset = used->offset + used->size;
	}

	out->heap = heap;
	out->offset = offset;
	out->size = size;

	cx_list_node_reset(&out->node);

	if (node == cx_list_knot(&heap->bufferList)) {
		if (used != NULL && size > (GLEX_HEAP_SIZE - offset))
			return GL_FALSE;
		cx_list_append(&heap->bufferList, &out->node);
	} else
		cx_list_insert(&heap->bufferList, &out->node, node);

	heap->freeSize -= size;

	return GL_TRUE;
}

static GLboolean glexAllocBuffer(GLEXBuffer *out, GLEXHeapType heapType, GLsizeiptr size)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(out != NULL);
	GLEX_ASSERT(0 <= heapType && heapType < GLEX_HEAP_TYPE_MAX);
	GLEX_ASSERT(size > 0);

	GLEXHeap *heap;
	cx_list_t *heapList = &glex->heapLists[heapType];
	cx_list_node_t *node;

	CX_LIST_FOREACH(node, heapList) {
		heap = CX_MEMBEROF(node, GLEXHeap, node);
		if (glexAllocBufferFromHeap(heap, out, size))
			return GL_TRUE;
	}

	GLenum usage;

	switch (heapType) {
	case GLEX_HEAP_TYPE_STATIC_DRAW:
		usage = GL_STATIC_DRAW;
		break;
	case GLEX_HEAP_TYPE_DYNAMIC_DRAW:
		usage = GL_DYNAMIC_DRAW;
		break;
	default:
		return GL_FALSE;
	}

	heap = malloc(sizeof(GLEXHeap));
	if (heap == NULL)
		return GL_FALSE;

	glGenBuffers(1, &heap->bufferObject);
	if (heap->bufferObject == 0) {
		free(heap);
		return GL_FALSE;
	}

	GLint arrayBufferBackup;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &arrayBufferBackup);
	glBindBuffer(GL_ARRAY_BUFFER, heap->bufferObject);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, usage);
	glBindBuffer(GL_ARRAY_BUFFER, arrayBufferBackup);

	heap->freeSize = GLEX_HEAP_SIZE;

	cx_list_reset(&heap->bufferList);
	cx_list_node_reset(&heap->node);
	cx_list_append(heapList, &heap->node);

	return glexAllocBufferFromHeap(heap, out, size);
}

static void glexFreeBuffer(GLEXBuffer *buf)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(buf->size > 0);
	GLEX_ASSERT(buf->heap != NULL);
	GLEX_ASSERT(cx_list_is_contains(&buf->heap->bufferList, &buf->node));

	buf->size = 0;
	buf->heap->freeSize += buf->size;
	buf->heap = NULL;
	buf->offset = 0;

	cx_list_node_unlink(&buf->node);
}

static GLuint glexCreateShader(GLenum type, const char *source)
{
	int len = strlen(source);
	if (len <= 0)
		return 0;

	GLuint id = glCreateShader(type);
	if (id == 0)
		return 0;

	glShaderSource(id, 1, &source, &len);
	glCompileShader(id);

	GLint compileStatus;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus) {
		char infoLog[256];
		GLsizei len;
		glGetShaderInfoLog(id, sizeof(infoLog), &len, infoLog);
		GLEX_LOGE(infoLog);
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static GLuint glexCreateProgram(GLenum type1, const char *source1, ...)
{
	va_list ap;
	va_start(ap, source1);

	GLuint id = glCreateProgram();
	if (id == 0)
		goto bad0;

	GLuint shader1 = glexCreateShader(type1, source1);
	if (shader1 == 0)
		goto bad1;

	glAttachShader(id, shader1);

	for (;;) {
		GLenum type = va_arg(ap, GLenum);
		if (type == GL_INVALID_ENUM)
			break;
		const char *source = va_arg(ap, const char *);
		if (source == NULL)
			continue;
		GLuint shader = glexCreateShader(type, source);
		if (shader == 0)
			goto bad1;
		glAttachShader(id, shader);
		glDeleteShader(shader);
	}

	va_end(ap);

	glLinkProgram(id);

	GLint linkStatus;
	glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus) {
		goto bad1;
	}

bad1:
	glDeleteProgram(id);

bad0:
	return 0;
}

static GLboolean glexInitLight(GLEXContext *context, GLEXLight *light, GLEXLightType lightType)
{
	GLEX_ASSERT(context != NULL);
	GLEX_ASSERT(light != NULL);
	GLEX_ASSERT((0 <= lightType && lightType < GLEX_LIGHT_TYPE_MAX) || lightType == GLEX_LIGHT_TYPE_AMBIENT);

	light->type = lightType;
	light->color = HMM_Vec3(1.0f, 1.0f, 1.0f);
	light->strength = 1.0f;

	cx_list_node_reset(&light->node);

	if (0 <= lightType && lightType < GLEX_LIGHT_TYPE_MAX)
		cx_list_append(&context->lights[lightType], &light->node);

	return GL_TRUE;
}

static GLboolean glexInitMaterial(GLEXContext *context, GLEXMaterial *material)
{
	GLEX_ASSERT(context != NULL);
	GLEX_ASSERT(material != NULL);

	// TODO

	return GL_TRUE;
}

static GLboolean glexInitBuiltinMaterial(GLEXContext *context)
{
	GLEX_ASSERT(context != NULL);

	// TODO glexInitMaterial();

	return GL_TRUE;
}

static GLboolean glexInitMesh(GLEXContext *context, GLEXMesh *mesh, GLEXHeapType heapType,
	GLsizeiptr vertexCount, GLsizeiptr vertexIndexCount)
{
	GLEX_ASSERT(context != NULL);
	GLEX_ASSERT(mesh != NULL);
	GLEX_ASSERT(0 <= heapType && heapType < GLEX_HEAP_TYPE_MAX);
	GLEX_ASSERT(vertexCount > 0);
	GLEX_ASSERT(vertexIndexCount > 0);

	GLsizeiptr vertexBufferSize = sizeof(GLEXVertex) * vertexCount;
	if (!glexAllocBuffer(&mesh->vertexBuffer, heapType, vertexBufferSize))
		return GL_FALSE;

	GLsizeiptr vertexIndexBufferSize = sizeof(GLEXVertexIndex) * vertexIndexCount;
	if (!glexAllocBuffer(&mesh->vertexIndexBuffer, heapType, vertexIndexBufferSize)) {
		glexFreeBuffer(&mesh->vertexBuffer);
		return GL_FALSE;
	}

	mesh->heapType = heapType;
	mesh->vertexMap = NULL;
	mesh->vertexIndexMap = NULL;

	cx_list_node_reset(&mesh->node);
	cx_list_append(&context->meshes, &mesh->node);

	return GL_TRUE;
}

GLEX_API GLEXContext *glexCreateContext(void *userData)
{
	GLEXContext *context = malloc(sizeof(GLEXContext));
	if (context == NULL)
		goto bad0;

	context->logEnabled = GL_FALSE;
	context->logLevel = GLEX_LOG_LEVEL_ERROR;
	context->logPrefix = NULL;
	context->logId = 0;

	context->viewport[0] = 0;
	context->viewport[1] = 0;
	context->viewport[2] = 1;
	context->viewport[3] = 1;

	context->clearEnabled = GL_TRUE;
	context->clearColor = HMM_Vec4(0.0f, 0.0f, 0.0f, 0.0f);

	for (GLint i = 0; i < GLEX_HEAP_TYPE_MAX; ++i)
		cx_list_reset(&context->heapLists[i]);

	context->transformStack[0].translate = HMM_Vec3(0.0f, 0.0f, 0.0f);
	context->transformStack[0].scale = HMM_Vec3(1.0f, 1.0f, 1.0f);
	context->transformStack[0].rotation = HMM_QuaternionFromAxisAngle(glexAxis[0], 0.0f);
	context->transformTop = 0;

	context->viewPosition = HMM_Vec3(0.0f, 0.0f, 0.0f);
	context->viewDirection = HMM_Vec3(0.0f, 0.0f, -1.0f);
	context->viewUp = HMM_Vec3(0.0f, 1.0f, 0.0f);

	context->viewFov = 90.0f;
	context->viewRatio = 4.0f / 3.0f;
	context->viewRange[0] = 0.1f;
	context->viewRange[1] = 100.0f;

	context->currentLight = NULL;

	if (!glexInitLight(context, &context->ambientLight, GLEX_LIGHT_TYPE_AMBIENT))
		goto bad1;

	for (GLint i = 0; i < GLEX_LIGHT_TYPE_MAX; ++i)
		cx_list_reset(&context->lights[i]);

	context->currentMaterial = NULL;

	if (!glexInitBuiltinMaterial(context))
		goto bad1;

	context->currentMesh = NULL;
	cx_list_reset(&context->meshes);

	context->framing = GL_FALSE;
	context->frameData.lightCount = 0;
	context->frameData.meshCount = 0;

	context->userData = userData;

	return context;

bad1:
	free(context);

bad0:
	return NULL;
}

GLEX_API void glexDeleteContext(GLEXContext *context)
{
	GLEX_ASSERT(context != NULL);

	if (glex == context)
		glex = NULL;

	free(context);
}

GLEX_API GLEXContext *glexCurrentContext(void)
{
	return glex;
}

GLEX_API void glexMakeCurrent(GLEXContext *context)
{
	glex = context;
}

GLEX_API void *glexUserData(void)
{
	GLEX_ASSERT(glex != NULL);

	return glex->userData;
}

GLEX_API void glexEnableLog(void)
{
	GLEX_ASSERT(glex != NULL);

	glex->logEnabled = GL_TRUE;
}

GLEX_API void glexDisableLog(void)
{
	GLEX_ASSERT(glex != NULL);

	glex->logEnabled = GL_FALSE;
}

GLEX_API GLboolean glexIsLogEnabled(void)
{
	GLEX_ASSERT(glex != NULL);

	return glex->logEnabled;
}

GLEX_API void glexLogPrefix(const char *prefix)
{
	GLEX_ASSERT(glex != NULL);

	if (glex->logPrefix != NULL) {
		free(glex->logPrefix);
		glex->logPrefix = NULL;
	}

	if (prefix != NULL) {
		glex->logPrefix = strdup(prefix);
		if (glex->logPrefix == NULL)
			GLEX_LOGE("Failed to set log prefix.");
	}
}

GLEX_API void glexLogLevel(GLEXLogLevel logLevel)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(0 <= logLevel && logLevel < GLEX_LOG_LEVEL_MAX);

	glex->logLevel = logLevel;
}

GLEX_API GLEXLogLevel glexGetLogLevel(void)
{
	GLEX_ASSERT(glex != NULL);

	return glex->logLevel;
}

GLEX_API void glexViewport(int x, int y, int width, int height)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(width >= 0);
	GLEX_ASSERT(height >= 0);

	glex->viewport[0] = x;
	glex->viewport[1] = y;
	glex->viewport[2] = width;
	glex->viewport[3] = height;
}

GLEX_API void glexEnableClear(void)
{
	GLEX_ASSERT(glex != NULL);

	glex->clearEnabled = GL_TRUE;
}

GLEX_API void glexDisableClear(void)
{
	GLEX_ASSERT(glex != NULL);

	glex->clearEnabled = GL_FALSE;
}

GLEX_API GLboolean glexIsClearEnabled(void)
{
	GLEX_ASSERT(glex != NULL);

	return glex->clearEnabled;
}

GLEX_API void glexClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	GLEX_ASSERT(glex != NULL);

	glex->clearColor.R = red;
	glex->clearColor.G = green;
	glex->clearColor.B = blue;
	glex->clearColor.A = alpha;
}

GLEX_API void glexBeginFrame(void)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(!glex->framing);

	glex->framing = GL_TRUE;
	glex->frameData.lightCount = 0;
	glex->frameData.meshCount = 0;
}

GLEX_API void glexEndFrame(void)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(glex->framing);

	glex->framing = GL_FALSE;

	if (glex->viewport[2] < 1 || glex->viewport[3] < 0)
		return;

	glViewport(glex->viewport[0], glex->viewport[1], glex->viewport[2], glex->viewport[3]);

	if (glex->clearEnabled) {
		glClearColor(glex->clearColor.R, glex->clearColor.G, glex->clearColor.B, glex->clearColor.A);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	if (glex->frameData.meshCount < 1)
		return;

	hmm_mat4 viewMatrix = HMM_LookAt(glex->viewPosition,
		HMM_AddVec3(glex->viewPosition, glex->viewDirection), glex->viewUp);

	hmm_mat4 projectionMatrix =
		HMM_Perspective(glex->viewFov, glex->viewRatio, glex->viewRange[0], glex->viewRange[1]);

	// TODO Deferred rendering.
}

GLEX_API void glexResetTransform(void)
{
	GLEX_ASSERT(glex != NULL);

	glex->transformStack[glex->transformTop].translate = HMM_Vec3(0.0f, 0.0f, 0.0f);
	glex->transformStack[glex->transformTop].scale = HMM_Vec3(1.0f, 1.0f, 1.0f);
	glex->transformStack[glex->transformTop].rotation = HMM_QuaternionFromAxisAngle(glexAxis[0], 0.0f);
}

GLEX_API void glexPushTransform(void)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(glex->transformTop < (GLEX_TRANSFORM_STACK_DEPTH - 1));

	GLint oldTop = glex->transformTop++;
	glex->transformStack[glex->transformTop] = glex->transformStack[oldTop];
}

GLEX_API void glexPopTransform(void)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(glex->transformTop > 0);

	glex->transformTop -= 1;
}

GLEX_API void glexTranslate(GLfloat x, GLfloat y, GLfloat z)
{
	GLEX_ASSERT(glex != NULL);

	glex->transformStack[glex->transformTop].translate =
		HMM_AddVec3(glex->transformStack[glex->transformTop].translate, HMM_Vec3(x, y, z));
}

GLEX_API void glexScale(GLfloat x, GLfloat y, GLfloat z)
{
	GLEX_ASSERT(glex != NULL);

	glex->transformStack[glex->transformTop].scale =
		HMM_MultiplyVec3(glex->transformStack[glex->transformTop].scale, HMM_Vec3(x, y, z));
}

GLEX_API void glexRotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	GLEX_ASSERT(glex != NULL);

	glex->transformStack[glex->transformTop].rotation =
		HMM_MultiplyQuaternion(glex->transformStack[glex->transformTop].rotation,
			HMM_QuaternionFromAxisAngle(HMM_Vec3(x, y, z), angle));
}

GLEX_API void glexLookAt(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ,
	GLfloat dirX, GLfloat dirY, GLfloat dirZ, GLfloat upX, GLfloat upY, GLfloat upZ)
{
	GLEX_ASSERT(glex != NULL);

	glex->viewPosition.X = eyeX;
	glex->viewPosition.Y = eyeY;
	glex->viewPosition.Z = eyeZ;

	glex->viewDirection.X = dirX;
	glex->viewDirection.Y = dirY;
	glex->viewDirection.Z = dirZ;

	glex->viewUp.X = upX;
	glex->viewUp.Y = upY;
	glex->viewUp.Z = upZ;
}

GLEX_API void glexPerpective(GLfloat fov, GLfloat ratio, GLfloat zNear, GLfloat zFar)
{
	GLEX_ASSERT(glex != NULL);

	glex->viewFov = fov;
	glex->viewRatio = ratio;
	glex->viewRange[0] = zNear;
	glex->viewRange[1] = zFar;
}

GLEX_API GLEXLight *glexCreateLight(GLEXLightType lightType)
{
	GLEX_ASSERT(glex != NULL);

	GLEXLight *light = malloc(sizeof(GLEXLight));
	if (light == NULL)
		return NULL;

	if (!glexInitLight(glex, light, lightType)) {
		free(light);
		return NULL;
	}

	return light;
}

GLEX_API GLEXLight *glexAmbientLight(void)
{
	GLEX_ASSERT(glex != NULL);

	return &glex->ambientLight;
}

GLEX_API void glexDeleteLight(GLEXLight *light)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(light != NULL);

	if (glex->currentLight == light)
		glex->currentLight = NULL;

	cx_list_node_unlink(&light->node);

	free(light);
}

GLEX_API void glexBindLight(GLEXLight *light)
{
	GLEX_ASSERT(glex != NULL);

	glex->currentLight = light;
}

GLEX_API void glexAddLight(void)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(glex->framing);
	GLEX_ASSERT(glex->currentLight != NULL);

	if (glex->frameData.lightCount < GLEX_FRAME_LIGHT_MAX) {
		GLEXFrameLight *frameLight = &glex->frameData.lightArray[glex->frameData.lightCount++];
		frameLight->light = glex->currentLight;
		frameLight->transform = glex->transformStack[glex->transformTop];
	}
}

GLEX_API void glexLightStrength(GLfloat strength)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(glex->currentLight != NULL);
	GLEX_ASSERT(strength >= 0.0f);

	glex->currentLight->strength = strength;
}

GLEX_API void glexLightColor(GLfloat red, GLfloat green, GLfloat blue)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(glex->currentLight != NULL);

	glex->currentLight->color = HMM_Vec3(red, green, blue);
}

GLEX_API void glexLightDirection(GLfloat x, GLfloat y, GLfloat z)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(glex->currentLight != NULL);
	GLEX_ASSERT(glex->currentLight->type == GLEX_LIGHT_TYPE_DIRECTIONAL || glex->currentLight->type == GLEX_LIGHT_TYPE_SPOT);

	glex->currentLight->direction = HMM_NormalizeVec3(HMM_Vec3(x, y, z));
}

GLEX_API void glexLightSpotShape(GLEXLightSpotShape lightSpotShape)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(glex->currentLight->type == GLEX_LIGHT_TYPE_SPOT);

	// TODO
}

GLEX_API void glexLightRange(GLfloat range)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(glex->currentLight != NULL);
	GLEX_ASSERT(glex->currentLight->type == GLEX_LIGHT_TYPE_POINT || glex->currentLight->type == GLEX_LIGHT_TYPE_SPOT);
	GLEX_ASSERT(range > 0.0f);

	glex->currentLight->range = range;
}

GLEX_API GLEXMaterial *glexCreateMaterial(void)
{
	GLEX_ASSERT(glex != NULL);

	GLEXMaterial *material = malloc(sizeof(GLEXMaterial));
	if (material == NULL)
		return NULL;

	if (!glexInitMaterial(glex, material)) {
		free(material);
		return NULL;
	}

	return material;
}

GLEX_API GLEXMaterial *glexGetBuiltinMaterial(GLEXBuiltinMaterial builtinMaterial)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(0 <= builtinMaterial && builtinMaterial < GLEX_BUILTIN_MATERIAL_MAX);

	return &glex->builtinMaterials[builtinMaterial];
}

GLEX_API void glexDeleteMaterial(GLEXMaterial *material)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(material != NULL);

	// TODO

	free(material);
}

GLEX_API void glexBindMaterial(GLEXMaterial *material)
{
	GLEX_ASSERT(glex != NULL);

	glex->currentMaterial = material;
}

GLEX_API GLEXMesh *glexCreateMesh(GLEXHeapType heapType, GLsizeiptr vertexCount, GLsizeiptr vertexIndexCount)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(0 <= heapType && heapType < GLEX_HEAP_TYPE_MAX);
	GLEX_ASSERT(vertexCount > 0);
	GLEX_ASSERT(vertexIndexCount > 0);

	GLEXMesh *mesh = malloc(sizeof(GLEXMesh));
	if (mesh == NULL)
		return NULL;

	if (!glexInitMesh(glex, mesh, heapType, vertexCount, vertexIndexCount)) {
		free(mesh);
		return NULL;
	}

	return mesh;
}

GLEX_API void glexDeleteMesh(GLEXMesh *mesh)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(mesh != NULL);
	GLEX_ASSERT(mesh->vertexMap == NULL);
	GLEX_ASSERT(mesh->vertexIndexMap == NULL);

	if (glex->currentMesh == mesh)
		glex->currentMesh = NULL;

	glexFreeBuffer(&mesh->vertexBuffer);
	glexFreeBuffer(&mesh->vertexIndexBuffer);

	cx_list_node_unlink(&mesh->node);

	free(mesh);
}

GLEX_API void glexBindMesh(GLEXMesh *mesh)
{
	GLEX_ASSERT(glex != NULL);

	glex->currentMesh = mesh;

	if (mesh != NULL) {
		GLEX_ASSERT(mesh->vertexBuffer.heap != NULL);
		GLEX_ASSERT(mesh->vertexIndexBuffer.heap != NULL);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer.heap->bufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vertexIndexBuffer.heap->bufferObject);
	} else {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

GLEX_API void glexAddMesh(void)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(glex->framing);
	GLEX_ASSERT(glex->currentMesh != NULL);

	if (glex->frameData.meshCount < GLEX_FRAME_MESH_MAX) {
		GLEXFrameMesh *frameMesh = &glex->frameData.meshArray[glex->frameData.meshCount++];
		frameMesh->mesh = glex->currentMesh;
		frameMesh->material = glex->currentMaterial;
		frameMesh->transform = glex->transformStack[glex->transformTop];
	}
}

GLEX_API void glexMeshVertexData(const GLEXVertex *p, GLsizeiptr count, GLintptr offset)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(p != NULL);
	GLEX_ASSERT(count > 0);
	GLEX_ASSERT(offset >= 0);

	GLEXMesh *mesh = glex->currentMesh;
	GLEX_ASSERT(mesh != NULL);

#ifdef GLEX_DEBUG
	GLint id;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &id);
	GLEX_ASSERT(id == mesh->vertexBuffer.heap->bufferObject);
#endif

	GLsizeiptr size = sizeof(GLEXVertex) * count;
	GLsizeiptr headSize = mesh->vertexBuffer.offset + sizeof(GLEXVertex) * offset;
	GLEX_ASSERT((mesh->vertexBuffer.size - headSize) >= size);

	glBufferSubData(GL_ARRAY_BUFFER, mesh->vertexBuffer.offset + headSize, size, p);
}

GLEX_API void glexMeshVertexIndexData(const GLEXVertexIndex *p, GLsizeiptr count, GLintptr offset)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(p != NULL);
	GLEX_ASSERT(count > 0);
	GLEX_ASSERT(offset >= 0);

	GLEXMesh *mesh = glex->currentMesh;
	GLEX_ASSERT(mesh != NULL);

#ifdef GLEX_DEBUG
	GLint id;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &id);
	GLEX_ASSERT(id == mesh->vertexIndexBuffer.heap->bufferObject);
#endif

	GLsizeiptr size = sizeof(GLEXVertexIndex) * count;
	GLsizeiptr headSize = mesh->vertexIndexBuffer.offset + sizeof(GLEXVertexIndex) * offset;
	GLEX_ASSERT((mesh->vertexIndexBuffer.size - headSize) >= size);

	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, headSize, size, p);
}

GLEX_API GLEXVertex *glexMapMeshVertexBuffer(GLintptr offset, GLsizeiptr count)
{
	GLEX_ASSERT(glex != NULL);

	GLEXMesh *mesh = glex->currentMesh;
	GLEX_ASSERT(mesh != NULL);
	GLEX_ASSERT(mesh->vertexBuffer.heap != NULL);
	GLEX_ASSERT(mesh->vertexMap == NULL);

#ifdef GLEX_DEBUG
	GLint id;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &id);
	GLEX_ASSERT(id == mesh->vertexBuffer.heap->bufferObject);
#endif

	GLsizeiptr size = sizeof(GLEXVertex) * count;
	mesh->vertexIndexMap = glMapBufferRange(GL_ARRAY_BUFFER, offset, size, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

	return mesh->vertexMap;
}

GLEX_API void glexUnmapMeshVertexBuffer(void)
{
	GLEX_ASSERT(glex != NULL);

	GLEXMesh *mesh = glex->currentMesh;
	GLEX_ASSERT(mesh != NULL);
	GLEX_ASSERT(mesh->vertexBuffer.heap != NULL);
	GLEX_ASSERT(mesh->vertexMap != NULL);

#ifdef GLEX_DEBUG
	GLint id;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &id);
	GLEX_ASSERT(id == mesh->vertexBuffer.heap->bufferObject);
#endif

	glUnmapBuffer(GL_ARRAY_BUFFER);
	mesh->vertexMap = NULL;
}

GLEX_API GLEXVertexIndex *glexMapMeshVertexIndexBuffer(GLintptr offset, GLsizeiptr count)
{
	GLEX_ASSERT(glex != NULL);

	GLEXMesh *mesh = glex->currentMesh;
	GLEX_ASSERT(mesh != NULL);
	GLEX_ASSERT(mesh->vertexIndexBuffer.heap != NULL);
	GLEX_ASSERT(mesh->vertexIndexMap == NULL);

#ifdef GLEX_DEBUG
	GLint id;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &id);
	GLEX_ASSERT(id == mesh->vertexIndexBuffer.heap->bufferObject);
#endif

	GLsizeiptr size = sizeof(GLEXVertex) * count;
	mesh->vertexIndexMap = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, offset, size, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

	return mesh->vertexIndexMap;
}

GLEX_API void glexUnmapMeshVertexIndexBuffer(void)
{
	GLEX_ASSERT(glex != NULL);

	GLEXMesh *mesh = glex->currentMesh;
	GLEX_ASSERT(mesh != NULL);
	GLEX_ASSERT(mesh->vertexIndexBuffer.heap != NULL);
	GLEX_ASSERT(mesh->vertexIndexMap != NULL);

#ifdef GLEX_DEBUG
	GLint id;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &id);
	GLEX_ASSERT(id == mesh->vertexIndexBuffer.heap->bufferObject);
#endif

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	mesh->vertexIndexMap = NULL;
}
