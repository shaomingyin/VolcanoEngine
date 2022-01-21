//
//
#ifndef VOLCANO_SOUND_OPENAL_SERVICE_HPP
#define VOLCANO_SOUND_OPENAL_SERVICE_HPP

#include <Volcano/Sound/Service.hpp>
#include <Volcano/Sound/OpenAL/Common.hpp>

VOLCANO_SOUND_OPENAL_BEGIN

class Service: public Volcano::Sound::Service {
    Q_OBJECT

public:
    Service(QObject *parent = nullptr);
    ~Service(void) override;
};

VOLCANO_SOUND_OPENAL_END

#endif // VOLCANO_SOUND_OPENAL_SERVICE_HPP
