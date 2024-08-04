//
//
#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <utils/async.h>
#include <coreplugin/progressmanager/progressmanager.h>

#include <Volcano/Editor/BuildSystem.hpp>

VOLCANO_EDITOR_BEGIN

BuildSystem::BuildSystem(ProjectExplorer::Target* target)
    : ProjectExplorer::BuildSystem(target) {
    requestParse();
}

QString BuildSystem::name() const {
    return "Volcano";
}

void BuildSystem::triggerParsing() {
    const auto future = Utils::asyncRun([this]() mutable {
        auto guard = guardParsingRun();
        if (parse()) {
            guard.markAsSuccess();
        }
    });

    Core::ProgressManager::addTask(future, "Parsing volcano project", "Volcano.Editor.Project.Parse");
}

bool BuildSystem::parse() {
    auto root_node = std::make_unique<ProjectExplorer::ProjectNode>(projectFilePath());

    QFile project_file(projectFilePath().toString());
    if (!project_file.open(QFile::ReadOnly)) {
        return false;
    }

    QJsonParseError root_parse_error;
    QJsonDocument root_document = QJsonDocument::fromJson(project_file.readAll(), &root_parse_error);
    if (root_parse_error.error != QJsonParseError::NoError) {
        qCritical("Failed to parse volcano project file: %s", qPrintable(root_parse_error.errorString()));
        return false;
    }
    if (!root_document.isObject()) {
        return false;
    }

    auto root = root_document.object();

    auto name_it = root.find("name");
    if (name_it != root.end() && name_it->isString()) {
        root_node->setDisplayName(name_it->toString());
    }

    auto version_it = root.find("version");
    if (version_it != root.end()) {
        auto version = parseVersion(*version_it);
    }

    auto engine_version_it = root.find("engineVersion");
    if (engine_version_it != root.end()) {
        auto engineVersion = parseVersion(*version_it);
        if (engineVersion > volcanoVersion) {
            return false;
        }
    }

    auto assets_node = std::make_unique<ProjectExplorer::VirtualFolderNode>(Utils::FilePath::fromString("Assets"));
    auto assets_it = root.find("assets");
    if (assets_it != root.end() && assets_it->isArray()) {
        auto assets = assets_it->toArray();
        for (const auto& asset: assets) {
            if (!asset.isObject()) {
                return false;
            }
            auto asset_node = parseAsset(asset.toObject());
            if (!asset_node) {
                return false;
            }
            assets_node->addNestedNode(std::move(asset_node), assets_node->filePath());
        }
    }
    root_node->addNode(std::move(assets_node));

    auto world_node = std::make_unique<ProjectExplorer::VirtualFolderNode>(Utils::FilePath::fromString("World"));
    auto world_it = root.find("world");
    if (world_it != root.end() && world_it->isObject()) {
        auto world = world_it->toObject();
        auto camera_it = world.find("camera");
        if (camera_it != world.end() && camera_it->isObject()) {

        }
        auto scenes_it = world.find("scenes");
        if (scenes_it != world.end() && scenes_it->isArray()) {
            auto scenes = scenes_it->toArray();
            Utils::FilePaths filePaths;
            for (const auto& scene: scenes) {
                if (scene.isString()) {
                    auto filePath = Utils::FilePath::fromString(scene.toString());
                    if (filePath.suffix() != "vsf") {
                        return false;
                    }
                    filePaths.append(std::move(filePath));
                }
            }
            world_node->addFiles(filePaths);
        }
    }
    root_node->addNode(std::move(world_node));

    setRootProjectNode(std::move(root_node));

    return true;
}

std::unique_ptr<ProjectExplorer::ResourceFileNode> BuildSystem::parseAsset(QJsonObject asset) {
    auto file_it = asset.find("file");
    if (file_it == asset.end()) {
        return nullptr;
    }

    if (!file_it->isString()) {
        return nullptr;
    }

    auto file_path = Utils::FilePath::fromString(file_it->toString());
    if (file_path.suffix() != "qrc") {
        return nullptr;
    }
    if (file_path.isRelativePath()) {
        file_path = projectFilePath().absoluteFilePath().parentDir().resolvePath(file_path);
    }

    QString mount_point("/");
    auto mount_point_it = asset.find("mountPoint");
    if (mount_point_it != asset.end() && mount_point_it->isString()) {
        mount_point = mount_point_it->toString();
    }

    return std::make_unique<ProjectExplorer::ResourceFileNode>(file_path, mount_point, file_path.fileName());
}

QVersionNumber BuildSystem::parseVersion(QJsonValue value) {
    if (value.isString()) {
        return QVersionNumber::fromString(value.toString());
    }

    int major = 0;
    int minor = 0;
    int patch = 0;

    if (value.isArray()) {
        auto array = value.toArray();
        auto size = array.size();
        if (size > 0) {
            major = array.at(0).toInt();
        }
        if (size > 1) {
            minor = array.at(1).toInt();
        }
        if (size > 2) {
            patch = array.at(2).toInt();
        }
        return QVersionNumber(major, minor, patch);
    }

    if (value.isObject()) {
        auto object = value.toObject();
        auto major_it = object.find("major");
        if (major_it != object.end()) {
            major = major_it->toInt();
            auto minor_it = object.find("minor");
            if (minor_it != object.end()) {
                minor = minor_it->toInt();
                auto patch_it = object.find("patch");
                if (patch_it != object.end()) {
                    patch = patch_it->toInt();
                }
            }
        }
        return QVersionNumber(major, minor, patch);
    }

    return QVersionNumber();
}

QByteArray BuildSystem::defaultProjectFileContents(const QString& project_name) {
    QJsonObject project;

    project["version"] = "0.1.0";
    project["engineVersion"] = VOLCANO_VERSION_STR;
    project["name"] = project_name;
    project["assets"] = QJsonArray();

    QJsonObject world;
    world["scenes"] = QJsonArray();
    project["world"] = world;

    QJsonDocument doc;
    doc.setObject(project);

    return doc.toJson();
}

VOLCANO_EDITOR_END
