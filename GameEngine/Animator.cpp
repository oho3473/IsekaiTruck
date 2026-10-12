#include "Animator.h"
#include "../DirectY/DirectY.h"

#include "Transform.h"
#include "GameObject.h"
#include "../GameProcess/SkinHeadNPC.h"

Animator::Animator() : ElementBase(), m_motionMap(NULL), m_motionIndex(0), m_currentMotion("link")
{
}

Animator::~Animator()
{
	Release();
}

void Animator::Init()
{

}

void Animator::FixedUpdate()
{

}

/// <summary>
/// 업데이트하면서 어떤 모션을 쓸지 셋팅해준다
/// </summary>
/// <param name="deltaTime"></param>
void Animator::Update(float deltaTime)
{
	//바뀌었는지 확인을 위한 이전 모션을 저장할 변수
	std::string prevMotion = m_currentMotion;

	//테스트용
	//키입력에 따라 어떤 상태인지 셋팅해 어떤 모션을 쓸지 정한다
	//if(InputManager::GetInstance().IsGetKeyDown(VK_UP/*test*/))
	//{
	//	if (prevMotion == m_currentMotion/*바뀌면*/)
	//	{
	//		m_motionIndex = 0;	//모션이 바뀌면서 다른 애니메이션 처음부터해야된다
	//		m_currentMotion = "test";	//key값을 바꾼다
	//	}
	//}

	int totalFrame = GetMotion(m_currentMotion)->GetTotalFrame();

	//static float totalTime = 0;
	//totalTime += deltaTime;

	m_totalTime += deltaTime;

	//시간이 누적될때마다 특정시간(delayTime)마다 index를 바꿔 스프라이트 교체
	if (m_totalTime > m_delayTime/*GetMotion(m_currentMotion)->GetSprite(m_motionIndex)->m_delayTime*/)
	{
		if (m_motionIndex >= totalFrame-1)
		{
			m_motionIndex = 0;
		}
		else
		{
			m_motionIndex++;
		}

		m_totalTime = 0;
	}
}

void Animator::LateUpdate()
{

}

void Animator::PreRender()
{
	
}

void Animator::Render(DirectY* directY)
{
	// 만약 크레딧 씬이라면
	SkinHeadNPC* temp = ((SkinHeadNPC*)(m_gameObject->GetElement(ElementType::Script)));

	if (temp->GetCurState() == SkinHeadNPC::State::Credit)
	{
		directY->SetTransform(((Transform*)(m_gameObject->GetElement(ElementType::Transform)))->GetTransformMatrix());
	}

	directY->DrawBitmap(GetMotion(m_currentMotion)->GetSprite(m_motionIndex));
}

void Animator::PostRender(DirectY* directY)
{
	//그릴거 다 그리고 이펙트 같은거
}

void Animator::Release()
{
	for (auto& motion : m_motionMap)
	{
		delete motion.second;
	}
	m_motionMap.clear();
}

ElementType Animator::GetElementType() const
{
	return ElementType::Animator;
}

GameObject* Animator::GetGameObject() const
{
	return m_gameObject;
}

void Animator::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}

/// <summary>
/// 저장된 모션을 애니메이터에 저장한다
/// 반드시 그래픽스엔진에서 ReadTextFile함수로 모션에 sprite 저장한 motion을 쓸 것
/// </summary>
/// <param name="motionName">key 값</param>
/// <param name="motion">value 값</param>
void Animator::AddMotion(std::string motionName, Graphics::Motion* motion)
{
	m_motionMap.insert({ motionName, motion });
}
