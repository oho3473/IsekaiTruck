#include "Button.h"

#include "InputManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "AudioSource.h"

#include "../MathEngine/Vector2.h"
#include "../DirectY/DirectY.h"

#include "SoundManager.h"

using namespace MathEngine;

Button::Button(int left, int top, int right, int bottom)
	:m_left(left), m_right(right), m_top(top), m_bottom(bottom),
	m_buttonState(ButtonState::Normal),
	m_sprite(nullptr), m_audioSource(nullptr),
	m_onClick([]() {}), m_onPress([]() {})
{

}

Button::~Button()
{
	if (m_sprite != nullptr)
	{
		delete m_sprite;
	}
}


void Button::Init()
{

}

void Button::FixedUpdate()
{

}

void Button::Update(float deltaTime)
{
	// 기본 이었다면
	if (m_buttonState != ButtonState::Disabled)
	{
		Vector2 curMousePosition = InputManager::GetInstance().GetMousePosition();
		if (curMousePosition.x > m_left && curMousePosition.x < m_right &&
			curMousePosition.y > m_top && curMousePosition.y < m_bottom)
		{
			if (InputManager::GetInstance().IsGetKey(VK_LBUTTON))
			{
				// 버튼 영역 밖에서 누르면서 들어왔을 때 pressed 함수가 작동하는 것을 막음
				if (m_buttonState == ButtonState::Pressed)
				{
					m_buttonState = ButtonState::Pressed;
					OnPressed();
				}
			}
			if (m_buttonState == ButtonState::Normal)
			{
				m_buttonState = ButtonState::Selected;
				m_audioSource->PlayBackVector(0);
			}
			if (InputManager::GetInstance().IsGetKeyDown(VK_LBUTTON))
			{
				m_buttonState = ButtonState::Pressed;
				OnPressed();
			}
			// 이전 상태가 getkey down 또는 getkey 일때
			if (InputManager::GetInstance().IsGetKeyUp(VK_LBUTTON))
			{
				if (m_buttonState == ButtonState::Pressed)
				{
					OnClick();
					m_buttonState = ButtonState::Selected;
					m_audioSource->PlayBackVector(1);
				}
			}
		}
		else
		{
			m_buttonState = ButtonState::Normal;
		}
	}
	// 마우스가 위에있다면
	// selecte로 바꾸고
	// 마우스 입력이 있다면
	// 확인해서 press, onclick 실행하기
	// 펑터를 초기화할 때 멤버 변수로 가지고 있어야 하나?
}

void Button::LateUpdate()
{

}

void Button::PreRender()
{

}

void Button::Render(DirectY* directY)
{
	if (m_sprite != nullptr)
	{
		directY->DrawBitmap(m_sprite);
		((Transform*)(m_gameObject->GetElement(ElementType::Transform)))->SetLocalPosition({ (float)m_left,(float)m_top });
	}
}

void Button::PostRender(DirectY* directY)
{

}

void Button::Release()
{

}

ElementType Button::GetElementType() const
{
	return ElementType::Button;
}

GameObject* Button::GetGameObject() const
{
	return m_gameObject;
}

void Button::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}

void Button::OnClick()
{
	m_onClick();
}

void Button::OnPressed()
{
	m_onPress();
}

void Button::SetOnClick(std::function<void(void)> onClick)
{
	m_onClick = onClick;
}

void Button::SetOnPressed(std::function<void(void)> onPress)
{
	m_onPress = onPress;
}

