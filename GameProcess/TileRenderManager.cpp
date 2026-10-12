#include "TileRenderManager.h"
#include "ResourceManager.h"
#include "GameEngine.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector2.h"
#include "Sprite.h"
#include "Scene.h"
#include "Tile.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "GameObject.h"

TileRenderManager::TileRenderManager(GameEngine* gameEngine, Scene* scene, DirectY* directY, TileRenderState state)
	: m_gameEngine(gameEngine)
	, m_scene(scene)
	, m_directY(directY)
	, m_tileRenderState(state)
	, m_tiles{}
	, m_size(0)
	, m_tileHeight(72)
	, m_tileWidth(144)
	, m_groundTileImages(nullptr)
	, m_roadTileImages(nullptr)
	, m_objectTileImages(nullptr)
	, m_chairTileImages(nullptr)
	, m_woodTileImages(nullptr)
	, m_roadTile2Images(nullptr)
{
	m_groundTileImages = new Graphics::Motion();
	m_roadTileImages = new Graphics::Motion();
	m_objectTileImages = new Graphics::Motion();
	m_chairTileImages = new Graphics::Motion();
	m_woodTileImages = new Graphics::Motion();
	m_roadTile2Images = new Graphics::Motion();

	directY->ReadSpriteTxtFile("Tile3x4", gameEngine->GetImage("Tile3x4"), m_groundTileImages);
	for (int i = 0; i < m_groundTileImages->GetTotalFrame(); i++)
	{
		//m_tileSprites.push_back(m_groundTileImages->GetSprite(i));
		m_otherSprites.push_back(m_groundTileImages->GetSprite(i));
	}
	directY->ReadSpriteTxtFile("roadTile", gameEngine->GetImage("RoadTile"), m_roadTileImages);
	for (int i = 0; i < m_roadTileImages->GetTotalFrame(); i++)
	{
		//m_tileSprites.push_back(m_roadTileImages->GetSprite(i));
		m_otherSprites.push_back(m_roadTileImages->GetSprite(i));
	}
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Zone001", gameEngine->GetImage("U_Zone.001 (360x216)")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Zone002", gameEngine->GetImage("U_Zone.002 (792x396)")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Zone003", gameEngine->GetImage("U_Zone.003 (792x396)")));

	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Zone001", gameEngine->GetImage("Zone.001 (360x216)")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Zone002", gameEngine->GetImage("Zone.002 (792x396)")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Zone003", gameEngine->GetImage("Zone.003 (792x396)")));

	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Crosswalk001", gameEngine->GetImage("U_Crosswalk.001 (216x108)")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Crosswalk002", gameEngine->GetImage("Crosswalk.002 (216x108)")));

	directY->ReadSpriteTxtFile("Object5x2", gameEngine->GetImage("Object5x2"), m_objectTileImages);
	for (int i = 0; i < m_objectTileImages->GetTotalFrame(); i++)
	{
		//m_tileSprites.push_back(m_objectTileImages->GetSprite(i));
		m_otherSprites.push_back(m_objectTileImages->GetSprite(i));
	}

	directY->ReadSpriteTxtFile("D_Chair2x1", gameEngine->GetImage("D_Chair2x1"), m_chairTileImages);
	for (int i = 0; i < m_chairTileImages->GetTotalFrame(); i++)
	{
		//m_tileSprites.push_back(m_chairTileImages->GetSprite(i));
		m_otherSprites.push_back(m_chairTileImages->GetSprite(i));
	}

	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Fountain", gameEngine->GetImage("Fountain (288x252)")));

	directY->ReadSpriteTxtFile("Wood", gameEngine->GetImage("tree3x2"), m_woodTileImages);
	for (int i = 0; i < m_woodTileImages->GetTotalFrame(); i++)
	{
		//m_tileSprites.push_back(m_woodTileImages->GetSprite(i));
		m_otherSprites.push_back(m_woodTileImages->GetSprite(i));
	}
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("wood7", gameEngine->GetImage("Wood.007 (144x252)")));

	// buildings	
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building001_2", gameEngine->GetImage("Building001_2")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building001", gameEngine->GetImage("U_Building001")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building002_2", gameEngine->GetImage("Building002_2")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building002", gameEngine->GetImage("U_Building002")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building003_2", gameEngine->GetImage("Building003_2")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building003", gameEngine->GetImage("U_Building003")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building004_2", gameEngine->GetImage("Building004_2")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building004", gameEngine->GetImage("U_Building004")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building005_2", gameEngine->GetImage("Building005_2")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_BUilding005", gameEngine->GetImage("U_BUilding005")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building006_1", gameEngine->GetImage("Building006_1")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building006_2", gameEngine->GetImage("U_Building006_2")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building007_1", gameEngine->GetImage("U_Building007_1")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building007_2", gameEngine->GetImage("U_Building007_2")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building008_1", gameEngine->GetImage("U_Building008_1")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building008_2", gameEngine->GetImage("U_Building008_2")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building009_1", gameEngine->GetImage("Building009_1")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building009_2", gameEngine->GetImage("Building009_2")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building010_1", gameEngine->GetImage("Building010_1")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building010_2", gameEngine->GetImage("Building010_2")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building011_1", gameEngine->GetImage("Building011_1")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building011_2", gameEngine->GetImage("U_Building011_2")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building012_1", gameEngine->GetImage("Building012_1")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building012_2", gameEngine->GetImage("Building012_2")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building013_1", gameEngine->GetImage("Building013_1")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building013_2", gameEngine->GetImage("Building013_2")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building014_1", gameEngine->GetImage("Building014_1")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building014_2", gameEngine->GetImage("U_Building014_2")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building015_1", gameEngine->GetImage("Building015_1")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building015_2", gameEngine->GetImage("U_Building015_2")));


	// other
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("roadTile012", gameEngine->GetImage("Road.Tile.012 (216x108)")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("roadTile013", gameEngine->GetImage("Road.Tile.013 (216x108)")));
	directY->ReadSpriteTxtFile("RoadTile2", gameEngine->GetImage("RoadTile2"), m_roadTile2Images);
	for (int i = 0; i < m_roadTile2Images->GetTotalFrame(); i++)
	{
		m_otherSprites.push_back(m_roadTile2Images->GetSprite(i));
	}
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_roadTile014", gameEngine->GetImage("U_roadTile014")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_roadTile015", gameEngine->GetImage("U_roadTile015")));

	// new

	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Object.005", gameEngine->GetImage("Object.005")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Object.006", gameEngine->GetImage("Object.006")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Object.007", gameEngine->GetImage("Object.007")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Object.008", gameEngine->GetImage("Object.008")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Object.009", gameEngine->GetImage("Object.009")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Obstruction.003", gameEngine->GetImage("Obstruction.003")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Obstruction.004", gameEngine->GetImage("Obstruction.004")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Obstruction.005", gameEngine->GetImage("Obstruction.005")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Obstruction.006", gameEngine->GetImage("Obstruction.006")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Obstruction.007", gameEngine->GetImage("Obstruction.007")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Obstruction.008", gameEngine->GetImage("Obstruction.008")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Obstruction.009", gameEngine->GetImage("Obstruction.009")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Obstruction.010", gameEngine->GetImage("Obstruction.010")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Obstruction.011", gameEngine->GetImage("Obstruction.011")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Obstruction.012", gameEngine->GetImage("Obstruction.012")));

	m_otherSprites.push_back(directY->ReadSpriteTxtFile("D_Sign.001", gameEngine->GetImage("D_Sign.001")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("D_Sign.002", gameEngine->GetImage("D_Sign.002")));

	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Streetlamp.001", gameEngine->GetImage("Streetlamp.001")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Streetlamp.002", gameEngine->GetImage("Streetlamp.002")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Streetlamp.003", gameEngine->GetImage("Streetlamp.003")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Streetlamp.004", gameEngine->GetImage("Streetlamp.004")));


	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building.016-1", gameEngine->GetImage("Building.016-1")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building.016-2", gameEngine->GetImage("Building.016-2")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building.017-1", gameEngine->GetImage("U_Building.017-1")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building.017-2", gameEngine->GetImage("U_Building.017-2")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building.018-1", gameEngine->GetImage("U_Building.018-1")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building.018-2", gameEngine->GetImage("U_Building.018-2")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building.019-1", gameEngine->GetImage("U_Building.019-1")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building.019-2", gameEngine->GetImage("U_Building.019-2")));

	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building.020-1", gameEngine->GetImage("Building.020-1")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building.020-2", gameEngine->GetImage("U_Building.020-2")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("Building.021-1", gameEngine->GetImage("Building.021-1")));
	m_otherSprites.push_back(directY->ReadSpriteTxtFile("U_Building.021-2", gameEngine->GetImage("U_Building.021-2")));

	for (Graphics::Sprite* sprite : m_otherSprites)
	{
		m_tileSprites.push_back(sprite);
	}
}

