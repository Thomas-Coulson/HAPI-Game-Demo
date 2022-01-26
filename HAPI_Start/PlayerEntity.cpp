#include "PlayerEntity.h"
#include "World.h"
#include "Rectangle.h"
#include "Vector2.h"

PlayerEntity::PlayerEntity(const std::string& currentName, int posX, int posY, bool alpha, bool clipped) :
	Entity(currentName, posX, posY, alpha, clipped),
	m_diagonalMoveAmount(sqrtf((pow(m_moveAmount, 2) / 2.0f))),
	m_fireSpeed(20),
	m_bulletTimeCounter(0),
	m_damageSpeed(50),
	m_damageTimeCounter(0),
	m_invincible(false)
{
	//hitbox for walking
	Rectangle legHitbox;
	m_hitBoxes.push_back(legHitbox);

	//hitbox for getting hit
	Rectangle bodyHitbox;
	m_hitBoxes.push_back(bodyHitbox);

	m_health = 10;
	m_moveAmount = 9;
	m_side = ESide::ePlayer;
	m_entityType = EEntityType::ePlayer;
	m_alive = true;
}

PlayerEntity::~PlayerEntity() {}

void PlayerEntity::update(World* world) {
	//move player

	m_moving = false;

	m_lastPosition.setX(m_posX);
	m_lastPosition.setY(m_posY);
	m_nextPosition.setX(m_posX);
	m_nextPosition.setY(m_posY);

	if (m_alive) {
		//take input for player
		if (world->getControllerData().isAttached) {
			//controller input - if plugged in
			if (world->getControllerData().analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > 7849) {

				m_currentDirection = EDirection::eUp;

				m_lastPosition.setY(m_posY);
				m_posY -= m_moveAmount;
				m_nextPosition.setY(m_posY);

				m_lastPosition.setX(m_posX);
				m_nextPosition.setX(m_posX);

				m_moving = true;
			}
			else if (world->getControllerData().analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -7849) {

				m_currentDirection = EDirection::eDown;

				m_lastPosition.setY(m_posY);
				m_posY += m_moveAmount;
				m_nextPosition.setY(m_posY);

				m_lastPosition.setX(m_posX);
				m_nextPosition.setX(m_posX);

				m_moving = true;
			}
			else if (world->getControllerData().analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -7849) {

				m_currentDirection = EDirection::eLeft;

				m_lastPosition.setX(m_posX);
				m_posX -= m_moveAmount;
				m_nextPosition.setX(m_posX);

				m_lastPosition.setY(m_posY);
				m_nextPosition.setY(m_posY);

				m_currentSprite = "PlayerLeft";
				m_moving = true;
			}
			else if (world->getControllerData().analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > 7849) {

				m_currentDirection = EDirection::eRight;

				m_lastPosition.setX(m_posX);
				m_posX += m_moveAmount;
				m_nextPosition.setX(m_posX);

				m_lastPosition.setY(m_posY);
				m_nextPosition.setY(m_posY);

				m_currentSprite = "PlayerRight";
				m_moving = true;
			}

		}
		//======================================================================================================================================================
		//keyboard controlls - moving

		if (world->getKeyboardData().scanCode['W']) {

			m_currentDirection = EDirection::eUp;

			m_lastPosition.setY(m_posY);
			m_posY -= m_moveAmount;
			m_nextPosition.setY(m_posY);

			m_lastPosition.setX(m_posX);
			m_nextPosition.setX(m_posX);

			m_moving = true;
		}
		else if (world->getKeyboardData().scanCode['S']) {

			m_currentDirection = EDirection::eDown;

			m_lastPosition.setY(m_posY);
			m_posY += m_moveAmount;
			m_nextPosition.setY(m_posY);

			m_lastPosition.setX(m_posX);
			m_nextPosition.setX(m_posX);

			m_moving = true;
		}
		else if (world->getKeyboardData().scanCode['A']) {

			m_currentDirection = EDirection::eLeft;

			m_lastPosition.setX(m_posX);
			m_posX -= m_moveAmount;
			m_nextPosition.setX(m_posX);

			m_lastPosition.setY(m_posY);
			m_nextPosition.setY(m_posY);

			m_currentSprite = "PlayerLeft";
			m_moving = true;
		}
		else if (world->getKeyboardData().scanCode['D']) {

			m_currentDirection = EDirection::eRight;

			m_lastPosition.setX(m_posX);
			m_posX += m_moveAmount;
			m_nextPosition.setX(m_posX);

			m_lastPosition.setY(m_posY);
			m_nextPosition.setY(m_posY);

			m_currentSprite = "PlayerRight";
			m_moving = true;
		}

		//=====================================================================================================================================================
		//calculate offsets for spawining bullets infront of player
		int rightXoffset = m_hitBoxes[0].getRight();
		int rightYoffset = m_hitBoxes[0].getTop();

		int leftXoffset = m_hitBoxes[0].getLeft() - world->getWindow()->getTextureFrameWidth("FireballLeft");
		int leftYoffset = m_hitBoxes[0].getTop();

		int topXoffset = (m_hitBoxes[0].getLeft() + m_hitBoxes[0].getWidth() / 2) - (world->getWindow()->getTextureFrameWidth("FireballUp") / 2);
		int topYoffset = (m_hitBoxes[0].getBottom() - m_hitBoxes[0].getHeight() * 3) - world->getWindow()->getTextureFrameWidth("FireballUp") - 15;

		int bottomXoffset = (m_hitBoxes[0].getLeft() + m_hitBoxes[0].getWidth() / 2) - (world->getWindow()->getTextureFrameWidth("FireballDown") / 2);
		int bottomYoffset = m_hitBoxes[0].getBottom() - 15;


		//fire bullets
		//controller
		if (world->getControllerData().isAttached) {
			if (world->getControllerData().analogueButtons[HK_ANALOGUE_RIGHT_THUMB_X] > 7849) {
				m_currentSprite = "PlayerRight";
				if (m_bulletTimeCounter >= m_fireSpeed) {
					world->fireBullet(rightXoffset, rightYoffset, EDirection::eRight, m_side);
					m_bulletTimeCounter = 0;
				}
			}
			else if (world->getControllerData().analogueButtons[HK_ANALOGUE_RIGHT_THUMB_X] < -7849) {
				m_currentSprite = "PlayerLeft";
				if (m_bulletTimeCounter >= m_fireSpeed) {
					world->fireBullet(leftXoffset, leftYoffset, EDirection::eLeft, m_side);
					m_bulletTimeCounter = 0;
				}
			}
			else if (world->getControllerData().analogueButtons[HK_ANALOGUE_RIGHT_THUMB_Y] > 7849) {
				if (m_bulletTimeCounter >= m_fireSpeed) {
					world->fireBullet(topXoffset, topYoffset, EDirection::eUp, m_side);
					m_bulletTimeCounter = 0;
				}
			}
			else if (world->getControllerData().analogueButtons[HK_ANALOGUE_RIGHT_THUMB_Y] < -7849) {
				if (m_bulletTimeCounter >= m_fireSpeed) {
					world->fireBullet(bottomXoffset, bottomYoffset, EDirection::eDown, m_side);
					m_bulletTimeCounter = 0;
				}
			}
		}

		//keyboard
		if (world->getKeyboardData().scanCode[HK_RIGHT]) {
			m_currentSprite = "PlayerRight";
			if (m_bulletTimeCounter >= m_fireSpeed) {
				world->fireBullet(rightXoffset, rightYoffset, EDirection::eRight, m_side);
				m_bulletTimeCounter = 0;
			}
		}
		else if (world->getKeyboardData().scanCode[HK_LEFT]) {
			m_currentSprite = "PlayerLeft";
			if (m_bulletTimeCounter >= m_fireSpeed) {
				world->fireBullet(leftXoffset, leftYoffset, EDirection::eLeft, m_side);
				m_bulletTimeCounter = 0;
			}
		}
		else if (world->getKeyboardData().scanCode[HK_UP]) {
			if (m_bulletTimeCounter >= m_fireSpeed) {
				world->fireBullet(topXoffset, topYoffset, EDirection::eUp, m_side);
				m_bulletTimeCounter = 0;
			}
		}
		else if (world->getKeyboardData().scanCode[HK_DOWN]) {
			if (m_bulletTimeCounter >= m_fireSpeed) {
				world->fireBullet(bottomXoffset, bottomYoffset, EDirection::eDown, m_side);
				m_bulletTimeCounter = 0;
			}
		}

		m_bulletTimeCounter++;
		m_damageTimeCounter++;

		//=====================================================================================================================================================
		//update hitboxes
		m_hitBoxes[0].setLeft(m_posX + 15);
		m_hitBoxes[0].setTop(m_posY + (world->getWindow()->getTextureHeight(m_currentSprite) / 3) * 2);
		m_hitBoxes[0].setRight(m_posX + world->getWindow()->getTextureFrameWidth(m_currentSprite) - 15);
		m_hitBoxes[0].setBottom(m_posY + world->getWindow()->getTextureHeight(m_currentSprite));

		m_hitBoxes[1].setLeft(m_posX + 15);
		m_hitBoxes[1].setTop(m_posY);
		m_hitBoxes[1].setRight(m_posX + world->getWindow()->getTextureFrameWidth(m_currentSprite) - 15);
		m_hitBoxes[1].setBottom(m_posY + world->getWindow()->getTextureHeight(m_currentSprite));

		//=====================================================================================================================================================


		if (m_health <= 0) {
			//if player health reaches 0 - game has been lost
			m_alive = false;
			m_visible = false;
			world->setLose(true);
			HAPI.PlaySound("Data\\Sounds\\LoseSound.ogg");
		}

		m_invincible = world->getInvincible();

	}
}

