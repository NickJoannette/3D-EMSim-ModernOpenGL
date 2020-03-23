#pragma once

class Electron {
public:
	Electron() { charge = 0.05; mass = 0.05; };
	Transform * getTransform() { return &transform; };
	float * getCharge() { return &charge; };
	void invertCharge() { charge *= -1; }
	void setPosition(glm::vec3 v) { position = v; }
    glm::vec3 * getPos() { return &position; }
	float mass;
	glm::vec3 velocity;
	float charge;
	float radius;

protected:
	Transform transform;
	glm::vec3 netForce;
	glm::vec3 center;
	glm::vec3 position;
	glm::vec3 acceleration;
};
