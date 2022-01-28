//
//
#ifndef VOLCANO_SERVICE_HPP
#define VOLCANO_SERVICE_HPP

#include <QStack>
#include <QVector3D>
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
    bool start(void);
    void stop(void);
    bool isStarted(void) const;
    void pushTransform(bool copyTop = false);
    void popTransform(void);
    void resetTransform(void);
    const Transform::Data &transform(void) const;
    void setTransform(const Transform::Data &v);

protected:
    virtual bool onStart(void);
    virtual void onStop(void);

private:
    using TransformStack = QStack<Transform::Data>;

private:
    bool m_isStarted;
    TransformStack m_transformStack;
};

VOLCANO_END

#endif // VOLCANO_SERVICE_HPP
