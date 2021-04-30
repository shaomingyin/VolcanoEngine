/*
 *
 */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <cx/list.h>
#include <GL/glex.h>

#define HANDMADE_MATH_IMPLEMENTATION
#include <HandmadeMath.h>

typedef struct {
	hmm_vec3 translate;
	hmm_vec3 scale;
	hmm_quaternion rotation;
} GLEXTransform;

#define GLEX_TRANSFORM_STACK_DEPTH 16

#define GLEX_HEAP_SIZE_SHIFT 26
#define GLEX_HEAP_SIZE (1 << GLEX_HEAP_SIZE_SHIFT)
#define GLEX_HEAP_MASK (GLEX_HEAP_SIZE - 1)

typedef struct {
	GLuint id;
	GLsizeiptr freeSize;
	cx_list_node_t node;
	cx_list_t bufferList;
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
	GLint width;
	GLint height;
	GLboolean sizeChanged;
	GLuint id;
	GLuint positionTex;
	GLuint normalTex;
	GLuint colorTex;
	GLuint program;
} GLEXGBuffer;

typedef struct {
	GLEXLight *light;
	GLEXTransform transform;
} GLEXFrameLight;

typedef struct {
	GLEXMesh *mesh;
	GLEXMaterial *material;
	GLEXTransform transform;
} GLEXFrameMesh;

#define GLEX_FRAME_LIGHT_MAX 512
#define GLEX_FRAME_MESH_MAX 4096

typedef struct {
	hmm_mat4 viewMatrix;
	hmm_mat4 projectionMatrix;
	GLEXFrameLight lightArray[GLEX_FRAME_LIGHT_MAX];
	GLint lightCount;
	GLEXFrameMesh meshArray[GLEX_FRAME_MESH_MAX];
	GLint meshCount;
} GLEXFrameData;

struct GLEXContext_ {
	GLboolean lightingEnabled;
	GLboolean shadowEnabled;
	GLuint debugDrawMask;

	cx_list_t heapLists[GLEX_HEAP_TYPE_MAX];

	GLEXTransform transformStack[GLEX_TRANSFORM_STACK_DEPTH];
	GLint transformTop;

	hmm_vec3 viewPosition;
	hmm_vec3 viewCenter;
	hmm_vec3 viewUp;

	GLfloat viewFov;
	GLfloat viewRatio;
	GLfloat viewRange[2];

