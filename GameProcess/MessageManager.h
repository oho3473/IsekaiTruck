#pragma once
#include "ElementBase.h"

#include <unordered_map>
#include <string>
#include <fstream>
#include <ostream>
#include <sstream>
#include <queue>

class SkinHeadNPC;
class NPCWaitManager;
class GameObject;

class MessageManager : public ElementBase
{
public:
	MessageManager(NPCWaitManager* npcWaitManager, GameObject* dialogManager);
	~MessageManager();

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

	std::unordered_map < int, std::unordered_map<std::string, std::string>>& GetDialogs() { return m_dialogs; }
	std::string GetDialog(int line, std::string cell) { return m_dialogs[line][cell]; }
	void ReadDialogCSV(std::string path);
	void ReadMessageCSV(std::string path);

	void SetQueue(std::queue<SkinHeadNPC*>& queue) { m_interviewQueue = &queue; };
	void SetClearCount(int val) { m_clearCount = val; }
	int GetClearCount()const { return m_clearCount; }
	std::queue<bool>& CheckNPCMessage() { return m_IsNPCMessage; }

private:
	std::unordered_map < int,std::unordered_map<std::string, std::string>> m_dialogs;
	std::unordered_map < int,std::unordered_map<std::string, std::string>> m_messages;
	std::queue<SkinHeadNPC*>* m_interviewQueue;

	std::vector<bool> m_useMessage;
	std::queue<bool> m_IsNPCMessage;
	std::queue<std::string> m_chatMessages;

	int m_clearCount;
	NPCWaitManager* m_npcWaitManager;
	GameObject* m_dialogManager;

private:
	void Pop();


};

