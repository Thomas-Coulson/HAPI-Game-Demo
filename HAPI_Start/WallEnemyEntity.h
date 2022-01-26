#pragma once
#include "Entity.h"
#include "Rectangle.h"

class WallEnemyEntity :
    public Entity
{
private:
    EDirection m_shootDirection;
    int m_fireSpeed;
    int m_bulletTimeCounter;
public:
    WallEnemyEntity(const std::string& currentName, int posX, int posY, bool alpha, bool clipped, EDirection currentDirection, EDirection shootDirection);
    ~WallEnemyEntity() override final;
    void update(World* world) override final;
    void checkCollisions(std::vector<Rectangle> otherHitboxes, ESide otherHitboxSide, EEntityType otherEntityType) override final;

    void setShootDirection(EDirection shootDirection) { m_shootDirection = shootDirection; }
};

