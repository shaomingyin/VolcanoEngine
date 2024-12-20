//
//
#ifndef VOLCANO_GRAPHICS_VIEW_H
#define VOLCANO_GRAPHICS_VIEW_H

#include <vector>

#include <QRectF>
#include <QVector3D>
#include <QMatrix4x4>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Camera.h>
#include <Volcano/Graphics/Light.h>
#include <Volcano/Graphics/DirectionalLight.h>
#include <Volcano/Graphics/PointLight.h>
#include <Volcano/Graphics/SpotLight.h>

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

    const QMatrix4x4& viewMatrix() {
		return view_matrix_;
	}

    const QMatrix4x4& viewMatrix() const {
		return view_matrix_;
	}

    QMatrix4x4& projectionMatrix() {
        return projection_matrix_;
	}

    const QMatrix4x4& projectionMatrix() const {
        return projection_matrix_;
	}

	const std::vector<DirectionalLight*>& directionalLights() const {
		return directional_lights_;
	}

	void addDirectionalLight(DirectionalLight* p) {
		directional_lights_.push_back(p);
	}

	const std::vector<PointLight*>& pointLights() const {
		return point_lights_;
	}

	void addPointLight(PointLight* p) {
		point_lights_.push_back(p);
	}

	const std::vector<SpotLight*>& spotLights() const {
		return spot_lights_;
	}

	void addSpotLight(SpotLight* p) {
		spot_lights_.push_back(p);
	}

	Light& ambientLight() {
		return ambient_light_;
	}

	const Light& ambientLight() const {
		return ambient_light_;
	}

private:
    QRectF viewport_;
    QMatrix4x4 view_matrix_;
    QMatrix4x4 projection_matrix_;
	std::vector<DirectionalLight*> directional_lights_;
	std::vector<PointLight*> point_lights_;
	std::vector<SpotLight*> spot_lights_;
	Light ambient_light_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VIEW_H
