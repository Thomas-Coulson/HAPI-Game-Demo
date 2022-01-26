#include "World.h"
#include "Visualisation.h"
#include "Rectangle.h"
#include "Entity.h"
#include "PlayerEntity.h"
#include "BackgroundEntity.h"
#include "BulletEntity.h"
#include "ExplosionEntity.h"
#include "WallEnemyEntity.h"
#include "ChaseEnemyEntity.h"
#include "WanderEnemyEntity.h"
#include "DoorEntity.h"

#include <ctime>

World::World() :
	m_keyboardData(HAPI.GetKeyboardData()),
	m_controllerData(HAPI.GetControllerData(0)),
	m_roomDirection(ERoomDirection::eNeutral),
	m_tickSpeed(20),
	m_currentRoomIndex(0),
	m_score(0),
	m_time(0),
	m_roomEmpty(true),
	m_leftDoorEntered(false),
	m_rightDoorEntered(false),
	m_displayGroundUI(false),
	m_displayMainMenu(true),
	m_win(false),
	m_lose(false),
	m_invincible(false)
{

	m_window = new Visualisation;

	//initialise window
	if (!m_window->initialise()) {
		return;
	}

	//initialise rooms
	//numbers in vector represent types of room
	m_rooms = std::vector<int>{ 3, 2, 1, 0, 1, 2, 3 };
	m_currentRoomIndex = (m_rooms.size() - 1) / 2;

}

World::~World() {
	delete m_window;
	
	for (auto& entity : m_allEntities) {
		delete entity;
	}
}

