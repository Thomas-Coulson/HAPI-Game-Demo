#pragma once
#include "Entity.h"
#include "Rectangle.h"

class ExplosionEntity :
    public Entity
{
private:

public:
    ExplosionEntity(const std::string& currentSprite, int posX, int posY, bool alpha, bool clipped, EDirection currentDirection);
    ~ExplosionEntity() override final;
    void update(World* world) override final;
    void checkCollisions(std::vector<Rectangle> otherHitboxes, ESide otherHitboxSide, EEntityType otherEntityType) override final;
};

