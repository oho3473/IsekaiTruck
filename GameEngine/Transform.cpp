#include "Transform.h"
#include "../DirectY/MakeMatrix.h"
#include "CameraManager.h"

Transform::Transform()
	:ElementBase(), 
	m_localPosition({ 0, 0 }), m_localRotation(0.0f), m_localScale({ 1, 1 }),
	m_transformMatrix({ 1, 0, 0, 1, 0, 0 }),
	m_parentTransform(nullptr), m_layer(0), m_isUseCamera(false)
{

}

Transform::~Transform()
{

}

void Transform::Init()
{

}

void Transform::FixedUpdate()
{

}

void Transform::Update(float deltaTime)
{
	//m_transformMatrix = MakeMatrix()({ m_localScale.x, m_localScale.y }, m_localRotation, { m_localPosition.x, m_localPosition.y });
}

void Transform::LateUpdate()
{

}

void Transform::PreRender()
{

}

void Transform::Render(DirectY* directY)
{
	SetTransform(directY);
}

void Transform::PostRender(DirectY* directY)
{
	SetTransform(directY);
}

void Transform::Release()
{
	
}

ElementType Transform::GetElementType() const
{
	return ElementType::Transform;
}

GameObject* Transform::GetGameObject() const
{
	return m_gameObject;
}

void Transform::SetGameObject(GameObject* gameObject)
{
	m_gameObject = gameObject;
}

void Transform::SetLocalPosition(Vector2 position)
{
	m_localPosition = position;
	m_transformMatrix = MakeMatrix()({ m_localScale.x, m_localScale.y }, m_localRotation, { m_localPosition.x, m_localPosition.y });
}

void Transform::SetLocalRotation(float rotation)
{
	m_localRotation = rotation;
	m_transformMatrix = MakeMatrix()({ m_localScale.x, m_localScale.y }, m_localRotation, { m_localPosition.x, m_localPosition.y });
}

void Transform::SetLocalScale(Vector2 scale)
{
	m_localScale = scale;
	m_transformMatrix = MakeMatrix()({ m_localScale.x, m_localScale.y }, m_localRotation, { m_localPosition.x, m_localPosition.y });
}

MathEngine::Vector2 Transform::GetWorldPosition() const
{
	if (m_parentTransform != nullptr)
	{
		D2D1_MATRIX_3X2_F worldMatrix = m_parentTransform->GetTransformMatrix();
		return DirectY::MarixToVector(worldMatrix, m_localPosition);
	}
	else
	{
		return m_localPosition;
	}
}

D2D1_MATRIX_3X2_F Transform::GetWorldMatrix() const
{
	if (m_parentTransform != nullptr)
	{
		return m_transformMatrix * m_parentTransform->GetTransformMatrix();
	}
	else
	{
		return m_transformMatrix;
	}
}

MathEngine::Vector2 Transform::GetScreenPosition() const
{
	if (m_parentTransform != nullptr)
	{
		if (m_isUseCamera)
		{
			D2D1_MATRIX_3X2_F screenMatrix = m_parentTransform->GetTransformMatrix() * CameraManager::GetInstance().GetCameraMatrix();
			return DirectY::MarixToVector(screenMatrix, m_localPosition);
		}
		else
		{
			D2D1_MATRIX_3X2_F screenMatrix = m_parentTransform->GetTransformMatrix();
			return DirectY::MarixToVector(screenMatrix, m_localPosition);
		}
	}
	else
	{
		if (m_isUseCamera)
		{
			D2D1_MATRIX_3X2_F screenMatrix = CameraManager::GetInstance().GetCameraMatrix();
			return DirectY::MarixToVector(screenMatrix, m_localPosition);
		}
		else
		{
			return m_localPosition;
		}
	}
}

D2D1_MATRIX_3X2_F Transform::GetScreenMatrix() const
{
	if (m_parentTransform != nullptr)
	{
		if (m_isUseCamera)
		{
			return m_transformMatrix * m_parentTransform->GetTransformMatrix() * CameraManager::GetInstance().GetCameraMatrix();
		}
		else
		{
			return m_transformMatrix * m_parentTransform->GetTransformMatrix();
		}
	}
	else
	{
		if (m_isUseCamera)
		{
			return m_transformMatrix * CameraManager::GetInstance().GetCameraMatrix();
		}
		else
		{
			return m_transformMatrix;
		}
	}
}

void Transform::SetTransform(DirectY* directY)
{
	if (m_parentTransform != nullptr)
	{
		if (m_isUseCamera)
		{
			directY->SetTransform(m_transformMatrix * m_parentTransform->GetWorldMatrix() * CameraManager::GetInstance().GetCameraMatrix());
		}
		else
		{
			directY->SetTransform(m_transformMatrix * m_parentTransform->GetWorldMatrix());
		}
	}
	else
	{
		if (m_isUseCamera)
		{
			directY->SetTransform(m_transformMatrix * CameraManager::GetInstance().GetCameraMatrix());
		}
		else
		{
			directY->SetTransform(m_transformMatrix);
		}
	}
}

void Transform::Translate(Vector2 translation)
{
	m_localPosition += translation;
	m_transformMatrix = MakeMatrix()({ m_localScale.x, m_localScale.y }, m_localRotation, { m_localPosition.x, m_localPosition.y });
}

void Transform::Rotate(float angle)
{
	m_localRotation += angle;
	m_transformMatrix = MakeMatrix()({ m_localScale.x, m_localScale.y }, m_localRotation, { m_localPosition.x, m_localPosition.y });
}

void Transform::Scale(Vector2 scale)
{
	m_localScale += scale;
	m_transformMatrix = MakeMatrix()({ m_localScale.x, m_localScale.y }, m_localRotation, { m_localPosition.x, m_localPosition.y });
}

void Transform::SetParent(Transform* parentTransform)
{
	m_parentTransform = parentTransform; 
	SetLayer(parentTransform->GetLayer() + 1); 
}
