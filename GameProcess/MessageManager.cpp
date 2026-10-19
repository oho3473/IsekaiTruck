#include "MessageManager.h"
#include "DirectY.h"

#include "SkinHeadNPC.h"
#include "NPCWaitManager.h"
#include "ClearManager.h"
#include "GameObject.h"
#include <locale>
#include <codecvt>
#include <exception>

MessageManager::MessageManager(NPCWaitManager* npcWaitManager, GameObject* dialogManager)
	: m_dialogs{}, m_messages{}, m_interviewQueue(nullptr)
	, m_useMessage(8, false), m_IsNPCMessage{}, m_chatMessages{}
	, m_clearCount(0), m_npcWaitManager(npcWaitManager), m_dialogManager(dialogManager)
	, m_time(0.0f), m_currentNPC(nullptr)
{
}

MessageManager::~MessageManager()
{
	m_dialogs.clear();
	m_messages.clear();
}

void MessageManager::Init() {}
void MessageManager::FixedUpdate() {}

void MessageManager::Update(float deltaTime)
{
	if (m_interviewQueue == nullptr || m_interviewQueue->empty())
	{
		ResetMessage();
		return;
	}

	SkinHeadNPC* npc = m_interviewQueue->front();
	if (npc == nullptr)
		return;

	if (m_currentNPC != npc)
	{
		ResetMessage();
		m_currentNPC = npc;
	}

	m_time += deltaTime;
	if (m_time <= 1.5f)
		return;

	m_time = 0.0f;

	if (!npc->GetIsWaiting())
		return;

	int NPCID = npc->GetNpcID();
	auto message = m_messages.find(NPCID);

	if (message != m_messages.end())
	{
		const auto& messages = message->second;

		for (int i = 0; i < (int)m_useMessage.size(); ++i)
		{
			if (m_useMessage[i])
				continue;

			std::string messageKey = "Message" + std::to_string(i + 1);
			auto message = messages.find(messageKey);
			if (message == messages.end() || message->second.empty())
				continue;

			int dialogID;
			try
			{
				dialogID = std::stoi(message->second);
			}
			catch (const std::exception&)
			{
				continue;
			}

			auto dialog = m_dialogs.find(dialogID);
			if (dialog == m_dialogs.end())
				continue;

			const auto& dialogs = dialog->second;
			auto text = dialogs.find("Text");
			auto speaker = dialogs.find("Speaker");
			if (text == dialogs.end() || speaker == dialogs.end())
				continue;

			if (speaker->second == "NPC" || speaker->second == "God")
			{
				m_chatMessages.push(text->second);
				m_IsNPCMessage.push(speaker->second == "NPC");
				m_useMessage[i] = true;
			}
		}
	}

	if (!m_chatMessages.empty())
	{
		static std::locale loc("");
		auto& facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
		std::wstring text = std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t>(&facet).from_bytes(m_chatMessages.front());
		npc->PrintLog(text, m_IsNPCMessage.front());
		m_IsNPCMessage.pop();
		m_chatMessages.pop();
	}
	else
	{
		Pop();
	}
}

