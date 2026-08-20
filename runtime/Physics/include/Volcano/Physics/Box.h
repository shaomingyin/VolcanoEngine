//
//
#ifndef VOLCANO_WORLD_BOX_H
#define VOLCANO_WORLD_BOX_H

#include <QVector3D>

#include <Volcano/World/Common.h>
#include <Volcano/World/Shape.h>

VOLCANO_WORLD_BEGIN

class Box: public Shape {
    Q_OBJECT
    Q_PROPERTY(float length READ length WRITE setLength NOTIFY lengthChanged)
    Q_PROPERTY(float width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged)

public:
    Box(QObject* parent = nullptr);

public:
    QVector3D size() const noexcept;
    float length() const noexcept;
    void setLength(float v) noexcept;
    float width() const noexcept;
    void setWidth(float v) noexcept;
    float height() const noexcept;
    void setHeight(float v) noexcept;

signals:
    void lengthChanged(float v);
    void widthChanged(float v);
    void heightChanged(float v);

private:
    QVector3D size_;

};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_BOX_H
