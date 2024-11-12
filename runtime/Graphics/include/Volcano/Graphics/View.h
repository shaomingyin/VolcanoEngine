//
//
#ifndef VOLCANO_GRAPHICS_VIEW_H
#define VOLCANO_GRAPHICS_VIEW_H

#include <vector>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Camera.h>
#include <Volcano/Graphics/Canvas.h>
#include <Volcano/Graphics/Light.h>
#include <Volcano/Graphics/DirectionalLight.h>
#include <Volcano/Graphics/PointLight.h>
#include <Volcano/Graphics/SpotLight.h>
#include <Volcano/Graphics/RenderableObject.h>

VOLCANO_GRAPHICS_BEGIN

class View {
public:
	View() = default;
	View(int w, int h);
	virtual ~View();

public:
	void reset();

	Eigen::Vector4i& viewport() {
		return viewport_;
	}

	const Eigen::Vector4i& viewport() const {
		return viewport_;
	}

	Eigen::Matrix4f& viewMatrix() {
		return view_matrix_;
	}

	const Eigen::Matrix4f& viewMatrix() const {
		return view_matrix_;
	}

	Eigen::Matrix4f& projectionMatrix() {
		projection_matrix_;
	}

	const Eigen::Matrix4f& projectionMatrix() const {
		projection_matrix_;
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

	const std::vector<RenderableObject*>& renderableObjects() const {
		return renderable_objects_;
	}

	void addRenderableObject(RenderableObject* p) {
		renderable_objects_.push_back(p);
	}

	Light& ambientLight() {
		return ambient_light_;
	}

	const Light& ambientLight() const {
		return ambient_light_;
	}

	Canvas& hud() {
		return hud_;
	}

	const Canvas& hud() const {
		return hud_;
	}

private:
	Eigen::Vector4i viewport_;
	Eigen::Matrix4f view_matrix_;
	Eigen::Matrix4f projection_matrix_;
	std::vector<DirectionalLight*> directional_lights_;
	std::vector<PointLight*> point_lights_;
	std::vector<SpotLight*> spot_lights_;
	std::vector<RenderableObject*> renderable_objects_;
	Light ambient_light_;
	Canvas hud_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VIEW_H
