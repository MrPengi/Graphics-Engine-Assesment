#include "Camera.h"
#include <ext.hpp>


Camera::Camera()
{
	WorldTransform = glm::mat4(1);

		ViewTransform = glm::mat4(1);

		ProjectionTransform = glm::mat4(1);

		ProjectionViewTransform = glm::mat4(1);

		//variables

		CameraPosition = glm::vec3(0);

		CameraFront = glm::vec3(0);

		CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		model = glm::mat4(1);

		projection = glm::mat4(1);

		speed = 1;
}

Camera::~Camera() {}

void Camera::Update(float deltatime, GLFWwindow* window)
{
	
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		CameraPosition.z += speed * deltatime;
		
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		CameraPosition.z -= speed * deltatime;

	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		CameraPosition.x -= speed * deltatime;
		//CameraPosition -= glm::normalize(glm::cross(CameraFront, CameraUp)) * speed * deltatime;
	
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		CameraPosition.x += speed * deltatime;
		//CameraPosition += glm::normalize(glm::cross(CameraFront, CameraUp)) * speed * deltatime;
	
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		CameraPosition.y -= speed * deltatime;
		//CameraPosition -= glm::normalize(glm::cross(CameraFront, CameraUp)) * speed * deltatime;
	
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		CameraPosition.y += speed * deltatime;
		//CameraPosition += glm::normalize(glm::cross(CameraFront, CameraUp)) * speed * deltatime;
	
	}
	//SetLookAt(CameraPosition, glm::vec3(0) * glm::vec3(0,0,-1), glm::vec3(0, 1, 0));
	WorldTransform = glm::lookAt(CameraPosition, glm::vec3(0), glm::vec3(0, 1, 0));

	/*if (WorldTransform[0][1] > 0.01 || WorldTransform[0][1] < -0.01)
	{
		int i = 0;
		i = i;
	}*/

	ViewTransform = glm::inverse(WorldTransform);
}

void Camera::SetPerspective(float FieldOfView, float AspectRatio, float Near, float Far)
{
	//projection = glm::perspective(1.507f, 16 / 9.0f, 0.0f, 7.0f);
	projection = glm::perspective(FieldOfView, AspectRatio, Near, Far);
}

void Camera::SetLookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
	CameraPosition = position;
	target = glm::normalize(position - target);

	glm::vec3 cameraRight = glm::normalize(glm::cross(up, target));
	up = glm::cross(target, cameraRight);

	CameraFront = target;
	CameraUp = up;

	//solved error due to implementing <ext.hpp> after forgetting about it... oops
	WorldTransform = glm::lookAt(CameraPosition, CameraFront, CameraUp);
}

void Camera::SetPosition(glm::mat4 position)
{
	//?
	WorldTransform = position;
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

void Camera::UpdateProjectionViewTransform(glm::mat4 a_model)
{
	model = a_model;
	ProjectionViewTransform = projection * ViewTransform/* * a_model*/;
}