#pragma once
#include <d2d1.h>
#include "Vector2.h"

/// <summary>
/// 변환행렬을 만들어줄 클래스
/// </summary>
class MakeMatrix
{
public:

	//functor로 만들어 쓰세요
	D2D1_MATRIX_3X2_F operator() (const D2D1_POINT_2F scale, const double rotation, const D2D1_POINT_2F translate)
	{
		D2D1_MATRIX_3X2_F scalematrix = TranslateScale(scale);
		D2D1_MATRIX_3X2_F rotationMatrix = TranslateRotation(rotation);
		D2D1_MATRIX_3X2_F translateMatrix = TranslateMove(translate);

		m_translateMatrix = scalematrix * rotationMatrix * translateMatrix;

		return m_translateMatrix;
	}

	void translate(D2D1_MATRIX_3X2_F matrix, int& x, int& y)
	{
		D2D1_POINT_2F temp = { (float)x,(float)y };
	}

private:
	D2D1_MATRIX_3X2_F m_translateMatrix;
private:
	D2D1_MATRIX_3X2_F TransformMatrix(D2D1_MATRIX_3X2_F& scale, D2D1_MATRIX_3X2_F& rotation, D2D1_MATRIX_3X2_F& translate);

	//변환행렬을 반환한다
	D2D1_MATRIX_3X2_F MakeTransformMatrix(const D2D1_POINT_2F scale, const double rotation, const D2D1_POINT_2F translate);

	//이동행렬
	D2D1_MATRIX_3X2_F TranslateMove(const D2D1_POINT_2F translate);
	//회전행렬
	D2D1_MATRIX_3X2_F TranslateRotation(double angle);
	//크기행렬
	D2D1_MATRIX_3X2_F TranslateScale(const D2D1_POINT_2F scale);
};

