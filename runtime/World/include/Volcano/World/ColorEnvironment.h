#ifndef VOLCANO_WORLD_COLORENVIRONMENT_H
#define VOLCANO_WORLD_COLORENVIRONMENT_H

#include <QColor>

#include <Volcano/World/Environment.h>

VOLCANO_WORLD_BEGIN

class ColorEnvironment: public Environment {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit ColorEnvironment(QObject* parent = nullptr);
    const QColor& color() const noexcept;
    void setColor(const QColor& color);

signals:
    void colorChanged(const QColor& color);

private:
    QColor color_ { Qt::black };
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_COLORENVIRONMENT_H
