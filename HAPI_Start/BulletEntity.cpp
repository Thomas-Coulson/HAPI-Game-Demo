#include "BulletEntity.h"
#include "Rectangle.h"
#include "World.h"

BulletEntity::BulletEntity(const std::string& currentName, int posX, int posY, bool alpha, bool clipped, EDirection currentDirection) :
	Entity(currentName, posX, posY, alpha, clipped)
{
	m_moveAmount = 12;
	m_entityType = EEntityType::eBullet;
	m_currentDirection = currentDirection;
	m_explosionType = EExplosionType::eBullet;
	m_alive = false;

	Rectangle hitbox;
	m_hitBoxes.push_back(hitbox);

}

BulletEntity::~BulletEntity() {}

void BulletEntity::update(World* world) {
	if (m_alive) {
		//move
		m_moving = true;
		if (m_currentDirection == EDirection::eRight) {
			m_lastPosition.setX(m_posX);
			m_posX += m_moveAmount;
			m_nextPosition.setX(m_posX);

			m_lastPosition.setY(m_posY);
			m_nextPosition.setY(m_posY);

			//set sprite based on side
			if (m_side == ESide::ePlayer) {
				m_currentSprite = "FireballRight";
			}
			else if (m_side == ESide::eEnemy) {
				m_currentSprite = "MagicRight";
			}

			
		}
		else if (m_currentDirection == EDirection::eLeft) {
			m_lastPosition.setX(m_posX);
			m_posX -= m_moveAmount;
			m_nextPosition.setX(m_posX);

			m_lastPosition.setY(m_posY);
			m_nextPosition.setY(m_posY);

			//set sprite based on side
			if (m_side == ESide::ePlayer) {
				m_currentSprite = "FireballLeft";
			}
			else if (m_side == ESide::eEnemy) {
				m_currentSprite = "MagicLeft";
			}
		}
		else if (m_currentDirection == EDirection::eUp) {
			m_lastPosition.setX(m_posX);
			m_nextPosition.setX(m_posX);

			m_lastPosition.setY(m_posY);
			m_posY -= m_moveAmount;
			m_nextPosition.setY(m_posY);

			//set sprite based on side
			if (m_side == ESide::ePlayer) {
				m_currentSprite = "FireballUp";
			}
			else if (m_side == ESide::eEnemy) {
				m_currentSprite = "MagicUp";
			}
		}
		else if (m_currentDirection == EDirection::eDown) {
			m_lastPosition.setX(m_posX);
			m_nextPosition.setX(m_posX);

			m_lastPosition.setY(m_posY);
			m_posY += m_moveAmount;
			m_nextPosition.setY(m_posY);

			//set sprite based on side
			if (m_side == ESide::ePlayer) {
				m_currentSprite = "FireballDown";
			}
			else if (m_side == ESide::eEnemy) {
				m_currentSprite = "MagicDown";
			}
		}

		//update hitbox
		if (m_currentDirection == EDirection::eLeft || m_currentDirection == EDirection::eRight) {
			m_hitBoxes[0].setLeft(m_posX + 15);
			m_hitBoxes[0].setTop(m_posY);
			m_hitBoxes[0].setRight(m_posX + world->getWindow()->getTextureFrameWidth(m_currentSprite) - 15);
			m_hitBoxes[0].setBottom(m_posY + world->getWindow()->getTextureHeight(m_currentSprite));
		}
		else if (m_currentDirection == EDirection::eUp || m_currentDirection == EDirection::eDown) {
			m_hitBoxes[0].setLeft(m_posX);
			m_hitBoxes[0].setTop(m_posY + 15);
			m_hitBoxes[0].setRight(m_posX + world->getWindow()->getTextureFrameWidth(m_currentSprite));
			m_hitBoxes[0].setBottom(m_posY + world->getWindow()->getTextureHeight(m_currentSprite) - 15);
		}
		

	}
	else {
		m_moving = false;
	}
	
	if (m_collided){
		//=======================================================================================================================================
		//set offsets for spawning explosions
		int rightXoffset = m_hitBoxes[0].getRight() - world->getWindow()->getTextureFrameWidth("FireExplosionLeft");
		int rightYoffset = m_hitBoxes[0].getTop() - 5;

		int leftXoffset = m_hitBoxes[0].getLeft();
		int leftYoffset = m_hitBoxes[0].getTop() - 5;

		int topXoffset = (m_hitBoxes[0].getLeft() - 5);
		int topYoffset = (m_hitBoxes[0].getTop());

		int bottomXoffset = (m_hitBoxes[0].getLeft() - 5);
		int bottomYoffset = m_hitBoxes[0].getBottom() - world->getWindow()->getTextureHeight("FireExplosionDown");

		//=======================================================================================================================================
		//make explosion when entity is hit
		switch (m_currentDirection) {
		case EDirection::eLeft:
			world->makeExplosion(leftXoffset, leftYoffset, m_currentDirection, m_side, m_explosionType);
			break;
		case EDirection::eUp:
			world->makeExplosion(topXoffset, topYoffset, m_currentDirection, m_side, m_explosionType);
			break;
		case EDirection::eRight:
			world->makeExplosion(rightXoffset, rightYoffset, m_currentDirection, m_side, m_explosionType);
			break;
		case EDirection::eDown:
			world->makeExplosion(bottomXoffset, bottomYoffset, m_currentDirection, m_side, m_explosionType);
			break;
		default:
			break;
		}

		m_collided = false;
		m_alive = false;
	}

}

void BulletEntity::checkCollisions(std::vector<Rectangle> otherHitboxes, ESide otherHitboxSide, EEntityType otherEntityType) {
	for (auto& rectangle : otherHitboxes) {
		if ((m_hitBoxes[0].getRight() < rectangle.getLeft() || m_hitBoxes[0].getLeft() > rectangle.getRight())
			|| (m_hitBoxes[0].getBottom() < rectangle.getTop() || m_hitBoxes[0].getTop() > rectangle.getBottom())) {
			/*return false;*/
		}
		else {

			/*return true;*/
			//bullets can not hit players or player bullets
			if (otherHitboxSide != m_side) {
				m_collided = true;
			}


		}
	}
}