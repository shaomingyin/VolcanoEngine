//
//
#ifndef VOLCANO_RESOURCEMANAGER_HPP
#define VOLCANO_RESOURCEMANAGER_HPP

#include <QList>
#include <QObject>
#include <QNetworkReply>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

namespace Resource {

class Context: public QObject {
    Q_OBJECT

private:
    Context(QObject *parent = nullptr);
    ~Context(void) override;

public:
    static Context *defaultContext(void);
    static bool registerContext(Context *context);
    static bool unregisterContext(Context *context);
    virtual void poll(void);
};

bool registerResource(QUrl url, Context *context = nullptr);

}

VOLCANO_END

#endif // VOLCANO_RESOURCEMANAGER_HPP
