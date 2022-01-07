//
//
#ifndef VOLCANO_SHAPE_HPP
#define VOLCANO_SHAPE_HPP

#include <QObject>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class Shape: public QObject {
    Q_OBJECT

public:
    Shape(QObject *parent = nullptr);
    ~Shape(void) override;
};

VOLCANO_END

#endif // VOLCANO_SHAPE_HPP
