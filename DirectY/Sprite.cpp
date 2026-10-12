#include "Sprite.h"

#include<iostream>
#include <fstream>

Graphics::Motion::Motion() : m_name{ "" }, m_spriteArray(NULL), m_totalFrame(0)
{

}

Graphics::Motion::~Motion()
{
	std::vector<Sprite*>::iterator itr;
	itr = m_spriteArray.begin();

	while (itr != m_spriteArray.end())
	{
		delete *itr;
		itr++;
	}

	m_spriteArray.clear();
}

void Graphics::Motion::AddSprite(Sprite* sprite)
{
	std::vector<Sprite*>::iterator itr;
	itr = m_spriteArray.begin();
	if (sprite->m_index < m_totalFrame)
	{
		m_spriteArray.insert(itr+ sprite->m_index,sprite);
	}
}

void Graphics::Motion::SetTotalFrame(int totalFrame)
{
	m_totalFrame = totalFrame;
	ResizeMotion();
}

void Graphics::Motion::ResizeMotion()
{
	m_spriteArray = std::vector<Sprite*>(m_totalFrame, nullptr);
}