//
//
#ifndef VOLCANO_CONTEXT_HPP
#define VOLCANO_CONTEXT_HPP

#include <QObject>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class VOLCANO_API Context: public QObject {
    Q_OBJECT

public:
    Context(QObject *parent = nullptr);
    ~Context(void) override;

public:
    bool init(void);
};

VOLCANO_END

#endif // VOLCANO_CONTEXT_HPP
