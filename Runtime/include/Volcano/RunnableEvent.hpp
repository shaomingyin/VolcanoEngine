//
//
#ifndef VOLCANO_RUNNABLEVENT_HPP
#define VOLCANO_RUNNABLEVENT_HPP

#include <QEvent>
#include <QRunnable>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class RunnableEvent: public QEvent {

public:
    RunnableEvent(QRunnable *p);
    ~RunnableEvent(void) override;

public:
    void run(void);
    static Type typeId(void);

private:
    QRunnable *m_runnable;
};

VOLCANO_END

#endif // VOLCANO_RUNNABLEVENT_HPP
