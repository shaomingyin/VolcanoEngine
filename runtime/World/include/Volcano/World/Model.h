#ifndef VOLCANO_WORLD_MODEL_H
#define VOLCANO_WORLD_MODEL_H

#include <QUrl>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Model: public QObject {
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)

public:
    explicit Model(QObject* parent = nullptr);

    const QUrl& source() const noexcept;
    void setSource(const QUrl& source);

signals:
    void sourceChanged(const QUrl& source);

private:
    QUrl source_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_MODEL_H
