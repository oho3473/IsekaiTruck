#pragma once
#include "ElementBase.h"

#include <vector>
#include <string>
#include <queue>


namespace Graphics { class Motion; }

namespace Graphics { struct Sprite; }

class Scrollbar : public ElementBase
{
public:
	Scrollbar(int left, int top, int right,int bottom,Graphics::Sprite* background, Graphics::Sprite* bar, Graphics::Motion* messagebubble1, Graphics::Motion* messagebubble2);
	~Scrollbar();

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

	std::vector<std::pair<std::wstring, bool>>& GetTexts() { return m_texts; }

	virtual void SetGameObject(GameObject* gameObject) override;
	void SetHeroChatWindow(Graphics::Motion* herochatwindow) { m_herochatwindow = herochatwindow; }
	void SetGoddessChatWindow(Graphics::Motion* chatwindow) { m_goddesschatwindow = chatwindow; }
	void AddText(std::wstring text, bool isNpc);

private:

	int m_left;
	int m_right;
	int m_top;
	int m_bottom;

	Graphics::Sprite* m_background;
	Graphics::Sprite* m_bar;

	Graphics::Motion* m_herochatwindow;
	Graphics::Motion* m_goddesschatwindow;
	std::vector<std::pair<std::wstring,bool>> m_texts;
	
	const int m_lineInteval;
	float m_interval;
	int m_maxLine;

	int m_offsetY = 25;
	int m_offsetX = 35;
};