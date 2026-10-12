#include <algorithm>

#include "CollisionManager.h"

#include "GameObject.h"
#include "Collider.h"
#include "Transform.h"
#include "../GameProcess/Player.h"

#include "../DirectY/DirectY.h"

#include "../PhysicsEngine/CircleCollider.h"
#include "../PhysicsEngine/RectangleCollider.h"
#include "../PhysicsEngine/IsometricCollider.h"
#include "../PhysicsEngine/VehicleCollider.h"
#include "../PhysicsEngine/CollisionPoints.h"

#include "../MathEngine/MathUtils.h"

#include "CameraManager.h"
#include "../GameProcess/DebugManager.h"
#include "../GameProcess/Police.h"

//#include <string>
CollisionManager::CollisionManager()
	: m_playerObject(nullptr)
	, m_playerCollider(nullptr)
	, m_playerNormal({})
{

}

CollisionManager::~CollisionManager()
{
	for (auto& collision : m_colliders)
	{
		// 건물의 충돌체는 따로 지워준다.
		if (collision.first->GetName() == "TileManager")
		{
			delete collision.second;
		}
	}
}

void CollisionManager::Init()
{
	for (auto& collision : m_colliders)
	{
		// 건물의 충돌체는 따로 지워준다.
		if (collision.first->GetName() == "TileManager")
		{
			delete collision.second;
		}
	}
	m_colliders.clear();
}

void CollisionManager::Update(float deltaTime)
{
	/// 예외 처리
	if (m_colliders.empty()) return;
	
	float val = ((Player*)(m_playerObject->GetElement(ElementType::Script)))->GetSpeed();
	//if (/*MathEngine::isEqual(val, 0.f) &&*/ m_playerCollider->GetIsCollision() == false) return;

	bool isReversed = false;

	int collisionCount = 0;
	m_playerNormal = {};

	/// 플레이어 vs 다른 게임 오브젝트
	for (const auto& collider : m_colliders)
	{
		const PhysicsEngine::Collider& testCollider =
			*collider.second->GetCollider();

		PhysicsEngine::CollisionPoints point = 
			m_playerCollider->GetCollider()->TestCollision(testCollider);

		// 충돌한 경우
		if (point.hasCollision)
		{
			// 다른 게임 오브젝트의 충돌은 참
			collider.second->SetIsCollision(true);
			ResolveCollsion(collider.first, point, deltaTime, isReversed);
			if (collider.first->GetName() == "TileManager")
			{
				collisionCount++;
			}
			if (collider.first->GetName().find("Car_") != std::string::npos)
			{
				collisionCount++;
			}
		}
		else
		{
			collider.second->SetIsCollision(false);
		}
	}

	Player* player = ((Player*)(m_playerObject->GetElement(ElementType::Script)));
	if (collisionCount > 0)
	{
		player->SetMoveVector(m_playerNormal.Normalize());
		player->SetStun(true);
	}

	isReversed = false;

	// 플레이어의 충돌여부 검사
	for (const auto& collider : m_colliders)
	{
		if (collider.second->GetIsCollision())
		{
			m_playerCollider->SetIsCollision(true);
			return;
		}

		m_playerCollider->SetIsCollision(false);
	}
}

void CollisionManager::ResolveCollsion(
	GameObject* obj,
	const PhysicsEngine::CollisionPoints& cp,
	const float& dt,
	bool& isTurnDirection)
{
	if (obj->GetName() == "TileManager")
	{
		m_playerNormal += cp.normal;
	}

	if (obj->GetName() == "Police")
	{
		//Police* police = (Police*)(obj->GetElement(ElementType::Script));
		//police->ChangeState(Police::State::Collision);
	}

	if (obj->GetName() == "시민")
	{
		/* 예시: 시민을 씬에서 삭제한다...*/

		//delete otherObject;
	}

	if (obj->GetName().find("Car_") != std::string::npos)
	{
		m_playerNormal += cp.normal;
	}

}

void CollisionManager::Render(DirectY& directY)
{
	if (DebugManager::GetInstance().GetIsDebuging())
	{
		directY.SetTransform(CameraManager::GetInstance().GetCameraMatrix());
		for (auto& collider : m_colliders)
		{
			if (collider.first->GetName() == "TileManager")
				collider.second->Render(&directY);
		}
	}
}

void CollisionManager::Release()
{

}

void CollisionManager::AddCollider(
	GameObject* gameObject, Collider* collider)
{
	m_colliders.insert({ gameObject, collider });
}

void CollisionManager::DeleteCollider(GameObject* gameObject)
{
	m_colliders.erase(gameObject);
}

Collider* CollisionManager::CreateCircleCollider(
	MathEngine::Vector2 center, float r)
{
	// 반환할 게임 엔진의 충돌체
	std::unique_ptr<Collider> tempCollider =
		std::make_unique<Collider>();

	// 물리 엔진의 충돌체
	std::unique_ptr<PhysicsEngine::Collider> engineCollider =
		std::make_unique<PhysicsEngine::CircleCollider>(center, r);
	tempCollider->SetCollider(std::move(engineCollider));

	// 할당 해제 및 소유권 이전
	return tempCollider.release();
}

Collider* CollisionManager::CreateRectangleCollider(
	float x, float y, float width, float height)
{
	// 반환할 게임 엔진의 충돌체
	std::unique_ptr<Collider> tempCollider =
		std::make_unique<Collider>();

	// 물리 엔진의 충돌체
	std::unique_ptr<PhysicsEngine::Collider> engineCollider =
		std::make_unique<PhysicsEngine::RectangleCollider>(
			x, y, width, height);
	tempCollider->SetCollider(std::move(engineCollider));

	// 할당 해제 및 소유권 이전
	return tempCollider.release();
}

Collider* CollisionManager::CreateIsometricCollider(
	float x, float y, float height)
{
	// 반환할 게임 엔진의 충돌체
	std::unique_ptr<Collider> tempCollider = 
		std::make_unique<Collider>();

	// 물리 엔진의 충돌체
	std::unique_ptr<PhysicsEngine::Collider> engineCollider =
		std::make_unique<PhysicsEngine::IsometricCollider>
		(x, y, height);
	tempCollider->SetCollider(std::move(engineCollider));

	// 할당 해제 및 소유권 이전
	return tempCollider.release();
}

Collider* CollisionManager::CreateVehicleCollider(
	float x, float y, float height, float width)
{
	// 반환할 게임 엔진의 충돌체
	std::unique_ptr<Collider> tempCollider = 
		std::make_unique<Collider>();

	// 물리 엔진의 충돌체
	std::unique_ptr<PhysicsEngine::Collider> engineCollider =
		std::make_unique<PhysicsEngine::VehicleCollider>
		(x, y, height, width);
	tempCollider->SetCollider(std::move(engineCollider));

	// 할당 해제 및 소유권 이전
	return tempCollider.release();
}