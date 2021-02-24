//
//
#ifndef VOLCANO_UI_GRPHICS_LIGHT_HPP
#define VOLCANO_UI_GRPHICS_LIGHT_HPP

#include <QObject>

#include <Volcano/UI/Graphics/Common.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

class VOLCANO_UI_API Light: public QObject {
    Q_OBJECT

public:
    Light(QObject *parent = nullptr);
    ~Light(void) override;
};

VOLCANO_UI_GRAPHICS_END

#endif // VOLCANO_UI_GRPHICS_LIGHT_HPP
