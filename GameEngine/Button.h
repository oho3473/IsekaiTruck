#pragma once
#include "ElementBase.h"
#include <functional>

namespace Graphics { struct Sprite; }
namespace Graphics { class Motion; }
class AudioSource;

enum class ButtonState
{
	Normal,		// 기본
	Pressed,	// 눌림
	Selected,	// 마우스가 위에 있음
	Disabled	// 상호 작용 x
};

template <typename ReturnType, typename ParameterType>
class EventFuction
{
	std::function<ReturnType(ParameterType)> m_function;
};

class Button : public ElementBase
{
public:
	Button(int left, int top, int right, int bottom); // 너비, 높이 중심점
	~Button(); 

	/// Element Override
	virtual void Init() override;

	virtual void FixedUpdate() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate() override;

	virtual void PreRender() override;
	virtual void Render(DirectY* directY) override;
	virtual void PostRender(DirectY* directY) override;

	virtual void Release() override;

	virtual ElementType GetElementType() const override;

	virtual GameObject* GetGameObject() const override;
	virtual void SetGameObject(GameObject* gameObject) override;

	// 클릭 했을 때 실행할 내용 (미리 람다로 등록해둔 내용 실행)
	void OnClick();
	void OnPressed();	// 눌리는 중

	void SetDisabled() { m_buttonState = ButtonState::Disabled; }
	void SetEnable() { m_buttonState = ButtonState::Normal; }

	void SetOnClick(std::function<void(void)> onClick);
	void SetOnPressed(std::function<void(void)> onPress);

//	void OnNormal();	// 평소 상태
//	void OnSelected();	// 마우스가 위에 있음
//	void OnDisabled();	// 상호작용 안될 때

	//test
	Graphics::Sprite* m_sprite;
	void SetAudioSource(AudioSource* audioSource) { m_audioSource = audioSource; }

private:
	// 버튼 영역 - circle, ellipse 모양의 버튼도 필요할까
	int m_left;
	int m_right;
	int m_top;
	int m_bottom;

	std::function<void(void)> m_onClick;
	std::function<void(void)> m_onPress;

	// 사용해야 될 때가 오면 Get, Set 만들어주기
	ButtonState m_buttonState;
	AudioSource* m_audioSource;
	

	
};