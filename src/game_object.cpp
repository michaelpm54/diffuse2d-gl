#include "game_object.h"

#include "render_component.h"
#include "transform_component.h"

GameObject::GameObject() :
	_render(new RenderComponent()),
	_transform(new TransformComponent())
{}

void GameObject::SetPosition(glm::vec2 pos)
{
	_transform->SetTranslation(pos);
}

void GameObject::SetSize(glm::vec2 size)
{
	_transform->SetScale(size);
}

void GameObject::SetTexture(GLTexture tex)
{
	_render->SetTexture(tex);
}

std::shared_ptr<RenderComponent> &GameObject::GetRenderComponent()
{
	return _render;
}

const glm::mat4 &GameObject::getTransform() const
{
	return _transform->getTransform();
}

void GameObject::Move(float x, float y)
{
	_transform->Move(x, y);
}

glm::vec2 GameObject::getPosition() const
{
	return _transform->getPosition();
}

glm::vec2 GameObject::getSize() const
{
	return _transform->getSize();
}
