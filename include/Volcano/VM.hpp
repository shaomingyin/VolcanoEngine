//
//
#ifndef VOLCANO_VM_HPP
#define VOLCANO_VM_HPP

#include <QThread>

#include <Volcano/Common.hpp>
#include <Volcano/Gui.hpp>
#include <Volcano/Scene.hpp>
#include <Volcano/Module.hpp>

VOLCANO_BEGIN

class VOLCANO_API VM: public QThread
{
    Q_OBJECT
    Q_PROPERTY(int fps READ fps WRITE setFps NOTIFY fpsChanged)

public:
    VM(QObject *parent = nullptr);
    ~VM(void) override;

public:
    int fps(void) const;
    void setFps(int v);

signals:
    void fpsChanged(int v);

protected: // methods that run in vm thread.
    void run(void) override;
    virtual bool init(void);
    virtual void shutdown(void);
    virtual void frame(float elapsed);

private:
    int m_fps;
    // Resouce Manager;
    // Driver Manager;
    // Gui Context;
    // Scene Context;
    // Module Manager;
};

VOLCANO_INLINE int VM::fps(void) const
{
    return m_fps;
}

VOLCANO_END

#endif // VOLCANO_VM_HPP
