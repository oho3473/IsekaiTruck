#pragma once
#include <ElementBase.h>
#include <queue>
#include "Sprite.h"

class SkinHeadNPC;

class PotraitUI :
    public ElementBase
{
public:
	PotraitUI();
	~PotraitUI();

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

	void SetInterviewList(std::queue<SkinHeadNPC*>& queue) { m_interviewlist = &queue; }
	void SetPotraitGroup(Graphics::Motion* motion) { m_potraitGroup = motion; }

private:
	std::queue<SkinHeadNPC*>* m_interviewlist;
	Graphics::Motion* m_potraitGroup;
};

