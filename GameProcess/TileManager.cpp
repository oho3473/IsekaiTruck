#include "TileManager.h"
#include "ResourceManager.h"
#include "GameEngine.h"
#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"
#include "DebugManager.h"
// 파일 읽기용 헤더
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

TileManager::TileManager(GameEngine* _gameEngine, DirectY* directY)
	: m_cManager(_gameEngine->GetCollisionManager())
	, m_gameEngine(_gameEngine)
	, m_tileMap{}
	, m_posVector{}, m_posVectors{}
	, m_stringVector{}, m_stringVectors{}
	, m_colliders{}
	, m_tileBitmap(_gameEngine->GetImage("TileExample"))
	, m_tile(nullptr)
	, m_colliderCount(0)
{
	m_tile = directY->ReadSpriteTxtFile("TileExample", m_tileBitmap);
	// 파일을 읽어서 타일정보 읽어오기
	//ReadSource2("TestCheck");
}

TileManager::~TileManager()
{
	// 타일을 삭제한다.
	delete m_tile;
}

void TileManager::Init()
{
	//Graphics::BaseFigure::Point center = { (float)(960), (float)(540 - (m_col * 36)) };
	Graphics::BaseFigure::Point center = { 72, 108 };
	// 타일라인을 그리기 위한 구조체
	Graphics::BaseFigure::IsomatricTile nowCenter;

	for (int i = 0; i < m_size; i++)
	{
		// 가로행
		for (int j = 0; j < m_size; j++)
		{
			nowCenter = { (center.x + (72 * j)),(center.y + (36 * j)) };
			switch ((int)m_tileMap[i][j]%2)
			{
			case(0):
				break;
			case(1):
				Collider* collider = GetCManager()->
					CreateIsometricCollider(
						nowCenter.centerXY.x, 
						nowCenter.centerXY.y, 72);
				GetCManager()->AddCollider(	m_gameObject, collider);
				m_colliderCount++;
				break;
			}
			// 현재 위치를 바꿔준다.

			// 타일 위치를 저장한다.
			m_posVector.push_back(nowCenter.centerXY);
		}
		// 한 행의 위치정보를 담은 벡터를 2차원 벡터에 담는다.
		m_posVectors.push_back(m_posVector);
		// 한 행의 스프라이트정보를 담은 벡터를 2차원 벡터에 담는다.
		m_stringVectors.push_back(m_stringVector);
		// 한 행의 벡터를 초기화해준다.
		m_posVector.clear();
		center = { center.x - 72 , center.y + 36 };
	}
	
}

void TileManager::FixedUpdate()
{
 	

}

void TileManager::Update(float deltaTime)
{

}

void TileManager::LateUpdate()
{

}

void TileManager::PreRender()
{

}

void TileManager::Render(DirectY* directY)
{
	//return;
	//Graphics::BaseFigure::Point center = { (float)(960), (float)(540 - (m_col * 36)) };
	Graphics::BaseFigure::Point center = { 72, 108 };
	// 타일라인을 그리기 위한 구조체
	Graphics::BaseFigure::IsomatricTile nowCenter;

	// 세로열
	for (int i = 0; i < m_size; i++)
	{
		// 가로행
		for (int j = 0; j < m_size; j++)
		{
			/// 라인그리기
			// 배열의 내용에 따라 색상을 변경한다.
			switch ((int)m_tileMap[i][j])
			{
			case(0):
				directY->SetBrushColor(D2D1::ColorF::Gray);
				break;
			case(1):
				directY->SetBrushColor(D2D1::ColorF::Green);

				if (DebugManager::GetInstance().GetTileDebuging())
				{
					nowCenter = { (center.x + (72 * j)),(center.y + (36 * j)) };
					directY->DrawIsometric(nowCenter, 8);
				}

				break;
			}
			// 현재 위치를 바꿔준다.
			//nowCenter = { (center.x + (72 * j)),(center.y + (36 * j)) };
			// 한 행의 위치정보를 벡터에 담는다.
			// 타일을 그린다.

			/// 타일그리기
			// 파일 생성위치를 지정한다.
			m_tile->m_drawPos = { nowCenter.centerXY.x - 72, nowCenter.centerXY.y - 36 };
			// 배열의 내용에 따라 타일 생성 종류 및 여부를 결정한다.
			switch ((int)m_tileMap[i][j]%2)
			{
			case(0):
				break;
			case(1):
				directY->DrawBitmap(m_tile, 1.f);
				break;
			default:
				break;
			}
		}
		center = { center.x - 72 , center.y + 36 };
	}
}


