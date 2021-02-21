//
//
#ifndef VOLCANO_GRPHICS_MATERIAL_HPP
#define VOLCANO_GRPHICS_MATERIAL_HPP

#include <QObject>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_GRAPHICS_API Material: public QObject {
    Q_OBJECT

public:
    Material(QObject *parent = nullptr);
    ~Material(void) override;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRPHICS_MATERIAL_HPP
