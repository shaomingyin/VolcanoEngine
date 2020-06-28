//
//
#ifndef VOLCANO_RENDERER_CONTEXT_H
#define VOLCANO_RENDERER_CONTEXT_H

#include <QObject>
#include <QSharedPointer>
#include <QVector>
#include <QRect>
#include <QMatrix4x4>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>

#include <Volcano/Renderer/Common.h>
#include <Volcano/Renderer/View.h>

VOLCANO_RENDERER_BEGIN

typedef QOpenGLFunctions_3_3_Core OpenGLFunctions;
typedef QSharedPointer<QOpenGLShaderProgram> OpenGLShaderProgramPtr;
typedef QVector<OpenGLShaderProgramPtr> OpenGLShaderProgramArray;

class VOLCANO_API Context: public QObject
{
    Q_OBJECT

public:
    Context(QObject *parent = nullptr);
    ~Context(void) override;

public:
    bool init(void);
    void makeCurrent(void);
    static Context *current(void);
    void render(View &view);

private:
    OpenGLFunctions *m_gl;
    OpenGLShaderProgramArray m_progs;
    GLint m_viewportLoc;
    GLint m_viewMatrixLoc;
    GLint m_projectMatrixLoc;
    static Context *c_current;
};

VOLCANO_INLINE void Context::makeCurrent(void)
{
    c_current = this;
}

VOLCANO_INLINE Context *Context::current(void)
{
    return c_current;
}

VOLCANO_RENDERER_END

#endif // VOLCANO_RENDERER_CONTEXT_H
