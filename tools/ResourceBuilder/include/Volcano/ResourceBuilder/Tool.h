//
//
#ifndef VOLCANO_RESOURCECOMPILER_TOOL_H
#define VOLCANO_RESOURCECOMPILER_TOOL_H

#include <QObject>
#include <QCommandLineParser>

#include <Volcano/ResourceBuilder/Common.h>

VOLCANO_RESOURCEBUILDER_BEGIN

class Tool: public QObject {
    Q_OBJECT

public:
    Tool(const QCommandLineParser& cmdline, QObject* parent = nullptr);

public:
	virtual int run() = 0;

protected:
    const QCommandLineParser& cmdline() const {
        return cmdline_;
    }

private:
    const QCommandLineParser& cmdline_;
};

VOLCANO_RESOURCEBUILDER_END

#endif // VOLCANO_RESOURCECOMPILER_TOOL_H