	GLEXGBuffer gBuffer;

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

static struct {
	GLboolean logEnabled;
	GLEXLogLevel logLevel;
	GLchar *logPrefix;
	GLint logId;
	GLint contextCount;
} glexState;

static CX_THREAD GLEXContext *glexCurrent = NULL;

static const GLchar *GLEX_VS_BASIC =
	"#version 330 core\n"
	"layout(location = 0) in vec3 inPosition;\n"
	"layout(location = 1) in vec3 inNormal;\n"
	"layout(location = 2) in vec2 inTexCoord;\n"
	"uniform mat4 modelMatrix;\n"
	"uniform mat4 viewMatrix;\n"
	"uniform mat4 projectionMatrix;\n"
	"uniform mat4 normalMatrix;\n"
	"void main() {\n"
	"	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);\n"
	"}\n";

static const GLchar *GLEX_FS_GBUFFER =
	"#version 330 core\n"
	"void main() {\n"
	"}\n";

static void glexWriteLog(GLEXLogLevel logLevel, const char *fmt, ...)
{
	GLEX_ASSERT(0 <= logLevel && logLevel <= GLEX_LOG_LEVEL_MAX);

	if (!glexState.logEnabled || logLevel > glexState.logLevel)
		return;

	GLchar buf[256];
	GLint prefixLen;
	static const GLchar ind[5] = { 'F', 'E', 'W', 'I', 'D' };
	
	if (glexState.logPrefix != NULL) {
		prefixLen = snprintf(buf, sizeof(buf),
			"[GLEX] %c %p: %s", ind[logLevel], glexCurrent, glexState.logPrefix);
	} else {
		prefixLen = snprintf(buf, sizeof(buf),
			"[GLEX] %c %p: ", ind[logLevel], glexCurrent);
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

	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION,
		type, glexState.logId++, severity, prefixLen + userLen, buf);
}

#define GLEX_LOGF(fmt, ...) glexWriteLog(GLEX_LOG_LEVEL_FATAL, fmt, __VA_ARGS__)
#define GLEX_LOGE(fmt, ...) glexWriteLog(GLEX_LOG_LEVEL_ERROR, fmt, __VA_ARGS__)
#define GLEX_LOGW(fmt, ...) glexWriteLog(GLEX_LOG_LEVEL_WARNING, fmt, __VA_ARGS__)
#define GLEX_LOGI(fmt, ...) glexWriteLog(GLEX_LOG_LEVEL_INFO, fmt, __VA_ARGS__)
#define GLEX_LOGD(fmt, ...) glexWriteLog(GLEX_LOG_LEVEL_DEBUG, fmt, __VA_ARGS__)

static GLboolean glexAllocBufferFromHeap(GLEXBuffer *out, GLEXHeap *heap, GLsizeiptr size)
{
	GLEX_ASSERT(out != NULL);
	GLEX_ASSERT(heap != NULL);
	GLEX_ASSERT(size > 0);

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

	cx_list_node_reset(&out->node);

	if (node == cx_list_knot(&heap->bufferList)) {
		if (used != NULL && size > (GLEX_HEAP_SIZE - offset))
			return GL_FALSE;
		cx_list_append(&heap->bufferList, &out->node);
	} else
		cx_list_insert(&heap->bufferList, &out->node, node);

	out->offset = offset;
	out->size = size;
	out->heap = heap;

	heap->freeSize -= size;

	return GL_TRUE;
}

static GLboolean glexAllocBuffer(GLEXBuffer *out, GLEXHeapType heapType, GLsizeiptr size)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(out != NULL);
	GLEX_ASSERT(0 <= heapType && heapType < GLEX_HEAP_TYPE_MAX);
	GLEX_ASSERT(size > 0);

	GLEXHeap *heap;
	cx_list_t *heapList = &glexCurrent->heapLists[heapType];
	cx_list_node_t *node;

	CX_LIST_FOREACH(node, heapList) {
		heap = CX_MEMBEROF(node, GLEXHeap, node);
		if (glexAllocBufferFromHeap(out, heap, size))
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

	glGenBuffers(1, &heap->id);
	if (heap->id == 0) {
		free(heap);
		return GL_FALSE;
	}

	GLint arrayBufferBackup;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &arrayBufferBackup);
	glBindBuffer(GL_ARRAY_BUFFER, heap->id);
	glBufferData(GL_ARRAY_BUFFER, GLEX_HEAP_SIZE, NULL, usage);
	glBindBuffer(GL_ARRAY_BUFFER, arrayBufferBackup);

	heap->freeSize = GLEX_HEAP_SIZE;

	cx_list_reset(&heap->bufferList);
	cx_list_node_reset(&heap->node);
	cx_list_append(heapList, &heap->node);

	return glexAllocBufferFromHeap(out, heap, size);
}

static void glexFreeBuffer(GLEXBuffer *buf)
{
	GLEX_ASSERT(glexCurrent != NULL);
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
	GLint len = (GLint)strlen(source);
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
		GLEX_LOGE("Failed to create shader.");
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
		GLEX_LOGE("Failed to create program.");
		goto bad1;
	}

	return id;

bad1:
	glDeleteProgram(id);

bad0:
	return 0;
}

