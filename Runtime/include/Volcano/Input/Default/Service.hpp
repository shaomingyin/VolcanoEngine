//
//
#ifndef VOLCANO_INPUT_DEFAULT_SERVICE_HPP
#define VOLCANO_INPUT_DEFAULT_SERVICE_HPP

#include <Volcano/Input/Service.hpp>
#include <Volcano/Input/Default/Common.hpp>

VOLCANO_INPUT_DEFAULT_BEGIN

class Service: public Volcano::Input::Service {
    Q_OBJECT

public:
    Service(QObject *parent = nullptr);
    ~Service(void) override;
};

VOLCANO_INPUT_DEFAULT_END

#endif // VOLCANO_INPUT_DEFAULT_SERVICE_HPP