void TileManager::PostRender(DirectY* directY)
{

}


void TileManager::Release()
{
// 	// 콜라이더를 삭제한다
// 	for (int i = 0; i < m_colliderCount; i++)
// 	{
// 		m_cManager->DeleteCollider(m_gameObject);
}
// 	}

/// <summary>
/// 타일맵의 elementype을 반환한다.
/// </summary>
/// <returns></returns>
ElementType TileManager::GetElementType() const
{
	return ElementType::Script;
}

/// <summary>
/// 타일맵의 오브젝트를 갖고있는 오브젝트를 반환한다.
/// </summary>
/// <returns></returns>
GameObject* TileManager::GetGameObject() const
{
	return m_gameObject;
}

/// <summary>
/// 타일맵의 오브젝트를 설정한다.
/// </summary>
/// <param name="gameObject"></param>
void TileManager::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}

void TileManager::ReadSource(std::string tileSource)
{
	// 1. 파일명을 입력받고 파일명.text 파일을 연다
	std::ifstream resourceTextFile("Resource/Text/" + tileSource + ".txt");
	std::string infoLine;
	// 파일이 열려있는지 확인한다.
	if(resourceTextFile.is_open())
	{
		// 2. 파일을 한 줄 씩 읽는다
		while (std::getline(resourceTextFile, infoLine))
		{
			// 3. 원하는 정보를 추출한다.
			std::string sourceType = infoLine.substr(0,3);
			std::string source;
			if (sourceType._Equal("col"))
			{
				source = infoLine.substr(infoLine.find(" "), infoLine.size());
				//m_col = stoi(source.substr(1));
			}
			else if (sourceType._Equal("row"))
			{
				source = infoLine.substr(infoLine.find(" "), infoLine.size());
				//m_row = stoi(source.substr(1));
			}
			else if (infoLine._Equal(""))
			{
				break;
			}
			else if(!infoLine._Equal(tileSource))
			{
				// 4. 추출된 정보를 내부 변수에 입력한다.
				m_tileMap.push_back(SplitPush(infoLine));
			}
		}
		resourceTextFile.close();
	}
	else
	{
		std::cout << "파일 오픈 실패";
	}
}

void TileManager::ReadSource2(std::string path)
{
	std::wifstream ifs;
	ifs.open(path);

	std::wstring mapName;
	ifs >> mapName;
	std::wstring temp;
	float size;
	ifs >> temp >> temp >> temp >> size;

	m_size = static_cast<int>(size);
	m_tileMap = std::vector<std::vector<int>>(m_size, std::vector<int>(m_size));

	//MyMap* newMap = new MyMap();
	//newMap->SetName(mapName);
	//newMap->CreateMap(size, size);

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			//int idx = y * size + x;
			/*ifs >> newMap->GetWorldData()[idx];*/
			ifs >> m_tileMap[y][x];

		}
	}

	ifs.close();
}

std::vector<int> TileManager::SplitPush(std::string source)
{
	
	// 스트링스트림에 소스 입력
	std::stringstream fullSource(source);
	std::string temp;
	std::vector<int> TileRow;
	while (std::getline(fullSource, temp, ' '))
	{
		TileRow.push_back(stoi(temp));
	}
	return TileRow;
}

