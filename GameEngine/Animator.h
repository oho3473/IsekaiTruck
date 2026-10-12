#pragma once
#include <unordered_map>
#include <string>

#include "ElementBase.h"
#include "../DirectY/Sprite.h"	//Motion
#include "InputManager.h"	//keyinput에 따른 모션 변경을 위해 사용

/// <summary>
/// 애니메이션을 이루는 Motion 클래스를 관리하고 재생할 클래스
/// 2023.08.01 유승운
/// </summary>
class Animator : public ElementBase
{
public:
	Animator();
	~Animator();

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

	//모션을 추가한다
	void AddMotion(std::string motionName, Graphics::Motion* motion);
	//현재 모션을 지정한다
	void SetCurrentMotion(std::string motionName) { m_currentMotion = motionName; }
	//모션을 받아온다 - const 쓰면 [] 안써짐
	Graphics::Motion* GetMotion(const std::string key)/*const*/ { return m_motionMap[key]; }


	float GetTotalTime() const { return m_totalTime; }
	void SetTotalTime(float val) { m_totalTime = val; }

	float GetDelayTime() const { return m_delayTime; }
	void SetDelayTime(float val) { m_delayTime = val; }
private:
	std::unordered_map<std::string, Graphics::Motion*> m_motionMap;
	std::string m_currentMotion;
	int m_motionIndex;

	// gyu code
	float m_totalTime = 0;
	float m_delayTime = 0.2f;
};

