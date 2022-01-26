#include "DoorEntity.h"
#include "Rectangle.h"
#include "World.h"

DoorEntity::DoorEntity(const std::string& currentName, int posX, int posY, bool alpha, bool clipped, EDirection currentDirection) :
	Entity(currentName, posX, posY, alpha, clipped)
{
	m_side = ESide::eNeutral;
	m_entityType = EEntityType::eDoor;
	m_currentDirection = currentDirection;
	m_alive = true;
	m_open = false;

	Rectangle closedBox;
	m_hitBoxes.push_back(closedBox);
}

DoorEntity::~DoorEntity() {}

void DoorEntity::update(World* world) {
	//only open doors if the room is empty and the player can move there
	//stops player from moving too far left or right in the rooms vector
	if (m_currentDirection == EDirection::eLeft && world->getCurrentRoomIndex() == 0) {
		m_open = false;
	}
	else if (m_currentDirection == EDirection::eRight && world->getCurrentRoomIndex() == world->getMaxRoomIndex()) {
		m_open = false;
	}
	else if(world->getRoomEmpty()) {
		m_open = true;
	}
	else {
		m_open = false;
	}

	//update hitboxes
	if (m_currentDirection == EDirection::eLeft) {
		if (m_open) {
			//open hitbox
			m_hitBoxes[0].setLeft(0);
			m_hitBoxes[0].setTop(420);
			m_hitBoxes[0].setRight(80);
			m_hitBoxes[0].setBottom(485);
		}
		else {
			//closed hitbox
			m_hitBoxes[0].setLeft(0);
			m_hitBoxes[0].setTop(420);
			m_hitBoxes[0].setRight(110);
			m_hitBoxes[0].setBottom(485);
		}
		
	}
	else if (m_currentDirection == EDirection::eRight) {
		if (m_open) {
			//open hitbox
			m_hitBoxes[0].setLeft(944);
			m_hitBoxes[0].setTop(420);
			m_hitBoxes[0].setRight(1024);
			m_hitBoxes[0].setBottom(485);
		}
		else {
			//closed hitbox
			m_hitBoxes[0].setLeft(914);
			m_hitBoxes[0].setTop(420);
			m_hitBoxes[0].setRight(1024);
			m_hitBoxes[0].setBottom(485);
		}
	}

	//check if player needs to change rooms
	if (m_collided && m_currentDirection == EDirection::eLeft) {
		world->setLeftDoorEntered(true);
		world->setRightDoorEntered(false);
		world->setRoomDirection(ERoomDirection::eLeft);
	}
	else if (m_collided && m_currentDirection == EDirection::eRight) {
		world->setRightDoorEntered(true);
		world->setLeftDoorEntered(false);
		world->setRoomDirection(ERoomDirection::eRight);
	}

	//set sprites based on open or closed
	if (m_open) {
		if (m_currentDirection == EDirection::eLeft) {
			m_currentSprite = "LeftDoorOpen";
		}
		else if (m_currentDirection == EDirection::eRight) {
			m_currentSprite = "RightDoorOpen";
		}
	}
	else {
		if (m_currentDirection == EDirection::eLeft) {
			m_currentSprite = "LeftDoorClosed";
		}
		else if (m_currentDirection == EDirection::eRight) {
			m_currentSprite = "RightDoorClosed";
		}
	}
}

void DoorEntity::checkCollisions(std::vector<Rectangle> otherHitboxes, ESide otherHitboxSide, EEntityType otherEntityType) {
	for (auto& rectangle : otherHitboxes) {
		if ((m_hitBoxes[0].getRight() < rectangle.getLeft() || m_hitBoxes[0].getLeft() > rectangle.getRight())
			|| (m_hitBoxes[0].getBottom() < rectangle.getTop() || m_hitBoxes[0].getTop() > rectangle.getBottom())) {
			/*return false;*/
			//player is not inside the door
			if (otherEntityType == EEntityType::ePlayer && m_open == true) {
				m_collided = false;
			}
		}
		else {
			/*return true;*/
			//player is inside the door
			if (otherEntityType == EEntityType::ePlayer && m_open == true) {
				m_collided = true;
			}
		}
	}
}