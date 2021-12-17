//
//
#ifndef VOLCANO_GAME_SPHERE_HPP
#define VOLCANO_GAME_SPHERE_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Shape.hpp>

VOLCANO_GAME_BEGIN

class Sphere: public Shape {
    Q_OBJECT
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
    class Data {
    public:
        Data(void);
        Data(const Data &that);
        Data(qreal radius);

    public:
        qreal radius(void) const;
        void setRadius(qreal v);
        Data &operator=(const Data &that);
        bool operator==(const Data &that) const;
        bool operator!=(const Data &that) const;

    private:
        qreal m_radius;
    }

public:
    Sphere(QObject *parent = nullptr);
    Sphere(const Data &data, QObject *parent = nullptr);
    Sphere(qreal radius, QObject *parent = nullptr);
    ~Sphere(void) override;

public:
    const Data &data(void) const;
    qreal radius(void) const;
    void setRadius(qreal v);

signals:
    void radiusChanged(qreal v);

private:
    Data m_data;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SPHERE_HPP
