#include "BackgroundEntity.h"
#include "Rectangle.h"
#include "World.h"

BackgroundEntity::BackgroundEntity(const std::string& currentName, int posX, int posY, bool alpha, bool clipped) :
	Entity(currentName, posX, posY, alpha, clipped),
	m_finalLeftPosX(0),
	m_finalRightPosX(0)
{
	//set all wall hitboxes

	Rectangle bottomWall(0, 1024, 700, 770);
	m_hitBoxes.push_back(bottomWall);

	Rectangle LeftWallTop(0, 110, 147, 422);
	m_hitBoxes.push_back(LeftWallTop);

	Rectangle LeftWallBottom(0, 110, 482, 768);
	m_hitBoxes.push_back(LeftWallBottom);

	Rectangle topWall(0, 1024, 0, 210);
	m_hitBoxes.push_back(topWall);

	Rectangle rightWallTop(914, 1024, 147, 422);
	m_hitBoxes.push_back(rightWallTop);

	Rectangle rightWallBottom(914, 1024, 482, 768);
	m_hitBoxes.push_back(rightWallBottom);

	m_moveAmount = 64;
	m_side = ESide::eNeutral;
	m_entityType = EEntityType::eBackground;
	m_alive = true;
}

BackgroundEntity::~BackgroundEntity() {}

void BackgroundEntity::update(World* world) {

	if (m_finishedMoving) {
		//sets the direction the background needs to move in
		if (m_posX == -1024) {
			m_finalRightPosX = 0;
		}
		else if (m_posX == 0) {
			m_finalRightPosX = 1024;
			m_finalLeftPosX = -1024;
		}
		else if (m_posX == 1024) {
			m_finalLeftPosX = 0;
		}
	}

	if (m_alive) {
		m_lastPosition.setX(m_posX);
		m_nextPosition.setX(m_posX);
		m_lastPosition.setY(m_posY);
		m_nextPosition.setY(m_posY);

		if (world->getLeftDoorEntered()) {
			//if left door entered, move background right

			if (m_posX < m_finalRightPosX) {
				m_lastPosition.setX(m_posX);
				m_posX += m_moveAmount;
				m_nextPosition.setX(m_posX);

				m_lastPosition.setY(m_posY);
				m_nextPosition.setY(m_posY);
			}
			else {
				if (m_posX < 0 || m_posX >= 1024) {
					//if background is now off screen - reset position and make it not alive
					m_alive = false;
					m_posX = 0;
				}
			}

			if (m_posX == 0 || m_posX == 1024 || m_posX == -1024) {
				//stop moving of background is in place
				m_finishedMoving = true;
			}

			
		}
		else if (world->getRightDoorEntered()) {
			//if right door entered, move background left

			if (m_posX > m_finalLeftPosX) {
				m_lastPosition.setX(m_posX);
				m_posX -= m_moveAmount;
				m_nextPosition.setX(m_posX);

				m_lastPosition.setY(m_posY);
				m_nextPosition.setY(m_posY);
			}
			else {
				if (m_posX < 0 || m_posX >= 1024) {
					//if background is now off screen - reset position and make it not alive
					m_alive = false;
					m_posX = 0;
				}
			}

			if (m_posX == 0 || m_posX == 1024 || m_posX == -1024) {
				//stop moving of background is in place
				m_finishedMoving = true;
			}
		}
	}

}

void BackgroundEntity::checkCollisions(std::vector<Rectangle> otherHitboxes, ESide otherHitboxSide, EEntityType otherEntityType) {
	for (auto& rectangle : otherHitboxes) {
		if ((m_hitBoxes[0].getRight() < rectangle.getLeft() || m_hitBoxes[0].getLeft() > rectangle.getRight())
			|| (m_hitBoxes[0].getBottom() < rectangle.getTop() || m_hitBoxes[0].getTop() > rectangle.getBottom())) {
			/*return false;*/
		}
		else {
			/*return true;*/
		}
	}
}
