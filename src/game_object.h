#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <memory>
#include <glm/glm.hpp>
#include "gl_types.h"

class RenderComponent;
class TransformComponent;

class GameObject
{
public:
	GameObject();
	void SetPosition(glm::vec2);
	void SetSize(glm::vec2);
	void SetTexture(GLTexture);
	void Move(float x, float y);
	glm::vec2 getPosition() const;
	glm::vec2 getSize() const;

	std::shared_ptr<RenderComponent> &GetRenderComponent();
	const glm::mat4 &getTransform() const;

private:
	std::shared_ptr<RenderComponent> _render;
	std::shared_ptr<TransformComponent> _transform;	
};

#endif // GAME_OBJECT_H
