#pragma once
#include "ElementBase.h"
#include "Vector2.h"

namespace Graphics { struct Sprite; }
namespace Graphics { class Motion; }

class InterviewUI : public ElementBase
{
public:
	InterviewUI();
	~InterviewUI();

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

	//면접자가 바뀌때 마다 이미지 바꿔야 함
	void SetImage(Graphics::Motion* motion) { m_motion = motion; }

private:
	Graphics::Motion* m_motion;
	int m_index;
	float m_time;
};

