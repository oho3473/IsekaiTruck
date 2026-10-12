#pragma once

#include <map>
#include <memory>

#include "../MathEngine/Vector2.h"

// Component 이름이 Collider physicsEngine의 Collider와 겹친다.
// using namespace PhysicsEngine 을 하면 안된다.
namespace PhysicsEngine
{
	struct Collider;
	struct CircleCollider;
	struct RectangleCollider;
	struct IsometricCollider;
	struct CollisionPoints;
}

class DirectY;
class GameObject;
class Collider;

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	void Init();

	void Update(float deltaTime);

	// 아마도 테스트를 위한 Render
	void Render(DirectY& directY);

	void Release();

	void AddCollider(GameObject* gameObject, Collider* collider);
	void DeleteCollider(GameObject* gameObject);

	// CollisionManager에서 만들어서 Object 초기화할 때 Collider Element에 추가
	Collider* CreateCircleCollider(MathEngine::Vector2 center, float r);
	Collider* CreateRectangleCollider(float x, float y, float width, float height);
	Collider* CreateIsometricCollider(float x, float y, float height);
	Collider* CreateVehicleCollider(float x, float y, float height, float width);
	
	GameObject* GetPlayerObject() const { return m_playerObject; }
	void SetPlayerObject(GameObject* val) { m_playerObject = val; }

	Collider* GetPlayerCollider() const { return m_playerCollider; }
	void SetPlayerCollider(Collider* val) { m_playerCollider = val; }

private: 
	// Current Scene에 존재하는 모든 Collider를 관리하기 위해 저장
	// 삭제는 GameObject에서 하므로 delete 안 해도 됨
	// Collider는 Element의 Collider임
	std::multimap<GameObject*, Collider*> m_colliders;

	// 플레이어의 객체와 콜라이더 정보
	GameObject* m_playerObject;
	Collider* m_playerCollider;

	void ResolveCollsion( 
		GameObject* obj, 
		const PhysicsEngine::CollisionPoints& cp, 
		const float& dt, 
		bool& isTurnDirection);

	// 여러 개의 콜라이더에 동시에 충돌했을 때 벡터 합
	MathEngine::Vector2 m_playerNormal;
};