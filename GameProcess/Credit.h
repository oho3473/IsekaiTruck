#pragma once
#include "ElementBase.h"

#include <vector>
#include <string>
#include <functional>

#include "Vector2.h"

namespace Graphics { class Sprite; }

class Credit : public ElementBase
{
public:
	Credit(Graphics::Sprite* background, Graphics::Sprite* truck, int maxIndex);
	~Credit();

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

	//----------------------

	void AddSprite(Graphics::Sprite* sprite);
	void SetNPCFly(std::function<void(void)> npcFly) { m_npcFly = npcFly; }

	void SetGoTitle(std::function<void(void)> val) { m_goTitle = val; }
private:
	Graphics::Sprite* m_background;
	Graphics::Sprite* m_truck;

	std::vector<Graphics::Sprite*> m_cutScenes;
	std::vector<std::string> m_texts;

	MathEngine::Vector2 m_truckPos;

	std::function<void(void)> m_npcFly;
	std::function<void(void)> m_goTitle;
	bool isFly;

	int m_maxIndex;
	int m_index;
	float m_time;
};

