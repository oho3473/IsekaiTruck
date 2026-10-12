#include "GameObject.h"
#include "../DirectY/DirectY.h"

GameObject::GameObject(std::string name)
	: m_name(name),
	m_elements((int)ElementType::End, nullptr)
{

}

GameObject::~GameObject()
{
	Release();
}

void GameObject::Init()
{
	for (int i = 0; i < (int)ElementType::End; i++)
	{
		if (m_elements[i] != nullptr)
		{
			m_elements[i]->Init();
		}
	}
}

void GameObject::FixedUpdate()
{
	// 일단은 물리 충돌만 FixedUpdate를 쓴다. 
	// m_elements[(int)ElementType::Collider]->FixedUpdate();
}

void GameObject::Update(float deltaTime)
{
	if (m_isActive)
	{
		for (int i = 0; i < (int)ElementType::End; i++)
		{
			if (m_elements[i] != nullptr)
			{
				m_elements[i]->Update(deltaTime);
			}
		}
	}
}

void GameObject::LateUpdate()
{
	for (int i = 0; i < (int)ElementType::End; i++)
	{
		if (m_elements[i] != nullptr)
		{
			m_elements[i]->LateUpdate();
		}
	}
}

/// 일단은 전체를 Render 하는데 Render를 하는 Element가 정해지면 그것들만 하게 수정
void GameObject::PreRender()
{
	for (int i = 0; i < (int)ElementType::End; i++)
	{
		if (m_elements[i] != nullptr)
		{
			m_elements[i]->PreRender();
		}
	}
}

void GameObject::Render(DirectY* directY)
{
	if (m_isDraw)
	{
		for (int i = 0; i < (int)ElementType::End; i++)
		{
			if (m_elements[i] != nullptr)
			{
				m_elements[i]->Render(directY);
			}
		}
	}
}

void GameObject::PostRender(DirectY* directY)
{
	for (int i = 0; i < (int)ElementType::End; i++)
	{
		if (m_elements[i] != nullptr)
		{
			m_elements[i]->PostRender(directY);
		}
	}
}

void GameObject::Release()
{
	for (int i = 0; i < (int)ElementType::End; i++)
	{
		if (m_elements[i] != nullptr)
		{
			delete m_elements[i];
		}
	}
}

void GameObject::SetElement(ElementBase* element, ElementType type)
{
	m_elements[(int)type] = element; 
	element->SetGameObject(this);
}
