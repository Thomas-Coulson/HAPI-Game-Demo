#include "Visualisation.h"
#include "Sprite.h"
#include "Rectangle.h"

Visualisation::Visualisation() :
	m_screenWidth(1024),
	m_screenHeight(768),
	m_screenColour(HAPI_TColour(10, 14, 17))
{
}

Visualisation::~Visualisation() {
	for (auto& sprite : m_spriteMap) {
		delete sprite.second;
	}

}

void Visualisation::clearScreen() {
	//clear screen to colour
	for (int i = 0; i < m_screenWidth * m_screenHeight; i++) {
		memcpy(m_screenPointer + (size_t)i * 4, &m_screenColour, 4);

	}
}

bool Visualisation::initialise() {
	//create HAPI window
	if (!HAPI.Initialise(m_screenWidth, m_screenHeight)) {
		return false;
	}

	HAPI.SetShowFPS(true);

	m_screenPointer = HAPI.GetScreenPointer();

	return true;
}

bool Visualisation::createSprite(const std::string& spriteName, const std::string& filename, int maxFrames) {
	//create a sprite and add it to a spritemap
	Sprite* newSprite = new Sprite(maxFrames);
	if (!newSprite->load(filename)) {
		delete newSprite;
		return false;
	}

	m_spriteMap[spriteName] = newSprite;
	return true;
}

void Visualisation::renderSprite(const std::string& spriteName, int posX, int posY, bool withAlpha, int animFrame) {
	//render a specified sprite
	if (m_spriteMap.find(spriteName) == m_spriteMap.end()) {
		return;
	}

	m_spriteMap[spriteName]->render(m_screenPointer, m_screenWidth, posX, posY, withAlpha, animFrame);

}

void Visualisation::clipSprite(const std::string& spriteName, Rectangle screenRect, Rectangle textureRect, int posX, int posY, int animFrame) {
	//clip a specified sprite
	if (m_spriteMap.find(spriteName) == m_spriteMap.end()) {
		return;
	}

	m_spriteMap[spriteName]->clip(m_screenPointer, screenRect, textureRect, posX, posY, animFrame);
}

int Visualisation::getTextureWidth(const std::string& spriteName)
{
	return (m_spriteMap[spriteName]->getWidth());
}

int Visualisation::getTextureHeight(const std::string& spriteName)
{
	return (m_spriteMap[spriteName]->getHeight());
}

int Visualisation::getTextureFrameWidth(const std::string& spriteName)
{
	return (m_spriteMap[spriteName]->getFrameWidth());
}

int Visualisation::getTextureMaxFrames(const std::string& spriteName)
{
	return (m_spriteMap[spriteName]->getMaxFrames());
}
