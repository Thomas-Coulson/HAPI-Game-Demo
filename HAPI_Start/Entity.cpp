#include "Entity.h"
#include "Rectangle.h"
#include "Visualisation.h"

Entity::Entity(const std::string& currentSprite, int posX, int posY, bool alpha, bool clipped) :
	m_currentSprite(currentSprite),
	m_currentDirection(EDirection::eDown),
	m_side(ESide::eNeutral),
	m_entityType(EEntityType::eBackground),
	m_explosionType(EExplosionType::eBullet),
	m_posX(posX),
	m_posY(posY),
	m_alpha(alpha),
	m_clipped(clipped),
	m_currentFrame(0),
	m_moveAmount(0),
	m_health(0),
	m_scoreAmount(0),
	m_moving(false),
	m_collided(false),
	m_alive(false),
	m_isDead(false),
	m_finishedMoving(true)
{
	m_visible = m_alive;

	m_lastPosition.setX(m_posX);
	m_lastPosition.setY(m_posY);
	m_nextPosition.setX(m_posX);
	m_nextPosition.setY(m_posY);
}

void Entity::render(Visualisation* window, float deltaTime) {
	if (m_entityType == EEntityType::ePlayer) {
		//dont render player if they are not visible
		if (m_visible) {
			Rectangle screenRectangle(0, window->getWindowWidth(), 0, window->getWindowHeight());
			Rectangle textureRectangle(0, window->getTextureFrameWidth(m_currentSprite), 0, window->getTextureHeight(m_currentSprite));

			//interpolation
			Vector2 lerp = m_lastPosition + (m_nextPosition - m_lastPosition) * deltaTime;

			window->clipSprite(m_currentSprite, screenRectangle, textureRectangle, lerp.getX(), lerp.getY(), m_currentFrame);
		}
	}
	else {
		//dont render entities if they are not alive
		if (m_alive) {
			Rectangle screenRectangle(0, window->getWindowWidth(), 0, window->getWindowHeight());
			Rectangle textureRectangle(0, window->getTextureFrameWidth(m_currentSprite), 0, window->getTextureHeight(m_currentSprite));

			//interpolation
			Vector2 lerp = m_lastPosition + (m_nextPosition - m_lastPosition) * deltaTime;

			window->clipSprite(m_currentSprite, screenRectangle, textureRectangle, lerp.getX(), lerp.getY(), m_currentFrame);
		}
	}

	
}

void Entity::setPosition(int x, int y) {
	m_posX = x;
	m_posY = y;
}

void Entity::setAllPositions(int x, int y) {
	//sets interpolated position
	m_posX = x;
	m_posY = y;
	m_lastPosition = Vector2(x, y);
	m_nextPosition = Vector2(x, y);
}

void Entity::resetHealth(int health) {
	m_health = health;
	m_scoreAmount = 10 * m_health;
}