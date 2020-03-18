#pragma once
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/transform.hpp>
#include <iostream>
#include<glm/gtx/string_cast.hpp>

class Camera
{
public:
	Camera(const glm::vec3& pos = glm::vec3(0, 10, -60), float fov = 70.0f, float aspect = (float)1400 / (float)900, float zNear = 0.01f, float zFar = 1000.0f)
	{
		m_perspective = glm::perspective(fov, aspect, zNear, zFar);
		m_up = glm::vec3(0, 1, 0);
		m_position = pos;
		m_forward = glm::vec3(0, 0, 1);
		m_view = glm::lookAt(m_position, m_position + m_forward, m_up);
		cam_target = glm::vec3(0.0f, 0.0f, 0.0f);
		direction = glm::normalize(m_position - m_forward);
		cam_right = glm::normalize(glm::cross(m_up, direction));
	};


	inline void TurnLeft(float unit)
	{
		//	m_view = glm::rotate(m_view,0.01f*glm::radians(3.14159f*(unit/720)),glm::vec3(0,1,0));

	}

	inline glm::vec3 LookAtPoint() {
		return m_position + m_forward;
	}

	inline glm::mat4 GetM_View()
	{
		return m_view;

	}
	inline void UpdateView()
	{
		//if (!turningleft)
		m_view = glm::lookAt(m_position, m_position + m_forward, m_up);

	}

	inline glm::vec3* GetCameraPos()
	{

		return &m_position;

	}

	inline glm::vec3 getDirection()
	{

		return direction;
	}


	inline glm::mat4 GetPerspective() const {
		return m_perspective;
	}

		inline glm::mat4 GetViewProject() const
	{

		return m_perspective * m_view;

	}

	inline glm::vec3 SetM_U(float unit)
	{

		m_up += unit;
		return m_up;

	}
	inline glm::vec3 SetM_Forward(float unit)
	{

		m_forward.z += unit;
		return m_forward;

	}

	inline glm::vec3 * GetM_U()
	{


		return &m_up;

	}
	inline glm::vec3 * GetM_Forward()
	{


		return &m_forward;

	}

	inline glm::vec3 * GetM_Right()
	{
		return &cam_right;
	}


	float cam_speed;

	bool turningleft = false;
	glm::mat4 m_perspective;
	glm::mat4 m_view;
	float xzangle = (float)(3.14159f/2);
	float xyangle = (float)(3.141519f/2);
protected:
private:

	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_up;
	glm::vec3 direction;
	glm::vec3 cam_right;
	glm::vec3 cam_target;
	glm::vec3 cam_up;

	float yaw, pitch, roll;


};

