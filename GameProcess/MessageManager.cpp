#include "MessageManager.h"
#include "DirectY.h"

#include "SkinHeadNPC.h"
#include "NPCWaitManager.h"
#include "ClearManager.h"
#include "GameObject.h"
#include <locale>
#include <codecvt>

MessageManager::MessageManager(NPCWaitManager* npcWaitManager, GameObject* dialogManager)
	: m_dialogs{}, m_messages{}, m_interviewQueue{}, m_useMessage(8, false)
	, m_IsNPCMessage{}, m_chatMessages{}
	, m_npcWaitManager(npcWaitManager) , m_dialogManager(dialogManager)
{

}

MessageManager::~MessageManager()
{
	m_dialogs.clear();
	m_messages.clear();
}

void MessageManager::Init()
{

}

void MessageManager::FixedUpdate()
{

}

void MessageManager::Update(float deltaTime)
{
	static float time = 0;

	time += deltaTime;

	if (time > 1.5)
	{
		if (!m_interviewQueue->empty())
		{
			int NPCID = (m_interviewQueue->front())->GetNpcID();
			std::string IDStr = std::to_string(NPCID);

			if ((m_interviewQueue->front())->GetIsWaiting())
			{
				for (int i = 0; i < (int)m_messages.size(); i++)
				{
					if ((IDStr.compare(m_messages[i]["NPC ID"])) == 0)
					{
						for (int j = 0; j < (int)m_dialogs.size(); j++)
						{
							if (!m_useMessage[0] && (m_messages[i]["Message1"]).compare(m_dialogs[j]["ID"]) == 0)
							{
								m_chatMessages.push(m_dialogs[j]["Text"]);
								m_useMessage[0] = true;
								if (m_dialogs[j]["Speaker"].compare("NPC") == 0)
								{
									m_IsNPCMessage.push(true);
								}
								else if (m_dialogs[j]["Speaker"].compare("God") == 0)
								{
									m_IsNPCMessage.push(false);
								}
							}

							if (!m_useMessage[1] && (m_messages[i]["Message2"]).compare(m_dialogs[j]["ID"]) == 0)
							{
								m_chatMessages.push(m_dialogs[j]["Text"]);
								m_useMessage[1] = true;
								if (m_dialogs[j]["Speaker"].compare("NPC") == 0)
								{
									m_IsNPCMessage.push(true);
								}
								else if (m_dialogs[j]["Speaker"].compare("God") == 0)
								{
									m_IsNPCMessage.push(false);
								}
							}

							if (!m_useMessage[2] && (m_messages[i]["Message3"]).compare(m_dialogs[j]["ID"]) == 0)
							{
								m_chatMessages.push(m_dialogs[j]["Text"]);
								m_useMessage[2] = true;
								if (m_dialogs[j]["Speaker"].compare("NPC") == 0)
								{
									m_IsNPCMessage.push(true);
								}
								else if (m_dialogs[j]["Speaker"].compare("God") == 0)
								{
									m_IsNPCMessage.push(false);
								}
							}

							if (!m_useMessage[3] && (m_messages[i]["Message4"]).compare(m_dialogs[j]["ID"]) == 0)
							{
								m_chatMessages.push(m_dialogs[j]["Text"]);
								m_useMessage[3] = true;
								if (m_dialogs[j]["Speaker"].compare("NPC") == 0)
								{
									m_IsNPCMessage.push(true);
								}
								else if (m_dialogs[j]["Speaker"].compare("God") == 0)
								{
									m_IsNPCMessage.push(false);
								}
							}

							if (!m_useMessage[4] && (m_messages[i]["Message5"]).compare(m_dialogs[j]["ID"]) == 0)
							{
								m_chatMessages.push(m_dialogs[j]["Text"]);
								m_useMessage[4] = true;
								if (m_dialogs[j]["Speaker"].compare("NPC") == 0)
								{
									m_IsNPCMessage.push(true);
								}
								else if (m_dialogs[j]["Speaker"].compare("God") == 0)
								{
									m_IsNPCMessage.push(false);
								}
							}

							if (!m_useMessage[5] && (m_messages[i]["Message6"]).compare(m_dialogs[j]["ID"]) == 0)
							{
								m_chatMessages.push(m_dialogs[j]["Text"]);
								m_useMessage[5] = true;
								if (m_dialogs[j]["Speaker"].compare("NPC") == 0)
								{
									m_IsNPCMessage.push(true);
								}
								else if (m_dialogs[j]["Speaker"].compare("God") == 0)
								{
									m_IsNPCMessage.push(false);
								}
							}

							if (!m_useMessage[6] && (m_messages[i]["Message7"]).compare(m_dialogs[j]["ID"]) == 0)
							{
								m_chatMessages.push(m_dialogs[j]["Text"]);
								m_useMessage[6] = true;
								if (m_dialogs[j]["Speaker"].compare("NPC") == 0)
								{
									m_IsNPCMessage.push(true);
								}
								else if (m_dialogs[j]["Speaker"].compare("God") == 0)
								{
									m_IsNPCMessage.push(false);
								}
							}

							if (!m_useMessage[7] && (m_messages[i]["Message8"]).compare(m_dialogs[j]["ID"]) == 0)
							{
								m_chatMessages.push(m_dialogs[j]["Text"]);
								m_useMessage[7] = true;
								if (m_dialogs[j]["Speaker"].compare("NPC") == 0)
								{
									m_IsNPCMessage.push(true);
								}
								else if (m_dialogs[j]["Speaker"].compare("God") == 0)
								{
									m_IsNPCMessage.push(false);
								}
							}
						}
					}
				}

				if (!m_chatMessages.empty())
				{
					static std::locale loc("");
					auto& facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
					std::wstring text = std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t>(&facet).from_bytes(m_chatMessages.front());
					(m_interviewQueue->front())->PrintLog(text,m_IsNPCMessage.front());
					m_IsNPCMessage.pop();
					m_chatMessages.pop();
				}
				else
				{
					Pop();
				}
			}
		}
		time = 0;
	}

}

