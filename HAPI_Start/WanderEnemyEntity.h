#pragma once
#include "Entity.h"
#include "Rectangle.h"

class WanderEnemyEntity :
    public Entity
{
private:
    int m_fireSpeed;
    int m_bulletTimeCounter;
public:
    WanderEnemyEntity(const std::string& currentName, int posX, int posY, bool alpha, bool clipped);
    ~WanderEnemyEntity() override final;
    void update(World* world) override final;
    void checkCollisions(std::vector<Rectangle> otherHitboxes, ESide otherHitboxSide, EEntityType otherEntityType) override final;
};