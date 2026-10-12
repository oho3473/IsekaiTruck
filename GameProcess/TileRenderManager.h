#pragma once
#include "ElementBase.h"
#include "../GameEngine/ElementBase.h"
#include "../DirectY/DirectY.h"

class ResourceManager;
class GameEngine;
struct Graphics::Sprite;
class Scene;
class GameObject;

enum class TileRenderState
{
	Ground,
	Zone,
	Object,
};

class TileRenderManager : public ElementBase
{
public:
	TileRenderManager(GameEngine* gameEngine, Scene* scene, DirectY* directY, TileRenderState state = TileRenderState::Ground);
	virtual ~TileRenderManager();

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

	/// <summary>
	/// 받아온 타일들을 그립니다.
	/// </summary>
	void DrawTiles(DirectY* directY);

	/// <summary>
	/// 받아온 타일들을 생성합니다. (레이어 때문)
	/// </summary>
	void MakeTileObject();

	/// <summary>
	/// 지정한 경로의 파일을 불러온다.
	/// </summary>
	/// <param name="path"></param>
	void ReadSource(std::string path);

private:
	GameEngine* m_gameEngine;
	Scene* m_scene;
	DirectY* m_directY;
private:
	// 타일 이차원 벡터
	std::vector<std::vector<int>> m_tiles;

	// 타일 스프라이트 정보를 가지는 벡터 // 툴의 인덱스와 맞춰서 사용할 것.
	std::vector<Graphics::Sprite*> m_tileSprites;

	// 사실 가로 세로 사이즈가 같으니 한 사이즈 변수로 쓰겠다.
	int m_size;

	// 타일의 기본 사이즈
	const int m_tileHeight;
	const int m_tileWidth;

	Graphics::Motion* m_groundTileImages;
	Graphics::Motion* m_roadTileImages;
	Graphics::Motion* m_objectTileImages;
	Graphics::Motion* m_chairTileImages;
	Graphics::Motion* m_woodTileImages;
	Graphics::Motion* m_roadTile2Images;
	

	std::vector<Graphics::Sprite*> m_otherSprites;

	// Test ( 레이어를 다르게 해서 게임오브젝트를 여기서 생성함)
private:
	// 땅일 경우 굳이 레이어 분리는 낭비라 생각하므로 그냥 한번에 렌더.
	
	bool m_ground = false;
	TileRenderState m_tileRenderState = TileRenderState::Ground;

	void MakeTile(float x, float y, int tileIdx, int tileLayer);
	int m_tileLayer = 0;

	// test
	std::vector<GameObject*> test;
	int TestCount = 0;

};

