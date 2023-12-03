//
//
#ifndef VOLCANO_RESOURCEMANAGER_HPP
#define VOLCANO_RESOURCEMANAGER_HPP

#include <QString>
#include <QObject>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class ResourceManager: public QObject {
    Q_OBJECT

public:
    ResourceManager(QObject* parent = nullptr);

public:
    bool start();
    void stop();
    bool add(const QString& path);
};

VOLCANO_END

#endif // VOLCANO_RESOURCEMANAGER_HPP
