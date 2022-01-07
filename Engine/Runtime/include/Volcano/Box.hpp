//
//
#ifndef VOLCANO_BOX_HPP
#define VOLCANO_BOX_HPP

#include <QObject>

#include <Volcano/Common.hpp>
#include <Volcano/Shape.hpp>

VOLCANO_BEGIN

class Box: public Shape {
    Q_OBJECT
    Q_PROPERTY(qreal length READ length WRITE setLength NOTIFY lengthChanged)
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged)

public:
    Box(QObject *parent = nullptr);
    Box(qreal length, qreal width, qreal height, QObject *parent = nullptr);
    ~Box(void) override;

public:
    qreal length(void) const;
    void setLength(qreal v);
    qreal width(void) const;
    void setWidth(qreal v);
    qreal height(void) const;
    void setHeight(qreal v);

signals:
    void lengthChanged(qreal v);
    void widthChanged(qreal v);
    void heightChanged(qreal v);

private:
    qreal m_length;
    qreal m_width;
    qreal m_height;
};

VOLCANO_END

#endif // VOLCANO_BOX_HPP
