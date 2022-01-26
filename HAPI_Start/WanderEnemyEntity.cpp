#include "WanderEnemyEntity.h"
#include "World.h"

WanderEnemyEntity::WanderEnemyEntity(const std::string& currentName, int posX, int posY, bool alpha, bool clipped) :
	Entity(currentName, posX, posY, alpha, clipped),
	m_fireSpeed(20),
	m_bulletTimeCounter(0)
{
	m_health = rand() % 3 + 1;
	m_side = ESide::eEnemy;
	m_entityType = EEntityType::eWanderEnemy;
	m_explosionType = EExplosionType::eEnemy;
	m_currentDirection = EDirection::eLeft;
	m_moveAmount = 4;
	m_alive = false;
	m_moving = true;
	m_scoreAmount = 10 * m_health;

	//set hitbox for walking
	Rectangle legHitbox;
	m_hitBoxes.push_back(legHitbox);

	//set hitbox for getting hit
	Rectangle bodyHitbox;
	m_hitBoxes.push_back(bodyHitbox);

	//set hitboxes
	m_hitBoxes[0].setLeft(m_posX);
	m_hitBoxes[0].setTop(m_posY);
	m_hitBoxes[0].setRight(m_posX);
	m_hitBoxes[0].setBottom(m_posY);

	m_hitBoxes[1].setLeft(m_posX);
	m_hitBoxes[1].setTop(m_posY);
	m_hitBoxes[1].setRight(m_posX);
	m_hitBoxes[1].setBottom(m_posY);

}

WanderEnemyEntity::~WanderEnemyEntity() {}

