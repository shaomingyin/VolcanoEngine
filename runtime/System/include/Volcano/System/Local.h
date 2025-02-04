//
//
#ifndef VOLCANO_SYSTEM_LOCAL_H
#define VOLCANO_SYSTEM_LOCAL_H

#include <QList>

#include <Volcano/Graphics/Renderer.h>
#include <Volcano/System/Common.h>
#include <Volcano/System/Base.h>
#include <Volcano/System/ScreenController.h>

VOLCANO_SYSTEM_BEGIN

class Local: public Base {
    Q_OBJECT

public:
    Local(World::Scene& scene, QObject* parent = nullptr);

protected:
    bool event(QEvent* evt) override;
    void update(Duration elapsed) override;

private:
    void addScreensToView(Graphics::View& view) const;
    void addSceneToView(Graphics::View& view) const;

private:
    Graphics::Renderer renderer_;
    QList<ScreenController> screen_controller_list_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_LOCAL_H
