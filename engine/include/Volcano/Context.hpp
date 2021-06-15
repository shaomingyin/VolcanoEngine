//
//
#ifndef VOLCANO_CONTEXT_HPP
#define VOLCANO_CONTEXT_HPP

#include <QObject>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class Context: public QObject {
    Q_OBJECT
    Q_PROPERTY(Mode mode READ mode)

public:
    enum Mode {
        Launcher,
        Editor
    };
    Q_ENUM(Mode)

public:
    Context(QObject *parent = nullptr);
    ~Context(void) override;

public:
    virtual Mode mode(void) const = 0;
};

VOLCANO_END

#endif // VOLCANO_CONTEXT_HPP
