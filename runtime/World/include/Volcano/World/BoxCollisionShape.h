//
//
#ifndef VOLCANO_WORLD_BOXCOLLISIONSHAPE_H
#define VOLCANO_WORLD_BOXCOLLISIONSHAPE_H

#include <QVector3D>

#include <Volcano/World/Common.h>
#include <Volcano/World/CollisionShape.h>

VOLCANO_WORLD_BEGIN

class BoxCollisionShape: public CollisionShape {
    Q_OBJECT
    Q_PROPERTY(float length READ length WRITE setLength NOTIFY lengthChanged FINAL)
    Q_PROPERTY(float width READ width WRITE setWidth NOTIFY widthChanged FINAL)
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged FINAL)

public:
    BoxCollisionShape(QObject* parent = nullptr);

public:
    QVector3D size() const {
        return size_;
    }

    float length() const {
        return size_.x();
    }

    void setLength(float v) {
        if (!qFuzzyCompare(size_.x(), v)) {
            size_.setX(v);
            emit lengthChanged(v);
        }
    }

    float width() const {
        return size_.y();
    }

    float height() const {
        return size_.z();
    }

    void setWidth(float v);
    void setHeight(float v);

signals:
    void lengthChanged(float v);
    void widthChanged(float v);
    void heightChanged(float v);

private:
    QVector3D size_;

};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_BOXCOLLISIONSHAPE_H
