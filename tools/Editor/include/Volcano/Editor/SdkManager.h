//
//
#ifndef VOLCANO_EDITOR_SDKMANAGER_H
#define VOLCANO_EDITOR_SDKMANAGER_H

#include <memory>

#include <QDir>
#include <QList>
#include <QObject>

#include <Volcano/Editor/Common.h>
#include <Volcano/Editor/Sdk.h>

VOLCANO_EDITOR_BEGIN

class SdkManager: public QObject {
    Q_OBJECT

public:
    using SdkPtr = std::unique_ptr<Sdk>;
    using SdkList = QList<SdkPtr>;

public:
    SdkManager(QObject* parent = nullptr);

public:
    const SdkList& sdkList() const;
    void addSdk(const QDir& dirpath);
    void removeSdk(const QDir& dirpath);
    void removeAllSdks();

signals:
    void sdkAdded(const Sdk& sdk);
    void sdkDirectoryRemoved(const Sdk& sdk);

private:
    SdkList sdk_list_;

};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SDKMANAGER_H
