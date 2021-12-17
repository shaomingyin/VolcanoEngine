//
//
#ifndef VOLCANO_GAME_PLANE_HPP
#define VOLCANO_GAME_PLANE_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Shape.hpp>

VOLCANO_GAME_BEGIN

class Plane: public Shape {
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(qreal z READ z WRITE setZ NOTIFY zChanged)
    Q_PROPERTY(qreal D READ D WRITE setD NOTIFY DChanged)

public:
    class Data {
    public:
        Data(void);
        Data(const Data &that);
        Data(qreal x, qreal y, qreal z, qreal d);

    public:
        qreal x(void) const;
        void setX(qreal v);
        qreal y(void) const;
        void setY(qreal v);
        qreal z(void) const;
        void setZ(qreal v);
        qreal d(void) const;
        void setD(qreal v);
        Data &operator=(const Data &that);
        bool operator==(const Data &that) const;
        bool operator!=(const Data &that) const;

    private:
        qreal m_x;
        qreal m_y;
        qreal m_z;
        qreal m_d;
    };

public:
    Plane(QObject *parent = nullptr);
    Plane(const Data &data, QObject *parent = nullptr);
    Plane(qreal x, qreal y, qreal z, qreal d, QObject *parent = nullptr);
    ~Plane(void) override;

public:
    const Data &data(void) const;
    qreal x(void) const;
    void setX(qreal v);
    qreal y(void) const;
    void setY(qreal v);
    qreal z(void) const;
    void setZ(qreal v);
    qreal d(void) const;
    void setD(qreal v);

signals:
    void xChanged(qreal v);
    void yChanged(qreal v);
    void zChanged(qreal v);
    void dChanged(qreal v);

private:
    Data m_data;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_PLANE_HPP
