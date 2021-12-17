//
//
//
//
#ifndef VOLCANO_GAME_SHAPE_HPP
#define VOLCANO_GAME_SHAPE_HPP

#include <QObject>

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Shape: public QObject {
    Q_OBJECT

public:
    Shape(QObject *parent = nullptr);
    ~Shape(void) override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SHAPE_HPP
