#include "NpcManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include "GameEngine.h"
#include "Scene.h"
#include "Transform.h"
#include "MotionAnimator.h"
#include "NPC.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "SkinHeadNPC.h"
#include "Animator.h"
#include "CarNpc.h"
#include "AudioSource.h"
#include "CollisionManager.h"

bool CompareNpcData(const NpcData& n1, const NpcData& n2)
{
	return n1.pathNumber < n2.pathNumber;
}


NpcManager::NpcManager(GameEngine* gameEngine, Scene* scene, DirectY* directY, NpcType type /*= NpcType::Person*/)
	: m_type(type)
{
	m_gameEngine = gameEngine;
	m_scene = scene;
	m_directY = directY;
	m_collisionManager = m_gameEngine->GetCollisionManager();
}

NpcManager::~NpcManager()
{
	for (unsigned int i = 0; i < m_npcPathData.size(); i++)
	{
		m_npcPathData[i].clear();
	}
	m_npcPathData.clear();

	for (unsigned int i = 0; i < m_npcData.size(); i++)
	{
		m_npcData[i].clear();
	}
	m_npcData.clear();

	//for (auto& m : m_motions)
	//{
	//	delete m;
	//}
	//m_motions.clear();
}

void NpcManager::Init()
{
	/*Graphics::Motion* motion = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile("Npc/1_back_left_walking", m_gameEngine->GetImage("1_back_left_walking"), motion);
	int a = 3;
	return;*/
	//MakeAnimation(TODO);

}

void NpcManager::FixedUpdate()
{

}

void NpcManager::Update(float deltaTime)
{

}

void NpcManager::LateUpdate()
{

}

void NpcManager::PreRender()
{

}

void NpcManager::Render(DirectY* directY)
{

}

void NpcManager::PostRender(DirectY* directY)
{

}

void NpcManager::Release()
{

}

ElementType NpcManager::GetElementType() const
{
	return ElementType::Script;
}

GameObject* NpcManager::GetGameObject() const
{
	return m_gameObject;
}

void NpcManager::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}

void NpcManager::ReadSource(std::string path)
{
	std::ifstream ifs;
	ifs.open(path);

	// test
	std::string name;
	ifs >> name;
	std::string temp;
	int size;
	ifs >> temp >> size >> temp >> size;
	m_mapSize = size;
	m_npcPathData = std::vector<std::vector<NpcPathData>>(size, std::vector<NpcPathData>(size));
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			ifs >> m_npcPathData[y][x].npcIndex;
			ifs >> m_npcPathData[y][x].npcPathIndex;
			ifs >> m_npcPathData[y][x].npcID;
		}
	}
	ifs.close();

	switch (m_type)
	{
		case NpcType::Person:
			MakeNpcPath();
			break;
		case NpcType::Car:
			MakeCarPath();
			break;
	}

}

void NpcManager::MakeNpcPath()
{
	// 0번에 이 npc가 무슨 캐릭터 id인지 정함

	for (int y = 0; y < m_mapSize; y++)
	{
		for (int x = 0; x < m_mapSize; x++)
		{
			if (m_npcPathData[y][x].npcIndex == -1) continue;
			// 현재 맵에 추가된 npc의 인덱스 값
			int npcIndex = m_npcPathData[y][x].npcIndex;
			// npc의 최대 카운트를 구함
			m_npcCount = max(m_npcCount, npcIndex + 1);
			// npc의 현재 경로가 몇번째 인덱스인지
			int npcPathIndex = m_npcPathData[y][x].npcPathIndex;
			m_npcData[npcIndex].push_back({ npcPathIndex,y,x });
			// npc의 아이디를 설정
			//if (npcPathIndex == 1)
			m_npcData[npcIndex][0].npcID = m_npcPathData[y][x].npcID;

		}
	}

	// npc카운트 만큼 각각의 npc 경로 순서를 정렬
	for (int i = 0; i < m_npcCount; i++)
	{
		sort(m_npcData[i].begin(), m_npcData[i].end(), CompareNpcData);
	}
}

