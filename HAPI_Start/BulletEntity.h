#pragma once
#include "Entity.h"

class BulletEntity :
    public Entity
{
private:

public:
    BulletEntity(const std::string& currentSprite, int posX, int posY, bool alpha, bool clipped, EDirection currentDirection);
    ~BulletEntity() override final;
    void update(World* world) override final;
    void checkCollisions(std::vector<Rectangle> otherHitboxes, ESide otherHitboxSide, EEntityType otherEntityType) override final;
  
};

