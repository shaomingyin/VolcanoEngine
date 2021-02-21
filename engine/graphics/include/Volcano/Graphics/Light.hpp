//
//
#ifndef VOLCANO_GRPHICS_LIGHT_HPP
#define VOLCANO_GRPHICS_LIGHT_HPP

#include <QObject>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_GRAPHICS_API Light: public QObject {
    Q_OBJECT

public:
    Light(QObject *parent = nullptr);
    ~Light(void) override;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRPHICS_LIGHT_HPP
