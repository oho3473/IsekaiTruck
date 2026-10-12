#pragma once
#include "ElementBase.h"

class NPC : public ElementBase
{
	virtual void Init() abstract;

	virtual void FixedUpdate() abstract;
	virtual void Update(float deltaTime) abstract;
	virtual void LateUpdate() abstract;

	virtual void PreRender() abstract;
	virtual void Render(DirectY* directY) abstract;
	virtual void PostRender(DirectY* directY) abstract;

	virtual void Release() abstract;

	virtual ElementType GetElementType() const abstract;

	virtual GameObject* GetGameObject() const abstract;
	virtual void SetGameObject(GameObject* gameObject) abstract;
};

