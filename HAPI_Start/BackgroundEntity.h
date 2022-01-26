#pragma once
#include "Entity.h"

class Rectangle;
class World;

class BackgroundEntity : public Entity
{
private:
	int m_finalLeftPosX;
	int m_finalRightPosX;
public:
	BackgroundEntity(const std::string& currentName, int posX, int posY, bool alpha, bool clipped);
	~BackgroundEntity() override final;
	void update(World* world) override final;
	void checkCollisions(std::vector<Rectangle> otherHitboxes, ESide otherHitboxSide, EEntityType otherEntityType) override final;
};

