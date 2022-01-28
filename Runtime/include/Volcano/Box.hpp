//
//
#ifndef VOLCANO_BOX_HPP
#define VOLCANO_BOX_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Shape.hpp>

VOLCANO_BEGIN

class Box: public Shape {
    Q_OBJECT
    Q_PROPERTY(qreal length READ length WRITE setLength NOTIFY lengthChanged)
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged)

public:
    class Data: public Shape::Data {
    public:
        Data(void);
        Data(const Data &that);
        Data(float length, float width, float height);
        ~Data(void) override;

    public:
        float length(void) const;
        void setLength(float v);
        float width(void) const;
        void setWidth(float v);
        float height(void) const;
        void setHeight(float v);
        Data &operator=(const Data &that);
        bool operator==(const Data &that);
        bool operator!=(const Data &that);

    private:
        float m_length;
        float m_width;
        float m_height;
    };

public:
    Box(QObject *parent = nullptr);
    Box(const Data &data, QObject *parent = nullptr);
    Box(qreal length, qreal width, qreal height, QObject *parent = nullptr);
    ~Box(void) override;

public:
    const Data &data(void) const;
    void setData(const Data &data);
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

VOLCANO_END

#endif // VOLCANO_BOX_HPP
