#ifndef VOLCANO_WORLD_SKYBOXENVIRONMENT_H
#define VOLCANO_WORLD_SKYBOXENVIRONMENT_H

#include <QUrl>

#include <Volcano/World/Environment.h>

VOLCANO_WORLD_BEGIN

class SkyboxEnvironment: public Environment {
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)

public:
    explicit SkyboxEnvironment(QObject* parent = nullptr);
    const QUrl& source() const noexcept;
    void setSource(const QUrl& source);

signals:
    void sourceChanged(const QUrl& source);

private:
    QUrl source_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SKYBOXENVIRONMENT_H
