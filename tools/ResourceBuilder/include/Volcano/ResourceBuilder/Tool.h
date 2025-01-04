//
//
#ifndef VOLCANO_RESOURCECOMPILER_TOOL_H
#define VOLCANO_RESOURCECOMPILER_TOOL_H

#include <QFuture>
#include <QCommandLineParser>
#include <QObject>

#include <Volcano/ResourceBuilder/Common.h>

VOLCANO_RESOURCEBUILDER_BEGIN

class Tool: public QObject {
    Q_OBJECT

public:
    Tool(const QCommandLineParser& cmdline, QObject* parent = nullptr);

public:
    void cancel() {
        future_.cancel();
    }

    void init();

signals:
    void finished();
    void canceled();
    void failed(const QString& error_message);

protected:
    const QCommandLineParser& cmdline() const {
        return cmdline_;
    }

    virtual void run() = 0;

private:
    const QCommandLineParser& cmdline_;
    QFuture<void> future_;
};

VOLCANO_RESOURCEBUILDER_END

#endif // VOLCANO_RESOURCECOMPILER_TOOL_H
