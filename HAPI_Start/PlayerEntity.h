#pragma once
#include "Entity.h"
#include "Rectangle.h"

class PlayerEntity : public Entity
{
private:
	int m_diagonalMoveAmount;
	int m_fireSpeed;
	int m_bulletTimeCounter;
	int m_damageSpeed;
	int m_damageTimeCounter;
	bool m_invincible;
public:
	PlayerEntity(const std::string& currentName, int posX, int posY, bool alpha, bool clipped);
	~PlayerEntity() override final;
	void update(World* world) override final;
	void checkCollisions(std::vector<Rectangle> otherHitboxes, ESide otherHitboxSide, EEntityType otherEntityType) override final;
};

