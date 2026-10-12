#pragma once
#include "Paticle.h"
#include <algorithm>

class PaticleMaintain 
{


public:
	PaticleMaintain();
	~PaticleMaintain();

	void CreatePaticle(MathEngine::Vector2 pos,float lifeTime, int movement,Graphics::Sprite* sprite);
	void Update(float deltaTime); 
	void Render(DirectY* directY, D2D1_MATRIX_3X2_F matrix);
	void DeletePaticle();
private:
	std::vector<Paticle*> m_paticleVector;
};

