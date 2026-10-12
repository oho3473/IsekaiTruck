#include "ResourceManager.h"
#include <iostream>
#include <fstream>

#include <algorithm>

#include "../DirectY/DirectY.h"

#include "SoundManager.h"

/// <summary>
/// 파일 명만 잘라서 리턴한다.
/// </summary>
/// <param name="path">파일 path</param>
/// <returns>파일 이름</returns>
std::string GetFileName(std::string path)
{
	int lastPoint = path.find_last_of(".");
	int lastSlash = path.find_last_of("/");

	return path.substr(lastSlash + 1, lastPoint - lastSlash - 1);
}

void ResourceManager::Init()
{

}

/// <summary>
/// 텍스트 파일에서 Path를 읽어 리소스를 불러온다. 
/// 매 씬에서 한 번만 해야 한다.
/// 
/// SceneManager에서 Scene을 Load할 때 해 주는 게 좋을 것 같다.
/// </summary>
/// <param name="sceneName"></param>
/// <param name="directY"></param>
void ResourceManager::LoadResource(std::string sceneName, DirectY& directY)
{
	std::ifstream resourceTextFile("Resource/Text/Scene/" + sceneName + ".txt");
	std::string path;
	if (resourceTextFile.is_open())
	{
		// txt 파일 한 줄 씩 읽기
		while (std::getline(resourceTextFile, path))
		{
			// 받아온 string이(Line이) 공백일 경우 프로그램 종료
			if (path._Equal(""))
			{
				break;
			}

			// 파일 확장자 추출
			std::string fileExtension = path.substr(path.length() - 3);
			if (fileExtension._Equal("png"))
			{
				AddImage(path, directY);
			}
			if (fileExtension._Equal("jpg"))
			{
				AddImage(path, directY);
			}
			if (fileExtension._Equal("mp3"))
			{
				AddSound(path);
			}
			if (fileExtension._Equal("wav"))
			{
				AddSound(path);
			}
		}
		resourceTextFile.close();
	}
	else
	{
		std::cout << "파일 오픈 실패...";
	}
	return;
}

void ResourceManager::Release()
{
	for (auto& image : m_images)
	{
		if (image.second != nullptr)
		{
			image.second->Release();
		}
	}
	m_images.clear();

	for (auto& sound : m_sounds)
	{
		if (sound.second != nullptr)
		{
			sound.second->release();
		}
	}
	m_sounds.clear();
}

void ResourceManager::AddImage(std::string path, DirectY& directY)
{
	// string -> wstring 변환
	std::wstring pathWStr;
	pathWStr.assign(path.begin(), path.end());

	// 그래픽스 엔진을 이용해서 Bitmap 불러오기
	ID2D1Bitmap* loadedImage = directY.LoadImageFile(pathWStr.c_str());

	// 확장자와 경로를 제외한 파일 이름을 가져온다. 
	std::string fileName = GetFileName(path);

	m_images.insert({ fileName, loadedImage });
}

void ResourceManager::AddSound(std::string path)
{
	m_sounds.insert({ GetFileName(path), SoundManager::GetInstance().LoadSound(path) });
}