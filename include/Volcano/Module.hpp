//
//
#ifndef VOLCANO_MODULE_HPP
#define VOLCANO_MODULE_HPP

#include <QLibrary>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class VOLCANO_API Module: public QLibrary
{
    Q_OBJECT

public:
    Module(QObject *parent = nullptr);
    ~Module(void) override;
};

VOLCANO_END

#endif // VOLCANO_MODULE_HPP
