#pragma once

#include <string>
#include <vector>

#include "ElementBase.h"

class DirectY;

class GameObject
{
public:
	GameObject(std::string name);
	~GameObject();

	void Init();

	void FixedUpdate();

	void Update(float deltaTime);
	void LateUpdate();

	/// 유니티에서는 Renderer라는 Component가 따로 있는데 오브젝트에 넣어도 문제가 없을 것 같다. 
	void PreRender();
	void Render(DirectY* directY);
	void PostRender(DirectY* directY);

	void Release();

	void SetElement(ElementBase* element, ElementType type);
	ElementBase* GetElement(ElementType type) const { return m_elements[(int)type]; }

	std::string GetName() const { return m_name; }
	void SetName(std::string name) { m_name = name; }

	void SetActive(bool isActive) { m_isActive = isActive; m_isDraw = isActive; }
	bool GetActive() { return m_isActive; }

	bool GetIsDraw() const { return m_isDraw; }
	void SetIsDraw(bool val) { m_isDraw = val; }

private:
	std::string m_name;
	/// element 를 종류별로 만들어서 초기화 시켜주는 함수 만들기
	std::vector<ElementBase*> m_elements;

	bool m_isActive = true;
	bool m_isDraw = true;
};