#pragma once

#include <unordered_map>
#include <string>

struct ID2D1Bitmap;
namespace FMOD { class Sound; } // SoundManager가 생기면 FMOD의 Sound로 변경
class DirectY;

class ResourceManager
{
public:
	void Init();

	/// Text 파일에서 Path를 불러오고 파일 이름을 읽어서 식별자로 등록해준다. 
	/// 
	/// 인자로 받은 씬에 해당하는 Images, Sounds를 불러온다. 또 필요한 리소스가 있다면 추가한다.
	/// 이미지 확장자: png, jpg는 파일 자체에 투명도가 없어서 아트랑 협의 후 필요하면 추가 예정
	/// 사운드 확장자: mp3, wav, 더 필요하다면 추가
	void LoadResource(std::string sceneName, DirectY& directY);

	FMOD::Sound* GetSound(std::string soundName) { return m_sounds[soundName]; }
	ID2D1Bitmap* GetImage(std::string imageName) { return m_images[imageName]; }

	void Release();

private:
	void AddImage(std::string path, DirectY& directY);
	void AddSound(std::string path);

private:
	std::unordered_map<std::string, ID2D1Bitmap*> m_images;
	std::unordered_map<std::string, FMOD::Sound*> m_sounds;
};