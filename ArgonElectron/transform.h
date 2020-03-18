#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/transform.hpp>

class Transform {
public:
	Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
		: pos(pos), rot(rot), scale(scale) {}

	// Model matrix sent to vertex shader
	inline glm::mat4 GetModel() const
	{
		glm::mat4 posMatrix = glm::translate(pos);

		glm::mat4 rotX = glm::rotate(rot.x, glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 rotY = glm::rotate(rot.y, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 rotZ = glm::rotate(rot.z, glm::vec3(0.0, 0.0, 1.0));

		glm::mat4 rotMatrix = rotX * rotY * rotZ;

		glm::mat4 scaleMatrix = glm::scale(scale);

		return posMatrix * rotMatrix * scaleMatrix;
	}

	inline glm::vec3* GetPos() { return &pos; }
	inline glm::vec3* GetRot() { return &rot; }
	inline glm::vec3* GetScale() { return &scale; }

	inline bool XIsLevel() {
		if (rot != glm::vec3(0, 0, 0)) {
			if (rot.x != 0) return false;
			else return true;
		}
		else return true;
	}

	inline bool YIsLevel() {
		if (rot != glm::vec3(0, 0, 0)) {
			if (rot.y != 0) return false;
			else return true;
		}
		else return true;
	}

	inline bool ZIsLevel() {
		if (rot != glm::vec3(0, 0, 0)) {
			if (rot.z != 0) return false;
			else return true;
		}
		else return true;
	}
	inline void SetPos(const glm::vec3& pos) { this->pos = pos; }
	inline void SetRot(const glm::vec3& rot) { this->rot = rot; }
	inline void SetScale(const glm::vec3& scale) { this->scale = scale; }

protected:
private:

	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;

};

#endif