void World::loadAssets() {

	//create all needed entities

	BackgroundEntity* newBackground = new BackgroundEntity("Background", 0, 145, false, true);
	m_allEntities.push_back(newBackground);

	BackgroundEntity* newBackground2 = new BackgroundEntity("Background", 0, 145, false, true);
	newBackground2->setAlive(false);
	m_allEntities.push_back(newBackground2);

	DoorEntity* leftDoor = new DoorEntity("LeftDoorClosed", 41, 403, true, true, EDirection::eLeft);
	m_allEntities.push_back(leftDoor);

	DoorEntity* rightDoor = new DoorEntity("RightDoorClosed", 912, 404, true, true, EDirection::eRight);
	m_allEntities.push_back(rightDoor);

	PlayerEntity* newPlayer = new PlayerEntity("PlayerRight", 470, 390, true, true);
	m_allEntities.push_back(newPlayer);

	for (int i = 0; i < 2; i++) {
		m_allEntities.push_back(new WallEnemyEntity("WallShooterTop", 500, 180, true, true, EDirection::eLeft, EDirection::eDown));
	}

	for (int i = 0; i < 3; i++) {
		m_allEntities.push_back(new ChaseEnemyEntity("WolfRight", 800, 300, true, true));
	}

	for (int i = 0; i < 2; i++) {
		m_allEntities.push_back(new WanderEnemyEntity("ShamanLeft", 600, 400, true, true));
	}

	for (int i = 0; i < 10; i++) {
		m_allEntities.push_back(new BulletEntity("FireballRight", 0, 0, true, true, EDirection::eRight));
	}

	for (int i = 0; i < 10; i++) {
		m_allEntities.push_back(new ExplosionEntity("FireExplosionLeft", 0, 0, true, true, EDirection::eLeft));
	}

	//==============================================================================================================================================================
	//create all needed sprites

	if (!m_window->createSprite("Background", "Data\\Backgrounds\\DungeonBackground.png", 0)) {
		return;
	}
	if (!m_window->createSprite("LeftDoorOpen", "Data\\Doors\\LeftDoorOpen.png", 0)) {
		return;
	}
	if (!m_window->createSprite("LeftDoorClosed", "Data\\Doors\\LeftDoorClosed.png", 0)) {
		return;
	}
	if (!m_window->createSprite("RightDoorOpen", "Data\\Doors\\RightDoorOpen.png", 0)) {
		return;
	}
	if (!m_window->createSprite("RightDoorClosed", "Data\\Doors\\RightDoorClosed.png", 0)) {
		return;
	}

	if (!m_window->createSprite("ShamanRight", "Data\\Enemies\\Shaman\\ShamanRight.png", 4)) {
		return;
	}
	if (!m_window->createSprite("ShamanLeft", "Data\\Enemies\\Shaman\\ShamanLeft.png", 4)) {
		return;
	}

	if (!m_window->createSprite("WolfRight", "Data\\Enemies\\Wolf\\WolfRight.png", 4)) {
		return;
	}
	if (!m_window->createSprite("WolfLeft", "Data\\Enemies\\Wolf\\WolfLeft.png", 4)) {
		return;
	}

	if (!m_window->createSprite("WallShooterTop", "Data\\Enemies\\WallShooter\\WallShooterTop.png", 4)) {
		return;
	}
	if (!m_window->createSprite("WallShooterBottom", "Data\\Enemies\\WallShooter\\WallShooterBottom.png", 4)) {
		return;
	}

	if (!m_window->createSprite("PlayerRight", "Data\\Wizard\\WizardRight.png", 4)) {
		return;
	}
	if (!m_window->createSprite("PlayerLeft", "Data\\Wizard\\WizardLeft.png", 4)) {
		return;
	}

	if (!m_window->createSprite("FireballRight", "Data\\Fireball\\FireballRight.png", 30)) {
		return;
	}
	if (!m_window->createSprite("FireballLeft", "Data\\Fireball\\FireballLeft.png", 30)) {
		return;
	}
	if (!m_window->createSprite("FireballUp", "Data\\Fireball\\FireballUp.png", 30)) {
		return;
	}
	if (!m_window->createSprite("FireballDown", "Data\\Fireball\\FireballDown.png", 30)) {
		return;
	}
	if (!m_window->createSprite("MagicRight", "Data\\Fireball\\MagicRight.png", 30)) {
		return;
	}
	if (!m_window->createSprite("MagicLeft", "Data\\Fireball\\MagicLeft.png", 30)) {
		return;
	}
	if (!m_window->createSprite("MagicUp", "Data\\Fireball\\MagicUp.png", 30)) {
		return;
	}
	if (!m_window->createSprite("MagicDown", "Data\\Fireball\\MagicDown.png", 30)) {
		return;
	}

	if (!m_window->createSprite("FireExplosionRight", "Data\\Explosions\\FireExplosionRight.png", 3)) {
		return;
	}
	if (!m_window->createSprite("FireExplosionLeft", "Data\\Explosions\\FireExplosionLeft.png", 3)) {
		return;
	}
	if (!m_window->createSprite("FireExplosionDown", "Data\\Explosions\\FireExplosionDown.png", 3)) {
		return;
	}
	if (!m_window->createSprite("FireExplosionUp", "Data\\Explosions\\FireExplosionUp.png", 3)) {
		return;
	}
	if (!m_window->createSprite("MagicExplosionRight", "Data\\Explosions\\MagicExplosionRight.png", 3)) {
		return;
	}
	if (!m_window->createSprite("MagicExplosionLeft", "Data\\Explosions\\MagicExplosionLeft.png", 3)) {
		return;
	}
	if (!m_window->createSprite("MagicExplosionDown", "Data\\Explosions\\MagicExplosionDown.png", 3)) {
		return;
	}
	if (!m_window->createSprite("MagicExplosionUp", "Data\\Explosions\\MagicExplosionUp.png", 3)) {
		return;
	}
	if (!m_window->createSprite("EnemyExplosion", "Data\\Explosions\\EnemyExplosion.png", 15)) {
		return;
	}

	//load ground sprites
	if (!m_window->createSprite("MoveUI", "Data\\UI\\MoveUI.png", 0)) {
		return;
	}
	if (!m_window->createSprite("ShootUI", "Data\\UI\\ShootUI.png", 0)) {
		return;
	}

	//load sounds
	if (!HAPI.LoadSound("Data\\Sounds\\Fireball.wav")) {
		return;
	}
	if (!HAPI.LoadSound("Data\\Sounds\\Explosion.wav")) {
		return;
	}
	if (!HAPI.LoadSound("Data\\Sounds\\Door.ogg")) {
		return;
	}
	if (!HAPI.LoadSound("Data\\Sounds\\Hit.wav")) {
		return;
	}
	if (!HAPI.LoadSound("Data\\Sounds\\MenuStart.ogg")) {
		return;
	}
	if (!HAPI.LoadSound("Data\\Sounds\\BackgroundMusic.wav")) {
		return;
	}
	if (!HAPI.LoadSound("Data\\Sounds\\WinSound.ogg")) {
		return;
	}
	if (!HAPI.LoadSound("Data\\Sounds\\LoseSound.ogg")) {
		return;
	}

	//load fonts
	if (!HAPI.ChangeFontFromFile("Data\\Fonts\\upheavtt.ttf")) {
		return;
	}

	//load menu sprites
	if (!m_window->createSprite("MainMenu", "Data\\UI\\Main Menu.png", 0)) {
		return;
	}
	if (!m_window->createSprite("WinBackground", "Data\\UI\\WinBackground.png", 0)) {
		return;
	}
	if (!m_window->createSprite("LoseBackground", "Data\\UI\\LoseBackground.png", 0)) {
		return;
	}
}

