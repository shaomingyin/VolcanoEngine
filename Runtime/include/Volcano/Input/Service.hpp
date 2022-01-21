//
//
#ifndef VOLCANO_INPUT_SERVICE_HPP
#define VOLCANO_INPUT_SERVICE_HPP

#include <Volcano/Service.hpp>
#include <Volcano/Input/Common.hpp>

VOLCANO_INPUT_BEGIN

class Service: public Volcano::Service {
    Q_OBJECT

public:
    Service(QObject *parent = nullptr);
    ~Service(void) override;
};

VOLCANO_INPUT_END

#endif // VOLCANO_INPUT_SERVICE_HPP
