#include "Camera.h"


Camera::Camera(){}

Camera::~Camera() {}

void Camera::Update(float deltatime)
{
	


}

void Camera::SetPerspective(float FieldOfView, float AspectRatio, float Near, float Far)
{

}

void Camera::SetLookAt(glm::vec3 From, glm::vec3 To, glm::vec3 Up)
{
	//cant find lookAt???:??????????????
	//ViewTransform = glm::lookAt(From, To, Up);
}

void Camera::SetPosition(glm::mat4 position)
{
	WorldTransform = position;
}

void Camera::SetCameraPos(glm::vec3 position)
{
	cameraPos = position;
}

void Camera::SetCameraDirection(glm::vec3 target)
{
	cameraDirection = glm::normalize(cameraPos - target);
}

glm::mat4 Camera::GetWorldTransform()
{
	return WorldTransform;
}

glm::mat4 Camera::GetView()
{
	return ViewTransform;
}

glm::mat4 Camera::GetProjection()
{
	return ProjectionTransform;
}

glm::mat4 Camera::GetProjectionView()
{
	return ProjectionViewTransform;
}

void Camera::UpdateProjectionViewTransform()
{

}