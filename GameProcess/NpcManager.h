#pragma once
#include "../DirectY/DirectY.h"
#include "../GameEngine/ElementBase.h"

#include <functional>

class ResourceManager;
class GameEngine;

class SkinHeadNPC;

struct NpcPathData
{
	int npcIndex;
	int npcPathIndex;
	int npcID;
};

struct NpcData
{
	int pathNumber;
	int y, x;
	int npcID;
};

enum class NpcType
{
	Person,
	Car,
};

class Scene;
class CollisionManager;
class NpcManager : public ElementBase
{
public:
	NpcManager(GameEngine* gameEngine, Scene* scene, DirectY* directY, NpcType type = NpcType::Person);
	virtual ~NpcManager();

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
	
	void SetPrintLog(std::function<void(std::wstring, bool) > printLog) { m_printLog = printLog; }

public:
	/// <summary>
	/// Npc와 Npc의 경로를 가지고 있는 데이터를 불러온다.
	/// </summary>
	void ReadSource(std::string path);

	void MakeNpcPath();
	void MakeCarPath();

	void MakeObject();

	/// <summary>
	/// npc를 현재 씬에 생성
	/// </summary>
	void MakeNpcObject();

	void MakeNpcObjectInCredit();

	/// <summary>
	/// 차를 현재 씬에 생성
	/// </summary>
	void MakeCarObject();

	std::vector<SkinHeadNPC*> GetNpcs() const { return m_npcs; }
private:
	// 테스트용으로 헤더에서 일단 모두 초기화
	const int m_maxNpcNumber = 100;
	int m_mapSize = 0;
	int m_npcCount = 0;
	// 모든 npc와 그 npc의 경로를 가지고 있는 데이터
	std::vector<std::vector<NpcPathData>> m_npcPathData;
	// [npc][경로]
	std::vector<std::vector<NpcData>> m_npcData
		= std::vector<std::vector<NpcData>>(m_maxNpcNumber);

	//std::vector<GameObject*> m_npcs;
	std::vector<SkinHeadNPC*> m_npcs = {};

	GameEngine* m_gameEngine = nullptr;
	Scene* m_scene = nullptr;
	DirectY* m_directY = nullptr;

	std::function<void(std::wstring,bool)> m_printLog;

	// 모션 vector에 미리 저장하고 사용!
	std::vector<Graphics::Motion*> m_motions;
	int m_motionsIndex = 0;

	Graphics::Motion* MakeNpcAnimation(int npcId, int animIndex);
	Graphics::Motion* MakeCarAnimation(int carId, int animIndex);
	// npc 캐릭터 종류 개수!
	const int m_AnimationCount = 30;



	// 현재 npcmanager의 종류
	NpcType m_type;

	// collision manager

	CollisionManager* m_collisionManager = nullptr;
};

