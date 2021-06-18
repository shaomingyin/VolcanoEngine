//
//
#ifndef VOLCANO_OPENGLCOMMON_HPP
#define VOLCANO_OPENGLCOMMON_HPP

#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

void glMakeSurfaceFormat(QSurfaceFormat &out);

using OpenGLFunctions = QOpenGLFunctions_3_3_Core;

inline OpenGLFunctions *glFunctions(QOpenGLContext *context = nullptr)
{
    return QOpenGLVersionFunctionsFactory::get<OpenGLFunctions>(context);
}

VOLCANO_END

#endif // VOLCANO_OPENGLCOMMON_HPP
