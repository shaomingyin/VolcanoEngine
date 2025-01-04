//
//
#ifndef VOLCANO_RESOURCECOMPILER_PACKAGER_H
#define VOLCANO_RESOURCECOMPILER_PACKAGER_H

#include <Volcano/ResourceBuilder/Common.h>
#include <Volcano/ResourceBuilder/Tool.h>

VOLCANO_RESOURCEBUILDER_BEGIN

class Packager: public Tool {
    Q_OBJECT

public:
    Packager(const QCommandLineParser& cmdline, QObject* parent = nullptr);

public:
    void run() override;
};

VOLCANO_RESOURCEBUILDER_END

#endif // VOLCANO_RESOURCECOMPILER_PACKAGER_H
