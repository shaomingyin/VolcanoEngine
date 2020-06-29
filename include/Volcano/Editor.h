//
//
#ifndef VOLCANO_EDITOR_H
#define VOLCANO_EDITOR_H

#include <Volcano/Common.h>
#include <Volcano/Launcher.h>

VOLCANO_BEGIN

class Editor: public Launcher
{
    Q_OBJECT

public:
    Editor(void);
    ~Editor(void) override;

public:
    bool init(const QUrl &url) override;
};

VOLCANO_END

#endif // VOLCANO_EDITOR_H