static GLboolean glexInitLight(GLEXContext *context, GLEXLight *light, GLEXLightType lightType)
{
	GLEX_ASSERT(context != NULL);
	GLEX_ASSERT(light != NULL);
	GLEX_ASSERT(0 <= lightType && lightType < GLEX_LIGHT_TYPE_MAX);

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

static void glexFreeGBuffer(GLEXGBuffer *gBuffer)
{
	GLEX_ASSERT(gBuffer != NULL);

	if (gBuffer->positionTex > 0) {
		glDeleteTextures(1, &gBuffer->positionTex);
		gBuffer->positionTex = 0;
	}

	if (gBuffer->normalTex > 0) {
		glDeleteTextures(1, &gBuffer->normalTex);
		gBuffer->normalTex = 0;
	}

	if (gBuffer->colorTex > 0) {
		glDeleteTextures(1, &gBuffer->colorTex);
		gBuffer->colorTex = 0;
	}

	if (gBuffer->id > 0) {
		glDeleteFramebuffers(1, &gBuffer->id);
		gBuffer->id = 0;
	}
}

GLEX_API void glexUpdateGBuffer(GLEXGBuffer *gBuffer)
{
	GLEX_ASSERT(gBuffer != NULL);

	if (gBuffer->sizeChanged)
		return;

	glexFreeGBuffer(gBuffer);

	gBuffer->sizeChanged = GL_FALSE;

	if (gBuffer->width < 1 || gBuffer->height < 1)
		return;

	GLuint id;
	GLuint positionTex, normalTex, colorTex;

	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	glGenTextures(1, &positionTex);
	glBindTexture(GL_TEXTURE_2D, positionTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, gBuffer->width, gBuffer->height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, positionTex, 0);

	glGenTextures(1, &normalTex);
	glBindTexture(GL_TEXTURE_2D, normalTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, gBuffer->width, gBuffer->height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTex, 0);

	glGenTextures(1, &colorTex);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gBuffer->width, gBuffer->height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, colorTex, 0);

	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	gBuffer->id = id;
	gBuffer->positionTex = positionTex;
	gBuffer->normalTex = normalTex;
	gBuffer->colorTex = colorTex;
}

static GLboolean glexInitGBuffer(GLEXContext *context, GLint width, GLint height)
{
	GLEX_ASSERT(context != NULL);
	GLEX_ASSERT(context->gBuffer.program == 0);

	context->gBuffer.program = glexCreateProgram(
		GL_VERTEX_SHADER, GLEX_VS_BASIC,
		GL_FRAGMENT_SHADER, GLEX_FS_GBUFFER,
		GL_INVALID_ENUM);

	if (context->gBuffer.program == 0)
		return GL_FALSE;

	return GL_TRUE;
}

static void glexReleaseGBuffer(GLEXContext *context)
{
	GLEX_ASSERT(context != NULL);
	GLEX_ASSERT(context->gBuffer.program > 0);

	glexFreeGBuffer(&context->gBuffer);

	glDeleteProgram(context->gBuffer.program);
	context->gBuffer.program = 0;
}

static void glexRenderShadows(void)
{
	GLEX_ASSERT(glexCurrent != NULL);

	if (!glexCurrent->shadowEnabled)
		return;

	// TODO
}

static void glexRenderDebugTriangles(void)
{
	GLEX_ASSERT(glexCurrent != NULL);

	if (!(glexCurrent->debugDrawMask & GLEX_DEBUG_DRAW_TRIANGLES))
		return;
}

static void glexRenderDebugNormals(void)
{
	GLEX_ASSERT(glexCurrent != NULL);

	if (!(glexCurrent->debugDrawMask & GLEX_DEBUG_DRAW_NORMALS))
		return;
}

static void glexDeferredRender(void)
{
	GLEX_ASSERT(glexCurrent != NULL);

	glexUpdateGBuffer(&glexCurrent->gBuffer);

	if (glexCurrent->gBuffer.width < 1 || glexCurrent->gBuffer.height < 1)
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, glexCurrent->gBuffer.id);
	glUseProgram(glexCurrent->gBuffer.program);

	// TODO draw meshes to gbuffer...
	for (GLint i = 0; i < glexCurrent->frameData.meshCount; ++i) {
	}

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

#if 0
	glUseProgram(glexCurrent->program);
	// TODO deferred shading...
	// TODO render op objects...
	// TODO render debug drawing...
#endif

	glexRenderShadows();
	glexRenderDebugTriangles();
	glexRenderDebugNormals();
}

static void glexSimpleRender(void)
{
	GLEX_ASSERT(glexCurrent != NULL);

	// TODO
}

GLEX_API GLboolean glexInit(void)
{
	glexState.logEnabled = GL_FALSE;
	glexState.logLevel = GLEX_LOG_LEVEL_INFO;
	glexState.logPrefix = NULL;
	glexState.logId = 0;
	glexState.contextCount = 0;

	glexCurrent = NULL;

	return GL_TRUE;
}

