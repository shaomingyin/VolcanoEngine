//
//
#ifndef VOLCANO_GAME_MATERIAL_HPP
#define VOLCANO_GAME_MATERIAL_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

class Material: public Object {
    Q_OBJECT

public:
    Material(QObject *parent = nullptr);
    ~Material(void) override;

protected:
    void draw(void) override;

private:
    Graphics::Material *m_graphicsMaterial;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_MATERIAL_HPP
