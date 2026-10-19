#pragma once
#include <d2d1.h>		//D2D
#include <vector>
#include <string>

namespace Graphics
{
	//기본 도형
	namespace BaseFigure
	{
		//사각형
		struct Rectangle
		{
			float left;
			float top;
			float right;
			float bottom;
		};

		//선
		struct Line
		{
			float startX;
			float startY;
			float endX;
			float endY;
		};

		//원
		struct Circle
		{
			float centerX;
			float centerY;

			float radius;
		};

		//타원
		struct Ellipse
		{
			float centerX;
			float centerY;

			float radiusX;
			float radiusY;
		};

		struct Point
		{
			float x;
			float y;
		};

		//isomatric 바닥의 점 위치를 갖고 있는 구조체
		struct IsomatricTile
		{
			Graphics::BaseFigure::Point centerXY;
			float height = 72 / 2;
			float width = 144 / 2;
		};

	}

	//sprite 하나 하나의 정보를 담을 구조체
	struct Sprite
	{
		std::string m_name;	//이름
		int m_index;	//애니메이션 전체에서 몇번째인지 인덱스
		float m_delayTime;	//모션 딜레이 시간
		ID2D1Bitmap* m_image;	//이미지
		D2D1_RECT_F m_drawSize;	//그릴 이미지 사이즈
		D2D1_RECT_F m_drawArea;	//이미지 내부에 어느 부분을 그릴지
		D2D1_POINT_2F m_drawPos;	//이미지를 어느 위치에 그릴지
		D2D1_POINT_2F m_pivot = {0.5,0.5};	//강규석의 텍스트 파일을 받아 그릴 이미지 사이즈(drawSize)의 가로,세로 값을 곱해 실제 크기의 pivot을 지정해줘야한다

		Sprite(const std::string& name, const int& index, const float& delayTime, ID2D1Bitmap*& image, const D2D1_RECT_F& drawSize, const D2D1_RECT_F drawArea, const D2D1_POINT_2F& drawPos, const D2D1_POINT_2F& pivot)
			: m_name(name), m_index(index),m_delayTime(delayTime),m_image(image)
			,m_drawSize(drawSize), m_drawArea(drawArea), m_drawPos(drawPos),m_pivot(pivot)
		{
		}
	};

	//sprtie묶어 하나의 모션으로 관리하기위한 클래스
	class Motion
	{
	public:
		Motion();
		~Motion();

		void AddSprite(Sprite* sprite);
		void SetTotalFrame(int totalFrame);
		void SetName(std::string name) { m_name = name; }

		Sprite* GetSprite(int index) const { /*if (index < 0) { index = 0; }*/ return m_spriteArray[index]; }
		std::string GetName() const { return m_name; }
		int GetTotalFrame() const { return m_totalFrame; }

		void SetImage(ID2D1Bitmap*& image)
		{
			static int index = 0;

			if (m_totalFrame <= 0)
			{
				return;
			}

			while (m_totalFrame > index)
			{
				m_spriteArray[index]->m_image = image;

				index++;
			}
		}

	private:
		std::string m_name;	//이름
		std::vector<Sprite*> m_spriteArray;	//모션 하나
		int m_totalFrame;	//애니메이션 전체 프레임 수

		void ResizeMotion();
	};
}
