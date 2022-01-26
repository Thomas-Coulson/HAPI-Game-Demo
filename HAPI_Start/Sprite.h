#pragma once
#include <HAPI_lib.h>
#include <string>
#include <unordered_map>

using namespace HAPISPACE;

class Rectangle;

class Sprite
{
private:
	BYTE* m_texturePointer{ nullptr };
	int m_textureWidth;
	int m_textureHeight;
	int m_frameWidth;
	int m_maxFrames;
public:
	Sprite(int maxFrames);
	~Sprite();

	bool load(const std::string& filename);
	void render(BYTE* screenPointer, int screenWidth, int posX, int posY, bool withAlpha, int animFrame);
	void clip(BYTE* screenPointer, const Rectangle& screenRectangle, const Rectangle& textureRectangle, int posX, int posY, int animFrame);

	int getWidth() const { return m_textureWidth; }
	int getFrameWidth() const { return m_frameWidth; }
	int getHeight() const { return m_textureHeight; }
	int getMaxFrames() const { return m_maxFrames; }
};

