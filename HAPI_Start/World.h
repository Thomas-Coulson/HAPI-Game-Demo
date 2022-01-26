#pragma once
#include <HAPI_lib.h>
#include <cmath>
#include <string>
#include <unordered_map>

#include "Visualisation.h"

using namespace HAPISPACE;

class Rectangle;
class Entity;

enum class EDirection;
enum class ESide;
enum class EEntityType;
enum class EExplosionType;

enum class ERoomDirection {
	eLeft,
	eRight,
	eNeutral
};

class World
{
private:
	Visualisation* m_window = nullptr;

	std::vector<Entity*> m_allEntities;
	std::vector<int> m_rooms;
	std::vector<int> m_visitedIndexes;

	HAPI_TKeyboardData m_keyboardData;
	HAPI_TControllerData m_controllerData;

	ERoomDirection m_roomDirection;

	int m_tickSpeed;
	int m_currentRoomIndex;
	int m_score;
	int m_time;

	bool m_roomEmpty;
	bool m_leftDoorEntered;
	bool m_rightDoorEntered;
	bool m_displayGroundUI;
	bool m_displayMainMenu;
	bool m_win;
	bool m_lose;
	bool m_invincible;

public:
	World();
	~World();

	void loadAssets();
	void run();

	void loadRoom(int roomID);

	void fireBullet(int posX, int posY, EDirection currentDirection, ESide side);
	void makeExplosion(int posX, int posY, EDirection currentDirection, ESide side, EExplosionType type);

	void spawnChaseEnemy(int posX, int posY, EDirection currentDirection);
	void spawnWallEnemy(int posX, int posY, EDirection currentDirection);
	void spawnWanderEnemy(int posX, int posY, EDirection currentDirection);

	Visualisation* getWindow() const { return m_window; }
	std::vector<Entity*> getAllEntities() const { return m_allEntities; }
	const HAPI_TKeyboardData& getKeyboardData() const { return m_keyboardData; }
	const HAPI_TControllerData& getControllerData() const { return m_controllerData; }
	int getTickSpeed() const { return m_tickSpeed; }
	int getCurrentRoomIndex() const { return m_currentRoomIndex; }
	int getMaxRoomIndex() const { return (m_rooms.size() - 1); }
	bool getRoomEmpty() const { return m_roomEmpty; }
	bool getLeftDoorEntered() const { return m_leftDoorEntered; }
	bool getRightDoorEntered() const { return m_rightDoorEntered; }
	bool getInvincible() const { return m_invincible; }

	void setRoomDirection(ERoomDirection roomDirection) { m_roomDirection = roomDirection; }
	void setLeftDoorEntered(bool entered) { m_leftDoorEntered = entered; }
	void setRightDoorEntered(bool entered) { m_rightDoorEntered = entered; }
	void setLose(bool lose) { m_lose = lose; }
	void increaseScore(int scoreIncrease) { m_score += scoreIncrease; }

	void resetGame();
};

