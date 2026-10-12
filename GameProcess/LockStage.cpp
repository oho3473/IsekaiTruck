#include "LockStage.h"
#include "ClearManager.h"

#include "DirectY.h"

#include "Transform.h"
#include "GameObject.h"

LockStage::~LockStage()
{
	delete m_clearSprite;
}

void LockStage::Render(DirectY* directY)
{
	// clear
	Transform* transform = ((Transform*)(m_gameObject->GetElement(ElementType::Transform)));

	for (int i = 0; i < ClearManager::GetInstance().GetClearStage(); i++)
	{
		if (i == 0)
		{
			transform->SetLocalPosition({ 60 + 50, 259 });
			transform->SetTransform(directY);
			directY->DrawBitmap(m_clearSprite);
		}
		if (i == 1)
		{
			transform->SetLocalPosition({ 676 + 50, 259 });
			transform->SetTransform(directY);
			directY->DrawBitmap(m_clearSprite);
		}
		if (i == 2)
		{
			transform->SetLocalPosition({ 1292 + 50, 259 });
			transform->SetTransform(directY);
			directY->DrawBitmap(m_clearSprite);
		}
	}

	for (int i = ClearManager::GetInstance().GetClearStage() + 1; i < 3; i++)
	{
		if (i == 1)
		{
			transform->SetLocalPosition({ 676 + 50, 259 });
			transform->SetTransform(directY);
			directY->DrawBitmap(m_image);
		}
		if (i == 2)
		{
			transform->SetLocalPosition({ 1292 + 50, 259 });
			transform->SetTransform(directY);
			directY->DrawBitmap(m_image);
		}
	}
}
