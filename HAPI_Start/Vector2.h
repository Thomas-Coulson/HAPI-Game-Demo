#pragma once
class Vector2
{
private:
	float m_x;
	float m_y;
public:
	//override operators to allow + - and * of vectors
	Vector2 operator + (const Vector2& rhs) const {
		return Vector2(m_x + rhs.m_x, m_y + rhs.m_y);
	}
	Vector2 operator - (const Vector2& rhs) const {
		return Vector2(m_x - rhs.m_x, m_y - rhs.m_y);
	}
	Vector2 operator * (float rhs) const {
		return Vector2(m_x * rhs, m_y * rhs);
	}

	Vector2(int x = 0, int y = 0);

	int getX() const { return m_x; }
	int getY() const { return m_y; }

	void setX(int x) { m_x = x; }
	void setY(int y) { m_y = y; }
};


