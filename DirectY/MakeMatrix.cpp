#include "MakeMatrix.h"

#define _USE_MATH_DEFINES
#include <cmath>

/// <summary>
/// 이동행렬
/// </summary>
/// <param name="speedX">X이동량</param>
/// <param name="speedY">Y이동량</param>
/// <returns></returns>
D2D1_MATRIX_3X2_F MakeMatrix::TranslateMove(const D2D1_POINT_2F translate)
{
	D2D1_MATRIX_3X2_F move;

	move.m11 = 1;		move.m12 = 0;
	move.m21 = 0;		move.m22 = 1;
	move.dx = (float)translate.x;	move.dy = (float)translate.y;

	return move;
}

/// <summary>
/// 회전행렬
/// </summary>
/// <param name="angle">회전각도</param>
/// <returns></returns>
D2D1_MATRIX_3X2_F MakeMatrix::TranslateRotation(double angle)
{
	D2D1_MATRIX_3X2_F rotationMatrix;
	rotationMatrix.m11 = (float)cos(angle * M_PI / 180);	rotationMatrix.m12 = (float)sin(angle * M_PI / 180);
	rotationMatrix.m21 = (float)-sin(angle * M_PI / 180);	rotationMatrix.m22 = (float)cos(angle * M_PI / 180);
	rotationMatrix.dx = 0; rotationMatrix.dy = 0;

	return rotationMatrix;
}

/// <summary>
/// 크기변환행렬
/// </summary>
/// <param name="scaleX">X축 배율</param>
/// <param name="scaleY">Y축 배율</param>
/// <returns></returns>
D2D1_MATRIX_3X2_F MakeMatrix::TranslateScale(const D2D1_POINT_2F scale)
{
	D2D1_MATRIX_3X2_F scalematrix;

	scalematrix.m11 = (float)scale.x;  scalematrix.m12 = 0;
	scalematrix.m21 = 0;	scalematrix.m22 = (float)scale.y;
	scalematrix.dx = 0;		scalematrix.dy = 0;

	return scalematrix;
}

/// <summary>
/// 변환행렬 S R T
/// </summary>
/// <param name="scale">크기 변환행렬</param>
/// <param name="rotation">회전행렬</param>
/// <param name="translate">이동행렬</param>
/// <returns></returns>
D2D1_MATRIX_3X2_F MakeMatrix::TransformMatrix(D2D1_MATRIX_3X2_F& scale, D2D1_MATRIX_3X2_F& rotation, D2D1_MATRIX_3X2_F& translate)
{
	D2D1_MATRIX_3X2_F translatematrix;

	//행렬은 결합 법칙이 성립한다
	//d2d는 행벡터 기준
	// S R T 순서로 해야 강체 변환이 가능함

	translatematrix = scale * rotation * translate;

	return translatematrix;
}

/// <summary>
/// 변환 행렬 만들기
/// </summary>
/// <param name="scale">크기 변환</param>
/// <param name="rotation">회전 변환</param>
/// <param name="translate">이동 변환</param>
/// <returns></returns>
D2D1_MATRIX_3X2_F MakeMatrix::MakeTransformMatrix(const D2D1_POINT_2F scale, const double rotation, const D2D1_POINT_2F translate)
{
	D2D1_MATRIX_3X2_F scalematrix = TranslateMove(scale);
	D2D1_MATRIX_3X2_F rotationMatrix = TranslateRotation(rotation);
	D2D1_MATRIX_3X2_F translateMatrix = TranslateScale(translate);

	m_translateMatrix = scalematrix * rotationMatrix * translateMatrix;

	return m_translateMatrix;
}

