#include "Camera.h"
#include <ext.hpp>

//constructor for the camera class, initalises variables
Camera::Camera()
{
	WorldTransform = glm::mat4(1);

	ViewTransform = glm::mat4(1);

	ProjectionTransform = glm::mat4(1);

	ProjectionViewTransform = glm::mat4(1);

	speed = 1;
}

//destructor for camera class
Camera::~Camera() {}

//update function
void Camera::Update(float deltatime, GLFWwindow* window)
{
	//create a displacement vector
	glm::vec4 displacement = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);


	//if the shift key is pressed (lets add a sprint yo)
	//The speeds are really high due to the size of the obj models present in the scene
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
	{
		//you now go faster
		speed = 300;
		angularSpeed = 0.02f;
	}
	else
	{
		//you go at a regular speed
		speed = 30;
		angularSpeed = 0.01f;
	}

	//if W,A,S,D,Q or E are pressed, move in relevant direction - (INPUT commands yay)
	displacement.y -= (glfwGetKey(window, GLFW_KEY_Q)); //up
	displacement.y += (glfwGetKey(window, GLFW_KEY_E)); //down

	displacement.x += (glfwGetKey(window, GLFW_KEY_D)); //left
	displacement.x -= (glfwGetKey(window, GLFW_KEY_A)); //right

	displacement.z -= (glfwGetKey(window, GLFW_KEY_W)); //forward
	displacement.z += (glfwGetKey(window, GLFW_KEY_S)); //backwards

	//make sure it accomodates direction (so if i look left W makes me go forward still instead of previous forward)
	glm::vec4 updatedDisplacement = -displacement.z * WorldTransform[2] + displacement.y * WorldTransform[1] - displacement.x * WorldTransform[0];
	//normalze the updated displacement
	glm::normalize(updatedDisplacement);

	//actually apply the movement change
	WorldTransform[3] -= (updatedDisplacement * speed * deltatime);

	//only update the projection view matrix if the displacement has been modified (if we moved the camera)
	if (displacement != glm::vec4(0.0f))
	{
		UpdateProjectionViewTransform();
	}

	//If the M key is pressed (start of the mouse code wooooo)
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && !mouseCatch && timer < 0)
	{
		//disable mouse
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		mouseCatch = true;
		timer = 10;
	}
	else if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && mouseCatch && timer < 0)
	{
		//enable mouse
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		mouseCatch = false;
		timer = 10;
	}

	//if timer is greater than -1, take away from timer
	if (timer > -1)
	{
		timer--;
	}


	//Creation of cursor positon doubles (time to actually look with the mouse oo oo)
	double cursorPositionX;
	double cursorPositionY;

	//get mouse position
	glfwGetCursorPos(window, &cursorPositionX, &cursorPositionY);


	//calculate offset from screen centre this frame (assumes size of 1280 by 720)
	double deltaX = cursorPositionX - (1280 * 0.5);
	double deltaY = cursorPositionY - (720 * 0.5);

	//set the cursor position to be the middle of the window
	glfwSetCursorPos(window, 1280 * 0.5, 720 * 0.5);

	//if this is the first mouse call, set the position of the cursor to be the middle to prevent funky start rotations
	if (firstMouseCall)
	{
		deltaX = 0;
		deltaY = 0;
		firstMouseCall = false;
	}

	//if the mouse has been moved
	if (deltaX || deltaY)
	{
		//create rotation variable
		auto rotation = glm::mat4(1.0f);

		//rotate the X of the camera to line up with the cursor position
		rotation = glm::rotate(rotation, float(angularSpeed * deltatime * -deltaX), glm::vec3(ViewTransform[1]));

		//rotate the Y of the camera to line up with the cursor position
		rotation = glm::rotate(rotation, float(angularSpeed * deltatime * -deltaY), glm::vec3(1.0f, 0.0f, 0.0f));

		//multiply the world transform by the rotation
		WorldTransform *= rotation;

		//updtae the projection view matrix
		UpdateProjectionViewTransform();
	}
}

//set the perspective of the camera. This should be used when creating the camera
void Camera::SetPerspective(float FieldOfView, float AspectRatio, float Near, float Far)
{
	//projection = glm::perspective(1.507f, 16 / 9.0f, 0.0f, 7.0f);
	//update the projection transform
	ProjectionTransform = glm::perspective(FieldOfView, AspectRatio, Near, Far);
	//update the projection view transform
	UpdateProjectionViewTransform();
}

//set where the camera is looking
void Camera::SetLookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{

	//look at the new postion
	ViewTransform = glm::lookAt(position, target, up);
	//update the world transform
	WorldTransform = glm::inverse(ViewTransform);
	//update the projection view transform
	UpdateProjectionViewTransform();


}

//set the position of the camera
void Camera::SetPosition(glm::vec3 position)
{
	//update the position of the camera
	WorldTransform[3] = glm::vec4(position, 1);
	//update the view transform
	ViewTransform = glm::inverse(WorldTransform);
	//upate the projection view transform
	UpdateProjectionViewTransform();
}


//gets the world transform
glm::mat4 Camera::GetWorldTransform()
{
	return WorldTransform;
}

//gets the view transform
glm::mat4 Camera::GetView()
{
	return ViewTransform;
}

//gets the projection transform
glm::mat4 Camera::GetProjection()
{
	return ProjectionTransform;
}

//gets the projection view transform
glm::mat4 Camera::GetProjectionView()
{
	return ProjectionViewTransform;
}

//updates the projection view transform
void Camera::UpdateProjectionViewTransform()
{
	//view transform is now the inverse of the world transform
	ViewTransform = glm::inverse(WorldTransform);
	//the projection view transform is the projection transform multiplied by the view transform
	ProjectionViewTransform = ProjectionTransform * ViewTransform;
}