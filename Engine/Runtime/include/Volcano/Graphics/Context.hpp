//
//
#ifndef VOLCANO_GRAPHICS_CONTEXT_HPP
#define VOLCANO_GRAPHICS_CONTEXT_HPP

#include <QList>
#include <QHash>
#include <QCache>
#include <QString>
#include <QUrl>
#include <QMutex>
#include <QSharedPointer>
#include <QObject>
#include <QOpenGLContext>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Buffer.hpp>
#include <Volcano/Graphics/BufferHeap.hpp>
#include <Volcano/Graphics/Mesh.hpp>

VOLCANO_GRAPHICS_BEGIN

using MeshPtr = QSharedPointer<Mesh>;

class Context: public QObject {
    Q_OBJECT

public:
    Context(QOpenGLContext *glContext, QObject *parent = nullptr);
    virtual ~Context(void);

public:
    static Context *current(void);
    QOpenGLContext *glContext(void);
    QOpenGLDebugLogger *glDebugLogger(void);
    OpenGLFunctions *glFunctions(void);
    Buffer *allocStaticVertexBuffer(GLsizeiptr count);
    Buffer *allocStaticVertexIndexBuffer(GLsizeiptr count);
    Buffer *allocDynamicVertexBuffer(GLsizeiptr count);
    Buffer *allocDynamicVertexIndexBuffer(GLsizeiptr count);
    // OpenGLTexture *allocTexture(GLint width, GLint height, GLenum format);
    QOpenGLShaderProgram *shaderProgram(const QUrl &url);
    MeshPtr getMesh(const QUrl &url);

private slots:
    void onDebugMessageLogged(const QOpenGLDebugMessage &msg);

private:
    using BufferHeapList = QList<BufferHeap *>;
    using ContextHashTable = QHash<QOpenGLContext *, Context *>;
    using ShaderProgramCache = QCache<QUrl, QOpenGLShaderProgram>;

private:
    bool init(void);
    Buffer *allocBuffer(BufferHeapList &heapList, GLsizeiptr size, QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage);

private:
    static ContextHashTable c_contextHashTable;
    static QMutex c_contextHashTableMutex;
    QOpenGLContext *m_glContext;
    QOpenGLDebugLogger *m_glDebugLogger;
    OpenGLFunctions *m_glFunctions;
    BufferHeapList m_staticVertexBufferHeapList;
    BufferHeapList m_staticVertexIndexBufferHeapList;
    BufferHeapList m_dynamicVertexBufferHeapList;
    BufferHeapList m_dynamicVertexIndexBufferHeapList;
    ShaderProgramCache m_shaderProgramCache;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CONTEXT_HPP

