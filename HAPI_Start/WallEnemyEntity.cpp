#include "WallEnemyEntity.h"
#include "World.h"

WallEnemyEntity::WallEnemyEntity(const std::string& currentName, int posX, int posY, bool alpha, bool clipped, EDirection currentDirection, EDirection shootDirection) :
	Entity(currentName, posX, posY, alpha, clipped),
	m_fireSpeed(20),
	m_bulletTimeCounter(0)
{
	m_health = rand() % 3 + 1;
	m_side = ESide::eEnemy;
	m_entityType = EEntityType::eWallEnemy;
	m_explosionType = EExplosionType::eEnemy;
	m_currentDirection = currentDirection;
	m_shootDirection = shootDirection;
	m_moveAmount = 5;
	m_alive = false;
	m_moving = true;
	m_scoreAmount = 10 * m_health;

	Rectangle hitbox;
	m_hitBoxes.push_back(hitbox);
}

WallEnemyEntity::~WallEnemyEntity() {}

void WallEnemyEntity::update(World* world) {
	if (m_alive) {
		//set sprite based on where the enemy spawns
		if (m_posY == 685) {
			m_currentSprite = "WallShooterBottom";
			m_shootDirection = EDirection::eUp;
		}
		else if (m_posY == 180) {
			m_currentSprite = "WallShooterTop";
			m_shootDirection = EDirection::eDown;
		}

		//1 in 75 chance to shoot every tick
		int shootChance = rand() % 75;

		//AI that moves left anmd right across wall to the edge
		if (m_posX <= 110) {
			m_currentDirection = EDirection::eRight;
		}
		else if (m_posX >= 854) {
			m_currentDirection = EDirection::eLeft;
		}

		//move in current direction
		if (m_currentDirection == EDirection::eLeft) {
			m_lastPosition.setX(m_posX);
			m_posX -= m_moveAmount;
			m_nextPosition.setX(m_posX);

			m_lastPosition.setY(m_posY);
			m_nextPosition.setY(m_posY);

			m_moving = true;
		}
		else if (m_currentDirection == EDirection::eRight) {
			m_lastPosition.setX(m_posX);
			m_posX += m_moveAmount;
			m_nextPosition.setX(m_posX);

			m_lastPosition.setY(m_posY);
			m_nextPosition.setY(m_posY);

			m_moving = true;
		}

		//update hitboxes
		m_hitBoxes[0].setLeft(m_posX);
		m_hitBoxes[0].setTop(m_posY);
		m_hitBoxes[0].setRight(m_posX + world->getWindow()->getTextureFrameWidth(m_currentSprite));
		m_hitBoxes[0].setBottom(m_posY + world->getWindow()->getTextureHeight(m_currentSprite));

		//===================================================================================================================================================
		//make offsets for spawning bullets

		int topXoffset = (m_hitBoxes[0].getLeft() + m_hitBoxes[0].getWidth() / 2);
		int topYoffset = (m_hitBoxes[0].getTop() - world->getWindow()->getTextureHeight("FireballUp") - 15);

		int bottomXoffset = (m_hitBoxes[0].getLeft() + m_hitBoxes[0].getWidth() / 2);
		int bottomYoffset = m_hitBoxes[0].getBottom() - 15;
		//===================================================================================================================================================

		if (shootChance == 0) {
			if (m_bulletTimeCounter >= m_fireSpeed) {
				//fire bullets
				if (m_shootDirection == EDirection::eUp) {
					world->fireBullet(topXoffset, topYoffset, m_shootDirection, m_side);
				}
				else if (m_shootDirection == EDirection::eDown) {
					world->fireBullet(bottomXoffset, bottomYoffset, m_shootDirection, m_side);
				}

				m_bulletTimeCounter = 0;

				//50% chance to change direction when shooting
				if (rand() % 2 == 0) {
					m_currentDirection = EDirection::eLeft;
				}
				else {
					m_currentDirection = EDirection::eRight;
				}
			}
		}

		m_bulletTimeCounter++;

		if (m_health <= 0) {
			//when health <= 0 - make explosiona nd kill enemy
			m_alive = false;
			world->makeExplosion(m_posX, m_posY, m_currentDirection, m_side, m_explosionType);
			world->increaseScore(m_scoreAmount);
		}

	}
}

void WallEnemyEntity::checkCollisions(std::vector<Rectangle> otherHitboxes, ESide otherHitboxSide, EEntityType otherEntityType) {
	for (auto& rectangle : otherHitboxes) {
		if ((m_hitBoxes[0].getRight() < rectangle.getLeft() || m_hitBoxes[0].getLeft() > rectangle.getRight())
			|| (m_hitBoxes[0].getBottom() < rectangle.getTop() || m_hitBoxes[0].getTop() > rectangle.getBottom())) {
			/*return false;*/
			m_collided = false;
		}
		else {
			/*return true;*/
			m_collided = true;

			if (otherEntityType == EEntityType::eBullet) {
				if (otherHitboxSide == ESide::ePlayer) {
					//if hit by a player bullet - take damage
					m_health--;
					if (m_health > 0) {
						HAPI.PlaySound("Data\\Sounds\\Hit.wav", HAPI_TSoundOptions(0.3));
					}
				}
			}

		}
	}
}
