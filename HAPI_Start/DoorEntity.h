#pragma once
#include "Entity.h"

class Rectangle;
class World;

enum class ERoomDirection;

class DoorEntity :
    public Entity
{
private:
	bool m_open;
public:
	DoorEntity(const std::string& currentName, int posX, int posY, bool alpha, bool clipped, EDirection currentDirection);
	~DoorEntity() override final;
	void update(World* world) override final;
	void checkCollisions(std::vector<Rectangle> otherHitboxes, ESide otherHitboxSide, EEntityType otherEntityType) override final;
};

