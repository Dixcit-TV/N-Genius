#include "PCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

ngenius::GameObject::~GameObject() = default;

void ngenius::GameObject::Update(){}

void ngenius::GameObject::Render() const
{
	if (m_Texture)
	{
		const auto pos = m_Transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

void ngenius::GameObject::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void ngenius::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}
