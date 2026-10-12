#pragma once
#include <ElementBase.h>
#include "../DirectY/DirectY.h"

class GameEngine;
class DirectY;

namespace Graphics { struct Sprite; }
namespace Graphics { class Motion; }



class CloudObject : public ElementBase
{
public:
	enum class State
	{
		cloud1_1,
		cloud1_2,
		cloud2_1,
		cloud2_2,
		cloud3_1,
		cloud3_2,
		cloud4_1,
		cloud4_2,
		cloud5_1,
		cloud5_2,
		cloud6_1,
		cloud6_2,
	};
public:
	CloudObject(GameEngine* gameEngine, DirectY* directY, CloudObject::State state);
	virtual ~CloudObject();
public:
	
	/*------------------------------------------------------------*/
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
	/*------------------------------------------------------------*/

public:

private:
	GameEngine* m_gameEngine = nullptr;
	DirectY* m_directY = nullptr;

	Graphics::Sprite* cloud1_1 = nullptr;
	Graphics::Sprite* cloud1_2 = nullptr;
	Graphics::Sprite* cloud2_1 = nullptr;
	Graphics::Sprite* cloud2_2 = nullptr;
	Graphics::Sprite* cloud3_1 = nullptr;
	Graphics::Sprite* cloud3_2 = nullptr;
	Graphics::Sprite* cloud4_1 = nullptr;
	Graphics::Sprite* cloud4_2 = nullptr;
	Graphics::Sprite* cloud5_1 = nullptr;
	Graphics::Sprite* cloud5_2 = nullptr;
	Graphics::Sprite* cloud6_1 = nullptr;
	Graphics::Sprite* cloud6_2 = nullptr;

	State m_currentImage = {};
};

