//
//
#ifndef VOLCANO_OBJECT_HPP
#define VOLCANO_OBJECT_HPP

#include <QObject>
#include <QVector3D>
#include <QQuaternion>
#include <QQmlListProperty>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class Object: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

public:
    Object(QObject *parent = nullptr);
    ~Object(void) override;

public:
    bool isEnabled(void) const;
    void setEnabled(bool v);

signals:
    void enabledChanged(bool v);

private:
    bool m_isEnabled;
};

VOLCANO_END

#endif // VOLCANO_OBJECT_HPP
