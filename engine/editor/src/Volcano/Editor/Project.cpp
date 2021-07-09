//
//
#include <memory>

#include <QFile>
#include <QFileInfo>
#include <QScopeGuard>

#include <Volcano/Editor/Project.hpp>

VOLCANO_EDITOR_BEGIN

Project::Project(QObject *parent):
    QObject(parent)
{
}

Project::~Project(void)
{
}

bool Project::init(const QString &rootDirName, const QString &name)
{
    qDebug("Initialize project '%s' in '%s'.", qPrintable(name), qPrintable(rootDirName));

    Q_ASSERT(!name.isEmpty());

    QFileInfo fi(rootDirName);
    if (!fi.isDir())
        return false;

    auto gameWorld = std::make_unique<Game::World>(this);
    if (!gameWorld) {
        qCritical("Failed to create game world object for project '%s'.", qPrintable(name));
        return false;
    }

    gameWorld->setObjectName(name);

    QJsonObject projectJsonObject;
    projectJsonObject["name"] = name;
    projectJsonObject["version"] = VOLCANO_VERSION_STR;
    projectJsonObject["gameWorld"] = gameWorldToJsonObject(gameWorld.get());

    QFile projectJsonFile(rootDirName + "/VolcanoProject.json");
    if (!projectJsonFile.open(QFile::WriteOnly)) {
        qCritical("Failed to create project file for project '%s'.", qPrintable(name));
        return false;
    }

    projectJsonFile.write(QJsonDocument(projectJsonObject).toJson());

    setName(name);

    m_rootDir = rootDirName;
    m_resourcesDir = rootDirName + "/Resources";
    m_gameWorld = gameWorld.release();
    m_resourcesModel.setRootPath(m_resourcesDir.path());

    return true;
}

bool Project::load(const QString &rootDirName)
{
    QFileInfo fi(rootDirName);
    if (!fi.isDir())
        return false;

    QDir rootDir = rootDirName;

    if (!rootDir.exists("VolcanoProject.json")) {
        qCritical("Failed to load project in '%s', no project file found.", qPrintable(rootDirName));
        return false;
    }

    if (!rootDir.exists("Resources")) {
        qCritical("Failed to load project without resources.");
        return false;
    }

    // TODO loading...

    return true;
}

bool Project::save(const QString &rootDirName)
{
    auto saveRootDirName = rootDirName.isEmpty() ? m_rootDir.path() : rootDirName;

    // TODO

    return true;
}

const QString &Project::name(void) const
{
    return m_name;
}

void Project::setName(const QString &v)
{
    if (m_name != v) {
        m_name = v;
        emit nameChanged(v);
    }
}

const QDir &Project::rootDir(void) const
{
    return m_rootDir;
}

WorldModel &Project::worldModel(void)
{
    return m_worldModel;
}

QFileSystemModel &Project::resourcesModel(void)
{
    return m_resourcesModel;
}

Game::World *Project::gameWorldFromJsonObject(const QJsonObject &jsonObject)
{
    QJsonValue value;

    auto world = std::make_unique<World>();

    value = jsonObject["name"];
    if (value.isUndefined())
        return nullptr;

    world->setObjectName(jsonObject["name"].toString());

    value = jsonObject["dynamic"];
    if (value.isUndefined())
        world->setDynamic(false);
    else
        world->setDynamic(jsonObject["dynamic"].toBool());

    value = jsonObject["gravity"];
    if (value.isArray()) {
        auto gravityArray = value.toArray();
        if (gravityArray.size() == 3) {
            world->m_gravity.setX(gravityArray[0].toDouble());
            world->m_gravity.setY(gravityArray[1].toDouble());
            world->m_gravity.setZ(gravityArray[1].toDouble());
        }
    }

    value = jsonObject["objects"];
    if (value.isArray()) {
        auto objectArray = value.toArray();
        for (auto object: objectArray) {

        }
    }

    return world;
}

QJsonObject Project::gameWorldToJsonObject(Game::World *gameWorld)
{
    QJsonObject gameWorldObject;

    gameWorldObject["name"] = gameWorld->objectName();
    gameWorldObject["dynamic"] = gameWorld->isDynamic();

    auto &gravity = gameWorld->gravity();
    QJsonArray gravityArray { gravity[0], gravity[1], gravity[2] };

    gameWorldObject["gravity"] = gravityArray;

    QJsonArray objectArray;

    auto &objects = gameWorld->objects();
    for (auto object: objects) {
        objectArray.append(object->toJson());
    }

    gameWorldObject["objects"] = objectArray;

    return gameWorldObject;
}

VOLCANO_EDITOR_END
