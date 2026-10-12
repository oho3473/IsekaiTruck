#pragma once
#include <ElementBase.h>
#include <vector>
#include "queue"

class NpcManager;
class SkinHeadNPC;

class NPCWaitManager : public ElementBase
{
public:
	NPCWaitManager(NpcManager* npcManager);
	~NPCWaitManager();

	/*------------------------------------------------------------*/
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
	/*------------------------------------------------------------*/

	// npc의 대기열을 배정한다.
	void SetWaitNumber(SkinHeadNPC* waitingNpc);
	// 충돌상태를 확인
	void CheckCollision();

	std::queue<SkinHeadNPC*>& GetWaitQueue() { return m_waitQueue; }
	void SetWaitQueue(std::queue<SkinHeadNPC*> val) { m_waitQueue = val; }

	void isPoP();

private:
	// npc들을 받아오기위한 npc매니저
	NpcManager* m_npcManager;
	SkinHeadNPC* firstNpc = nullptr;

	// 씬에 존재하는 모든 npc
	std::vector<SkinHeadNPC*> m_npcList;
	
	// 면접장에 있는 npc를 관리하기위한 queue
	std::queue<SkinHeadNPC*> m_waitQueue;
};

