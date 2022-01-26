#include "ChaseEnemyEntity.h"
#include "World.h"

ChaseEnemyEntity::ChaseEnemyEntity(const std::string& currentName, int posX, int posY, bool alpha, bool clipped) :
	Entity(currentName, posX, posY, alpha, clipped)
{
	m_health = rand() % 3 + 1;
	m_side = ESide::eEnemy;
	m_entityType = EEntityType::eChaseEnemy;
	m_explosionType = EExplosionType::eEnemy;
	m_currentDirection = EDirection::eLeft;
	m_moveAmount = 3;
	m_alive = false;
	m_moving = true;
	m_scoreAmount = 10 * m_health;

	Rectangle hitbox;
	m_hitBoxes.push_back(hitbox);

	//set hitboxes
	m_hitBoxes[0].setLeft(m_posX);
	m_hitBoxes[0].setTop(m_posY);
	m_hitBoxes[0].setRight(m_posX);
	m_hitBoxes[0].setBottom(m_posY);
}

ChaseEnemyEntity::~ChaseEnemyEntity() {}

void ChaseEnemyEntity::update(World* world) {
	if (m_alive) {

		m_lastPosition.setX(m_posX);
		m_nextPosition.setX(m_posX);
		m_lastPosition.setY(m_posY);
		m_nextPosition.setY(m_posY);

		for (auto& entity : world->getAllEntities()) {
			if (entity->getEntityType() == EEntityType::ePlayer) {
				//move towards the player hitbox
				if (m_hitBoxes[0].getLeft() > entity->getHitboxes()[0].getRight() - 5) {
					//move left
					m_lastPosition.setX(m_posX);
					m_posX -= m_moveAmount;
					m_nextPosition.setX(m_posX);

					m_lastPosition.setY(m_posY);
					m_nextPosition.setY(m_posY);

					m_currentDirection = EDirection::eLeft;
					m_currentSprite = "WolfLeft";
					m_moving = true;
				}
				else if (m_hitBoxes[0].getBottom() < entity->getHitboxes()[0].getTop() + 5) {
					//move down
					m_lastPosition.setY(m_posY);
					m_posY += m_moveAmount;
					m_nextPosition.setY(m_posY);

					m_lastPosition.setX(m_posX);
					m_nextPosition.setX(m_posX);

					m_currentDirection = EDirection::eDown;
					m_moving = true;
				}
				else if (m_hitBoxes[0].getRight() < entity->getHitboxes()[0].getLeft() + 5) {
					//move right
					m_lastPosition.setX(m_posX);
					m_posX += m_moveAmount;
					m_nextPosition.setX(m_posX);

					m_lastPosition.setY(m_posY);
					m_nextPosition.setY(m_posY);

					m_currentDirection = EDirection::eRight;
					m_currentSprite = "WolfRight";
					m_moving = true;
				}
				else if (m_hitBoxes[0].getTop() > entity->getHitboxes()[0].getBottom() - 5) {
					//move up
					m_lastPosition.setY(m_posY);
					m_posY -= m_moveAmount;
					m_nextPosition.setY(m_posY);

					m_lastPosition.setX(m_posX);
					m_nextPosition.setX(m_posX);

					m_currentDirection = EDirection::eUp;
					m_moving = true;
				}
			}
		}

		//update hitboxes
		m_hitBoxes[0].setLeft(m_posX);
		m_hitBoxes[0].setTop(m_posY);
		m_hitBoxes[0].setRight(m_posX + world->getWindow()->getTextureFrameWidth(m_currentSprite));
		m_hitBoxes[0].setBottom(m_posY + world->getWindow()->getTextureHeight(m_currentSprite));
		

		if (m_health <= 0) {
			//when health <= 0 - make explosiona nd kill enemy
			m_alive = false;
			world->makeExplosion(m_posX, m_posY, m_currentDirection, m_side, m_explosionType);
			world->increaseScore(m_scoreAmount);
		}

	}

}

void ChaseEnemyEntity::checkCollisions(std::vector<Rectangle> otherHitboxes, ESide otherHitboxSide, EEntityType otherEntityType) {
	for (auto& rectangle : otherHitboxes) {
		if ((m_hitBoxes[0].getRight() < rectangle.getLeft() || m_hitBoxes[0].getLeft() > rectangle.getRight())
			|| (m_hitBoxes[0].getBottom() < rectangle.getTop() || m_hitBoxes[0].getTop() > rectangle.getBottom())) {
			/*return false;*/
			m_collided = false;
		}
		else {
			/*return true;*/

			if (otherEntityType == EEntityType::eBackground || otherEntityType == EEntityType::eDoor) {

				m_collided = true;

				switch (m_currentDirection) {
				case EDirection::eDown:
					//if wall below enemy is hit - move them back to edge of the wall
					m_posY = rectangle.getTop() - m_hitBoxes[0].getHeight() - 1;
					m_lastPosition.setY(m_posY);
					m_nextPosition.setY(m_posY);
					m_lastPosition.setX(m_posX);
					m_nextPosition.setX(m_posX);
					break;
				case EDirection::eLeft:
					//if wall left of enemy is hit - move them back to edge of the wall
					m_posX = rectangle.getRight() + 1;
					m_lastPosition.setY(m_posY);
					m_nextPosition.setY(m_posY);
					m_lastPosition.setX(m_posX);
					m_nextPosition.setX(m_posX);
					break;
				case EDirection::eUp:
					//if wall above enemy is hit - move them back to edge of the wall
					m_posY = rectangle.getBottom() - 1;
					m_lastPosition.setY(m_posY);
					m_nextPosition.setY(m_posY);
					m_lastPosition.setX(m_posX);
					m_nextPosition.setX(m_posX);
					break;
				case EDirection::eRight:
					//if wall right of enemy is hit - move them back to edge of the wall
					m_posX = rectangle.getLeft() - (m_hitBoxes[0].getWidth()) - 1;
					m_lastPosition.setY(m_posY);
					m_nextPosition.setY(m_posY);
					m_lastPosition.setX(m_posX);
					m_nextPosition.setX(m_posX);
					break;
				default:
					break;
				}
			}
			else if (otherEntityType == EEntityType::eBullet) {
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

