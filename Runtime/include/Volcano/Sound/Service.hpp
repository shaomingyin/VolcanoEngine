//
//
#ifndef VOLCANO_SOUND_SERVICE_HPP
#define VOLCANO_SOUND_SERVICE_HPP

#include <Volcano/Service.hpp>
#include <Volcano/Sound/Common.hpp>

VOLCANO_SOUND_BEGIN

class Service: public Volcano::Service {
    Q_OBJECT

public:
    Service(QObject *parent = nullptr);
    ~Service(void) override;
};

VOLCANO_SOUND_END

#endif // VOLCANO_SOUND_SERVICE_HPP
