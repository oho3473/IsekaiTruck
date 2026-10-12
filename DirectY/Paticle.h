#pragma once
#include "../MathEngine/Vector2.h"
#include "Sprite.h"

class DirectY;

class Paticle
{
public:
	
	Paticle();
	~Paticle();

	void SetPos(float x, float y) { m_pos = { x,y }; }
	void SetSprite(Graphics::Sprite* sprite) { m_sprite = sprite; }
	void SetLifeTime(float time) { m_lifeTime = time; }
	void SetMoveMent(int movement) { m_movementSeed = movement; }


	bool CheckAlive()  const { return isAlive; }

	Graphics::Sprite* GetSprite() const { return m_sprite; }
	float GetAngle() const { return m_angle; }

	void Render(DirectY* directY, D2D1_MATRIX_3X2_F matrix);
	void Update(float deltaTime);



private:
	bool isAlive = true;
	int m_movementSeed = 0;
	float m_angle = 0;
	Graphics::Sprite* m_sprite;
	float m_lifeTime;
	MathEngine::Vector2 m_pos;

	//test
	float m_moveSpeed = 100;
	float m_acceleration = 1;
};

