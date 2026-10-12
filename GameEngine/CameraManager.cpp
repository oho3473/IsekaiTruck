#include "CameraManager.h"
#include "InputManager.h"

#include "../DirectY/MakeMatrix.h"

void CameraManager::Init()
{

}

void CameraManager::Update()
{
	//크기 변환
	if (InputManager::GetInstance().IsGetKey('Z'))
	{
		if (m_cameraScale.x < 1)
		{
			Scale({ 0.005f, 0.005f });
		}
	}
	if (InputManager::GetInstance().IsGetKey('X'))
	{
		if (m_cameraScale.x > 0.5)
		{
			Scale({ -0.005f, -0.005f });
		}
	}

	m_cameraMatrix = MakeMatrix()({ m_cameraScale.x, m_cameraScale.y }, m_cameraRotation, { m_cameraPosition.x, m_cameraPosition.y });
}

void CameraManager::Render()
{
	//Graphics::BaseFigure::Rectangle testRect{ 0, 0, 100, 100 };

	//directY->SetBrushColor(0, 0, 0, 1);
	//directY->DrawRectangle(testRect);
}

void CameraManager::Release()
{

}

void CameraManager::Translate(Vector2 position)
{
	m_cameraPosition += position;
}

void CameraManager::Rotate(float angle)
{
	m_cameraRotation += angle;
}

void CameraManager::Scale(Vector2 scale)
{
	m_cameraScale += scale;
}
