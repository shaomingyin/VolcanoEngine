//
//
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>

#include <utils/async.h>
#include <utils/fileutils.h>
#include <coreplugin/progressmanager/progressmanager.h>

#include <Volcano/Editor/Project.hpp>

VOLCANO_EDITOR_BEGIN

Project::Project(const Utils::FilePath &filename)
    : ProjectExplorer::Project(VOLCANO_EDITOR_MIME_PROJECT_FILE, filename)
    , version_(0, 0, 0) {
    setDisplayName("Volcano Project");
    setBuildSystemCreator([](ProjectExplorer::Target*){
        return nullptr;
    });
    startLoading();
}

void Project::startLoading() {
    Core::ProgressManager::addTask(
        Utils::asyncRun([this] {
            load();
        }), "Loading volcano project...", "");
}

void Project::load() {
    Utils::FileReader file_reader;
    file_reader.fetch(projectFilePath(), QIODevice::ReadOnly | QIODevice::Text);
    QJsonParseError parse_error;
    auto document = QJsonDocument::fromJson(file_reader.data(), &parse_error);
    if (parse_error.error != QJsonParseError::NoError) {
        return;
    }
    if (!document.isObject()) {
        return;
    }
    auto root = document.object();
    parseVersion(root, "version", version_, { 0, 0, 0 });
    parseVersion(root, "engine_version", engine_version_, { 0, 0, 0 });
    if (!loadGame(root)) {
        return;
    }
    loadExtraProjects(root);
}

bool Project::loadGame(const QJsonObject& obj) {
    auto it = obj.find("game");
    if (it == obj.end()) {
        return false;
    }
    if (!it->isObject()) {
        return false;
    }
    return true;
}

bool Project::loadWorld(const QJsonObject& obj) {
    return true;
}

bool Project::loadScene(const QJsonObject& obj) {
    return true;
}

void Project::loadExtraProjects(const QJsonObject& obj) {
    auto it = obj.find("extra_projects");
    if (it == obj.end()) {
        return;
    }
}

void Project::parseVersion(const QJsonObject& obj, const QString& key, QVersionNumber& out, const QVersionNumber& def) {
    auto it = obj.find(key);
    if (it == obj.end()) {
        out = def;
        return;
    }
    if (it->isArray()) {
        int major = 0;
        int minor = 0;
        int patch = 0;
        auto array = it->toArray();
        auto array_size = array.size();
        if (array_size > 2) {
            patch = array.at(2).toInt(0);
        }
        if (array_size > 1) {
            minor = array.at(1).toInt(0);
        }
        if (array_size > 0) {
            major = array.at(0).toInt(0);
        }
        out = QVersionNumber(major, minor, patch);
    } else if (it->isString()) {
        out = QVersionNumber::fromString(it->toString(VOLCANO_VERSION_STR));
    } else {
        out = def;
    }
}

VOLCANO_EDITOR_END
