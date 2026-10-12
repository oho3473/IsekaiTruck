#include "Slider.h"

#include "GameObject.h"
#include "Transform.h"
#include "InputManager.h"
#include "../MathEngine/Vector2.h"
#include "../DirectY/Sprite.h"
#include "../DirectY/DirectY.h"

Slider::Slider(int left, int right, int top, int bottom, int controllerX, int controllerY, float ratio)
	:m_isControll(false),
	m_background(nullptr), m_fillArea(nullptr), m_handle(nullptr),
	m_myTransform(nullptr),
	m_areaRatio(ratio)
{
	m_backgroundLeft = left;
	m_backgroundRight = right;
	m_backgroundTop = top;
	m_backgroundBottom = bottom;
	
}

Slider::~Slider()
{
	delete m_background;
	delete m_fillArea;
	delete m_handle;
}

void Slider::Init()
{
	m_myTransform = ((Transform*)(m_gameObject->GetElement(ElementType::Transform)));

	m_backgroundLeft = (int)(m_backgroundLeft + m_myTransform->GetParent()->GetLocalPosition().x + m_myTransform->GetLocalPosition().x);
	m_backgroundRight = (int)(m_backgroundRight + m_myTransform->GetParent()->GetLocalPosition().x + m_myTransform->GetLocalPosition().x);
	m_backgroundTop = (int)(m_backgroundTop + m_myTransform->GetParent()->GetLocalPosition().y + m_myTransform->GetLocalPosition().y);
	m_backgroundBottom = (int)(m_backgroundBottom + m_myTransform->GetParent()->GetLocalPosition().y + m_myTransform->GetLocalPosition().y);
	
	//m_areaRatio = 0.5;
	m_fillArea->m_drawSize.right= m_background->m_drawSize.right * m_areaRatio;
	m_handle->m_drawPos.x = m_background->m_drawSize.right * m_areaRatio - 12;
	m_handle->m_drawPos.y = -12;

	m_gameObject->SetIsDraw(false);
	m_gameObject->SetActive(false);
}

void Slider::FixedUpdate()
{

}

void Slider::Update(float deltaTime)
{
	// if 마우스가 컨트롤러 위에 있고 클릭했으면
	Vector2 mousePosition = InputManager::GetInstance().GetMousePosition();
	if (mousePosition.x >= m_backgroundLeft && mousePosition.x <= m_backgroundRight 
		&& mousePosition.y >= m_backgroundTop - 10 && mousePosition.y <= m_backgroundBottom + 10)
	{
		if (InputManager::GetInstance().IsGetKeyDown(VK_LBUTTON))
		{
			m_isControll = true;
		}
		// 클릭하는 동안 따라다니기 (배경 영역 안에서)
		else if (InputManager::GetInstance().IsGetKey(VK_LBUTTON) && m_isControll)
		{
			m_handle->m_drawPos.x = mousePosition.x - m_handle->m_pivot.x - m_myTransform->GetWorldPosition().x;
			m_fillArea->m_drawSize.right = mousePosition.x - m_myTransform->GetWorldPosition().x;
			m_fillArea->m_drawArea.right = mousePosition.x - m_myTransform->GetWorldPosition().x;

			m_areaRatio = (mousePosition.x - m_myTransform->GetWorldPosition().x) / (m_backgroundRight - m_backgroundLeft);
		}
		else
		{
			m_isControll = false;
		}
	}
	else
	{
		m_isControll = false;
	}
}

void Slider::LateUpdate()
{

}

void Slider::PreRender()
{

}

void Slider::Render(DirectY* directY)
{
	directY->DrawBitmap(m_background);
	directY->DrawBitmap(m_fillArea);
	directY->DrawBitmap(m_handle);
}

void Slider::PostRender(DirectY* directY)
{

}

void Slider::Release()
{
}


ElementType Slider::GetElementType() const
{
	return ElementType::Slider;
}

GameObject* Slider::GetGameObject() const
{
	return m_gameObject;
}

void Slider::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}

void Slider::OnChange()
{
	m_onChange();
}

void Slider::SetOnChange(std::function<void(void)> onChange)
{
	m_onChange = onChange;
}

void Slider::SetBackground(Graphics::Sprite* background)
{
	m_background = background;
}

void Slider::SetFillArea(Graphics::Sprite* fillArea)
{
	m_fillArea = fillArea;
}

void Slider::SetHandle(Graphics::Sprite* handle)
{
	m_handle = handle;
}

void Slider::SetDraw()
{
	m_gameObject->SetIsDraw(false);
	m_gameObject->SetActive(false);
}
