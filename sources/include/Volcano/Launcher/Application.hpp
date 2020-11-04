//
//
#ifndef VOLCANO_LAUNCHER_APPLICATION_HPP
#define VOLCANO_LAUNCHER_APPLICATION_HPP

#include <QStringList>
#include <QGuiApplication>

#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

class Application: public QGuiApplication
{
    Q_OBJECT

public:
    Application(const QStringList &args);
    ~Application(void) override;

private:
    QStringList m_args;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_APPLICATION_HPP
