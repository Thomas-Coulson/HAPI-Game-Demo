#pragma once
#include "Entity.h"
#include "Rectangle.h"

class ChaseEnemyEntity :
    public Entity
{
private:

public:
    ChaseEnemyEntity(const std::string& currentName, int posX, int posY, bool alpha, bool clipped);
    ~ChaseEnemyEntity() override final;
    void update(World* world) override final;
    void checkCollisions(std::vector<Rectangle> otherHitboxes, ESide otherHitboxSide, EEntityType otherEntityType) override final;
};