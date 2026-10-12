#include "PathFindRenderer.h"

#include "../MathEngine/Pathfinding.h"
#include "../MathEngine/FindPosition.h"
#include "Transform.h"
#include "GameObject.h"
#include "Police.h"

#include "../MathEngine/MathUtils.h"
#include "DebugManager.h"

PathFindRenderer::PathFindRenderer(
	Transform* playerTransform, 
	Transform* policeTrasform) : m_pathFind()
	, m_playerTransform(playerTransform)
	, m_policeTrasnform(policeTrasform)
{
	Init();
}

PathFindRenderer::~PathFindRenderer()
{

}

void PathFindRenderer::Init()
{
	GameObject* policeObj = m_policeTrasnform->GetGameObject();
	Police* police = ((Police*)policeObj->GetElement(ElementType::Script));
	m_pathFind = police->m_pathfind;
}

void PathFindRenderer::FixedUpdate()
{
}

void PathFindRenderer::Update(float deltaTime)
{
}

void PathFindRenderer::LateUpdate()
{

}

void PathFindRenderer::PreRender()
{

}

void PathFindRenderer::Render(DirectY* directY)
{
	if (DebugManager::GetInstance().GetIsDebuging())
	{
		// 타일 좌표 정보
		/*
 		directY->SetBrushColor(D2D1::ColorF::Red);
 		for (auto& data : m_pathFind->m_findPotition->m_mapData)
 		{
 			float locationX = data.second.x;
 			float locationY = data.second.y;
 			float coordinatesX = data.first.x;
 			float coordinatesY = data.first.y;
 
 			directY->DrawTextW(
 				(int)locationX,
 				(int)locationY,
 				10,
 				TextAlignMent::LEFT,
 				L"Verdana",
 				"%.0f, %.0f\n(%.0f, %.0f)",
 				coordinatesX,
 				coordinatesY,
 				locationX,
 				locationY);
 		}
		*/
		
		int pathSize = m_pathFind->m_finalPath.size();
		if (pathSize > 2)
		{
			directY->SetBrushColor(D2D1::ColorF::Yellow);
			for (auto& fp : m_pathFind->m_finalPath)
			{
				if (fp->parent)
				{
					MathEngine::Vector2 key1 = { (float)fp->x, (float)fp->y };
					MathEngine::Vector2 val1 = m_pathFind->m_findPotition->m_mapData[key1];

					MathEngine::Vector2 key2 = { (float)fp->parent->x, (float)fp->parent->y };
					MathEngine::Vector2 val2 = m_pathFind->m_findPotition->m_mapData[key2];

					directY->DrawLine(Graphics::BaseFigure::Line{ val1.x, val1.y, val2.x, val2.y }, 8.f);
				}
			}
			// 목적지
			MathEngine::Vector2 destnation = m_pathFind->GetLocation(0);
			directY->DrawCircle(Graphics::BaseFigure::Circle{ destnation.x, destnation.y }, 28.f);

			//  출발지
			directY->SetBrushColor(D2D1::ColorF::Green);
			MathEngine::Vector2 Starting = m_pathFind->GetLocation(pathSize - 1);
			directY->DrawCircle(Graphics::BaseFigure::Circle{ Starting.x, Starting.y }, 28.f);

			// 다음 경로
			directY->SetBrushColor(D2D1::ColorF::Red);
			MathEngine::Vector2 nextDestnation = m_pathFind->NearestLocation();
			directY->DrawCircle(Graphics::BaseFigure::Circle{ nextDestnation.x, nextDestnation.y }, 28.f);
		}
	}
}

void PathFindRenderer::PostRender(DirectY* directY)
{

}

void PathFindRenderer::Release()
{

}

ElementType PathFindRenderer::GetElementType() const
{
	return ElementType::Script;
}

GameObject* PathFindRenderer::GetGameObject() const
{
	return m_gameObject;
}

void PathFindRenderer::SetGameObject(GameObject* gameObject)
{

}
