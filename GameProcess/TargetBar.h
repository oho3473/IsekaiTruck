#pragma once
#include "ElementBase.h"
#include <queue>
#include "Sprite.h"

class SkinHeadNPC;
class NpcManager;
class MessageManager;
class NPCWaitManager;

class TargetBar : public ElementBase
{
public:

	TargetBar(MessageManager* messagemanager, NpcManager* npcmanager, NPCWaitManager* npcwaitmanager);
	~TargetBar();

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

	void SetImage(const Graphics::Motion* image) { m_image = image; }
	void SetTonedown(const Graphics::Motion* image) { m_tonedown = image; }
	void SetBackGround(const Graphics::Motion* image) { m_backGround = image; }

private:
	NpcManager* m_npcList;
	MessageManager* m_messageManager;
	NPCWaitManager* m_npcWaitManager;
	//정답 리스트
	std::vector<SkinHeadNPC*> m_answerList;
	//면접자 리스트
	std::queue<SkinHeadNPC*> m_interViewList;

	std::vector<bool> m_useList;


	int m_ClearCount;

	const Graphics::Motion* m_image;
	const Graphics::Motion* m_tonedown;
	const Graphics::Motion* m_backGround;

	bool m_setpos = false;
};

