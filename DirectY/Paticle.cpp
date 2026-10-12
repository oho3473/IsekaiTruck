#include "Paticle.h"
#include "DirectY.h"
#include "Sprite.h"

#include <ctime>
#include <cstdlib>
#include <cmath>

Paticle::Paticle() : m_sprite(NULL), m_lifeTime(NULL), m_pos(0, 0)
{

}

Paticle::~Paticle()
{

}

void Paticle::Render(DirectY* directY, D2D1_MATRIX_3X2_F matrix)
{
	if (isAlive)
	{
		//파티클 행렬 * 스크린행렬
		D2D1_MATRIX_3X2_F paticlematrix = MakeMatrix()({ 1,1 }, 0, { 0 ,0 }) * matrix;

		paticlematrix.dx += m_pos.x ;
		paticlematrix.dy += m_pos.y ;

		directY->SetTransform(paticlematrix);

		directY->DrawBitmap(m_sprite);
	}
}

void Paticle::Update(float deltaTime)
{
	static int callCount = 0;

	srand(callCount * 439);
	m_moveSpeed = (float)(rand() % 200);
	m_acceleration = (float)(rand() % 3 + 1);

	if (m_lifeTime <= 0)
	{
		isAlive = false;
		m_lifeTime = 0;
	}

	if (isAlive)
	{
		m_lifeTime -= deltaTime;

		m_angle += 360 * deltaTime;
		if (m_angle >= 360)
		{
			m_angle = m_angle - 360;
		}

		m_pos.x += (float)(m_moveSpeed * m_acceleration * cos(360 / 20 * m_movementSeed) * deltaTime) ;
		m_pos.y += (float)(m_moveSpeed * m_acceleration * sin(360 / 20 * m_movementSeed) * deltaTime) ;
	}
	callCount++;
}
