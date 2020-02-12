#include "Camera.h"
#include <ext.hpp>


Camera::Camera()
{
	WorldTransform = glm::mat4(1);

		ViewTransform = glm::mat4(1);

		ProjectionTransform = glm::mat4(1);

		ProjectionViewTransform = glm::mat4(1);

		//variables

		//CameraPosition = glm::vec3(0);

		//CameraFront = glm::vec3(0);

		//CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		//model = glm::mat4(1);

		//projection = glm::mat4(1);

		speed = 1;
}

Camera::~Camera() {}

void Camera::Update(float deltatime, GLFWwindow* window)
{
	glm::vec4 displacement = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	//WorldTransform[3]

	//lets add a sprint yo
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
	{
		speed = 10;
		angularSpeed = 0.02f;
	}
	else
	{
		speed = 2;
		angularSpeed = 0.01f;
	}

	//INPUT commands yay
	displacement.y -= (glfwGetKey(window, GLFW_KEY_Q)); //up
	displacement.y += (glfwGetKey(window, GLFW_KEY_E)); //down

	displacement.x += (glfwGetKey(window, GLFW_KEY_D)); //left
	displacement.x -= (glfwGetKey(window, GLFW_KEY_A)); //right

	displacement.z -= (glfwGetKey(window, GLFW_KEY_W)); //forward
	displacement.z += (glfwGetKey(window, GLFW_KEY_S)); //backwards

	//make sure it accomodates direction (so if i look left W makes me go forward still instead of previous forward)
	glm::vec4 updatedDisplacement = -displacement.z * WorldTransform[2] + displacement.y * WorldTransform[1] - displacement.x * WorldTransform[0];
	glm::normalize(updatedDisplacement);

	//actually apply the movement change
	WorldTransform[3] -= (updatedDisplacement * speed * deltatime);

	//only update if the displacement has been modified
	if (displacement != glm::vec4(0.0f))
	{
		UpdateProjectionViewTransform();
	}

	//start of the mouse code wooooo
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

	if (timer > -1)
	{
		timer--;
	}

	
	
	//time to actually look with the mouse oo oo
	double cursorPositionX;
	double cursorPositionY;
	
	//get mouse position
	glfwGetCursorPos(window, &cursorPositionX, &cursorPositionY);

	//to do store resolution of camera?

	//calculate offset from screen centre this frame
	double deltaX = cursorPositionX - (1280 * 0.5);
	double deltaY = cursorPositionY - (720 * 0.5);

	glfwSetCursorPos(window, 1280 * 0.5, 720 * 0.5);

	if (firstMouseCall)
	{
		deltaX = 0;
		deltaY = 0;
		firstMouseCall = false;
	}

	if (deltaX || deltaY)
	{
		auto rotation = glm::mat4(1.0f);

		rotation = glm::rotate(rotation, float(angularSpeed * deltatime * -deltaX), glm::vec3(ViewTransform[1]));

		rotation = glm::rotate(rotation, float(angularSpeed * deltatime * -deltaY), glm::vec3(1.0f, 0.0f, 0.0f));

		WorldTransform *= rotation;

		UpdateProjectionViewTransform();
	}

	//ViewTransform = glm::inverse(WorldTransform);
}

void Camera::SetPerspective(float FieldOfView, float AspectRatio, float Near, float Far)
{
	//projection = glm::perspective(1.507f, 16 / 9.0f, 0.0f, 7.0f);
	ProjectionTransform = glm::perspective(FieldOfView, AspectRatio, Near, Far);
	UpdateProjectionViewTransform();
}

void Camera::SetLookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
	//CameraPosition = position;
	//target = glm::normalize(position - target);

	//glm::vec3 cameraRight = glm::normalize(glm::cross(up, target));
	//up = glm::cross(target, cameraRight);

	//CameraFront = target;
	//CameraUp = up;

	//solved error due to implementing <ext.hpp> after forgetting about it... oops
	ViewTransform = glm::lookAt(position, target, up);
	WorldTransform = glm::inverse(ViewTransform);
	UpdateProjectionViewTransform();

	//solved error due to implementing <ext.hpp> after forgetting about it... oops
	//WorldTransform = glm::lookAt(CameraPosition, CameraFront, CameraUp);
}

void Camera::SetPosition(glm::vec3 position)
{
	WorldTransform[3] = glm::vec4(position, 1);
	ViewTransform = glm::inverse(WorldTransform);
	UpdateProjectionViewTransform();
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
	//model = a_model;
	//ProjectionViewTransform = projection * ViewTransform/* * a_model*/;

	ViewTransform = glm::inverse(WorldTransform);
	ProjectionViewTransform = ProjectionTransform * ViewTransform;
}