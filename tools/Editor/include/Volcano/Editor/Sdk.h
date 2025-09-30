//
//
#ifndef VOLCANO_EDITOR_SDK_H
#define VOLCANO_EDITOR_SDK_H

#include <QDir>
#include <QVersionNumber>

#include <utils/id.h>
#include <projectexplorer/kit.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class Sdk final {
public:
    Sdk() = default;
    Sdk(const Sdk&) = default;
    Sdk(Sdk&&) = default;
    Sdk(const QDir& dirpath, QObject* parent = nullptr);

public:
    Sdk& operator=(const Sdk&) = default;
    Sdk& operator=(Sdk&&) = default;
    Utils::Id id() const;
    const QDir& path() const;
    const QVersionNumber& version() const;
    bool isFitKit(const ProjectExplorer::Kit& kit) const;

private:
    Utils::Id id_;
    QDir dirpath_;
    QVersionNumber version_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SDK_H
