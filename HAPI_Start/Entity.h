#pragma once
#include <HAPI_lib.h>
#include <cmath>
#include <string>
#include <unordered_map>

#include "Vector2.h"

using namespace HAPISPACE;

class World;
class Visualisation;
class Rectangle;

enum class EExplosionType {
	eBullet,
	eEnemy
};

enum class EDirection {
	eUp,
	eRight,
	eDown,
	eLeft
};

enum class EEntityType {
	ePlayer,
	eBullet,
	eExplosion,
	eBackground,
	eWallEnemy,
	eWanderEnemy,
	eChaseEnemy,
	eDoor
};

enum class ESide {
	ePlayer,
	eEnemy,
	eNeutral
};

class Entity
{
protected:
	std::string m_currentSprite;
	std::vector<Rectangle> m_hitBoxes;

	EDirection m_currentDirection;
	ESide m_side;
	EEntityType m_entityType;
	EExplosionType m_explosionType;

	int m_posX;
	int m_posY;
	int m_moveAmount;
	int m_health;
	int m_scoreAmount;
	int m_currentFrame;

	Vector2 m_lastPosition;
	Vector2 m_nextPosition;

	bool m_alpha;
	bool m_clipped;
	bool m_moving;
	bool m_collided;
	bool m_alive;
	bool m_isDead;
	bool m_visible;
	bool m_finishedMoving;

public:
	Entity(const std::string& currentSprite, int posX, int posY, bool alpha, bool clipped);
	virtual ~Entity() = default;

	virtual void update(World* world) = 0;
	virtual void checkCollisions(std::vector<Rectangle> otherHitboxes, ESide otherHitboxSide, EEntityType otherEntityType) = 0;
	void render(Visualisation* window, float deltaTime);

	EDirection getDirection() const { return m_currentDirection; }
	ESide getSide() const { return m_side; };
	EEntityType getEntityType() const { return m_entityType; }
	std::string getCurrentSprite() const { return m_currentSprite; }
	std::vector<Rectangle> getHitboxes() const { return m_hitBoxes; }
	int getPosX() const { return m_posX; }
	int getPosY() const { return m_posY; }
	int getCurrentFrame() const { return m_currentFrame; }
	int getHealth() const { return m_health; }
	bool getAlpha() const { return m_alpha; }
	bool getClipped() const { return m_clipped; }
	bool getMoving() const { return m_moving; }
	bool getAlive() const { return m_alive; }
	bool getVisible() const { return m_visible; }
	bool getFinishedMoving() const { return m_finishedMoving; }

	void setVisible(bool visible) { m_visible = visible; }
	void setFinishedMoving(bool moving) { m_finishedMoving = moving; }
	void setCurrentFrame(int currentFrame) { m_currentFrame = currentFrame; }
	void setPosition(int x, int y);
	void setAllPositions(int x, int y);
	void setDirection(EDirection direction) { m_currentDirection = direction; }
	void setAlive(bool alive) { m_alive = alive;}
	void setSide(ESide side) { m_side = side; }
	void setExplosionType(EExplosionType type) { m_explosionType = type; }
	void resetHealth(int health);
};