void World::run() {

	DWORD prevTime = 0;
	DWORD currentTime = 0;
	DWORD timerPrevTime = 0;
	DWORD timerCurrentTime = 0;

	srand(time(NULL));


	int frameRate = 0;
	float deltaTime = 0;

	//play background music
	HAPI.PlaySound("Data\\Sounds\\BackgroundMusic.wav", HAPI_TSoundOptions(0.2f, true));

	while (HAPI.Update()) {

		currentTime = HAPI.GetTime();
		timerCurrentTime = HAPI.GetTime();

		m_keyboardData = HAPI.GetKeyboardData();
		m_controllerData = HAPI.GetControllerData(0);

		if (m_displayMainMenu) {
			//if at the start of game, display main menu
			m_window->renderSprite("MainMenu", 0, 0, false, 0);
			if (m_keyboardData.scanCode[HK_SPACE] || m_controllerData.digitalButtons[HK_DIGITAL_A]) {
				m_displayMainMenu = false;
				HAPI.PlaySound("Data\\Sounds\\MenuStart.ogg", HAPI_TSoundOptions(0.3, false, 0, 2));
			}
		}
		else if (m_win) {
			//if game is won, diaplay win screen
			m_window->renderSprite("WinBackground", 0, 145, false, 0);

			//Make End Score Hud
			for (Entity* entity : m_allEntities) {
				if (entity->getEntityType() == EEntityType::ePlayer) {

					HAPI.RenderText(30, 0, HAPI_TColour::WHITE, "Base Score = " + std::to_string(m_score), 50);
					HAPI.RenderText(30, 40, HAPI_TColour::WHITE, "Time bonus = -" + std::to_string(m_time), 50);
					HAPI.RenderText(30, 80, HAPI_TColour::WHITE, "Health bonus = x" + std::to_string(entity->getHealth()), 50);

					int totalScore = (m_score - m_time) * entity->getHealth();
					HAPI.RenderText(650, 20, HAPI_TColour::WHITE, "Total Score\n\t  " + std::to_string(totalScore), 50);
				}
			}
			//reset game to play again
			if (m_keyboardData.scanCode[HK_SPACE] || m_controllerData.digitalButtons[HK_DIGITAL_A]) {
				resetGame();
				HAPI.PlaySound("Data\\Sounds\\MenuStart.ogg", HAPI_TSoundOptions(0.3, false, 0, 2));
			}
			
		}
		else if (m_lose) {
			//if game is lost, diaplay lose screen
			//m_window->renderSprite("LoseBackground", 0, 145, false, 0);

			////Make End Score Hud
			//for (Entity* entity : m_allEntities) {
			//	if (entity->getEntityType() == EEntityType::ePlayer) {

			//		HAPI.RenderText(30, 0, HAPI_TColour::WHITE, "Base Score = " + std::to_string(m_score), 50);
			//		HAPI.RenderText(30, 40, HAPI_TColour::WHITE, "Time bonus = -" + std::to_string(m_time), 50);
			//		HAPI.RenderText(30, 80, HAPI_TColour::WHITE, "Health bonus = x" + std::to_string(entity->getHealth()), 50);

			//		int totalScore = (m_score - m_time) * entity->getHealth();
			//		HAPI.RenderText(650, 20, HAPI_TColour::WHITE, "Total Score\n\t  " + std::to_string(totalScore), 50);
			//	}
			//}
			////reset game to play again
			//if (m_keyboardData.scanCode[HK_SPACE] || m_controllerData.digitalButtons[HK_DIGITAL_A]) {
			//	resetGame();
			//	HAPI.PlaySound("Data\\Sounds\\MenuStart.ogg", HAPI_TSoundOptions(0.3, false, 0, 2));
			//}

			resetGame();
		}
		else {

			deltaTime = currentTime - prevTime;

			if (deltaTime >= m_tickSpeed) {
				//only update on game tick

				//invincible mode
				if (m_keyboardData.scanCode['G']) {
					m_invincible = true;
				}
				else if (m_keyboardData.scanCode['H']) {
					m_invincible = false;
				}

				//check if player is changing rooms
				if (m_leftDoorEntered) {
					for (int entity = 0; entity < m_allEntities.size(); entity++) {
						//move backgrounds
						if (m_allEntities[entity]->getEntityType() == EEntityType::eBackground && !m_allEntities[entity]->getAlive()) {
							m_allEntities[entity]->setPosition(-1024, 145);
							m_allEntities[entity]->setAlive(true);
						}
						//make the player invisible
						else if (m_allEntities[entity]->getEntityType() == EEntityType::ePlayer) {
							m_allEntities[entity]->setVisible(false);
							m_allEntities[entity]->setPosition(830, m_allEntities[entity]->getPosY());
						}
						//make the doors invisible
						else if (m_allEntities[entity]->getEntityType() == EEntityType::eDoor) {
							m_allEntities[entity]->setAlive(false);
						}
						//kill any left over explosion sprites
						else if (m_allEntities[entity]->getEntityType() == EEntityType::eExplosion) {
							m_allEntities[entity]->setAlive(false);
						}
						//kill any left over bullet sprites
						else if (m_allEntities[entity]->getEntityType() == EEntityType::eBullet) {
							m_allEntities[entity]->setAlive(false);
						}
					}
					m_allEntities[0]->setFinishedMoving(false);
					m_allEntities[1]->setFinishedMoving(false);
					m_displayGroundUI = false;

				}
				else if (m_rightDoorEntered) {
					for (int entity = 0; entity < m_allEntities.size(); entity++) {
						//move backgrounds
						if (m_allEntities[entity]->getEntityType() == EEntityType::eBackground && !m_allEntities[entity]->getAlive()) {
							m_allEntities[entity]->setPosition(1024, 145);
							m_allEntities[entity]->setAlive(true);
						}
						//make the player invisible
						else if (m_allEntities[entity]->getEntityType() == EEntityType::ePlayer) {
							m_allEntities[entity]->setVisible(false);
							m_allEntities[entity]->setPosition(120, m_allEntities[entity]->getPosY());
						}
						//make the doors invisible
						else if (m_allEntities[entity]->getEntityType() == EEntityType::eDoor) {
							m_allEntities[entity]->setAlive(false);
						}
						//kill any left over explosion sprites
						else if (m_allEntities[entity]->getEntityType() == EEntityType::eExplosion) {
							m_allEntities[entity]->setAlive(false);
						}
						//kill any left over bullet sprites
						else if (m_allEntities[entity]->getEntityType() == EEntityType::eBullet) {
							m_allEntities[entity]->setAlive(false);
						}
					}
					m_allEntities[0]->setFinishedMoving(false);
					m_allEntities[1]->setFinishedMoving(false);
					m_displayGroundUI = false;
				}


				//update
				for (Entity* entity : m_allEntities) {
					entity->update(this);
				}

				//check if backgrounds need to stop moving
				if (m_allEntities[0]->getFinishedMoving() && m_allEntities[1]->getFinishedMoving()) {
					//stop moving rooms
					m_leftDoorEntered = false;
					m_rightDoorEntered = false;
					m_allEntities[0]->setFinishedMoving(false);
					m_allEntities[1]->setFinishedMoving(false);

					//make the player visible again
					for (int entity = 0; entity < m_allEntities.size(); entity++) {
						if (m_allEntities[entity]->getEntityType() == EEntityType::ePlayer) {
							m_allEntities[entity]->setVisible(true);
						}
						//make the doors visible again
						else if (m_allEntities[entity]->getEntityType() == EEntityType::eDoor) {
							m_allEntities[entity]->setAlive(true);
						}
					}

					//=======================================================================
					//Load next room

					if (m_roomDirection == ERoomDirection::eLeft) {
						m_currentRoomIndex--;
					}
					else if (m_roomDirection == ERoomDirection::eRight) {
						m_currentRoomIndex++;
					}

					//check if room is visited
					bool visited = false;
					for (auto& index : m_visitedIndexes) {
						if (index == m_currentRoomIndex) {
							visited = true;
						}
					}

					//load enemies, only if not visited
					if (!visited) {
						switch (m_rooms[m_currentRoomIndex]) {
						case 0:
							loadRoom(0);
							break;
						case 1:
							loadRoom(1);
							m_visitedIndexes.push_back(m_currentRoomIndex);
							HAPI.PlaySound("Data\\Sounds\\Door.ogg", HAPI_TSoundOptions(0.5));
							break;
						case 2:
							loadRoom(2);
							m_visitedIndexes.push_back(m_currentRoomIndex);
							HAPI.PlaySound("Data\\Sounds\\Door.ogg", HAPI_TSoundOptions(0.5));
							break;
						case 3:
							loadRoom(3);
							m_visitedIndexes.push_back(m_currentRoomIndex);
							HAPI.PlaySound("Data\\Sounds\\Door.ogg", HAPI_TSoundOptions(0.5));
							break;
						default:
							break;
						}
					}
				}


				//============================================================================================================================================================
				//check collisions

				//check doors against player
				for (auto& entity : m_allEntities) {
					if (entity->getEntityType() == EEntityType::eDoor && entity->getAlive()) {
						for (auto& player : m_allEntities) {
							if (player->getEntityType() == EEntityType::ePlayer && player->getAlive() && player->getVisible()) {
								entity->checkCollisions(player->getHitboxes(), player->getSide(), player->getEntityType());
							}
						}
					}
				}

				//check player, Wander, Chase and Bullets against Background and doors
				for (auto& entity : m_allEntities) {
					if (entity->getEntityType() != EEntityType::eBackground &&
						entity->getEntityType() != EEntityType::eWallEnemy &&
						entity->getEntityType() != EEntityType::eExplosion &&
						entity->getEntityType() != EEntityType::eDoor &&
						entity->getAlive()) {
						for (auto& entity2 : m_allEntities) {
							if ((entity2->getEntityType() == EEntityType::eBackground ||
								entity2->getEntityType() == EEntityType::eDoor) &&
								entity2->getAlive()) {
								entity->checkCollisions(entity2->getHitboxes(), entity2->getSide(), entity2->getEntityType());
							}

						}
					}
				}

				//check player, wallShooter, Wander and Chase against bullets
				for (auto& entity : m_allEntities) {
					if ((entity->getEntityType() != EEntityType::eBackground &&
						entity->getEntityType() != EEntityType::eExplosion &&
						entity->getEntityType() != EEntityType::eBullet) &&
						entity->getAlive()) {
						for (auto& bullet : m_allEntities) {
							if (bullet->getEntityType() == EEntityType::eBullet && bullet->getAlive()) {
								entity->checkCollisions(bullet->getHitboxes(), bullet->getSide(), EEntityType::eBullet);
							}
						}
					}
				}

				//check bullets against player, wallShooter, chase, wander and bullets
				for (auto& bullet : m_allEntities) {
					if (bullet->getEntityType() == EEntityType::eBullet && bullet->getAlive()) {
						for (auto& entity : m_allEntities) {
							if (entity->getEntityType() != EEntityType::eBackground &&
								entity->getEntityType() != EEntityType::eExplosion &&
								entity->getAlive()) {
								bullet->checkCollisions(entity->getHitboxes(), entity->getSide(), entity->getEntityType());
							}
						}
					}
				}

				//check player against Wanderer, Chase and WallShooter
				for (auto& entity : m_allEntities) {
					if ((entity->getEntityType() == EEntityType::ePlayer) && entity->getAlive()) {
						for (auto& enemy : m_allEntities) {
							if ((enemy->getEntityType() == EEntityType::eChaseEnemy ||
								enemy->getEntityType() == EEntityType::eWallEnemy ||
								enemy->getEntityType() == EEntityType::eWanderEnemy) &&
								enemy->getAlive()) {
								entity->checkCollisions(enemy->getHitboxes(), enemy->getSide(), enemy->getEntityType());
							}
						}
					}
				}


				//============================================================================================================================================================
				//check if room cleared (if any enemies are alive)
				int enemiesAlive = 0;
				for (int entity = 0; entity < m_allEntities.size(); entity++) {
					if ((m_allEntities[entity]->getEntityType() == EEntityType::eChaseEnemy ||
						m_allEntities[entity]->getEntityType() == EEntityType::eWallEnemy ||
						m_allEntities[entity]->getEntityType() == EEntityType::eWanderEnemy) &&
						m_allEntities[entity]->getAlive()) {
						enemiesAlive++;
					}
				}

				if (enemiesAlive > 0) {
					m_roomEmpty = false;
				}
				else {
					m_roomEmpty = true;
					//if all rooms visited, and no enemies in this room - Game has been won
					if (m_visitedIndexes.size() == m_rooms.size() - 1) {
						m_win = true;
						HAPI.PlaySound("Data\\Sounds\\WinSound.ogg");
					}
				}

				deltaTime = 0;

				prevTime = currentTime;

			}

			//============================================================================================================================================================
			//Rendering
			//render every frame and interpolate movement

			//clear screen
			m_window->clearScreen();

			//render all of the sprites
			for (Entity* entity : m_allEntities) {
				//reset frame loop
				if (entity->getCurrentFrame() > (m_window->getTextureMaxFrames(entity->getCurrentSprite()) - 1)) {
					entity->setCurrentFrame(0);
				}

				//render all entities
				if (entity->getEntityType() == EEntityType::eBackground && entity->getAlive()) {
					entity->render(m_window, deltaTime / m_tickSpeed);
					if (m_displayGroundUI) {
						m_window->renderSprite("MoveUI", 250, 350, true, 0);
						m_window->renderSprite("ShootUI", 600, 350, true, 0);
					}
				}
				else {
					entity->render(m_window, deltaTime / m_tickSpeed);
				}

				//Make HUD
				if (!m_win && !m_lose) {
					if (entity->getEntityType() == EEntityType::ePlayer) {
						HAPI.RenderText(720, 15, HAPI_TColour(255, 120, 120), "Health\n\t" + std::to_string(entity->getHealth()), 50);
					}

					HAPI.RenderText(400, 15, HAPI_TColour::WHITE, "Score\n" + std::to_string(m_score), 50);

					if (timerCurrentTime - timerPrevTime >= 1000) {
						m_time++;
						timerPrevTime = timerCurrentTime;
					}

					HAPI.RenderText(100, 15, HAPI_TColour::WHITE, "Time\n  " + std::to_string(m_time), 50);

					if (m_invincible) {
						HAPI.RenderText(800, 710, HAPI_TColour::WHITE, "God Mode", 40);
					}

				}

				//set current frames
				if (entity->getAlive()) {
					if (frameRate >= 30 && entity->getMoving()) {
						entity->setCurrentFrame(entity->getCurrentFrame() + 1);
						frameRate = 0;
					}
					else if (!entity->getMoving()) {
						entity->setCurrentFrame(0);
					}
					else {
						frameRate++;
					}
				}
			}
		}
	}
}


