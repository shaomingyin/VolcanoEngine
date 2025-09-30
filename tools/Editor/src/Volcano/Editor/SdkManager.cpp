//
//

#include <Volcano/Editor/SdkManager.h>

VOLCANO_EDITOR_BEGIN

SdkManager::SdkManager(QObject* parent)
    : Utils::ListModel<Sdk>(parent)
    , current_(nullptr) {
}

const Sdk* SdkManager::sdk(Utils::Id id) const {
    for (auto& sdk: sdk_list_) {
        if (sdk.id() == id) {
            return &sdk;
        }
    }
    return nullptr;
}

const Sdk* SdkManager::firstFitKit(const ProjectExplorer::Kit& kit) const {
    for (auto& sdk: sdk_list_) {
        if (sdk.isFitKit(kit)) {
            return &sdk;
        }
    }
    return nullptr;
}

const Sdk* SdkManager::current() const {
    return current_;
}

void SdkManager::setCurrent(const Sdk* sdk) {
    if (current_ != sdk) {
        current_ = sdk;
        emit currentChanged(sdk);
    }
}

void SdkManager::reset() {
    emit beginResetModel();
    sdk_list_.clear();
    sdk_list_.emplaceBack(QDir("/usr/lib"));
    sdk_list_.emplaceBack(QDir("/lib"));
    setAllData(sdk_list_);
    emit endResetModel();
}

VOLCANO_EDITOR_END
