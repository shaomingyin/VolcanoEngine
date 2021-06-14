//
//
#ifndef VOLCANO_SERVER_APPLICATION_HPP
#define VOLCANO_SERVER_APPLICATION_HPP

#include <QCoreApplication>

#include <Volcano/Server/Common.hpp>

VOLCANO_SERVER_BEGIN

class Application: public QCoreApplication {
    Q_OBJECT

public:
    Application(int &argc, char *argv[]);
    ~Application(void) override;

public:
    bool init(void);
};

VOLCANO_SERVER_END

#endif // VOLCANO_SERVER_APPLICATION_HPP
