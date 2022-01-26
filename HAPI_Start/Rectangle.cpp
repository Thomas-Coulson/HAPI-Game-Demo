#include "Rectangle.h"

Rectangle::Rectangle(int left, int right, int top, int bottom) :
	m_left(left),
	m_right(right),
	m_top(top),
	m_bottom(bottom)
{
}

void Rectangle::translate(int dx, int dy) {
	//moves rectangle to location
	m_left += dx;
	m_right += dx;
	m_top += dy;
	m_bottom += dy;
}

void Rectangle::clipTo(const Rectangle& other) {
	if (m_left < other.getLeft()) {
		m_left = other.getLeft();
	}
	if (m_right > other.getRight()) {
		m_right = other.getRight();
	}
	if (m_top < other.getTop()) {
		m_top = other.getTop();
	}
	if (m_bottom > other.getBottom()) {
		m_bottom = other.getBottom();
	}
}