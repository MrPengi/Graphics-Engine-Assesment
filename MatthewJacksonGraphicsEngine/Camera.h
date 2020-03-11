#pragma once
#include "glm.hpp"
#include "glfw3.h"

class Camera
{
public:

	//world transform
	glm::mat4 WorldTransform;

	//view transform
	glm::mat4 ViewTransform;

	//projection transform
	glm::mat4 ProjectionTransform;

	//projection view transform
	glm::mat4 ProjectionViewTransform;

	//used with mouse catching
	float timer = 0;

	//bool to lock mouse to centre of screen
	bool mouseCatch = true;

	//rotation speed
	float angularSpeed = 0.01f;

	//is it the first mouse call?
	bool firstMouseCall = true;

	//speed the camera moves 
	float speed;

	//constructor
	Camera();

	//destructor
	~Camera();

	//camera update function
	void Update(float deltatime, GLFWwindow* window);

	//set perspective, used at start of creating
	void SetPerspective(float FieldOfView, float AspectRatio, float Near, float Far);
	
	//(requires CameraPos, direction and up/right to be setup
	void SetLookAt(glm::vec3 position, glm::vec3 direction, glm::vec3 up);

	//sets the position of the camera
	void SetPosition(glm::vec3 position);



	//gets the world transform of the camera
	glm::mat4 GetWorldTransform();

	//gets the view transform of the camera
	glm::mat4 GetView();

	//gets the projection transform of the camera
	glm::mat4 GetProjection();

	//gets the projection view transform
	glm::mat4 GetProjectionView();

	//updates the projection view matrix
	void UpdateProjectionViewTransform();

};

