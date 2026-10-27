//
//
#ifndef VOLCANO_EDITOR_CONTEXT_H
#define VOLCANO_EDITOR_CONTEXT_H

#include <QObject>

#include <Volcano/Editor/Common.h>
#include <Volcano/Editor/SdkManager.h>

VOLCANO_EDITOR_BEGIN

class Context: public QObject {
    Q_OBJECT

public:
    Context(QObject* parent = nullptr);

public:
    SdkManager& sdkManager();

public:
    SdkManager sdk_manager_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_CONTEXT_H
