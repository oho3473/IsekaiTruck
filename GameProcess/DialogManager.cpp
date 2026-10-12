#include "DialogManager.h"
#include "Sprite.h"
#include "DirectY.h"

#include <fstream>
#include <locale>
#include <codecvt>

#include "InputManager.h"
#include "AudioSource.h"
#include "Transform.h"
#include "GameObject.h"

DialogManager::DialogManager(int left, int top, int right, int bottom, int ID, Graphics::Sprite* dialogImage, Graphics::Sprite* dialogGotImage)
	: m_left(left), m_right(right), m_top(top), m_bottom(bottom)
	, m_ID(ID), m_currentIndex(0)
	, m_audiosource(nullptr)
	, m_sprites() 
	, m_dialogPosition({})
	, m_dialogText(), m_speaker()
	, m_imageIndexL(), m_imageIndexM(), m_imageIndexR()
	, m_BGI(), m_Fade()
	, m_dialogImage(dialogImage), m_dialogGodImage(dialogGotImage)
{

}

DialogManager::~DialogManager()
{
	delete m_dialogImage;
	delete m_dialogGodImage;

	for (auto& sprite : m_sprites)
	{
		delete sprite.second;
	}
}

void DialogManager::Init()
{

}

void DialogManager::FixedUpdate()
{

}

void DialogManager::Update(float deltaTime)
{
	if ((int)m_dialogText.size() >= m_currentIndex + 1)
	{
		if (InputManager::GetInstance().IsAnyKeyDown())
		{
			m_audiosource->PlayBack("sfx_dialog_1");
			m_currentIndex++;
		}
	}
	else
	{
		StartGame();
	}
}

void DialogManager::LateUpdate()
{

}

void DialogManager::PreRender()
{

}

void DialogManager::Render(DirectY* directY)
{
	Transform* transform = ((Transform*)(m_gameObject->GetElement(ElementType::Transform)));

	if ((unsigned int)m_currentIndex < m_dialogText.size())
	{
		if (!m_BGI[m_currentIndex]._Equal(""))
		{
			transform->SetLocalPosition({ 0, 0 });
			transform->SetTransform(directY);
			directY->DrawBitmap(m_sprites[m_BGI[m_currentIndex]]);
		}

		if (!m_imageIndexL[m_currentIndex]._Equal(""))
		{
			transform->SetLocalPosition({ (float)m_left, 110 });
			transform->SetTransform(directY);
			directY->DrawBitmap(m_sprites[m_imageIndexL[m_currentIndex]]);
		}
		if (!m_imageIndexM[m_currentIndex]._Equal(""))
		{
			transform->SetLocalPosition({ (float)((m_right - m_left) / 2 - m_sprites[m_imageIndexM[m_currentIndex]]->m_drawSize.right / 2), -50 });
			transform->SetTransform(directY);
			directY->DrawBitmap(m_sprites[m_imageIndexM[m_currentIndex]]);
		}
		if (!m_imageIndexR[m_currentIndex]._Equal(""))
		{
			transform->SetLocalPosition({ (float)(m_right - m_sprites[m_imageIndexR[m_currentIndex]]->m_drawSize.right), -50 });
			transform->SetTransform(directY);
			directY->DrawBitmap(m_sprites[m_imageIndexR[m_currentIndex]]);
		}

		transform->SetLocalPosition({ 0, 0 });
		transform->SetTransform(directY);

		if (m_speaker[m_currentIndex] == "여신")
		{
			directY->DrawBitmap(m_dialogGodImage);
		}
		else
		{
			directY->DrawBitmap(m_dialogImage);
		}

		{
			transform->SetLocalPosition({ m_textOffset.x - 30, m_textOffset.y - 40 });
			transform->SetTransform(directY);

			static std::locale loc("");
			auto& facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
			std::wstring text = std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t>(&facet).from_bytes(m_speaker[m_currentIndex]);

			directY->SetBrushColor(1.f, 1.f, 0.3f, 1.f);
			directY->DrawText(text.c_str(), 0, 0, 40, TextAlignMent::LEFT, L"Danjo");
		}

		transform->SetLocalPosition({ m_textOffset.x, m_textOffset.y + 40 });
		transform->SetTransform(directY);

		static std::locale loc("");
		auto& facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
		std::wstring text = std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t>(&facet).from_bytes(m_dialogText[m_currentIndex]);

		directY->SetBrushColor(1.f, 1.f, 1.f, 1.f);
		directY->DrawText(text.c_str(), 0, 0, 30, TextAlignMent::LEFT, L"Danjo");
	}
}