void NpcManager::MakeCarPath()
{
	for (int y = 0; y < m_mapSize; y++)
	{
		for (int x = 0; x < m_mapSize; x++)
		{
			if (m_npcPathData[y][x].npcIndex == -1) continue;
			int npcIndex = m_npcPathData[y][x].npcIndex;
			m_npcCount = max(m_npcCount, npcIndex + 1);
			int npcPathIndex = m_npcPathData[y][x].npcPathIndex;
			m_npcData[npcIndex].push_back({ npcPathIndex,y,x });

			m_npcData[npcIndex][0].npcID = m_npcPathData[y][x].npcID;
		}
	}

	// npc카운트 만큼 각각의 npc 경로 순서를 정렬
	for (int i = 0; i < m_npcCount; i++)
	{
		sort(m_npcData[i].begin(), m_npcData[i].end(), CompareNpcData);
	}
}

void NpcManager::MakeObject()
{
	switch (m_type)
	{
		case NpcType::Person:
			MakeNpcObject();
			break;
		case NpcType::Car:
			MakeCarObject();
			break;
		default:
			break;
	}
}

void NpcManager::MakeNpcObject()
{
	for (int i = 0; i < m_npcCount; i++)
	{
		// 이름 설정
		std::string npcName = "Npc_";
		npcName += std::to_string(i);

		// 게임오브젝트
		GameObject* npcObject = m_gameEngine->CreateObject(npcName, m_scene);
		// 트랜스폼
		Transform* npcTransform = new Transform();
		npcObject->SetElement(npcTransform, ElementType::Transform);
		npcTransform->SetUseCamera(true);

		// skinHeadNpc
		SkinHeadNPC* skinHead = new SkinHeadNPC(npcTransform);
		npcObject->SetElement(skinHead, ElementType::Script);

		///오디오소스 붙이기
		AudioSource* npcAudioSource = new AudioSource(m_gameEngine);
		npcObject->SetElement(npcAudioSource, ElementType::AudioSource);
		skinHead->SetAudioSource(npcAudioSource);
		npcAudioSource->AddSound("sfx_hit_npc_1");

		skinHead->SetPrintLog(m_printLog);

		m_npcs.push_back(skinHead);

		// 아래 주석과 for문 주석, SkinHeadNpc의 스케일 조정으로 오차범위 조정
		// scale 1, 1 => 0, 105, 72    ,    72, 0
		// scale 0.5, 0.5 => 0, 65, 36	,	72, 54
		// 
		//Collider* skinHeadCollider = m_gameEngine->GetCollisionManager()->CreateIsometricCollider(0, 105, 72);

		// collider
		//Collider* skinHeadCollider = m_gameEngine->GetCollisionManager()->CreateIsometricCollider(0, 65, 36);
		Collider* skinHeadCollider = m_gameEngine->GetCollisionManager()->CreateIsometricCollider(0, 65, 54);
		m_gameEngine->GetCollisionManager()->AddCollider(npcObject, skinHeadCollider);
		npcObject->SetElement(skinHeadCollider, ElementType::Collider);

		int PrevNpcId = 0;
		for (int pathIdx = 0; pathIdx < (int)m_npcData[i].size(); pathIdx++)
		{
			// 현재의 경로를 포지션으로 변환하기 전에 npc에 추가
			skinHead->AddPathArr(m_npcData[i][pathIdx].x, m_npcData[i][pathIdx].y);

			//Vector2 pathPos = { 72,0 };
			Vector2 pathPos = { 72,54 };
			pathPos.y += (m_npcData[i][pathIdx].y * 36);
			pathPos.x -= (m_npcData[i][pathIdx].y * 72);
			pathPos.y += (m_npcData[i][pathIdx].x * 36);
			pathPos.x += (m_npcData[i][pathIdx].x * 72);

			skinHead->AddPath(pathPos);

			if (m_npcData[i][pathIdx].npcID != 0) PrevNpcId = m_npcData[i][pathIdx].npcID;
		}
		npcTransform->SetLocalPosition(skinHead->GetPaths()[0]);
		npcTransform->SetLayer(5);




		// Npc의 아이디를 불러옴.
		// Npc의 Id를 수정하려면 및 테스트 여기서 하셈.
		//int npcID = m_npcData[i][0].npcID;
		int npcID = PrevNpcId;
		//int npcID = 8;
		if (npcID <= 9)
		{
			skinHead->SetHero(true);
		}

		skinHead->SetNpcID(npcID);

		// 애니메이션 추가
		Animator* animator = new Animator();
		npcObject->SetElement(animator, ElementType::Animator);
		//skinHead->SetAnimator(animator);

		if (npcID == 0)
			int a = 3;

		for (int animationIdx = 1; animationIdx <= 4; animationIdx++)
		{
			std::string motionName = "";
			switch (animationIdx)
			{
				case 1:
					motionName = "_back_left_walking";
					break;
				case 2:
					motionName = "_back_right_walking";
					break;
				case 3:
					motionName = "_front_left_walking";
					break;
				case 4:
					motionName = "_front_right_walking";
					break;
				default:
					break;
			}
			/*if (m_motionsIndex == m_npcCount) {
				m_motionsIndex = 0;
			}*/

			Graphics::Motion* motion = MakeNpcAnimation(npcID, animationIdx);
			animator->AddMotion(motionName, motion);

			// draw Pos를 바꿔줌

			// 고양이만 피봇 위치가 다름!
			if (npcID == 4)
			{
				for (int motionIdx = 0; motionIdx < motion->GetTotalFrame(); motionIdx++)
				{
					motion->GetSprite(motionIdx)->m_drawPos.y -= motion->GetSprite(motionIdx)->m_pivot.y / 2;
					motion->GetSprite(motionIdx)->m_drawPos.x -= motion->GetSprite(motionIdx)->m_pivot.x / 2;
				}
			}
			else
			{
				for (int motionIdx = 0; motionIdx < motion->GetTotalFrame(); motionIdx++)
				{
					motion->GetSprite(motionIdx)->m_drawPos.y -= motion->GetSprite(motionIdx)->m_pivot.y;
					motion->GetSprite(motionIdx)->m_drawPos.x -= motion->GetSprite(motionIdx)->m_pivot.x;
				}
			}
			//m_motionsIndex++;
		}
		skinHead->SetAnimator(animator);
		skinHead->Init();

		// 애니메이션
		//Graphics::Motion* npcMotion = new Graphics::Motion();
		// drawPos를 바꿔준다.
		/*for (int motionIdx = 0; motionIdx < npcMotion->GetTotalFrame(); motionIdx++)
		{
			npcMotion->GetSprite(motionIdx)->m_drawPos.x -= npcMotion->GetSprite(motionIdx)->m_pivot.x;
			npcMotion->GetSprite(motionIdx)->m_drawPos.y -= npcMotion->GetSprite(motionIdx)->m_pivot.y;
		}*/
	}
}

