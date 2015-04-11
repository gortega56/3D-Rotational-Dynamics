#pragma once
#include <glm\glm.hpp>
#include "Transform.h"

using namespace glm;

class Camera
{
public:
	mat4x4	projection;
	mat4x4	view;
	
	Transform* transform;

	Camera();
	~Camera();

	void setProjection(float fieldOfView, float aspectRatio, float zNear, float zFar);
	void update(double deltaSeconds);
	void moveForward();
	void moveBackward();
	void moveRight();
	void moveLeft();
	void setRotation(float yaw, float pitch, float roll);

	mat4 GetView();
private:
	mat4 View;
};