void World::loadRoom(int roomID) {

	//load different enemies into rooms
	for (int i = 0; i < roomID; i++) {
		switch (rand() % 4) {
		case 0:
			spawnChaseEnemy((rand() % 300) + 300, (rand() % 350) + 250, EDirection::eRight);
			break;
		case 1:
			spawnWanderEnemy((rand() % 300) + 300, (rand() % 350) + 250, EDirection::eRight);
			break;
		case 2:
			spawnWallEnemy((rand() % 730) + 120, 180, EDirection::eRight);
			break;
		case 3:
			spawnWallEnemy((rand() % 730) + 120, 685, EDirection::eLeft);
			break;
		default:
			break;
		}
	}

	if (roomID == 0) {
		m_displayGroundUI = true;
	}

}


void World::fireBullet(int posX, int posY, EDirection currentDirection, ESide side) {
	//look for a not alive bullet
	for (auto& bullet : m_allEntities) {
		if (!bullet->getAlive() && bullet->getEntityType() == EEntityType::eBullet) {
			//spawn bullet
			bullet->setPosition(posX, posY);
			bullet->setDirection(currentDirection);
			bullet->setSide(side);
			bullet->setAlive(true);

			HAPI.PlaySound("Data\\Sounds\\Fireball.wav", HAPI_TSoundOptions(0.3));

			return;
		}
	}
}