void NpcManager::MakeNpcObjectInCredit()
{
	int creditNpcCount = 10;
	for (int i = 0; i < creditNpcCount; i++)
	{
		// 이름 설정
		std::string npcName = "Npc_";
		npcName += std::to_string(i);

		// 게임오브젝트
		GameObject* npcObject = m_gameEngine->CreateObject(npcName, m_scene);
		// 트랜스폼
		Transform* npcTransform = new Transform();
		npcObject->SetElement(npcTransform, ElementType::Transform);
		npcTransform->SetUseCamera(true);
		if (i < 5)
		{
			npcTransform->SetLocalPosition({ 640.f + 160 * i,670.f + -60 * i});
		}
		else
		{
			npcTransform->SetLocalPosition({ 760.f + 160 * (i - 5),760.f + -60 * (i - 5) });
		}
		// skinHeadNpc
		SkinHeadNPC* skinHead = new SkinHeadNPC(npcTransform);
		skinHead->ChangeState(SkinHeadNPC::State::Credit);
		npcObject->SetElement(skinHead, ElementType::Script);

		///오디오소스 붙이기
		AudioSource* npcAudioSource = new AudioSource(m_gameEngine);
		npcObject->SetElement(npcAudioSource, ElementType::AudioSource);
		skinHead->SetAudioSource(npcAudioSource);
		npcAudioSource->AddSound("sfx_hit_npc_1");

		skinHead->SetPrintLog(m_printLog);

		m_npcs.push_back(skinHead);

		//int PrevNpcId = 0;
		//for (int pathIdx = 0; pathIdx < (int)m_npcData[i].size(); pathIdx++)
		//{
		//	// 현재의 경로를 포지션으로 변환하기 전에 npc에 추가
		//	skinHead->AddPathArr(m_npcData[i][pathIdx].x, m_npcData[i][pathIdx].y);

		//	//Vector2 pathPos = { 72,0 };
		//	Vector2 pathPos = { 72,54 };
		//	pathPos.y += (m_npcData[i][pathIdx].y * 36);
		//	pathPos.x -= (m_npcData[i][pathIdx].y * 72);
		//	pathPos.y += (m_npcData[i][pathIdx].x * 36);
		//	pathPos.x += (m_npcData[i][pathIdx].x * 72);

		//	skinHead->AddPath(pathPos);

		//	if (m_npcData[i][pathIdx].npcID != 0) PrevNpcId = m_npcData[i][pathIdx].npcID;
		//}
		//npcTransform->SetLocalPosition(skinHead->GetPaths()[0]);
		//npcTransform->SetLayer(5);




		// Npc의 아이디를 불러옴.
		// Npc의 Id를 수정하려면 및 테스트 여기서 하셈.
		//int npcID = m_npcData[i][0].npcID;
		int npcID = i + 21;

		skinHead->SetNpcID(npcID);

		// 애니메이션 추가
		Animator* animator = new Animator();
		npcObject->SetElement(animator, ElementType::Animator);

		for (int animationIdx = 1; animationIdx <= 4; animationIdx++)
		{
			std::string motionName = "";
			switch (animationIdx)
			{
				case 1:
					motionName = "_back_left_walking";
					break;
				case 2:
					motionName = "_back_right_walking";
					break;
				case 3:
					motionName = "_front_left_walking";
					break;
				case 4:
					motionName = "_front_right_walking";
					break;
				default:
					break;
			}

			Graphics::Motion* motion = MakeNpcAnimation(npcID, animationIdx);
			animator->AddMotion(motionName, motion);

			// draw Pos를 바꿔줌

			// 고양이만 피봇 위치가 다름!
			if (npcID == 4)
			{
				for (int motionIdx = 0; motionIdx < motion->GetTotalFrame(); motionIdx++)
				{
					motion->GetSprite(motionIdx)->m_drawPos.y -= motion->GetSprite(motionIdx)->m_pivot.y / 2;
					motion->GetSprite(motionIdx)->m_drawPos.x -= motion->GetSprite(motionIdx)->m_pivot.x / 2;
				}
			}
			else
			{
				for (int motionIdx = 0; motionIdx < motion->GetTotalFrame(); motionIdx++)
				{
					motion->GetSprite(motionIdx)->m_drawPos.y -= motion->GetSprite(motionIdx)->m_pivot.y;
					motion->GetSprite(motionIdx)->m_drawPos.x -= motion->GetSprite(motionIdx)->m_pivot.x;
				}
			}
		}
		skinHead->SetAnimator(animator);
		skinHead->Init();
	}
}