GLEX_API void glexShutdown(void)
{
	GLEX_ASSERT(glexState.contextCount == 0);

	if (glexState.logPrefix != NULL) {
		free(glexState.logPrefix);
		glexState.logPrefix = NULL;
	}
}

GLEX_API void glexEnableLog(void)
{
	glexState.logEnabled = GL_TRUE;
}

GLEX_API void glexDisableLog(void)
{
	glexState.logEnabled = GL_FALSE;
}

GLEX_API void glexLogPrefix(const char *prefix)
{
	if (glexState.logPrefix != NULL) {
		free(glexState.logPrefix);
		glexState.logPrefix = NULL;
	}

	if (prefix != NULL) {
		glexState.logPrefix = strdup(prefix);
		if (glexState.logPrefix == NULL)
			GLEX_LOGE("Failed to set log prefix.");
	}
}

GLEX_API void glexLogLevel(GLEXLogLevel logLevel)
{
	GLEX_ASSERT(0 <= logLevel && logLevel < GLEX_LOG_LEVEL_MAX);

	glexState.logLevel = logLevel;
}

GLEX_API GLEXContext *glexCreateContext(void *userData)
{
	GLEXContext *context = malloc(sizeof(GLEXContext));
	if (context == NULL)
		goto bad0;

	memset(context, 0, sizeof(GLEXContext));

	context->lightingEnabled = GL_TRUE;
	context->shadowEnabled = GL_TRUE;
	context->debugDrawMask = 0;

	for (GLint i = 0; i < GLEX_HEAP_TYPE_MAX; ++i)
		cx_list_reset(&context->heapLists[i]);

	context->transformStack[0].translate = HMM_Vec3(0.0f, 0.0f, 0.0f);
	context->transformStack[0].scale = HMM_Vec3(1.0f, 1.0f, 1.0f);
	context->transformStack[0].rotation = HMM_QuaternionFromAxisAngle(glexAxis[0], 0.0f);
	context->transformTop = 0;

	context->viewPosition = HMM_Vec3(0.0f, 0.0f, 0.0f);
	context->viewCenter = HMM_Vec3(0.0f, 0.0f, -1.0f);
	context->viewUp = HMM_Vec3(0.0f, 1.0f, 0.0f);

	context->viewFov = 90.0f;
	context->viewRatio = 4.0f / 3.0f;
	context->viewRange[0] = 0.1f;
	context->viewRange[1] = 100.0f;

	if (!glexInitGBuffer(context, 0, 0))
		goto bad1;

	context->currentLight = NULL;

	for (GLint i = 0; i < GLEX_LIGHT_TYPE_MAX; ++i)
		cx_list_reset(&context->lights[i]);

	context->currentMaterial = NULL;

	if (!glexInitBuiltinMaterial(context))
		goto bad2;

	context->currentMesh = NULL;
	cx_list_reset(&context->meshes);

	context->framing = GL_FALSE;
	context->frameData.lightCount = 0;
	context->frameData.meshCount = 0;

	context->userData = userData;

	glexState.contextCount += 1;

	return context;

bad2:
	glexReleaseGBuffer(context);

bad1:
	free(context);

bad0:
	return NULL;
}

GLEX_API void glexDeleteContext(GLEXContext *context)
{
	GLEX_ASSERT(context != NULL);
	GLEX_ASSERT(glexState.contextCount > 0);

	if (glexCurrent == context)
		glexCurrent = NULL;

	glexReleaseGBuffer(context);
	free(context);

	glexState.contextCount -= 1;
}

GLEX_API GLEXContext *glexCurrentContext(void)
{
	return glexCurrent;
}

GLEX_API void glexMakeCurrent(GLEXContext *context)
{
	glexCurrent = context;
}

GLEX_API void *glexGetUserData(void)
{
	GLEX_ASSERT(glexCurrent != NULL);

	return glexCurrent->userData;
}

GLEX_API void glexEnableLighting(void)
{
	GLEX_ASSERT(glexCurrent != NULL);

	glexCurrent->lightingEnabled = GL_TRUE;
}

