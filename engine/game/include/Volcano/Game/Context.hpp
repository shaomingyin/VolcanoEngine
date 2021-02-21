//
//
#ifndef VOLCANO_GAME_CONTEXT_HPP
#define VOLCANO_GAME_CONTEXT_HPP

#include <QString>
#include <QObject>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

class VOLCANO_GAME_API Context: public QObject {
    Q_OBJECT

public:
    Context(QObject *parent = nullptr);
    ~Context(void) override;

public:
    virtual const QString &rootDir(void) const = 0;
    virtual World &world(void) = 0;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CONTEXT_HPP
