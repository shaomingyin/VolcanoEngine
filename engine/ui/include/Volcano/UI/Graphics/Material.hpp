//
//
#ifndef VOLCANO_UI_GRPHICS_MATERIAL_HPP
#define VOLCANO_UI_GRPHICS_MATERIAL_HPP

#include <QObject>

#include <Volcano/UI/Graphics/Common.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

class VOLCANO_UI_API Material: public QObject {
    Q_OBJECT

public:
    Material(QObject *parent = nullptr);
    ~Material(void) override;
};

VOLCANO_UI_GRAPHICS_END

#endif // VOLCANO_UI_GRPHICS_MATERIAL_HPP
