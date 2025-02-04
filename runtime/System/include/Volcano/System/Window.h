//
//
#ifndef VOLCANO_SYSTEM_WINDOW_H
#define VOLCANO_SYSTEM_WINDOW_H

#include <QOpenGLWindow>

#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Window: public QOpenGLWindow {
    Q_OBJECT

public:
    Window();

private:
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_WINDOW_H
