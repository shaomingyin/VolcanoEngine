//
//
#ifndef VOLCANO_SYSTEM_STANDALONE_HPP
#define VOLCANO_SYSTEM_STANDALONE_HPP

#include <QTimerEvent>
#include <QElapsedTimer>

#include <Volcano/System/Common.hpp>
#include <Volcano/System/Engine.hpp>
#include <Volcano/System/Server.hpp>
#include <Volcano/System/Client.hpp>

VOLCANO_SYSTEM_BEGIN

class Standalone: public Engine {
    Q_OBJECT

public:
    Standalone(QObject* parent = nullptr);

public:
    bool init() override;

protected:
    void timerEvent(QTimerEvent* e) override;

private:
    void frame();

private:
    int frameTimer_;
    QElapsedTimer elapsedTimier_;
    Server server_;
    Client client_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_STANDALONE_HPP
