#include "Sprite.h"
#include "Rectangle.h"

Sprite::Sprite(int maxFrames) :
	m_textureWidth(0),
	m_textureHeight(0),
	m_frameWidth(0),
	m_maxFrames(maxFrames)
{
}

Sprite::~Sprite() {
	delete m_texturePointer;
}

bool Sprite::load(const std::string& filename) {
	//load the texture given by the filename
	if (!HAPI.LoadTexture(filename, &m_texturePointer, m_textureWidth, m_textureHeight)) {
		return false;
	}

	//sets the individual frame with for animated sprites
	if (m_maxFrames > 0) {
		m_frameWidth = m_textureWidth / m_maxFrames;
	}
	else if (m_maxFrames == 0) {
		m_frameWidth = m_textureWidth;
	}

	return true;
}

void Sprite::render(BYTE* screenPointer, int screenWidth, int posX, int posY, bool withAlpha, int animFrame) {
	if (!withAlpha) {
		//blit without alpha
		BYTE* tempTexture{ m_texturePointer };
		BYTE* tempScreen{ screenPointer + ((size_t)posX + (size_t)posY * screenWidth) * 4 };

		for (int y = 0; y < m_textureHeight; y++) {
			memcpy(tempScreen, tempTexture, (size_t)m_textureWidth * 4);
			tempTexture += (size_t)m_textureWidth * 4;
			tempScreen += (size_t)screenWidth * 4;
		}
	}
	else if (withAlpha && m_maxFrames == 0) {
		//blit with alpha
		BYTE* tempTexture{ m_texturePointer };
		BYTE* tempScreen{ screenPointer + ((size_t)posX + (size_t)posY * screenWidth) * 4 };

		int endOfLineOffset = (screenWidth - m_textureWidth) * 4;

		for (int y = 0; y < m_textureHeight; y++) {
			for (int x = 0; x < m_textureWidth; x++) {

				BYTE red = tempTexture[0];
				BYTE green = tempTexture[1];
				BYTE blue = tempTexture[2];
				BYTE alpha = tempTexture[3];

				if (alpha == 255) {
					memcpy(tempScreen, tempTexture, 4);
				}
				else if (alpha > 0) {
					float mod = alpha / 255.0f;
					tempScreen[0] = (BYTE)(mod * red + (1.0f - mod) * tempScreen[0]);
					tempScreen[1] = (BYTE)(mod * green + (1.0f - mod) * tempScreen[1]);
					tempScreen[2] = (BYTE)(mod * blue + (1.0f - mod) * tempScreen[2]);
				}

				tempTexture += 4;
				tempScreen += 4;

			}
			tempScreen += endOfLineOffset;
		}
	}
	else if (withAlpha && m_maxFrames > 0) {
		//blit with alpha and animation
		BYTE* tempTexture{ m_texturePointer + ((size_t)animFrame * m_frameWidth * 4) };
		BYTE* tempScreen{ screenPointer + ((size_t)posX + (size_t)posY * screenWidth) * 4 };

		int endOfLineScreenOffset = (screenWidth - m_frameWidth) * 4;
		int endOfLineTexOffset = (m_textureWidth - m_frameWidth) * 4;

		for (int y = 0; y < m_textureHeight; y++) {
			for (int x = 0; x < m_frameWidth; x++) {

				BYTE red = tempTexture[0];
				BYTE green = tempTexture[1];
				BYTE blue = tempTexture[2];
				BYTE alpha = tempTexture[3];

				if (alpha == 255) {
					memcpy(tempScreen, tempTexture, 4);
				}
				else if (alpha > 0) {
					float mod = alpha / 255.0f;
					tempScreen[0] = (BYTE)(mod * red + (1.0f - mod) * tempScreen[0]);
					tempScreen[1] = (BYTE)(mod * green + (1.0f - mod) * tempScreen[1]);
					tempScreen[2] = (BYTE)(mod * blue + (1.0f - mod) * tempScreen[2]);
				}

				tempTexture += 4;
				tempScreen += 4;

			}
			tempScreen += endOfLineScreenOffset;
			tempTexture += endOfLineTexOffset;
		}
	}
}

