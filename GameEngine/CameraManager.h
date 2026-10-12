#pragma once
#include "Singleton.h"
#include "../MathEngine/Vector2.h"
#include "../DirectY/DirectY.h"

using namespace MathEngine;

class CameraManager : public Singleton<CameraManager>
{
public:
	void Init();

	void Update();
	void Render();

	void Release();

	void SetCameraPosition(Vector2 cameraPosition) { m_cameraPosition = cameraPosition; }
	Vector2 GetCameraPosition() { return m_cameraPosition; };

	Vector2 GetCameraScale() { return m_cameraScale; }

	D2D1_MATRIX_3X2_F GetCameraMatrix() { return m_cameraMatrix; }

	void Translate(Vector2 position);
	void Rotate(float angle);
	void Scale(Vector2 scale);

private:
	Vector2 m_cameraPosition = {0, 0};
	float m_cameraRotation = 0;
	Vector2 m_cameraScale = { 1, 1 };

	D2D1_MATRIX_3X2_F m_cameraMatrix;

private:
	CameraManager() : m_cameraMatrix({1, 0, 0, 1, 0, 0}) {}
	~CameraManager() {}

	friend class Singleton<CameraManager>;
};