GLEX_API void glexDisableLighting(void)
{
	GLEX_ASSERT(glexCurrent != NULL);

	glexCurrent->lightingEnabled = GL_FALSE;
}

GLEX_API void glexEnableShadow(void)
{
	GLEX_ASSERT(glexCurrent != NULL);

	glexCurrent->shadowEnabled = GL_TRUE;
}

GLEX_API void glexDisableShadow(void)
{
	GLEX_ASSERT(glexCurrent != NULL);

	glexCurrent->shadowEnabled = GL_FALSE;
}

GLEX_API void glexDebugDrawMask(GLuint mask)
{
	GLEX_ASSERT(glexCurrent != NULL);

	glexCurrent->debugDrawMask = mask;
}

GLEX_API void glexBeginFrame(GLint width, GLint height)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(!glexCurrent->framing);

	glexCurrent->framing = GL_TRUE;
	glexCurrent->frameData.lightCount = 0;
	glexCurrent->frameData.meshCount = 0;

	if (glexCurrent->gBuffer.width != width || glexCurrent->gBuffer.height != height) {
		glexCurrent->gBuffer.width = width;
		glexCurrent->gBuffer.height = height;
		glexCurrent->gBuffer.sizeChanged = GL_TRUE;
	}
}

GLEX_API void glexEndFrame(void)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(glexCurrent->framing);

	glexCurrent->framing = GL_FALSE;

	if (glexCurrent->frameData.meshCount < 1)
		return;

	// TODO sort frame meshes by material and heap?

	glexCurrent->frameData.viewMatrix =
		HMM_LookAt(glexCurrent->viewPosition, glexCurrent->viewCenter, glexCurrent->viewUp);

	glexCurrent->frameData.projectionMatrix =
		HMM_Perspective(glexCurrent->viewFov, glexCurrent->viewRatio, glexCurrent->viewRange[0], glexCurrent->viewRange[1]);

	// hmm_mat4 normalMatrix;

	if (glexCurrent->lightingEnabled)
		glexDeferredRender();
	else
		glexSimpleRender();
}

GLEX_API void glexResetTransform(void)
{
	GLEX_ASSERT(glexCurrent != NULL);

	GLEXTransform *transform = &glexCurrent->transformStack[glexCurrent->transformTop];

	transform->translate = HMM_Vec3(0.0f, 0.0f, 0.0f);
	transform->scale = HMM_Vec3(1.0f, 1.0f, 1.0f);
	transform->rotation = HMM_QuaternionFromAxisAngle(glexAxis[0], 0.0f);
}

GLEX_API void glexPushTransform(void)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(glexCurrent->transformTop < (GLEX_TRANSFORM_STACK_DEPTH - 1));

	GLint oldTop = glexCurrent->transformTop++;
	glexCurrent->transformStack[glexCurrent->transformTop] = glexCurrent->transformStack[oldTop];
}

GLEX_API void glexPopTransform(void)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(glexCurrent->transformTop > 0);

	glexCurrent->transformTop -= 1;
}

GLEX_API void glexTranslate(GLfloat x, GLfloat y, GLfloat z)
{
	GLEX_ASSERT(glexCurrent != NULL);

	glexCurrent->transformStack[glexCurrent->transformTop].translate =
		HMM_AddVec3(glexCurrent->transformStack[glexCurrent->transformTop].translate, HMM_Vec3(x, y, z));
}

GLEX_API void glexScale(GLfloat x, GLfloat y, GLfloat z)
{
	GLEX_ASSERT(glexCurrent != NULL);

	glexCurrent->transformStack[glexCurrent->transformTop].scale =
		HMM_MultiplyVec3(glexCurrent->transformStack[glexCurrent->transformTop].scale, HMM_Vec3(x, y, z));
}

GLEX_API void glexRotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	GLEX_ASSERT(glexCurrent != NULL);

	glexCurrent->transformStack[glexCurrent->transformTop].rotation =
		HMM_MultiplyQuaternion(glexCurrent->transformStack[glexCurrent->transformTop].rotation,
			HMM_QuaternionFromAxisAngle(HMM_Vec3(x, y, z), angle));
}

