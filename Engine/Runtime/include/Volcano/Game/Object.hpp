//
//
#ifndef VOLCANO_GAME_OBJECT_HPP
#define VOLCANO_GAME_OBJECT_HPP

#include <QObject>
#include <QVector3D>
#include <QQuaternion>
#include <QQmlListProperty>

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Object: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

public:
    Object(QObject *parent = nullptr);
    ~Object(void) override;

public:
    virtual void tick(Duration elapsed);
    bool isEnabled(void) const;
    void setEnabled(bool v);

signals:
    void enabledChanged(bool v);

private:
    bool m_isEnabled;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_OBJECT_HPP
