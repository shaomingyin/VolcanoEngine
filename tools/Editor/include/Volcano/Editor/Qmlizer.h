//
//
#ifndef VOLCANO_EDITOR_QMLIZER_H
#define VOLCANO_EDITOR_QMLIZER_H

#include <QString>
#include <QObject>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class Qmlizer: public QObject {
    Q_OBJECT

public:
    Qmlizer(QObject& target, QObject *parent = nullptr);

public:
    QString toString() const;

private:
    QObject& target_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_QMLIZER_H