void Sprite::clip(BYTE* screenPointer, const Rectangle& screenRectangle, const Rectangle& textureRectangle, int posX, int posY, int animFrame) {
	//if the sprite is off screen - dont clip or render (optimisation 1)
	if (posX > screenRectangle.getRight()
		|| posX + textureRectangle.getRight() < screenRectangle.getLeft()
		|| posY > screenRectangle.getHeight()
		|| posY + textureRectangle.getBottom() < screenRectangle.getTop()) {
		return;
	}
	//if the sprite is completly on screen - dont clip, just render (optimisation 2)
	else if (posX > screenRectangle.getLeft()
		&& posX + textureRectangle.getRight() < screenRectangle.getRight()
		&& posY > screenRectangle.getTop()
		&& posY + textureRectangle.getBottom() < screenRectangle.getBottom()) {
		render(screenPointer, screenRectangle.getWidth(), posX, posY, true, animFrame);
	}
	else if (m_maxFrames == 0) {
		//clip without animation
		Rectangle clippedRectangle(textureRectangle);
		clippedRectangle.translate(posX, posY);
		clippedRectangle.clipTo(screenRectangle);
		clippedRectangle.translate(-posX, -posY);

		if (posX < 0) {
			posX = 0;
		}
		if (posY < 0) {
			posY = 0;
		}

		BYTE* tempScreen{ screenPointer + ((size_t)posX + (size_t)posY * screenRectangle.getWidth()) * 4 };
		BYTE* tempTexture = m_texturePointer + ((size_t)clippedRectangle.getLeft() + (size_t)clippedRectangle.getTop() * textureRectangle.getWidth()) * 4;

		int endOfLineScreenOffset = (screenRectangle.getWidth() - clippedRectangle.getWidth()) * 4;
		int endOfLineTextureOffset = (textureRectangle.getWidth() - clippedRectangle.getWidth()) * 4;

		for (int y = 0; y < clippedRectangle.getHeight(); y++) {
			for (int x = 0; x < clippedRectangle.getWidth(); x++) {

				BYTE red = tempTexture[0];
				BYTE green = tempTexture[1];
				BYTE blue = tempTexture[2];
				BYTE alpha = tempTexture[3];

				if (alpha == 255) {
					memcpy(tempScreen, tempTexture, 4);
				}
				else if (alpha > 0) {
					float mod = alpha / 255.0f;
					tempScreen[0] = (BYTE)(mod * red + (1.0f - mod) * tempScreen[0]);
					tempScreen[1] = (BYTE)(mod * green + (1.0f - mod) * tempScreen[1]);
					tempScreen[2] = (BYTE)(mod * blue + (1.0f - mod) * tempScreen[2]);
				}

				tempTexture += 4;
				tempScreen += 4;

			}
			tempScreen += endOfLineScreenOffset;
			tempTexture += endOfLineTextureOffset;
		}
	}
	else if (m_maxFrames > 0) {
		//clip with animation
		Rectangle clippedRectangle(textureRectangle);
		clippedRectangle.translate(posX, posY);
		clippedRectangle.clipTo(screenRectangle);
		clippedRectangle.translate(-posX, -posY);

		if (posX < 0) {
			posX = 0;
		}
		if (posY < 0) {
			posY = 0;
		}

		BYTE* tempScreen{ screenPointer + ((size_t)posX + (size_t)posY * screenRectangle.getWidth()) * 4 };
		BYTE* tempTexture = m_texturePointer + (((size_t)clippedRectangle.getLeft() + ((size_t)clippedRectangle.getTop() * m_textureWidth) + ((size_t)animFrame * m_frameWidth)) * 4);

		int endOfLineScreenOffset = (screenRectangle.getWidth() - clippedRectangle.getWidth()) * 4;
		int endOfLineTextureOffset = (m_textureWidth - clippedRectangle.getWidth()) * 4;

		for (int y = 0; y < clippedRectangle.getHeight(); y++) {
			for (int x = 0; x < clippedRectangle.getWidth(); x++) {

				BYTE red = tempTexture[0];
				BYTE green = tempTexture[1];
				BYTE blue = tempTexture[2];
				BYTE alpha = tempTexture[3];

				if (alpha == 255) {
					memcpy(tempScreen, tempTexture, 4);
				}
				else if (alpha > 0) {
					float mod = alpha / 255.0f;
					tempScreen[0] = (BYTE)(mod * red + (1.0f - mod) * tempScreen[0]);
					tempScreen[1] = (BYTE)(mod * green + (1.0f - mod) * tempScreen[1]);
					tempScreen[2] = (BYTE)(mod * blue + (1.0f - mod) * tempScreen[2]);
				}

				tempTexture += 4;
				tempScreen += 4;

			}
			tempScreen += endOfLineScreenOffset;
			tempTexture += endOfLineTextureOffset;
		}
	}

}