void WanderEnemyEntity::update(World* world) {
	if (m_alive) {
		//4 in 50 chance to change direction every tick
		//1 in 50 chance to shoot every tick
		int moveChance = rand() % 50;
		int shootChance = rand() % 50;

		switch (moveChance) {
		case 0:
			m_currentDirection = EDirection::eLeft;
			break;
		case 1:
			m_currentDirection = EDirection::eRight;
			break;
		case 2:
			m_currentDirection = EDirection::eUp;
			break;
		case 3:
			m_currentDirection = EDirection::eDown;
			break;
		}

		//===================================================================================================================================================
		//set offsets for spawning bullets
		int rightXoffset = m_hitBoxes[0].getRight();
		int rightYoffset = m_hitBoxes[0].getTop();

		int leftXoffset = m_hitBoxes[0].getLeft() - world->getWindow()->getTextureFrameWidth("FireballLeft");
		int leftYoffset = m_hitBoxes[0].getTop();

		int topXoffset = (m_hitBoxes[0].getLeft() + m_hitBoxes[0].getWidth() / 2) - (world->getWindow()->getTextureFrameWidth("FireballUp") / 2);
		int topYoffset = (m_hitBoxes[0].getBottom() - m_hitBoxes[0].getHeight() * 3) - world->getWindow()->getTextureFrameWidth("FireballUp") - 15;

		int bottomXoffset = (m_hitBoxes[0].getLeft() + m_hitBoxes[0].getWidth() / 2) - (world->getWindow()->getTextureFrameWidth("FireballDown") / 2);
		int bottomYoffset = m_hitBoxes[0].getBottom() - 15;
		//===================================================================================================================================================

		if (shootChance == 0) {
			if (m_bulletTimeCounter >= m_fireSpeed) {

				switch (m_currentDirection) {
				case EDirection::eRight:
					world->fireBullet(rightXoffset, rightYoffset, m_currentDirection, m_side);
					break;
				case EDirection::eLeft:
					world->fireBullet(leftXoffset, leftYoffset, m_currentDirection, m_side);
					break;
				case EDirection::eUp:
					world->fireBullet(topXoffset, topYoffset, m_currentDirection, m_side);
					break;
				case EDirection::eDown:
					world->fireBullet(bottomXoffset, bottomYoffset, m_currentDirection, m_side);
					break;
				default:
					break;
				}

				m_bulletTimeCounter = 0;
			}

		}

		//move in current direction
		if (m_currentDirection == EDirection::eLeft) {
			m_lastPosition.setX(m_posX);
			m_posX -= m_moveAmount;
			m_nextPosition.setX(m_posX);

			m_lastPosition.setY(m_posY);
			m_nextPosition.setY(m_posY);

			m_currentSprite = "ShamanLeft";
			m_moving = true;
		}
		else if (m_currentDirection == EDirection::eRight) {
			m_lastPosition.setX(m_posX);
			m_posX += m_moveAmount;
			m_nextPosition.setX(m_posX);

			m_lastPosition.setY(m_posY);
			m_nextPosition.setY(m_posY);

			m_currentSprite = "ShamanRight";
			m_moving = true;
		}
		else if (m_currentDirection == EDirection::eUp) {
			m_lastPosition.setY(m_posY);
			m_posY -= m_moveAmount;
			m_nextPosition.setY(m_posY);

			m_lastPosition.setX(m_posX);
			m_nextPosition.setX(m_posX);

			m_moving = true;
		}
		else if (m_currentDirection == EDirection::eDown) {
			m_lastPosition.setY(m_posY);
			m_posY += m_moveAmount;
			m_nextPosition.setY(m_posY);

			m_lastPosition.setX(m_posX);
			m_nextPosition.setX(m_posX);

			m_moving = true;
		}

		//update hitboxes
		m_hitBoxes[0].setLeft(m_posX);
		m_hitBoxes[0].setTop(m_posY + (world->getWindow()->getTextureHeight(m_currentSprite) / 3) * 2);
		m_hitBoxes[0].setRight(m_posX + world->getWindow()->getTextureFrameWidth(m_currentSprite));
		m_hitBoxes[0].setBottom(m_posY + world->getWindow()->getTextureHeight(m_currentSprite));

		m_hitBoxes[1].setLeft(m_posX);
		m_hitBoxes[1].setTop(m_posY);
		m_hitBoxes[1].setRight(m_posX + world->getWindow()->getTextureFrameWidth(m_currentSprite));
		m_hitBoxes[1].setBottom(m_posY + world->getWindow()->getTextureHeight(m_currentSprite));

		m_bulletTimeCounter++;
		
		if (m_health <= 0) {
			//if health <= 0 - spawn explosion and kill enemy
			m_alive = false;
			world->makeExplosion(m_posX, m_posY, m_currentDirection, m_side, m_explosionType);
			world->increaseScore(m_scoreAmount);
		}

	}


}

void WanderEnemyEntity::checkCollisions(std::vector<Rectangle> otherHitboxes, ESide otherHitboxSide, EEntityType otherEntityType) {
	Rectangle currentHitbox;
	if (otherEntityType == EEntityType::eBackground) {
		//check against leg hitbox for doors and background
		currentHitbox = m_hitBoxes[0];
	}
	else {
		//check against body hitbox for everything else
		currentHitbox = m_hitBoxes[1];
	}

	for (auto& rectangle : otherHitboxes) {
		if ((currentHitbox.getRight() < rectangle.getLeft() || currentHitbox.getLeft() > rectangle.getRight())
			|| (currentHitbox.getBottom() < rectangle.getTop() || currentHitbox.getTop() > rectangle.getBottom())) {
			/*return false;*/
			m_collided = false;
		}
		else {
			/*return true;*/
			m_collided = true;
			
			if (otherEntityType != EEntityType::eBullet) {
				switch (m_currentDirection) {
				case EDirection::eDown:
					//if wall below enemy is hit - move them back to edge of the wall
					m_posY = rectangle.getTop() - (m_hitBoxes[0].getHeight() * 3);
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
					m_posY = rectangle.getBottom() - (m_hitBoxes[0].getHeight() * 2) + 5;
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