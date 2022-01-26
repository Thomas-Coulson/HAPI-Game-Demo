#pragma once
class Rectangle
{
private:
	int m_left;
	int m_right;
	int m_top;
	int m_bottom;
public:
	Rectangle(int left = 0, int right = 0, int top = 0, int bottom = 0);

	int getLeft() const { return m_left; }
	int getRight() const { return m_right; }
	int getTop() const { return m_top; }
	int getBottom() const { return m_bottom; }
	int getWidth() const { return m_right - m_left; }
	int getHeight() const { return m_bottom - m_top; }

	void setLeft(int left) { m_left = left; }
	void setRight(int right) { m_right = right; }
	void setTop(int top) { m_top = top; }
	void setBottom(int bottom) { m_bottom = bottom; }

	void translate(int dx, int dy);
	void clipTo(const Rectangle& other);
};

