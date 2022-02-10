//
//
#ifndef VOLCANO_SERVICE_HPP
#define VOLCANO_SERVICE_HPP

#include <QStack>
#include <QVector3D>
#include <QString>
#include <QQuaternion>
#include <QObject>

#include <Volcano/Common.hpp>
#include <Volcano/Transform.hpp>

VOLCANO_BEGIN

class Service: public QObject {
    Q_OBJECT

public:
    Service(QObject *parent = nullptr);
    ~Service(void) override;

public:
    void pushTransform(bool copyTop = false);
    void popTransform(void);
    void resetTransform(void);
    const Transform::Data &transform(void) const;
    void setTransform(const Transform::Data &v);

private:
    using TransformStack = QStack<Transform::Data>;

private:
    TransformStack m_transformStack;
};

VOLCANO_END

#endif // VOLCANO_SERVICE_HPP