GLEX_API void glexLookAt(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ,
	GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat upX, GLfloat upY, GLfloat upZ)
{
	GLEX_ASSERT(glexCurrent != NULL);

	glexCurrent->viewPosition.X = eyeX;
	glexCurrent->viewPosition.Y = eyeY;
	glexCurrent->viewPosition.Z = eyeZ;

	glexCurrent->viewCenter.X = centerX;
	glexCurrent->viewCenter.Y = centerY;
	glexCurrent->viewCenter.Z = centerZ;

	glexCurrent->viewUp = HMM_NormalizeVec3(HMM_Vec3(upX, upY, upZ));
}

GLEX_API void glexPerpective(GLfloat fov, GLfloat ratio, GLfloat zNear, GLfloat zFar)
{
	GLEX_ASSERT(glexCurrent != NULL);

	glexCurrent->viewFov = fov;
	glexCurrent->viewRatio = ratio;
	glexCurrent->viewRange[0] = zNear;
	glexCurrent->viewRange[1] = zFar;
}

GLEX_API void glexAmbientLight(float strength, GLfloat red, GLfloat green, GLfloat blue)
{
	GLEX_ASSERT(glexCurrent != NULL);

	glexCurrent->ambientLight.strength = HMM_Clamp(0.0f, strength, 1.0f);
	glexCurrent->ambientLight.color.R = HMM_Clamp(0.0f, red, 1.0f);
	glexCurrent->ambientLight.color.G = HMM_Clamp(0.0f, green, 1.0f);
	glexCurrent->ambientLight.color.B = HMM_Clamp(0.0f, blue, 1.0f);
}

GLEX_API GLEXLight *glexCreateLight(GLEXLightType lightType)
{
	GLEX_ASSERT(glexCurrent != NULL);

	GLEXLight *light = malloc(sizeof(GLEXLight));
	if (light == NULL)
		return NULL;

	if (!glexInitLight(glexCurrent, light, lightType)) {
		free(light);
		return NULL;
	}

	return light;
}

GLEX_API void glexDeleteLight(GLEXLight *light)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(light != NULL);

	if (glexCurrent->currentLight == light)
		glexCurrent->currentLight = NULL;

	cx_list_node_unlink(&light->node);

	free(light);
}

GLEX_API void glexBindLight(GLEXLight *light)
{
	GLEX_ASSERT(glexCurrent != NULL);

	glexCurrent->currentLight = light;
}

GLEX_API void glexAddLight(void)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(glexCurrent->framing);
	GLEX_ASSERT(glexCurrent->currentLight != NULL);

	if (glexCurrent->frameData.lightCount < GLEX_FRAME_LIGHT_MAX) {
		GLEXFrameLight *frameLight = &glexCurrent->frameData.lightArray[glexCurrent->frameData.lightCount++];
		frameLight->light = glexCurrent->currentLight;
		frameLight->transform = glexCurrent->transformStack[glexCurrent->transformTop];
	}
}

GLEX_API void glexLightStrength(GLfloat strength)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(glexCurrent->currentLight != NULL);
	GLEX_ASSERT(strength >= 0.0f);

	glexCurrent->currentLight->strength = strength;
}

GLEX_API void glexLightColor(GLfloat red, GLfloat green, GLfloat blue)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(glexCurrent->currentLight != NULL);

	glexCurrent->currentLight->color = HMM_Vec3(red, green, blue);
}

GLEX_API void glexLightDirection(GLfloat x, GLfloat y, GLfloat z)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(glexCurrent->currentLight != NULL);
	GLEX_ASSERT(glexCurrent->currentLight->type == GLEX_LIGHT_TYPE_DIRECTIONAL || glexCurrent->currentLight->type == GLEX_LIGHT_TYPE_SPOT);

	glexCurrent->currentLight->direction = HMM_NormalizeVec3(HMM_Vec3(x, y, z));
}

GLEX_API void glexLightSpotShape(GLEXLightSpotShape lightSpotShape)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(glexCurrent->currentLight->type == GLEX_LIGHT_TYPE_SPOT);

	// TODO
}

GLEX_API void glexLightRange(GLfloat range)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(glexCurrent->currentLight != NULL);
	GLEX_ASSERT(glexCurrent->currentLight->type == GLEX_LIGHT_TYPE_POINT || glexCurrent->currentLight->type == GLEX_LIGHT_TYPE_SPOT);
	GLEX_ASSERT(range > 0.0f);

	glexCurrent->currentLight->range = range;
}

