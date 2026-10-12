#pragma once
#include <ElementBase.h>
#include <vector>
#include "../DirectY/DirectY.h"
#include "../GameEngine/ElementBase.h"
#include "CollisionManager.h"

class GameObject;
class GameEngine;

class BuildingManager :  public ElementBase
{
public:
	BuildingManager(GameEngine* gameEngine, DirectY* directY);
	~BuildingManager();

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

	std::vector<std::vector<int>> GetBuildingMap() const { return m_buildingMap; }
	void SetBuildingMap(std::vector<std::vector<int>> val) { m_buildingMap = val; }
private:

	std::vector<std::vector<int> > m_buildingMap;
	
	Graphics::Sprite* m_building;
	ID2D1Bitmap* m_buildingBitmap;
};