void World::makeExplosion(int posX, int posY, EDirection currentDirection, ESide side, EExplosionType type) {
	//look for a not alive explosion
	for (auto& explosion : m_allEntities) {
		if (!explosion->getAlive() && explosion->getEntityType() == EEntityType::eExplosion) {
			//spawn explosion
			explosion->setPosition(posX, posY);
			explosion->setDirection(currentDirection);
			explosion->setSide(side);
			explosion->setExplosionType(type);
			explosion->setAlive(true);
			
			if (type == EExplosionType::eEnemy) {
				HAPI.PlaySound("Data\\Sounds\\Explosion.wav", HAPI_TSoundOptions(0.3));
			}

			return;
		}
	}
}

void World::spawnChaseEnemy(int posX, int posY, EDirection currentDirection) {
	//look for dead enemy
	for (auto& entity : m_allEntities) {
		if (entity->getEntityType() == EEntityType::eChaseEnemy && !entity->getAlive()) {
			//spawn chase enemy
			entity->resetHealth(rand() % 3 + 1);
			entity->setAllPositions(posX,posY);
			entity->setDirection(currentDirection);
			entity->setAlive(true);
			return;
		}
	}
}

void World::spawnWallEnemy(int posX, int posY, EDirection currentDirection) {
	//look for dead enemy
	for (auto& entity : m_allEntities) {
		if (entity->getEntityType() == EEntityType::eWallEnemy && !entity->getAlive()) {
			//spawn wall enemy
			entity->resetHealth(rand() % 3 + 1);
			entity->setAllPositions(posX, posY);
			entity->setDirection(currentDirection);
			entity->setAlive(true);
			return;
		}
	}
}

