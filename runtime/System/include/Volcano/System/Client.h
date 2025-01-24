//
//
#ifndef VOLCANO_SYSTEM_CLIENT_H
#define VOLCANO_SYSTEM_CLIENT_H

#include <Volcano/System/Common.h>
#include <Volcano/System/Local.h>

VOLCANO_SYSTEM_BEGIN

class Client: public Local {
    Q_OBJECT

public:
    Client(QQmlEngine* engine, const QUrl& url, QObject* parent = nullptr);

public:
    void update(Duration elapsed) override;

private:
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_CLIENT_H
