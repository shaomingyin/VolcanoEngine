//
//
#ifndef VOLCANO_EDITOR_SCENEDOCUMENT_HPP
#define VOLCANO_EDITOR_SCENEDOCUMENT_HPP

#include <QString>

#include <coreplugin/idocument.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class SceneDocument: public Core::IDocument {
    Q_OBJECT

public:
    static const QString MimeType;

public:
    SceneDocument();
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SCENEDOCUMENT_HPP