void MessageManager::LateUpdate()
{

}

void MessageManager::PreRender()
{

}

void MessageManager::Render(DirectY* directY)
{
}

void MessageManager::PostRender(DirectY* directY)
{

}

void MessageManager::Release()
{

}

ElementType MessageManager::GetElementType() const
{
	return ElementType::Script;
}

GameObject* MessageManager::GetGameObject() const
{
	return m_gameObject;
}

void MessageManager::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}

void MessageManager::ReadDialogCSV(std::string path)
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

		//UTF 8용
		//line = line.substr(offset, line.size());
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
			CellData.insert({ columnName[index], line.substr(0,found) });
			line = line.substr(found + 1, line.size());
			index++;

			//text 뽑기
			prefound = 0;

			found = line.find_first_of("\"\"\"", prefound + offset);

			cell = line.substr(prefound + offset, found - offset);
			prefound = found;
			CellData.insert({ columnName[index],cell });
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
				CellData.insert({ columnName[endindex], line.substr(prefound + 1, found - prefound - 1) });
				index++;
				endindex++;
			}

			m_dialogs.insert({ lineindex ,CellData });

			CellData.clear();

			index = 0;
			lineindex++;
		}

		file.close();
	}
}

void MessageManager::ReadMessageCSV(std::string path)
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

		//UTF 8용
		//line = line.substr(offset, line.size());
		/*found = line.find_first_of(',');
		lastfound = line.find_last_of(',');

		columnName.push_back(line.substr(0, found));

		line = line.substr(found + 1, line.size());*/

		found = line.find_first_of(',');
		lastfound = line.find_last_of(',');
		prefound = 0;
		while (lastfound != prefound - 1)
		{
			columnName.push_back(line.substr(prefound, found - prefound));
			prefound = found + 1;
			found = line.find_first_of(',', found + 1);
			endindex++;
		}
		columnName.push_back(line.substr(lastfound + 1, line.size()));
		endindex++;

		std::unordered_map<std::string, std::string> CellData;

		///데이타 저장
		int lineindex = 0;
		while (std::getline(file, line))
		{
			if (line._Equal(""))
			{
				break;
			}

			endindex = 0;
			prefound = 0;
			found = line.find_first_of(',');
			lastfound = line.find_last_of(',');	//여기서 숫자가 이상하다 데이터 누락된다

			//','를 기준으로 하나씩 저장
			//주의 사항 cell text 값 안에 """ㅁ""ㄴㅇ """ 이건 오류남 이거 절대 하면 안대 
			while (lastfound != prefound - 1)
			{
				CellData.insert({ columnName[endindex], line.substr(prefound, found - prefound) });
				prefound = found + 1;
				found = line.find_first_of(',', found + 1);

				endindex++;
			}

			CellData.insert({ columnName[endindex], line.substr(lastfound + 1,line.size()) });


			m_messages.insert({ lineindex ,CellData });

			CellData.clear();

			index = 0;
			lineindex++;
		}

		file.close();
	}
}

void MessageManager::Pop()
{

	if (m_interviewQueue->front()->GetHero())
	{
		m_clearCount--;

		if (m_dialogManager != nullptr)
		{
			if (m_clearCount == 0)
			{
				ClearManager::GetInstance().SetClear(true);
				m_dialogManager->SetActive(true);
			}
		}
	}

	m_interviewQueue->pop();
	m_npcWaitManager->isPoP();

	for (int i = 0; i <(int)m_useMessage.size(); ++i)
	{
		m_useMessage[i] = false;
	}
}
