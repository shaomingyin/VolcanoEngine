//
//
#ifndef VOLCANO_EDITOR_SCENEDOCUMENT_H
#define VOLCANO_EDITOR_SCENEDOCUMENT_H

#include <memory>

#include <coreplugin/idocument.h>
#include <coreplugin/idocumentfactory.h>

#include <Volcano/Game/World.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class SceneDocument: public Core::IDocument {
    Q_OBJECT

public:
    SceneDocument(QObject* parent = nullptr);

public:
    Utils::Result<> open(const Utils::FilePath& filepath, const Utils::FilePath& real_fileath) override;
    QByteArray contents() const override;
    Utils::Result<> setContents(const QByteArray &contents) override;
    bool isModified() const override;
    bool isSaveAsAllowed() const override;

protected:
    //Utils::Result<> saveImpl(const Utils::FilePath& filepath, bool auto_save) override;

private:
    std::unique_ptr<Game::World> game_world_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SCENEDOCUMENT_H
