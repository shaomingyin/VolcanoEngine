//
//
#ifndef VOLCANO_EDITOR_SDKMANAGER_H
#define VOLCANO_EDITOR_SDKMANAGER_H

#include <QDir>
#include <QList>
#include <QObject>
#include <QAbstractItemModel>

#include <utils/listmodel.h>

#include <Volcano/Editor/Common.h>
#include <Volcano/Editor/Sdk.h>

VOLCANO_EDITOR_BEGIN

class SdkManager: public Utils::ListModel<Sdk> {
    Q_OBJECT

public:
    SdkManager(QObject* parent = nullptr);

public:
    const Sdk* sdk(Utils::Id id) const;
    const Sdk* firstFitKit(const ProjectExplorer::Kit& kit) const;
    const Sdk* current() const;
    void setCurrent(const Sdk* sdk);
    void reset();

signals:
    void currentChanged(const Sdk* sdk);

private:
    QList<Sdk> sdk_list_;
    const Sdk* current_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SDKMANAGER_H
