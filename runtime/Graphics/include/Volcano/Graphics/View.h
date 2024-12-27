//
//
#ifndef VOLCANO_GRAPHICS_VIEW_H
#define VOLCANO_GRAPHICS_VIEW_H

#include <vector>

#include <QRectF>
#include <QMatrix4x4>

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class View {
public:
	View() = default;
	View(int w, int h);
	virtual ~View();

public:
	void reset();

    QRectF& viewport() {
		return viewport_;
	}

    const QRectF& viewport() const {
		return viewport_;
	}

    const QMatrix4x4& viewMatrix() const {
		return view_matrix_;
	}

    const QMatrix4x4& projectionMatrix() const {
        return projection_matrix_;
	}

private:
    QRectF viewport_;
    QMatrix4x4 view_matrix_;
    QMatrix4x4 projection_matrix_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VIEW_H
