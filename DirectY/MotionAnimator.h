#pragma once
#include <map>
#include <string>
#include "Sprite.h"

/// <summary>
/// 모션을 관리하고 그려줄 클래스
/// </summary>
class MotionAnimator
{
public:


private:
	std::map<std::string, Graphics::Motion*> m_MotionManager;
};

