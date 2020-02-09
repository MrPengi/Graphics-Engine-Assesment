#pragma once
#include "glm.hpp"

class Camera
{
	glm::mat4 WorldTransform;

	glm::mat4 ViewTransform;

	glm::mat4 ProjectionTransform;

	glm::mat4 ProjectionViewTransform;

	glm::vec3 cameraPos;
	
	glm::vec3 cameraDirection;

	glm::vec3 up;

	glm::vec3 cameraRight;

	//constructor
	Camera();

	//destructor
	~Camera();

	void Update(float deltatime);

	void SetPerspective(float FieldOfView, float AspectRatio, float Near, float Far);

	void SetLookAt(glm::vec3 From, glm::vec3 To, glm::vec3 Up);

	void SetPosition(glm::mat4 position);

	void SetCameraPos(glm::vec3 position);

	void SetCameraDirection(glm::vec3 target);

	//gets the world transform of the camera
	glm::mat4 GetWorldTransform();

	//gets the view transform of the camera
	glm::mat4 GetView();

	//gets the projection transform of the camera
	glm::mat4 GetProjection();

	//gets the projection view transform
	glm::mat4 GetProjectionView();

	void UpdateProjectionViewTransform();

};

