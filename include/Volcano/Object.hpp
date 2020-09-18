//
//
#ifndef VOLCANO_OBJECT_HPP
#define VOLCANO_OBJECT_HPP

#include <QObject>
#include <QThread>
#include <QFuture>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class VOLCANO_API Object: public QObject
{
    Q_OBJECT

public:
    Object(QObject *parent = nullptr);
    ~Object(void) override;

public:
    // postRenderTask(F fn);

private:
    QThread *m_rendererThread;
};

VOLCANO_END

#endif // VOLCANO_OBJECT_HPP