void World::spawnWanderEnemy(int posX, int posY, EDirection currentDirection) {
	//look for dead enemy
	for (auto& entity : m_allEntities) {
		if (entity->getEntityType() == EEntityType::eWanderEnemy && !entity->getAlive()) {
			//spawn wander enemy
			entity->resetHealth(rand() % 3 + 1);
			entity->setAllPositions(posX, posY);
			entity->setDirection(currentDirection);
			entity->setAlive(true);
			return;
		}
	}
}

void World::resetGame() {
	//reset all aspects of game to starting values - resets the game
	m_roomEmpty = true;
	m_leftDoorEntered = false;
	m_rightDoorEntered = false;
	m_roomDirection = ERoomDirection::eNeutral;
	m_displayGroundUI = true;
	m_score = 0;
	m_time = 0;
	m_win = false;
	m_lose = false;

	//m_displayMainMenu is not reset, so main menu is not displayed again

	//initialise rooms
	//numbers in vector represent types of room
	m_rooms = std::vector<int>{ 3, 2, 1, 0, 1, 2, 3 };
	m_currentRoomIndex = (m_rooms.size() - 1) / 2;

	m_visitedIndexes.clear();

	for (auto& entity : m_allEntities) {
		//reset player position & health
		if (entity->getEntityType() == EEntityType::ePlayer) {
			entity->setAlive(true);
			entity->resetHealth(10);
			entity->setVisible(true);
			entity->setAllPositions(470, 390);
		}
		else if (entity->getSide() == ESide::eEnemy) {
			//make sure all enemies are dead
			entity->setAlive(false);
		}
	}
}