TileRenderManager::~TileRenderManager()
{

	for (int i = 0; i < m_groundTileImages->GetTotalFrame(); i++)
	{
		m_otherSprites.erase(find(m_otherSprites.begin(), m_otherSprites.end(), m_groundTileImages->GetSprite(i)));
	}
	delete m_groundTileImages;

	for (int i = 0; i < m_roadTileImages->GetTotalFrame(); i++)
	{
		m_otherSprites.erase(find(m_otherSprites.begin(), m_otherSprites.end(), m_roadTileImages->GetSprite(i)));
	}
	delete m_roadTileImages;

	for (int i = 0; i < m_objectTileImages->GetTotalFrame(); i++)
	{
		m_otherSprites.erase(find(m_otherSprites.begin(), m_otherSprites.end(), m_objectTileImages->GetSprite(i)));
	}
	delete m_objectTileImages;

	for (int i = 0; i < m_chairTileImages->GetTotalFrame(); i++)
	{
		m_otherSprites.erase(find(m_otherSprites.begin(), m_otherSprites.end(), m_chairTileImages->GetSprite(i)));
	}
	delete m_chairTileImages;

	for (int i = 0; i < m_woodTileImages->GetTotalFrame(); i++)
	{
		m_otherSprites.erase(find(m_otherSprites.begin(), m_otherSprites.end(), m_woodTileImages->GetSprite(i)));
	}
	delete m_woodTileImages;

	for (int i = 0; i < m_roadTile2Images->GetTotalFrame(); i++)
	{
		m_otherSprites.erase(find(m_otherSprites.begin(), m_otherSprites.end(), m_roadTile2Images->GetSprite(i)));
	}
	delete m_roadTile2Images;

	for (auto& s : m_otherSprites)
	{
		if (s != nullptr)
			delete s;
	}
}

