#pragma once

#include <string>
#include <vector>

class GameObject;

class DirectY;

class Scene
{
public:
	Scene(std::string sceneName);
	~Scene();

	void Init();

	void FixedUpdate();
	void Update(float deltaTime);
	void LateUpdate();

	void PreRender();
	void Render(DirectY* directY);
	void PostRender(DirectY* directY);

	void Release();

	GameObject* CreateObject(std::string name);
	void		AddObject(GameObject* gameObject);
	void		DeleteObject(std::string name);
	GameObject* FindObject(std::string name);

	void DeleteAllObject();

	/// 오브젝트 레이어가 낮은 것부터 (부모부터) TransformMatrix를 만들기 위해 Sort를 한다. 기준은 Transform의 Layer
	void		SortObjects();

	std::string GetName() const { return m_name; }

private:
	std::string m_name;
	std::vector<GameObject*> m_objects;
};