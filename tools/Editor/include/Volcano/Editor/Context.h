//
//
#ifndef VOLCANO_EDITOR_CONTEXT_H
#define VOLCANO_EDITOR_CONTEXT_H

#include <QObject>
#include <QQmlApplicationEngine>

#include <Volcano/World/Scene.h>

#include <Volcano/Editor/Common.h>
#include <Volcano/Editor/SdkManager.h>

VOLCANO_EDITOR_BEGIN

class Context: public QObject {
    Q_OBJECT

public:
    Context(QObject* parent = nullptr);

public:
    SdkManager& sdkManager() {
        return sdk_manager_;
    }

    QQmlEngine& qmlEngine() {
        return qml_engine_;
    }

public:
    SdkManager sdk_manager_;
    QQmlApplicationEngine qml_engine_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_CONTEXT_H
