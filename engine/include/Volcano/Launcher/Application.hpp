//
//
#ifndef VOLCANO_LAUNCHER_APPLICATION_HPP
#define VOLCANO_LAUNCHER_APPLICATION_HPP

#include <QTimerEvent>
#include <QQuickView>
#include <QGuiApplication>

#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

class Application: public QGuiApplication {
    Q_OBJECT

public:
    Application(int &argc, char *argv[]);
    ~Application(void) override;

public:
    bool init(void);

protected:
    void timerEvent(QTimerEvent *event) override;

private slots:
    void onWindowStatusChanged(QQuickView::Status st);

private:
    void setup(void);

private:
    QQuickView m_window;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_APPLICATION_HPP
