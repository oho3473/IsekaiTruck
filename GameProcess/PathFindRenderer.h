#pragma once

#include "ElementBase.h"
#include "Vector2.h"
#include <iostream>

class CameraManager;
class Transform;
namespace Algorithm { class PathFinding; }
namespace MathEngine { struct Vector2; }

class PathFindRenderer : public ElementBase
{
public:
	PathFindRenderer(Transform* playerTransform, Transform* policeTrasform);
	~PathFindRenderer();

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

private:
	Algorithm::PathFinding* m_pathFind;
	Transform* m_playerTransform;
	Transform* m_policeTrasnform;
};

