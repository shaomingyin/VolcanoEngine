//
//
#ifndef VOLCANO_GAME_CYLINDER_HPP
#define VOLCANO_GAME_CYLINDER_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Shape.hpp>

VOLCANO_GAME_BEGIN

class Cylinder: public Shape {
    Q_OBJECT
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged)

public:
    class Data {
    public:
        Data(void);
        Data(const Data &that);
        Data(qreal radius, qreal height);

    public:
        qreal radius(void) const;
        void setRadisu(qreal v);
        qreal height(void) const;
        void setHeight(qreal v);
        Data &operator=(const Data &that);
        bool operator==(const Data &that) const;
        bool operator!=(const Data &that) const;

    private:
        qreal m_radius;
        qreal m_height;
    };

public:
    Cylinder(QObject *parent = nullptr);
    Cylinder(const Data &data, QObject *parent = nullptr);
    Cylinder(qreal radius, qreal height, QObject *parent = nullptr);
    ~Cylinder(void) override;

public:
    const Data &data(void) const;
    qreal radius(void) const;
    void setRadius(qreal v);
    qreal height(void) const;
    void setHeight(qreal v);

signals:
    void radiusChanged(qreal v);
    void heightChanged(qreal v);

private:
    Data m_data;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CYLINDER_HPP
