//
//
#ifndef VOLCANO_RESOURCEHANDLER_HPP
#define VOLCANO_RESOURCEHANDLER_HPP

#include <QObject>
#include <QMetaObject>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class ResoueceHandler: public QObject {
    Q_OBJECT

public:
    ResoueceHandler();
    ~ResoueceHandler(void) override;

public:
    virtual
};

VOLCANO_END

#endif // VOLCANO_RESOURCEHANDLER_HPP
