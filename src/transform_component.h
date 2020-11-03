#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <glm/glm.hpp>

class TransformComponent
{
public:
	void SetTranslation(glm::vec2);
	void SetScale(glm::vec2);
	const glm::mat4 &getTransform();
	void Move(float x, float y);
	glm::vec2 getPosition() const;
	glm::vec2 getSize() const;

private:
	void Recalculate();

	glm::mat4 _transform { 1.0f };
	glm::vec2 _pos { 0.0f };
	glm::vec2 _scale { 1.0f };
};

#endif // TRANSFORM_COMPONENT_H
