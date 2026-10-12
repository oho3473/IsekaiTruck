#pragma once

#include <memory>

#include "ElementBase.h"
#include "../PhysicsEngine/Collider.h"

#include <d2d1.h>

// Component 이름이 Collider physicsEngine의 Collider와 겹친다.
// using namespace PhysicsEngine 을 하면 안된다.
namespace PhysicsEngine { struct Collider; }

class Collider : public ElementBase
{
public:
	Collider();
	~Collider();

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


	/// 접근자 & 설정자
	PhysicsEngine::Collider* GetCollider() { return m_collider.get(); }
	void SetCollider(std::unique_ptr<PhysicsEngine::Collider> collider) { m_collider = std::move(collider); }

	bool GetIsCollision() const { return m_isCollision; }
	void SetIsCollision(bool isCollision) { m_isCollision = isCollision; }
private:
	// 컬라이더의 충돌 여부
	bool m_isCollision;
	// 물리 엔진의 컬라이더
	std::unique_ptr<PhysicsEngine::Collider> m_collider;
};