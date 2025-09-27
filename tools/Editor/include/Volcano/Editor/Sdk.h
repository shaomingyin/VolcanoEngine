//
//
#ifndef VOLCANO_EDITOR_SDK_H
#define VOLCANO_EDITOR_SDK_H

#include <QDir>
#include <QVersionNumber>
#include <QObject>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class Sdk: public QObject {
    Q_OBJECT

public:
    Sdk(const QDir& dirpath, QObject* parent = nullptr);

public:
    const QDir& path() const;
    const QVersionNumber& version() const;

private:
    QDir dirpath_;
    QVersionNumber version_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SDK_H
