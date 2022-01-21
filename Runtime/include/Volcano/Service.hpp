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
    void translate(const QVector3D &v);
    void translateTo(const QVector3D &v);
    void scale(const QVector3D &v);
    void scaleTo(const QVector3D &v);
    void rotate(const QQuaternion &v);
    void rotateTo(const QQuaternion &v);
    void resetTransform(void);
    void pushTransform(bool copyTop = false);
    void popTransform(void);
    const Transform &transform(void);

protected:
    virtual bool onStart(void);
    virtual void onStop(void);

private:
    using TransformStack = QStack<Transform>;

private:
    bool m_isStarted;
    TransformStack m_transformStack;
};

VOLCANO_END

#endif // VOLCANO_SERVICE_HPP
