#ifndef VOLCANO_WORLD_ENVIRONMENT_H
#define VOLCANO_WORLD_ENVIRONMENT_H

#include <Volcano/World/Light.h>

VOLCANO_WORLD_BEGIN

class Environment: public QObject {
    Q_OBJECT
    Q_PROPERTY(Light* ambientLight READ ambientLight CONSTANT)

public:
    Light* ambientLight() noexcept;

protected:
    explicit Environment(QObject* parent = nullptr);

private:
    Light ambientLight_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_ENVIRONMENT_H
