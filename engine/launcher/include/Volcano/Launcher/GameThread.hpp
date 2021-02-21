//
//
#ifndef VOLCANO_LAUNCHER_GAMETHREAD_HPP
#define VOLCANO_LAUNCHER_GAMETHREAD_HPP

#include <QString>
#include <QThread>

#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

class GameThread: public QThread {
    Q_OBJECT

public:
    GameThread(const QString &rootDirPath, QObject *parent = nullptr);
    ~GameThread(void) override;

public:
    const QString &rootDirPath(void) const;

protected:
    void run(void) override;

private:
    QString m_rootDirPath;

};

VOLCANO_INLINE const QString &GameThread::rootDirPath(void) const
{
    return m_rootDirPath;
}

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_GAMETHREAD_HPP