void TileRenderManager::Init()
{

}

void TileRenderManager::FixedUpdate()
{

}

void TileRenderManager::Update(float deltaTime)
{

}

void TileRenderManager::LateUpdate()
{

}

void TileRenderManager::PreRender()
{

}

void TileRenderManager::Render(DirectY* directY)
{
	switch (m_tileRenderState)
	{
	case TileRenderState::Ground:
		DrawTiles(directY);
		break;
	case TileRenderState::Zone:
		break;
	case TileRenderState::Object:
		break;
	default:
		break;
	}
}

void TileRenderManager::PostRender(DirectY* directY)
{

}

void TileRenderManager::Release()
{
	m_tiles.clear();
}

ElementType TileRenderManager::GetElementType() const
{
	return ElementType::Script;
}

GameObject* TileRenderManager::GetGameObject() const
{
	return m_gameObject;
}

void TileRenderManager::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}

void TileRenderManager::DrawTiles(DirectY* directY)
{
	auto ToScreen = [&](int x, int y)
	{
		return MathEngine::Vector2
		(
			static_cast<float>((x - y) * m_tileWidth / 2), static_cast<float>((x + y) * m_tileHeight / 2)
		);
	};


	MathEngine::Vector2 renderPos(0, 0);
	int prevY = 0;
	int prevX = 0;
	while (true)
	{
		int y = static_cast<int>(renderPos.y);
		int x = static_cast<int>(renderPos.x);
		MathEngine::Vector2 world = ToScreen(static_cast<int>(renderPos.x), static_cast<int>(renderPos.y));

		int tileIdx = m_tiles[y][x];

		switch (m_tiles[y][x])
		{
		case -1:
			break;

		default:
		{
			// 현재 타일의 정보 ex) 1번 일 경우 1번 타일로 렌더
			MathEngine::Vector2 StartPos(0, 0);

			// 해강이형이 만든 콜라이더와 위치가 살짝 안맞어서 맞추기 위해 조정
			StartPos.x = world.x;
			world.y += 72;

			StartPos.y = world.y - (m_tileSprites[tileIdx]->m_drawSize.bottom - m_tileHeight);

			int heightValue = static_cast<int>(m_tileSprites[tileIdx]->m_drawSize.bottom);

			int v = heightValue / 72;
			if (v % 2 == 0)
				StartPos.y += 36;

			if (m_tileSprites[tileIdx]->m_name[0] == 'U')
			{
				StartPos.y += 36;
			}
			if (m_tileSprites[tileIdx]->m_name[0] == 'D')
			{
				StartPos.y -= 36;
			}

			m_tileSprites[tileIdx]->m_drawPos.x = StartPos.x;
			m_tileSprites[tileIdx]->m_drawPos.y = StartPos.y;

			directY->DrawBitmap(m_tileSprites[tileIdx]);
		}
		break;
		}

		if (renderPos.x == m_size - 1 && renderPos.y == m_size - 1)
		{
			break;
		}

		if (renderPos.x == m_size - 1)
		{
			prevX++;
			renderPos.x = static_cast<float>(prevX);
			renderPos.y = static_cast<float>(prevY);
		}
		else if (renderPos.y == 0)
		{
			prevY++;
			renderPos.y = static_cast<float>(prevY);
			renderPos.x = 0;
		}
		else
		{
			renderPos.y -= 1;
			renderPos.x += 1;
		}

		if (renderPos.y > m_size - 1)
			renderPos.y--;
		if (renderPos.x > m_size - 1)
			renderPos.x--;
	}
}