void DialogManager::PostRender(DirectY* directY)
{

}

void DialogManager::Release()
{

}

ElementType DialogManager::GetElementType() const
{
	return ElementType::Script;
}

GameObject* DialogManager::GetGameObject() const
{
	return m_gameObject;
}

void DialogManager::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}

void DialogManager::ReadDialogCSV(std::string path)
{
	std::ifstream file(path + ".csv");

	std::string line;
	std::string cell;

	std::vector<std::string> columnName;

	int offset = 3;

	std::size_t prefound = 0;
	std::size_t found = 0;
	std::size_t lastfound = 0;

	//파일을 제대로 못 읽으면 생성하면
	if (file.fail() == true)
	{
		return;
	}

	if (file.is_open())
	{
		int index = 0;
		int endindex = 0;

		/// cell 항목 받아오기
		std::getline(file, line);

		found = line.find_first_of(',');
		lastfound = line.find_last_of(',');

		columnName.push_back(line.substr(0, found));

		while (lastfound != prefound)
		{
			prefound = found;
			found = line.find_first_of(',', found + 1);
			columnName.push_back(line.substr(prefound + 1, found - prefound - 1));
			endindex++;
		}

		std::unordered_map<std::string, std::string> CellData;

		///데이타 저장
		int lineindex = 0;
		while (std::getline(file, line))
		{
			if (line._Equal(""))
			{
				break;
			}

			//행 인덱스 값
			found = line.find_first_of(',');
			int id = std::stoi(line.substr(0, found));
			if (id / m_ID != 1 || (id % m_ID) >= 20)
			{
				continue;
			}

			line = line.substr(found + 1, line.size());
			index++;

			//text 뽑기
			prefound = 0;

			found = line.find_first_of("\"\"\"", prefound + offset);

			cell = line.substr(prefound + offset, found - offset);
			prefound = found;
			m_dialogText.push_back(cell);
			//CellData.insert({ columnName[index],cell });
			index++;

			//text를 제외한 한줄 저장
			line = line.substr(found + offset, line.size());

			endindex = 2;
			prefound = 0;
			found = line.find_first_of(',');
			lastfound = line.find_last_of(',');

			//','를 기준으로 하나씩 저장
			//주의 사항 cell text 값 안에 """ㅁ""ㄴㅇ """ 이건 오류남 이거 절대 하면 안대 
			while (lastfound != prefound)
			{
				prefound = found;
				found = line.find_first_of(',', found + 1);
				if (index == 2)
				{
					m_speaker.push_back(line.substr(prefound + 1, found - prefound - 1));
				}
				if (index == 3)
				{
					m_imageIndexL.push_back(line.substr(prefound + 1, found - prefound - 1));
				}
				if (index == 4)
				{
					m_imageIndexM.push_back(line.substr(prefound + 1, found - prefound - 1));
				}
				if (index == 5)
				{
					m_imageIndexR.push_back(line.substr(prefound + 1, found - prefound - 1));
				}
				if (index == 6)
				{
					m_BGI.push_back(line.substr(prefound + 1, found - prefound - 1));
				}
				if (index == 7)
				{
					m_Fade.push_back(line.substr(prefound + 1, found - prefound - 1));
				}
				//CellData.insert({ columnName[endindex], line.substr(prefound + 1, found - prefound - 1) });
				index++;
				endindex++;
			}

			//m_dialogs.insert({ lineindex ,CellData });

			CellData.clear();

			index = 0;
			lineindex++;
		}

		if (file.is_open() == true)
		{
			file.close();
		}
	}
}

void DialogManager::AddSprite(std::string name, Graphics::Sprite* sprite)
{
	m_sprites[name] = sprite;
}
