//
//
#ifndef VOLCANO_EDITOR_BUILDSYSTEM_HPP
#define VOLCANO_EDITOR_BUILDSYSTEM_HPP

#include <projectexplorer/buildsystem.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class BuildSystem: public ProjectExplorer::BuildSystem {
    Q_OBJECT

public:
    explicit BuildSystem(ProjectExplorer::Target* target);

public:
    QString name() const override;
    void triggerParsing() override;
    static QByteArray defaultProjectFileContents(const QString& project_name);

private:
    bool parse();
    std::unique_ptr<ProjectExplorer::ResourceFileNode> parseAsset(QJsonObject asset);
    static QVersionNumber parseVersion(QJsonValue value);

private:
    QVersionNumber version_;
    QVersionNumber engine_version_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_BUILDSYSTEM_HPP
