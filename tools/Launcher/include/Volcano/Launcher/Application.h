//
//
#ifndef VOLCANO_LAUNCHER_APPLICATION_H
#define VOLCANO_LAUNCHER_APPLICATION_H

#include <memory>
#include <string>

#include <Volcano/System/Client.h>
#include <Volcano/Launcher/Common.h>

VOLCANO_LAUNCHER_BEGIN

class Application: public System::Client {
public:
    struct Options {
        bool threaded = false;
    };

public:
    Application(const Options& options);
    virtual ~Application();

public:
    bool init(const std::string& title, int width, int height) override;

protected:
    void frame(Duration elapsed) override;

private:
    Options options_;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_APPLICATION_H
