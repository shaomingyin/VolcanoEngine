//
//
#ifndef VOLCANO_GRAPHICS_MATERIAL_HPP
#define VOLCANO_GRAPHICS_MATERIAL_HPP

#include <QObject>

#include <Volcano/Game/Material.hpp>
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Material: public QObject {
    Q_OBJECT

public:
    Material(Game::Material *p, QObject *parent = nullptr);
    ~Material(void) override;

public:
    Game::Material *gameMaterial(void);

private slots:
    void onSourceChanged(const QUrl &v);

private:
    Game::Material *m_gameMaterial;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MATERIAL_HPP
