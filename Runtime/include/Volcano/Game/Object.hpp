//
//
#ifndef VOLCANO_GAME_OBJECT_HPP
#define VOLCANO_GAME_OBJECT_HPP

#include <QVector3D>
#include <QQuaternion>

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Object: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enableChanged)

public:
    Object(QObject* parent = nullptr);
    ~Object(void) override = default;

public:
    bool isEnabled(void) const;
    void setEnabled(bool v);

signals:
    void enableChanged(bool v);

private:
    bool isEnabled_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_OBJECT_HPP
