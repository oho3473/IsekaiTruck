#pragma once
#include "ElementBase.h"
#include "../MathEngine/Vector2.h"
// 부모의 행렬을 가져오고 내 행렬을 전달하려면 D2D1_MATRIX_3X2_F 가 필요한 것 같다... 없앨 수 있을까?
#include "../DirectY/DirectY.h"

using namespace MathEngine;

class Transform : public ElementBase
{
public:
	Transform();
	~Transform();

	/// Element Override
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

	/// Local Coordinate System
	void SetLocalPosition(Vector2 position);
	void SetLocalRotation(float rotation);
	void SetLocalScale(Vector2 scale);

	Vector2 GetLocalPosition() const { return m_localPosition; }
	float GetLocalRotation() const { return m_localRotation; }
	Vector2 GetLocalScale() const { return m_localScale; }

	D2D1_MATRIX_3X2_F GetTransformMatrix
	() const { return m_transformMatrix; }

	/// World 좌표계 부모 + 자신
	Vector2 GetWorldPosition() const;

	D2D1_MATRIX_3X2_F GetWorldMatrix() const;

	/// Screen 좌표계 - 추가해야됨

	Vector2 GetScreenPosition() const;

	D2D1_MATRIX_3X2_F GetScreenMatrix() const;

	// SetTransform
	void SetTransform(DirectY* directY);

	/// 변환 연산
	void Translate(Vector2 translation);
	void Rotate(float angle);
	void Scale(Vector2 scale);

	void SetUseCamera(bool isUseCamera) { m_isUseCamera = isUseCamera; }

public:
	void SetParent(Transform* parentTransform);
	Transform* GetParent() { return m_parentTransform; }

	void SetLayer(int layer) { m_layer = layer; }
	int GetLayer() const { return m_layer; }

private:
	Vector2 m_localPosition;
	float	m_localRotation;
	Vector2 m_localScale;

	D2D1_MATRIX_3X2_F m_transformMatrix;

	bool m_isUseCamera;

private:
	int m_layer;
	Transform* m_parentTransform;
};