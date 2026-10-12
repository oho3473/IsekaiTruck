#pragma once
#include <vector>
#include "../DirectY/DirectY.h"
#include "../GameEngine/ElementBase.h"
#include "CollisionManager.h"


class ResourceManager;
class GameEngine;

class TileManager : public ElementBase
{
public:
	TileManager(GameEngine* _gameEngine, DirectY* directY);
	~TileManager();

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
	
	// 타일맵을 생성한다.
	void ReadSource(std::string tileSource);
	void ReadSource2(std::string path);
	std::vector<int> SplitPush(std::string source);

	// 타일맵의 위치정보를 반환하는 2차원 벡터
	std::vector<std::vector<Graphics::BaseFigure::Point>> GetPosVectors() { return m_posVectors; }
	std::vector<std::vector<std::string>> GetStringVectors() { return m_stringVectors; }
	
	std::vector<std::vector<int>> GetTileMap() const { return m_tileMap; }
	void SetTileMap(std::vector<std::vector<int>> val) { m_tileMap = val; }

	CollisionManager* GetCManager() const { return m_cManager; }
	void SetCManager(CollisionManager* val) { m_cManager = val; }

private:
	// 콜리전 매니저
	CollisionManager* m_cManager;
	GameEngine* m_gameEngine;

	// int 벡터의 배열
	std::vector<std::vector<int>> m_tileMap;
	std::vector<Graphics::BaseFigure::Point> m_posVector;
	std::vector<std::vector<Graphics::BaseFigure::Point> > m_posVectors;
	std::vector<std::string> m_stringVector;
	std::vector<std::vector<std::string>> m_stringVectors;

	// 콜라이더를 담기위한 벡터
	std::vector<Collider> m_colliders;

	// 타일 출력을 위한 비트맵 정보
	Graphics::Sprite* m_tile;
	ID2D1Bitmap* m_tileBitmap;

	// 타일맵 행렬
	int m_size = 0;


	int m_colliderCount;
};