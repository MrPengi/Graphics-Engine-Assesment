#pragma once
#include "glm.hpp"
#include "glfw3.h"

class Camera
{
public:

	glm::mat4 WorldTransform;

	glm::mat4 ViewTransform;

	glm::mat4 ProjectionTransform;

	glm::mat4 ProjectionViewTransform;

	float timer = 0;

	bool mouseCatch = true;

	float angularSpeed = 0.01f;

	bool firstMouseCall = true;
	//variables

	
	//glm::vec3 CameraPosition;
	
	//glm::vec3 CameraFront;

	//glm::vec3 CameraUp;

	//glm::mat4 projection;

	//glm::mat4 model;

	float speed;

	//constructor
	Camera();

	//destructor
	~Camera();

	void Update(float deltatime, GLFWwindow* window);

	//
	void SetPerspective(float FieldOfView, float AspectRatio, float Near, float Far);
	
	// (requires CameraPos, direction and up/right to be setup
	void SetLookAt(glm::vec3 position, glm::vec3 direction, glm::vec3 up);

	
	void SetPosition(glm::vec3 position);



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