void PlayerEntity::checkCollisions(std::vector<Rectangle> otherHitboxes, ESide otherHitboxSide, EEntityType otherEntityType) {
	Rectangle currentHitbox;
	if (otherEntityType == EEntityType::eBackground || otherEntityType == EEntityType::eDoor) {
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

			if (otherEntityType == EEntityType::eBackground || otherEntityType == EEntityType::eDoor) {
				m_collided = true;

				switch (m_currentDirection) {
				case EDirection::eDown:
					//if wall below player is hit - move them back to edge of the wall
					m_posY = (rectangle.getTop() - (m_hitBoxes[0].getHeight()) * 3) - 1;
					m_lastPosition.setY(m_posY);
					m_nextPosition.setY(m_posY);
					m_lastPosition.setX(m_posX);
					m_nextPosition.setX(m_posX);
					break;
				case EDirection::eLeft:
					//if wall left of player is hit - move them back to edge of the wall
					m_posX = rectangle.getRight() - 15 + 1;
					m_lastPosition.setY(m_posY);
					m_nextPosition.setY(m_posY);
					m_lastPosition.setX(m_posX);
					m_nextPosition.setX(m_posX);
					break;
				case EDirection::eUp:
					//if wall above player is hit - move them back to edge of the wall
					m_posY = (rectangle.getBottom() - (m_hitBoxes[0].getHeight()) * 2) + 1;
					m_lastPosition.setY(m_posY);
					m_nextPosition.setY(m_posY);
					m_lastPosition.setX(m_posX);
					m_nextPosition.setX(m_posX);
					break;
				case EDirection::eRight:
					//if wall right of player is hit - move them back to edge of the wall
					m_posX = rectangle.getLeft() - ((m_hitBoxes[0].getWidth()) + 15) - 1;
					m_lastPosition.setY(m_posY);
					m_nextPosition.setY(m_posY);
					m_lastPosition.setX(m_posX);
					m_nextPosition.setX(m_posX);
					break;
				default:
					break;
				}

			}
			else if ((otherEntityType == EEntityType::eBullet || otherEntityType == EEntityType::eChaseEnemy ||
				otherEntityType == EEntityType::eWallEnemy || otherEntityType == EEntityType::eWanderEnemy) && otherHitboxSide == ESide::eEnemy) {
				//if player is hit by enemy or bullet - take damage
				if (m_damageTimeCounter >= m_damageSpeed && !m_invincible) {
					//can only take damage once every damageSpeed
					m_health--;
					if (m_health > 0) {
						HAPI.PlaySound("Data\\Sounds\\Hit.wav", HAPI_TSoundOptions(0.3));
					}
					m_damageTimeCounter = 0;
				}

			}

		}
	}

	

}