GLEX_API GLEXMaterial *glexCreateMaterial(void)
{
	GLEX_ASSERT(glexCurrent != NULL);

	GLEXMaterial *material = malloc(sizeof(GLEXMaterial));
	if (material == NULL)
		return NULL;

	if (!glexInitMaterial(glexCurrent, material)) {
		free(material);
		return NULL;
	}

	return material;
}

GLEX_API GLEXMaterial *glexGetBuiltinMaterial(GLEXBuiltinMaterial builtinMaterial)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(0 <= builtinMaterial && builtinMaterial < GLEX_BUILTIN_MATERIAL_MAX);

	return &glexCurrent->builtinMaterials[builtinMaterial];
}

GLEX_API void glexDeleteMaterial(GLEXMaterial *material)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(material != NULL);

	// TODO

	free(material);
}

GLEX_API void glexBindMaterial(GLEXMaterial *material)
{
	GLEX_ASSERT(glexCurrent != NULL);

	glexCurrent->currentMaterial = material;
}

GLEX_API GLEXMesh *glexCreateMesh(GLEXHeapType heapType, GLsizeiptr vertexCount, GLsizeiptr vertexIndexCount)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(0 <= heapType && heapType < GLEX_HEAP_TYPE_MAX);
	GLEX_ASSERT(vertexCount > 0);
	GLEX_ASSERT(vertexIndexCount > 0);

	GLEXMesh *mesh = malloc(sizeof(GLEXMesh));
	if (mesh == NULL)
		return NULL;

	if (!glexInitMesh(glexCurrent, mesh, heapType, vertexCount, vertexIndexCount)) {
		free(mesh);
		return NULL;
	}

	return mesh;
}

GLEX_API void glexDeleteMesh(GLEXMesh *mesh)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(mesh != NULL);
	GLEX_ASSERT(mesh->vertexMap == NULL);
	GLEX_ASSERT(mesh->vertexIndexMap == NULL);

	if (glexCurrent->currentMesh == mesh)
		glexCurrent->currentMesh = NULL;

	glexFreeBuffer(&mesh->vertexBuffer);
	glexFreeBuffer(&mesh->vertexIndexBuffer);

	cx_list_node_unlink(&mesh->node);

	free(mesh);
}

GLEX_API void glexBindMesh(GLEXMesh *mesh)
{
	GLEX_ASSERT(glexCurrent != NULL);

	glexCurrent->currentMesh = mesh;

	if (mesh != NULL) {
		GLEX_ASSERT(mesh->vertexBuffer.heap != NULL);
		GLEX_ASSERT(mesh->vertexIndexBuffer.heap != NULL);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer.heap->id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vertexIndexBuffer.heap->id);
	} else {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

GLEX_API void glexAddMesh(void)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(glexCurrent->framing);
	GLEX_ASSERT(glexCurrent->currentMesh != NULL);

	if (glexCurrent->frameData.meshCount < GLEX_FRAME_MESH_MAX) {
		GLEXFrameMesh *frameMesh = &glexCurrent->frameData.meshArray[glexCurrent->frameData.meshCount++];
		frameMesh->mesh = glexCurrent->currentMesh;
		frameMesh->material = glexCurrent->currentMaterial;
		frameMesh->transform = glexCurrent->transformStack[glexCurrent->transformTop];
	}
}

GLEX_API void glexMeshVertexData(const GLEXVertex *p, GLsizeiptr count, GLintptr offset)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(p != NULL);
	GLEX_ASSERT(count > 0);
	GLEX_ASSERT(offset >= 0);

	GLEXMesh *mesh = glexCurrent->currentMesh;
	GLEX_ASSERT(mesh != NULL);

#ifdef GLEX_DEBUG
	GLint id;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &id);
	GLEX_ASSERT(id == mesh->vertexBuffer.heap->id);
