//
//
#include <Volcano/ResourceManager.hpp>

VOLCANO_BEGIN

ResourceManager::ResourceManager(QObject* parent)
    : QObject(parent) {
}

bool ResourceManager::start() {
    return true;
}

void ResourceManager::stop() {
}

VOLCANO_END
