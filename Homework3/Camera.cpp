#include "Camera.h"


Camera::Camera()
{
	transform = new Transform(vec3(0.0, 0.0, 10.0), vec3(0), vec3(1.0));
	transform->Forward = { 0.0, 0.0, -1.0 };
}

Camera::~Camera()
{
}

void Camera::setProjection(float fieldOfView, float aspectRatio, float zNear, float zFar)
{

}

void Camera::update(double deltaSeconds)
{

}

void Camera::moveForward()
{

}

void Camera::moveBackward()
{

}

void Camera::moveRight()
{

}

void Camera::moveLeft()
{

}

void Camera::setRotation(float yaw, float pitch, float roll)
{

}

mat4 Camera::GetView()
{
	return lookAt(transform->Position, transform->Position + transform->Forward, transform->Up);
}