void NpcManager::MakeCarObject()
{
	for (int i = 0; i < m_npcCount; i++)
	{
		// 이름 설정
		std::string carName = "Car_";
		carName += std::to_string(i);

		// 게임오브젝트
		GameObject* carObject = m_gameEngine->CreateObject(carName, m_scene);
		// 트랜스폼
		Transform* carTransform = new Transform();
		carObject->SetElement(carTransform, ElementType::Transform);
		carTransform->SetUseCamera(true);
		carTransform->SetLayer(500);

		// skinHeadNpc
		/*SkinHeadNPC* skinHead = new SkinHeadNPC(carTransform);
		carObject->SetElement(skinHead, ElementType::Script);

		skinHead->SetPrintLog(m_printLog);*/

		//m_npcs.push_back(skinHead);

		CarNpc* car = new CarNpc(carTransform);
		carObject->SetElement(car, ElementType::Script);



		// 아래 주석과 for문 주석, SkinHeadNpc의 스케일 조정으로 오차범위 조정
		// scale 1, 1 => 0, 105, 72    ,    72, 0
		// scale 0.5, 0.5 => 0, 65, 36	,	72, 54
		// 
		//Collider* skinHeadCollider = m_gameEngine->GetCollisionManager()->CreateIsometricCollider(0, 105, 72);

		// collider
		//Collider* skinHeadCollider = m_gameEngine->GetCollisionManager()->CreateIsometricCollider(0, 65, 36);
		Collider* skinHeadCollider = m_gameEngine->GetCollisionManager()->CreateIsometricCollider(0, 65, 54);
		m_gameEngine->GetCollisionManager()->AddCollider(carObject, skinHeadCollider);
		carObject->SetElement(skinHeadCollider, ElementType::Collider);
		
		int PrevNpcId = 0;
		for (int pathIdx = 0; pathIdx < (int)m_npcData[i].size(); pathIdx++)
		{
			// 현재의 경로를 포지션으로 변환하기 전에 npc에 추가
			car->AddPathArr(m_npcData[i][pathIdx].x, m_npcData[i][pathIdx].y);

			//Vector2 pathPos = { 72,0 };
			Vector2 pathPos = { 72,54 };
			pathPos.y += (m_npcData[i][pathIdx].y * 36);
			pathPos.x -= (m_npcData[i][pathIdx].y * 72);
			pathPos.y += (m_npcData[i][pathIdx].x * 36);
			pathPos.x += (m_npcData[i][pathIdx].x * 72);

			car->AddPath(pathPos);

			if (m_npcData[i][pathIdx].npcID != 0) PrevNpcId = m_npcData[i][pathIdx].npcID;
		}
		//carTransform->SetLocalPosition(car->GetPaths()[0]);
		
		carTransform->SetLocalPosition(car->GetPaths()[0]);
		carTransform->SetLayer(5);



		// Npc의 아이디를 불러옴.
		//int npcID = m_npcData[i][0].npcID;
		int npcID = PrevNpcId;
		//int npcID = 8;

		car->SetCarID(npcID);
		// Npc의 Id를 수정하려면 여기서 하셈.



		// 자동차는 애니메이터를 쓰지 않고 모션을 가져다가 씀. 따라서 모션을 만들어주자.
		Graphics::Motion* motion = new Graphics::Motion();
		std::string filePath = "Car/";
		//std::string fileIdx = std::to_string(i + 1) + "_Car";
		std::string fileIdx = std::to_string(npcID) + "_Car";
		m_directY->ReadSpriteTxtFile(filePath + fileIdx, m_gameEngine->GetImage(fileIdx), motion);

		car->SetMotion(motion);

		for (int motionIdx = 0; motionIdx < motion->GetTotalFrame(); motionIdx++)
		{
			motion->GetSprite(motionIdx)->m_drawPos.y -= motion->GetSprite(motionIdx)->m_pivot.y / 4;
			motion->GetSprite(motionIdx)->m_drawPos.x -= motion->GetSprite(motionIdx)->m_pivot.x;
		}
	}
}
Graphics::Motion* NpcManager::MakeNpcAnimation(int npcId, int animIndex)
{
	std::string filePath = std::string("Npc/");
	std::string fileIdx = std::to_string(npcId);
	std::string fileName = "";
	switch (animIndex)
	{
		case 1:
			fileName = "_back_left_walking";
			break;
		case 2:
			fileName = "_back_right_walking";
			break;
		case 3:
			fileName = "_front_left_walking";
			break;
		case 4:
			fileName = "_front_right_walking";
			break;
		default:
			break;
	}

	fileIdx += fileName;
	Graphics::Motion* motion = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile(filePath + fileIdx, m_gameEngine->GetImage(fileIdx), motion);
	//m_motions.push_back(motion);
	return motion;
}

Graphics::Motion* NpcManager::MakeCarAnimation(int carId, int animIndex)
{
	std::string filePath = std::string("Car/");
	std::string fileIdx = std::to_string(carId + 1);
	std::string fileName = "";
	/*switch (animIndex)
	{
	case 0:
		fileName = "_back_left_walking";
		break;
	case 1:
		fileName = "_back_right_walking";
		break;
	case 2:
		fileName = "_front_left_walking";
		break;
	case 3:
		fileName = "_front_right_walking";
		break;
	default:
		break;
	}*/

	fileIdx += fileName;
	Graphics::Motion* motion = new Graphics::Motion();
	m_directY->ReadSpriteTxtFile(filePath + fileIdx, m_gameEngine->GetImage(fileIdx), motion);
	//m_motions.push_back(motion);
	return motion;
}
