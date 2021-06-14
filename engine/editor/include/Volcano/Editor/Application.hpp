//
//
#ifndef VOLCANO_EDITOR_APPLICATION_HPP
#define VOLCANO_EDITOR_APPLICATION_HPP

#include <QApplication>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class Application: public QApplication {
    Q_OBJECT

public:
    Application(int &argc, char *argv[]);
    ~Application(void) override;

public:
    bool init(void);
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_APPLICATION_HPP
