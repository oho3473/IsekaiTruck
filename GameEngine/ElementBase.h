#pragma once

enum class ElementType
{
	Transform,
	Animator,

	// UI 용, 분리를 하는 게 맞을까
	Button,
	Slider,
	Script,
	Scrollbar,
	Collider,
	// special ? Object? Mine? Script? 
	// 아무튼 각 오브젝트의 개별 스크립트. 
	// 상속 구조를 이용해서 만들어야 할 것 같다.
	
	// 오디오 소스는 렌더 하지 않는다.
	AudioSource,
	
	End
};

class GameObject;
class DirectY;

/// <summary>
/// 모든 Element가 상속 받기 위한 클래스
/// 
/// Entity Component System의 component와 헷갈릴 수 있어서 이름을 다르게 만들었다.
/// </summary>
class ElementBase
{
public:
	virtual void Init() abstract;

	virtual void FixedUpdate() abstract;
	virtual void Update(float deltaTime) abstract;
	virtual void LateUpdate() abstract;

	virtual void PreRender() abstract;
	virtual void Render(DirectY* directY) abstract;
	virtual void PostRender(DirectY* directY) abstract;

	virtual void Release() abstract;

	virtual ElementType GetElementType() const abstract;

	virtual GameObject* GetGameObject() const abstract;
	virtual void SetGameObject(GameObject* gameObject) abstract;

	virtual ~ElementBase() {};

protected:
	ElementBase() : m_gameObject(nullptr) {}

	// 이 element를 가지고 있는 Object
	GameObject* m_gameObject;
};