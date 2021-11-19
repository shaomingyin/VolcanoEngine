//
//
#ifndef VOLCANO_SYSTEM_CLIENT_HPP
#define VOLCANO_SYSTEM_CLIENT_HPP

#include <QObject>

#include <Volcano/Graphics/Viewable.hpp>
#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class Client: public Graphics::Viewable {
    Q_OBJECT

public:
    Client(QObject *parent = nullptr);
    ~Client(void) override;

public:
    void buildVisibleSet(Graphics::VisibleSet &out, Graphics::Camera &cam) override;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_CLIENT_HPP
