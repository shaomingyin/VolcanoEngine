//
//
#ifndef VOLCANO_SYSTEM_LOADER_H
#define VOLCANO_SYSTEM_LOADER_H

#include <QObject>

#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Loader: public QObject {
    Q_OBJECT

public:
    Loader(QObject* parent = nullptr);
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_LOADER_H