#endif

	GLsizeiptr size = sizeof(GLEXVertex) * count;
	GLsizeiptr headSize = mesh->vertexBuffer.offset + sizeof(GLEXVertex) * offset;
	GLEX_ASSERT((mesh->vertexBuffer.size - headSize) >= size);

	glBufferSubData(GL_ARRAY_BUFFER, mesh->vertexBuffer.offset + headSize, size, p);
}

GLEX_API void glexMeshVertexIndexData(const GLEXVertexIndex *p, GLsizeiptr count, GLintptr offset)
{
	GLEX_ASSERT(glexCurrent != NULL);
	GLEX_ASSERT(p != NULL);
	GLEX_ASSERT(count > 0);
	GLEX_ASSERT(offset >= 0);

	GLEXMesh *mesh = glexCurrent->currentMesh;
	GLEX_ASSERT(mesh != NULL);

#ifdef GLEX_DEBUG
	GLint id;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &id);
	GLEX_ASSERT(id == mesh->vertexIndexBuffer.heap->id);
#endif

	GLsizeiptr size = sizeof(GLEXVertexIndex) * count;
	GLsizeiptr headSize = mesh->vertexIndexBuffer.offset + sizeof(GLEXVertexIndex) * offset;
	GLEX_ASSERT((mesh->vertexIndexBuffer.size - headSize) >= size);

	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, headSize, size, p);
}

GLEX_API GLEXVertex *glexMapMeshVertexBuffer(GLintptr offset, GLsizeiptr count)
{
	GLEX_ASSERT(glexCurrent != NULL);

	GLEXMesh *mesh = glexCurrent->currentMesh;
	GLEX_ASSERT(mesh != NULL);
	GLEX_ASSERT(mesh->vertexBuffer.heap != NULL);
	GLEX_ASSERT(mesh->vertexMap == NULL);

#ifdef GLEX_DEBUG
	GLint id;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &id);
	GLEX_ASSERT(id == mesh->vertexBuffer.heap->id);
#endif

	GLsizeiptr size = sizeof(GLEXVertex) * count;
	mesh->vertexIndexMap = glMapBufferRange(GL_ARRAY_BUFFER, offset, size, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

	return mesh->vertexMap;
}

GLEX_API void glexUnmapMeshVertexBuffer(void)
{
	GLEX_ASSERT(glexCurrent != NULL);

	GLEXMesh *mesh = glexCurrent->currentMesh;
	GLEX_ASSERT(mesh != NULL);
	GLEX_ASSERT(mesh->vertexBuffer.heap != NULL);
	GLEX_ASSERT(mesh->vertexMap != NULL);

#ifdef GLEX_DEBUG
	GLint id;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &id);
	GLEX_ASSERT(id == mesh->vertexBuffer.heap->id);
#endif

	glUnmapBuffer(GL_ARRAY_BUFFER);
	mesh->vertexMap = NULL;
}

GLEX_API GLEXVertexIndex *glexMapMeshVertexIndexBuffer(GLintptr offset, GLsizeiptr count)
{
	GLEX_ASSERT(glexCurrent != NULL);

	GLEXMesh *mesh = glexCurrent->currentMesh;
	GLEX_ASSERT(mesh != NULL);
	GLEX_ASSERT(mesh->vertexIndexBuffer.heap != NULL);
	GLEX_ASSERT(mesh->vertexIndexMap == NULL);

#ifdef GLEX_DEBUG
	GLint id;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &id);
	GLEX_ASSERT(id == mesh->vertexIndexBuffer.heap->id);
#endif

	GLsizeiptr size = sizeof(GLEXVertex) * count;
	mesh->vertexIndexMap = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, offset, size, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

	return mesh->vertexIndexMap;
}

GLEX_API void glexUnmapMeshVertexIndexBuffer(void)
{
	GLEX_ASSERT(glexCurrent != NULL);

	GLEXMesh *mesh = glexCurrent->currentMesh;
	GLEX_ASSERT(mesh != NULL);
	GLEX_ASSERT(mesh->vertexIndexBuffer.heap != NULL);
	GLEX_ASSERT(mesh->vertexIndexMap != NULL);

#ifdef GLEX_DEBUG
	GLint id;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &id);
	GLEX_ASSERT(id == mesh->vertexIndexBuffer.heap->id);
#endif

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	mesh->vertexIndexMap = NULL;
}
