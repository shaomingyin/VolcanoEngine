#include <exception>
#include <stdexcept>

#include <QOpenGLContext>
#include <QString>
#include <QSurfaceFormat>

#include <Volcano/Editor/SceneEditorViewport.h>

VOLCANO_EDITOR_BEGIN

SceneEditorViewport::SceneEditorViewport(QWidget* parent)
    : QOpenGLWidget(parent) {
    setWindowTitle("Scene");
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    setFormat(format);
}

SceneEditorViewport::~SceneEditorViewport() {
    if (context() != nullptr) {
        disconnect(context(), nullptr, this, nullptr);
    }
    releaseGraphics();
}

void SceneEditorViewport::initializeGL() {
    try {
        if (gl3wInit(&graphics_procs_, [](const char* name) -> GL3WglProc {
            return QOpenGLContext::currentContext()->getProcAddress(name);
        }) != GL3W_OK) {
            throw std::runtime_error("Cannot load OpenGL functions from the Qt context");
        }
        gl3wProcs = &graphics_procs_;
        connect(context(), &QOpenGLContext::aboutToBeDestroyed,
            this, &SceneEditorViewport::releaseGraphics, Qt::DirectConnection);
        graphics_renderer_ = std::make_unique<Graphics::Renderer>();
    } catch (const std::exception& error) {
        qWarning("Cannot initialize Volcano renderer: %s", error.what());
        setToolTip(QString::fromUtf8(error.what()));
    }
}

void SceneEditorViewport::releaseGraphics() {
    if (graphics_renderer_) {
        makeCurrent();
        gl3wProcs = &graphics_procs_;
        graphics_renderer_.reset();
        doneCurrent();
    }
    if (gl3wProcs == &graphics_procs_) {
        gl3wProcs = nullptr;
    }
}

void SceneEditorViewport::paintGL() {
    gl3wProcs = &graphics_procs_;
    // shortcut: scene submission remains unimplemented; connect World data when adding editing support.
}

VOLCANO_EDITOR_END
