#include "transform_component.h"

#include <glm/gtc/matrix_transform.hpp>

void TransformComponent::SetTranslation(glm::vec2 pos)
{
	_pos = pos;
	Recalculate();
}

void TransformComponent::SetScale(glm::vec2 scale)
{
	_scale = scale;
	Recalculate();
}

const glm::mat4 &TransformComponent::getTransform()
{
	return _transform;
}

void TransformComponent::Recalculate()
{
	_transform = glm::mat4(1.0f);
	_transform = glm::translate(_transform, glm::vec3(_pos.x, _pos.y, 0.0f));
	_transform = glm::scale(_transform, glm::vec3(_scale.x, _scale.y, 1.0f));
}

void TransformComponent::Move(float x, float y)
{
	_pos.x += x;
	_pos.y += y;
	Recalculate();
}

glm::vec2 TransformComponent::getPosition() const
{
	return _pos;
}

glm::vec2 TransformComponent::getSize() const
{
	return _scale;
}
