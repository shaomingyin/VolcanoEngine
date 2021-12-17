//
//
#ifndef VOLCANO_GAME_BOX_HPP
#define VOLCANO_GAME_BOX_HPP

#include <QObject>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Shape.hpp>

VOLCANO_GAME_BEGIN

class Box: public Shape {
    Q_OBJECT
    Q_PROPERTY(qreal length READ length WRITE setLength NOTIFY lengthChanged)
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged)

public:
    class Data {
    public:
        Data(void);
        Data(const Data &that);
        Data(qreal length, qreal width, qreal height);

    public:
        qreal length(void) const;
        void setLength(qreal v);
        qreal width(void) const;
        void setWidth(qreal v);
        qreal height(void) const;
        void setHeight(qreal v);
        Data &operator=(const Data &that);
        bool operator==(const Data &that) const;
        bool operator!=(const Data &that) const;

    private:
        qreal m_length;
        qreal m_width;
        qreal m_height;
    };

public:
    Box(QObject *parent = nullptr);
    Box(const Data &data, QObject *parent = nullptr);
    Box(qreal length, qreal width, qreal height, QObject *parent = nullptr);
    ~Box(void) override;

public:
    const Data &data(void) const;
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
    Data m_data;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_BOX_HPP
