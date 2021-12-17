//
//
#ifndef VOLCANO_GAME_LIGHT_HPP
#define VOLCANO_GAME_LIGHT_HPP

#include <QColor>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Actor.hpp>

VOLCANO_GAME_BEGIN

class Light: public Actor {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    class Data {
    public:
        Data(void);
        Data(const Data &that);
        Data(const QColor &color);

    public:
        const QColor &color(void) const;
        void setColor(const QColor &v);
        Data &operator=(const Data &that);
        bool operator==(const Data &that) const;
        bool operator!=(const Data &that) const;

    private:
        QColor m_color;
    };

public:
    Light(QObject *parent = nullptr);
    ~Light(void) override;

public:
    const QColor &color(void) const;
    void setColor(const QColor &v);

signals:
    void colorChanged(const QColor &v);

private:
    QColor m_color;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_LIGHT_HPP
