//
//
#include <Volcano/Editor/SdkManager.h>

VOLCANO_EDITOR_BEGIN

SdkManager::SdkManager(QObject* parent)
    : QObject(parent) {
}

const SdkManager::SdkList& SdkManager::sdkList() const {
    return sdk_list_;
}

void SdkManager::addSdk(const QDir& dirpath) {
    auto sdk = std::make_unique<Sdk>(dirpath);
    Sdk* raw = sdk.get();
    //sdk_list_.emplaceBack(std::move(sdk));
    emit sdkAdded(*raw);
}

void SdkManager::removeSdk(const QDir& dirpath) {
}

void SdkManager::removeAllSdks() {
}

VOLCANO_EDITOR_END
