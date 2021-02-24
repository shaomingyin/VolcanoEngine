//
//
#ifndef VOLCANO_UI_GRPHICS_VIEW_HPP
#define VOLCANO_UI_GRPHICS_VIEW_HPP

#include <QObject>

#include <Volcano/UI/Graphics/Common.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

class VOLCANO_UI_API View: public QObject {
    Q_OBJECT

public:
    View(QObject *parent = nullptr);
    ~View(void) override;
};

VOLCANO_UI_GRAPHICS_END

#endif // VOLCANO_UI_GRPHICS_VIEW_HPP
