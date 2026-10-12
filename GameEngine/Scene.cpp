#include "Scene.h"

#include "GameObject.h"

// 정렬을 위해 필요함
#include <algorithm>
#include "Transform.h"

#include "../DirectY/DirectY.h"

Scene::Scene(std::string sceneName)
{
	m_name = sceneName;
}

Scene::~Scene()
{
	Release();
}

void Scene::Init()
{
	for (auto& object : m_objects)
	{
		object->Init();
	}
}

void Scene::FixedUpdate()
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate();
	}
}

void Scene::Update(float deltaTime)
{
	for (auto& object : m_objects)
	{
		object->Update(deltaTime);
	}
}

void Scene::LateUpdate()
{
	for (auto& object : m_objects)
	{
		object->LateUpdate();
	}
}

void Scene::PreRender()
{
	for (auto& object : m_objects)
	{
		object->PreRender();
	}
}

void Scene::Render(DirectY* directY)
{
	SortObjects();
	for (auto& object : m_objects)
	{
		object->Render(directY);
	}
}

void Scene::PostRender(DirectY* directY)
{
	for (auto& object : m_objects)
	{
		object->PostRender(directY);
	}
}

void Scene::Release()
{
	DeleteAllObject();
}

GameObject* Scene::CreateObject(std::string name)
{
	GameObject* newObject = new GameObject(name);
	AddObject(newObject);

	return newObject;
}

void Scene::AddObject(GameObject* gameObject)
{
	m_objects.push_back(gameObject);
}

void Scene::DeleteObject(std::string name)
{
	// Delete 어떻게 할지 생각 중...
	std::vector<GameObject*>::iterator findObject = std::find_if(m_objects.begin(), m_objects.end(), [=](GameObject* gameObject) {return gameObject->GetName() == name; });
	GameObject* deleteObject = *findObject;

	m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), [=](GameObject* gameObject) {return gameObject->GetName() == name; }), m_objects.end());
	delete deleteObject;
}

GameObject* Scene::FindObject(std::string name)
{
	std::vector<GameObject*>::iterator findObject = std::find_if(m_objects.begin(), m_objects.end(), [=](GameObject* gameObject) {return gameObject->GetName() == name; });
	return *findObject;
}

void Scene::DeleteAllObject()
{
	for (auto& object : m_objects)
	{
		delete object;
	}
	m_objects.clear();
}

bool Compare(GameObject* a, GameObject* b)
{
	int aLayer = ((Transform*)(a->GetElement(ElementType::Transform)))->GetLayer();
	int bLayer = ((Transform*)(b->GetElement(ElementType::Transform)))->GetLayer();

	return aLayer < bLayer;
}

void Scene::SortObjects()
{
	std::sort(m_objects.begin(), m_objects.end(), Compare);
}
