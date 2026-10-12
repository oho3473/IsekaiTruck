#include "PaticleMaintain.h"
#include "DirectY.h"

PaticleMaintain::PaticleMaintain()
{
}

PaticleMaintain::~PaticleMaintain()
{
	DeletePaticle();
}

void PaticleMaintain::CreatePaticle(MathEngine::Vector2 pos, float lifeTime, int movement, Graphics::Sprite* sprite)
{
	Paticle* temp = new Paticle();
	temp->SetSprite(sprite);
	temp->SetLifeTime(lifeTime);
	temp->SetPos(pos.x, pos.y);
	temp->SetMoveMent(movement);

	m_paticleVector.push_back(temp);
}

void PaticleMaintain::Update(float deltaTime)
{
	for (auto& paticle : m_paticleVector)
	{
		if (paticle != nullptr)
		{
			paticle->Update(deltaTime);
			if (!paticle->CheckAlive())
			{
				delete paticle;
				paticle = nullptr;
			}
		}
	}
	m_paticleVector.erase(std::remove(m_paticleVector.begin(), m_paticleVector.end(), nullptr), m_paticleVector.end());
}

void PaticleMaintain::Render(DirectY* directY,D2D1_MATRIX_3X2_F matrix)
{
	for (auto& paticle : m_paticleVector)
	{
		if (paticle->CheckAlive())
		{
			paticle->Render(directY, matrix);
		}
	}
}

void PaticleMaintain::DeletePaticle()
{
	for (auto& paticle : m_paticleVector)
	{
		if (paticle != nullptr)
		{
			delete paticle;
			paticle = nullptr;
		}
	}
	m_paticleVector.erase(std::remove(m_paticleVector.begin(), m_paticleVector.end(), nullptr), m_paticleVector.end());
}