void MessageManager::LateUpdate() {}
void MessageManager::PreRender() {}
void MessageManager::Render(DirectY* directY) {}
void MessageManager::PostRender(DirectY* directY) {}
void MessageManager::Release() {}

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
	std::ifstream file(path + ".csv", std::ios::binary);
	if (!file.is_open())
		return;

	std::string line;
	if (!std::getline(file, line))
		return;

	if (line.size() >= 3 &&
		(unsigned char)line[0] == 0xEF &&
		(unsigned char)line[1] == 0xBB &&
		(unsigned char)line[2] == 0xBF)
	{
		line.erase(0, 3);
	}

	std::vector<std::string> columnName;
	std::stringstream header(line);
	std::string cell;
	while (std::getline(header, cell, ','))
	{
		if (!cell.empty() && cell.back() == '\r')
			cell.pop_back();
		columnName.push_back(cell);
	}

	m_dialogs.clear();

	while (std::getline(file, line))
	{
		if (line.empty() || line == "\r")
			continue;

		std::vector<std::string> cells;
		cell.clear();
		bool quoted = false;

		for (int i = 0; i < (int)line.size(); ++i)
		{
			if (line[i] == '"')
			{
				if (quoted && i + 1 < (int)line.size() && line[i + 1] == '"')
				{
					cell += '"';
					++i;
				}
				else
				{
					quoted = !quoted;
				}
			}
			else if (line[i] == ',' && !quoted)
			{
				cells.push_back(cell);
				cell.clear();
			}
			else if (line[i] != '\r')
			{
				cell += line[i];
			}
		}
		cells.push_back(cell);

		std::unordered_map<std::string, std::string> CellData;
		for (int i = 0; i < (int)columnName.size() && i < (int)cells.size(); ++i)
		{
			CellData.insert({ columnName[i], cells[i] });
		}

		auto ID = CellData.find("ID");
		if (ID == CellData.end() || ID->second.empty())
			continue;

		try
		{
			int dialogID = std::stoi(ID->second);
			m_dialogs.insert({ dialogID, CellData });
		}
		catch (const std::exception&)
		{
			continue;
		}
	}

	ResetMessage();
}

void MessageManager::ReadMessageCSV(std::string path)
{
	std::ifstream file(path + ".csv", std::ios::binary);
	if (!file.is_open())
		return;

	std::string line;
	if (!std::getline(file, line))
		return;

	if (line.size() >= 3 &&
		(unsigned char)line[0] == 0xEF &&
		(unsigned char)line[1] == 0xBB &&
		(unsigned char)line[2] == 0xBF)
	{
		line.erase(0, 3);
	}

	std::vector<std::string> columnName;
	std::stringstream header(line);
	std::string cell;
	while (std::getline(header, cell, ','))
	{
		if (!cell.empty() && cell.back() == '\r')
			cell.pop_back();
		columnName.push_back(cell);
	}

	m_messages.clear();

	while (std::getline(file, line))
	{
		if (line.empty() || line == "\r")
			continue;

		std::vector<std::string> cells;
		cell.clear();
		bool quoted = false;

		for (int i = 0; i < (int)line.size(); ++i)
		{
			if (line[i] == '"')
			{
				if (quoted && i + 1 < (int)line.size() && line[i + 1] == '"')
				{
					cell += '"';
					++i;
				}
				else
				{
					quoted = !quoted;
				}
			}
			else if (line[i] == ',' && !quoted)
			{
				cells.push_back(cell);
				cell.clear();
			}
			else if (line[i] != '\r')
			{
				cell += line[i];
			}
		}
		cells.push_back(cell);

		std::unordered_map<std::string, std::string> CellData;
		for (int i = 0; i < (int)columnName.size() && i < (int)cells.size(); ++i)
		{
			CellData.insert({ columnName[i], cells[i] });
		}

		auto NPCID = CellData.find("NPC ID");
		if (NPCID == CellData.end() || NPCID->second.empty())
			continue;

		try
		{
			int npcID = std::stoi(NPCID->second);
			m_messages.insert({ npcID, CellData });
		}
		catch (const std::exception&)
		{
			continue;
		}
	}

	ResetMessage();
}

void MessageManager::ResetMessage()
{
	m_time = 0.0f;
	m_currentNPC = nullptr;
	for (int i = 0; i < (int)m_useMessage.size(); ++i)
		m_useMessage[i] = false;

	while (!m_IsNPCMessage.empty())
		m_IsNPCMessage.pop();
	while (!m_chatMessages.empty())
		m_chatMessages.pop();
}

void MessageManager::Pop()
{
	if (m_interviewQueue == nullptr || m_interviewQueue->empty())
		return;

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
	if (m_npcWaitManager != nullptr)
		m_npcWaitManager->isPoP();
	ResetMessage();
}
