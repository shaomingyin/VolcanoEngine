//
//
#ifndef VOLCANO_SYSTEM_LOCAL_H
#define VOLCANO_SYSTEM_LOCAL_H

#include <QAtomicInt>

#include <Volcano/Graphics/Renderer.h>
#include <Volcano/System/Common.h>
#include <Volcano/System/Base.h>
#include <Volcano/System/PotentialVisibleSetBuilder.h>

VOLCANO_SYSTEM_BEGIN

class Local: public Base {
    Q_OBJECT

public:
    Local(QQmlEngine* engine, const QUrl& url, QObject* parent = nullptr);

public:
    void update(Duration elapsed) override;
    virtual void render();

private:
    Graphics::Renderer renderer_;
    Graphics::View view_[2];
    QAtomicInt view_current_;
    PotentialVisibleSetBuilder potential_visible_set_builder_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_LOCAL_H
