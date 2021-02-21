//
//
#ifndef VOLCANO_GRPHICS_VIEW_HPP
#define VOLCANO_GRPHICS_VIEW_HPP

#include <QObject>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_GRAPHICS_API View: public QObject {
    Q_OBJECT

public:
    View(QObject *parent = nullptr);
    ~View(void) override;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRPHICS_VIEW_HPP
