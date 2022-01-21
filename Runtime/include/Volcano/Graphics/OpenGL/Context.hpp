//
//
#ifndef VOLCANO_GRAPHICS_OPENGL_CONTEXT_HPP
#define VOLCANO_GRAPHICS_OPENGL_CONTEXT_HPP

#include <functional>

#include <QList>
#include <QHash>
#include <QCache>
#include <QString>
#include <QUrl>
#include <QMutex>
#include <QFuture>
#include <QSharedPointer>
#include <QObject>
#include <QOpenGLContext>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>

#include <Volcano/Graphics/OpenGL/Common.hpp>
#include <Volcano/Graphics/OpenGL/Buffer.hpp>
#include <Volcano/Graphics/OpenGL/BufferHeap.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

class Context: public QObject {
    Q_OBJECT

public:
    using Job = std::function<void (Context *context)>;

private:
    Context(QOpenGLContext *glContext, QObject *parent = nullptr);
    virtual ~Context(void);

public:
    static Context *current(void);
    QOpenGLContext *context(void);
    QOpenGLDebugLogger *debugLogger(void);
    Functions *functions(void);
    Buffer *allocStaticVertexBuffer(GLsizeiptr count);
    Buffer *allocStaticVertexIndexBuffer(GLsizeiptr count);
    Buffer *allocDynamicVertexBuffer(GLsizeiptr count);
    Buffer *allocDynamicVertexIndexBuffer(GLsizeiptr count);
    // OpenGLTexture *allocTexture(GLint width, GLint height, GLenum format);
    QOpenGLShaderProgram *shaderProgram(const QUrl &url);
    static void scheduleJob(Job job);

protected:
    void customEvent(QEvent *event) override;

private slots:
    void onDebugMessageLogged(const QOpenGLDebugMessage &msg);

private:
    using JobList = QList<Job>;
    using ContextHashTable = QHash<QOpenGLContext *, Context *>;
    using BufferHeapList = QList<BufferHeap *>;
    using ShaderProgramCache = QCache<QUrl, QOpenGLShaderProgram>;

private:
    bool init(void);
    Buffer *allocBuffer(BufferHeapList &heapList, GLsizeiptr size, QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage);
    void onLoadMesh(QUrl url);

private:
    static ContextHashTable c_contextHashTable;
    static QMutex c_contextHashTableMutex;
    JobList m_jobList;
    QOpenGLContext *m_context;
    QSurface *m_surface;
    QOpenGLDebugLogger *m_debugLogger;
    Functions *m_functions;
    BufferHeapList m_staticVertexBufferHeapList;
    BufferHeapList m_staticVertexIndexBufferHeapList;
    BufferHeapList m_dynamicVertexBufferHeapList;
    BufferHeapList m_dynamicVertexIndexBufferHeapList;
    ShaderProgramCache m_shaderProgramCache;
};

VOLCANO_GRAPHICS_OPENGL_END

#endif // VOLCANO_GRAPHICS_OPENGL_CONTEXT_HPP

