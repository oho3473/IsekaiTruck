#pragma once
#include "ElementBase.h"
#include "Vector2.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>

namespace Graphics { struct Sprite; }
class AudioSource;

class DialogManager : public ElementBase
{
public:
	// -------------------------------------------
	DialogManager(int left, int top, int right, int bottom, int ID, Graphics::Sprite* dialogImage, Graphics::Sprite* dialogGotImage);
	virtual ~DialogManager();

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
	// -------------------------------------------

	void ReadDialogCSV(std::string path);
	void SetStartGame(std::function<void(void)> val) { StartGame = val; }

	void SetTextOffset(MathEngine::Vector2 offset) { m_textOffset = offset; }

	void AddSprite(std::string name, Graphics::Sprite* sprite);

	void SetCurrentIndex(int val) { m_currentIndex = val; }

	void SetAudioSource(AudioSource* audioSource) { m_audiosource = audioSource; }

private:
	// Rect
	// 왼쪽 위부터 0, 0, UI 있는 곳까지가 영역
	int m_left;
	int m_top;
	int m_right;
	int m_bottom;

	int m_ID;
	int m_currentIndex;

	AudioSource* m_audiosource;

	// 왼쪽에 캐릭터, 오른쪽에 여신? (스테이지 시작시) 
	/// 메모리 해제해야함
	std::unordered_map<std::string, Graphics::Sprite*> m_sprites;
	
	// 아래쪽에 다이얼로그 창 (스테이지 시작시)
	Graphics::Sprite* m_dialogImage;
	Graphics::Sprite* m_dialogGodImage;

	MathEngine::Vector2 m_dialogPosition;

	std::vector<std::string> m_speaker;
	std::vector<std::string> m_dialogText;
	MathEngine::Vector2 m_textOffset;

	std::vector<std::string> m_imageIndexL;
	std::vector<std::string> m_imageIndexM;
	std::vector<std::string> m_imageIndexR;

	std::vector<std::string> m_BGI;
	std::vector<std::string> m_Fade;
	// 근데 다이얼 로그 창은 그냥 중앙일 수 도 있음 (스테이지 종료시)
	// 스테이지 종료시 나오는 건 왼쪽 오른쪽이 아니고 그냥 이미지
	// 영역을 잡고 계산해야 될듯
	
	// 다이얼로그 CSV Reader가 필요함 - 승운 오빠거 가지고 오거나 부탁

	std::function<void(void)> StartGame;
};