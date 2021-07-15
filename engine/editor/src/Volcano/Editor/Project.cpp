//
//
#include <QChar>
#include <QFile>
#include <QFileInfo>
#include <QScopeGuard>

#include <Volcano/Editor/Project.hpp>

VOLCANO_EDITOR_BEGIN

Project::Project(QObject *parent):
    QObject(parent),
    m_gameWorld(nullptr)
{
}

Project::~Project(void)
{
    if (m_gameWorld != nullptr)
        delete m_gameWorld;
}

bool Project::init(const QString &rootDirName, const QString &name)
{
    qDebug("Initialize project '%s' in '%s'.", qPrintable(name), qPrintable(rootDirName));

    Q_ASSERT(m_gameWorld == nullptr);
    Q_ASSERT(!name.isEmpty());

    if (!checkName(name))
        return false;

    QFileInfo fi(rootDirName);
    if (!fi.isDir())
        return false;

    m_gameWorld = new Game::World(this);
    if (!m_gameWorld) {
        qCritical("Failed to create game world object for project '%s'.", qPrintable(name));
        return false;
    }

    m_gameWorld->setObjectName("World");
    m_worldModel.setGameWorld(m_gameWorld);

    QScopeGuard gameWorldGuard([this] {
        delete m_gameWorld;
        m_gameWorld = nullptr;
        m_worldModel.setGameWorld(nullptr);
    });

    m_rootDir = rootDirName;
    if (!m_rootDir.isEmpty()) {
        qCritical("Failed to create project '%s' inside a non-empty directory.", qPrintable(name));
        return false;
    }

    m_resourcesDir = rootDirName + "/Resources";
    if (!m_rootDir.exists("Resources")) {
        if (!m_rootDir.mkdir("Resources")) {
            qCritical("Failed to create resource directory for project '%s'.", qPrintable(name));
            return false;
        }
    }

    m_resourcesModel.setRootPath(m_resourcesDir.path());

    setName(name);

    if (!save())
        return false;

    gameWorldGuard.dismiss();

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

    QJsonObject projectObject;

    projectObject["name"] = m_name;
    projectObject["gameWorld"] = gameWorldToJson(m_gameWorld);
    projectObject["resourceDir"] = m_resourcesDir.path();

    QFile projectFile(saveRootDirName + "/" + m_name + ".vpf");
    if (!projectFile.open(QFile::WriteOnly)) {
        qCritical("Failed to create project file for project '%s'.", qPrintable(m_name));
        return false;
    }

    qint64 ret = projectFile.write(QJsonDocument(projectObject).toJson());

    return (ret > 0);
}

const QString &Project::name(void) const
{
    return m_name;
}

void Project::setName(const QString &v)
{
    if (!checkName(v))
        return;

    if (m_name != v) {
        m_name = v;
        emit nameChanged(v);
    }
}

const QDir &Project::rootDir(void) const
{
    return m_rootDir;
}

const QDir &Project::resourcesDir(void) const
{
    return m_resourcesDir;
}

WorldModel &Project::worldModel(void)
{
    return m_worldModel;
}

QFileSystemModel &Project::resourcesModel(void)
{
    return m_resourcesModel;
}

Game::World *Project::gameWorldFromJson(QJsonValue jsonValue)
{
    Q_ASSERT(!jsonValue.isUndefined());

    if (jsonValue.isNull() || !jsonValue.isObject())
        return nullptr;

    QJsonValue tmp;
    auto gameWorld = std::make_unique<Game::World>();

    tmp = jsonValue["name"];
    if (tmp.isString())
        gameWorld->setObjectName(tmp.toString());

    tmp = jsonValue["dynamic"];
    if (tmp.isBool())
        gameWorld->setDynamic(tmp.toBool());
    else
        gameWorld->setDynamic(false);

    tmp = jsonValue["gravity"];
    if (tmp.isArray()) {
        auto tmpArray = tmp.toArray();
        if (tmpArray.size() == 3) {
            QVector3D gravity(tmpArray[0].toDouble(), tmpArray[1].toDouble(), tmpArray[1].toDouble());
            gameWorld->setGravity(gravity);
        }
    }

    tmp = jsonValue["objects"];
    if (tmp.isArray()) {
        auto objectArray = jsonValue.toArray();
        for (auto object: objectArray) {
            auto gameObject = gameObjectFromJson(object.toObject());
            if (gameObject != nullptr)
                gameWorld->appendObject(gameObject);
        }
    }

    return gameWorld.release();
}

QJsonValue Project::gameWorldToJson(Game::World *gameWorld)
{
    if (m_gameWorld == nullptr)
        return QJsonValue(QJsonValue::Null);

    QJsonObject gameWorldJsonObject;

    gameWorldJsonObject["name"] = m_gameWorld->objectName();
    gameWorldJsonObject["dynamic"] = m_gameWorld->isDynamic();

    auto &gravity = m_gameWorld->gravity();
    QJsonArray gravityArray { gravity[0], gravity[1], gravity[2] };

    gameWorldJsonObject["gravity"] = gravityArray;

    QJsonArray gameObjectJsonArray;

    auto &gameObjects = m_gameWorld->objects();
    for (auto gameObject: gameObjects)
        gameObjectJsonArray.append(gameObjectToJson(gameObject));

    gameWorldJsonObject["objects"] = gameObjectJsonArray;

    return gameWorldJsonObject;
}

Game::Object *Project::gameObjectFromJson(QJsonValue jsonValue)
{
    Q_ASSERT(!jsonValue.isUndefined());

    if (jsonValue.isNull())
        return nullptr;

    return nullptr; // TODO
}

QJsonValue Project::gameObjectToJson(Game::Object *gameObject)
{
    if (gameObject == nullptr)
        return QJsonValue(QJsonValue::Null);

    QJsonObject gameJsonObject;

    gameJsonObject["enabled"] = gameObject->isEnabled();
    gameJsonObject["name"] = gameObject->objectName();

    // auto gameLight = qobject_cast<Game::Light *>(gameObject);
    // TODO

    return gameJsonObject;
}

bool Project::checkName(const QString &name)
{
    if (name.isNull() || name.isEmpty())
        return false;

    for (qsizetype i = 0; i < name.size(); ++i) {
        QChar c = name.at(i);
        if (!c.isLetterOrNumber() && c != '_')
            return false;
    }

    return true;
}

VOLCANO_EDITOR_END
