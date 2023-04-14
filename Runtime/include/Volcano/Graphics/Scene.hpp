//
//
#ifndef VOLCANO_GRAPHICS_SCENE_HPP
#define VOLCANO_GRAPHICS_SCENE_HPP

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Context.hpp>

VOLCANO_GRAPHICS_BEGIN

class Scene: public QObject {
    Q_OBJECT

public:
    Scene(Context &context, QObject *parent = nullptr);
    ~Scene(void) override;

public:
    Context &context(void);
    const Context &context(void) const;

private:
    Context &m_context;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_SCENE_HPP
