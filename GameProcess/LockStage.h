#pragma once
#include "BackgroundImage.h"

class LockStage : public BackgroundImage
{
public:
	LockStage(Graphics::Sprite* lock, Graphics::Sprite* clear) : BackgroundImage(lock), m_clearSprite(clear) {}
	virtual ~LockStage();

	virtual void Render(DirectY* directY) override;

private:
	Graphics::Sprite* m_clearSprite;
};

