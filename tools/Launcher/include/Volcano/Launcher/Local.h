//
//
#ifndef VOLCANO_LAUNCHER_LOCAL_H
#define VOLCANO_LAUNCHER_LOCAL_H

#include <async++.h>

#include <Volcano/Gui/Context.h>
#include <Volcano/World/Scene.h>
#include <Volcano/Launcher/Common.h>
#include <Volcano/Launcher/Console.h>

VOLCANO_LAUNCHER_BEGIN

class Local {
public:
    Local(World::Scene& scene);
    virtual ~Local() = default;

public:
    void run();
    unsigned long fps() const noexcept;
    unsigned long fpsMax() const noexcept;
    void setFpsMax(unsigned long v) noexcept;

protected:
    virtual void frame(Clock::duration elapsed);

private:
	async::fifo_scheduler scheduler_;
    async::task<void> loading_task_;
	World::Scene& scene_;
    Clock::duration elapsed_min_;
    Clock::time_point frame_last_;
    Clock::time_point frame_count_last_;
    unsigned long frame_count_;
    unsigned long frame_count_per_second_;
    unsigned long fps_max_ = 60;
    //Console* console_;
    //Gui::Context gui_context_;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_LOCAL_H