void TileRenderManager::MakeTileObject()
{
	auto ToScreen = [&](int x, int y)
	{
		return MathEngine::Vector2
		(
			static_cast<float>((x - y) * m_tileWidth / 2), static_cast<float>((x + y) * m_tileHeight / 2)
		);
	};


	MathEngine::Vector2 renderPos(0, 0);
	int prevY = 0;
	int prevX = 0;
	while (true)
	{

		int y = static_cast<int>(renderPos.y);
		int x = static_cast<int>(renderPos.x);
		MathEngine::Vector2 world = ToScreen(static_cast<int>(renderPos.x), static_cast<int>(renderPos.y));

		int tileIdx = m_tiles[y][x];

		if (renderPos.x == 0)
		{
			// 타일 레이어 증가
			m_tileLayer++;
		}

		if (renderPos.y == m_size - 1)
		{
			m_tileLayer++;
		}

		switch (m_tiles[y][x])
		{
		case -1:
			break;

		default:
		{
			// 현재 타일의 정보 ex) 1번 일 경우 1번 타일로 렌더
			MathEngine::Vector2 StartPos(0, 0);

			// 해강이형이 만든 콜라이더와 위치가 살짝 안맞어서 맞추기 위해 조정
			StartPos.x = world.x;
			world.y += 72;

			StartPos.y = world.y - (m_tileSprites[tileIdx]->m_drawSize.bottom - m_tileHeight);

			int heightValue = static_cast<int>(m_tileSprites[tileIdx]->m_drawSize.bottom);

			int v = heightValue / 72;
			if (v % 2 == 0)
				StartPos.y += 36;

			if (m_tileSprites[tileIdx]->m_name[0] == 'U')
			{
				StartPos.y += 36;
			}
			if (m_tileSprites[tileIdx]->m_name[0] == 'D')
			{
				StartPos.y -= 36;
			}

			m_tileSprites[tileIdx]->m_drawPos.x = StartPos.x;
			m_tileSprites[tileIdx]->m_drawPos.y = StartPos.y;

			MakeTile(StartPos.x, StartPos.y, tileIdx, m_tileLayer);
			TestCount++;
			//MakeTile(StartPos.x, StartPos.y, tileIdx, 6);
		}
		break;
		}

		if (renderPos.x == m_size - 1 && renderPos.y == m_size - 1)
		{
			break;
		}

		if (renderPos.x == m_size - 1)
		{
			prevX++;
			renderPos.x = static_cast<float>(prevX);
			renderPos.y = static_cast<float>(prevY);
		}
		else if (renderPos.y == 0)
		{
			prevY++;
			renderPos.y = static_cast<float>(prevY);
			renderPos.x = 0;
		}
		else
		{
			renderPos.y -= 1;
			renderPos.x += 1;
		}

		if (renderPos.y > m_size - 1)
			renderPos.y--;
		if (renderPos.x > m_size - 1)
			renderPos.x--;
	}

	int a = 3;
}

void TileRenderManager::ReadSource(std::string path)
{
	std::wifstream ifs;
	ifs.open(path);

	std::wstring mapName;
	ifs >> mapName;
	std::wstring temp;
	float size;
	ifs >> temp >> temp >> temp >> size;

	m_size = static_cast<int>(size);
	m_tiles = std::vector<std::vector<int>>(m_size, std::vector<int>(m_size));

	//MyMap* newMap = new MyMap();
	//newMap->SetName(mapName);
	//newMap->CreateMap(size, size);

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			//int idx = y * size + x;
			/*ifs >> newMap->GetWorldData()[idx];*/
			ifs >> m_tiles[y][x];

		}
	}

	ifs.close();
	//newMap->SetName(mapName);
	//AddMap(newMap);
	if (m_tileRenderState == TileRenderState::Object)
	{
		MakeTileObject();
	}
}

void TileRenderManager::MakeTile(float x, float y, int tileIdx, int tileLayer)
{
	int cx = static_cast<int>(x);
	int cy = static_cast<int>(y);
	std::string tileName = "Tile_";
	tileName += std::to_string(cx) + "_" + std::to_string(cy) + "_" + std::to_string(tileIdx);

	// 게임 오브젝트
	GameObject* object = m_gameEngine->CreateObject(tileName, m_scene);

	// 트랜스폼
	Transform* transform = new Transform();
	object->SetElement(transform, ElementType::Transform);
	transform->SetUseCamera(true);

	// 타일
	Tile* tile = new Tile();
	object->SetElement(tile, ElementType::Script);

	Graphics::Sprite* sprite = m_tileSprites[tileIdx];
	//sprite->m_drawPos.x = x;
	//sprite->m_drawPos.y = y;
	// 각자 타일의 그릴 위치를 저장
	tile->SetDrawPos(x, y);

	tile->SetSprite(sprite);

	transform->SetLayer(tileLayer);

	//test.push_back(object);
}
