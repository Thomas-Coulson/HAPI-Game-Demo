#pragma once
#include <HAPI_lib.h>
#include <cmath>
#include <string>
#include <unordered_map>

using namespace HAPISPACE;

class Sprite;
class Rectangle;

class Visualisation
{
private:
	int m_screenWidth;
	int m_screenHeight;
	HAPI_TColour m_screenColour;
	BYTE* m_screenPointer{ nullptr };
	std::unordered_map<std::string, Sprite*> m_spriteMap;
public:
	Visualisation();
	~Visualisation();

	bool initialise();
	void clearScreen();

	bool createSprite(const std::string& spriteName, const std::string& filename, int maxFrames);
	void renderSprite(const std::string& spriteName, int posX, int posY, bool withAlpha, int animFrame);
	void clipSprite(const std::string& spriteName, Rectangle screenRect, Rectangle textureRect, int posX, int posY, int animFrame);

	int getWindowWidth() const { return m_screenWidth; }
	int getWindowHeight() const { return m_screenHeight; }
	int getTextureWidth(const std::string& spriteName);
	int getTextureHeight(const std::string& spriteName);
	int getTextureFrameWidth(const std::string& spriteName);
	int getTextureMaxFrames(const std::string& spriteName);
};

