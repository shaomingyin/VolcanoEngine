//
//
#include <QObject>

#include <Volcano/VM.hpp>

VOLCANO_BEGIN

VM::VM(QObject *parent):
    QThread(parent),
    m_fps(60)
{
}

VM::~VM(void)
{
}

void VM::setFps(int v)
{
    int tmp = (v < 1 ? 1 : v);
    if (m_fps == tmp)
        return;

    // TODO notify vm thread to restart the frame timer.

    m_fps = tmp;
    fpsChanged(tmp);
}

// In VM thread.

void VM::run(void)
{
    if (!init())
        return;

    // TODO main loop.
    for (;;)
    {
        frame(1000 / m_fps);
        msleep(1000 / m_fps);
    }

    shutdown();
}

bool VM::init(void)
{
    return true;
}

void VM::shutdown(void)
{
}

void VM::frame(float elapsed)
{
    // update events
    // update modules
    // update scene
    // update gui

    // build data for drivers.
    // update driver.
}

VOLCANO_END
