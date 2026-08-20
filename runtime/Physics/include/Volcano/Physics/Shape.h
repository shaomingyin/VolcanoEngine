//
//
#ifndef VOLCANO_WORLD_SHAPE_H
#define VOLCANO_WORLD_SHAPE_H

#include <QObject>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Shape: public QObject {
    Q_OBJECT

public:
    Shape(QObject* parent = nullptr);
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SHAPE_H
