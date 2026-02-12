#pragma once

#include "Entity.h"
#include "EntityManager.h"

#include<SFML\Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "imguiThemes.h"

struct WindowConfig { int WW, WH, FL, FS; };
struct FontConfig	{ int S, R, G, B; std::string F; };
struct PlayerConfig	{ int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig	{ int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };

class Game
{
	sf::RenderWindow	m_window;			
	EntityManager		m_entities;			//Vector of entities to maintain
	sf::Font			m_font;				//the font which will be use to draw
	sf::Text			m_text;				//the score text to be drawn to the screen
	sf::Clock			m_deltaClock;		//for imgui 
	WindowConfig		m_windowConfig;
	FontConfig			m_fontConfig;
	PlayerConfig		m_playerConfig;
	EnemyConfig			m_enemyConfig;
	BulletConfig		m_bulletConfig;
	int					m_score = 0;
	int					m_currentFrame = 0;
	int					m_lastEnemySpawnTime = 0;
	bool				m_paused = false;	//whether we update game logic
	bool				m_running = false;  //if game is running

	//std::shared_ptr<Entity> m_player;

	void init(const std::string& config);	//intializing game state with a config file path
	void setPaused(bool paused);			//pause the game

	std::shared_ptr<Entity> player();

	void sMovement();						//System : Entity position / movement update
	void sUserInput();						//System : User Input
	void sLifespan();						//System : Lifespan
	void sRender();							//System : Render / Drawing
	void sEnemySpawner();					//System : Spawns Enemies
	void sCollision();						//System : Collisions

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnBullet(std::shared_ptr<Entity> entity , const Vec2& mousePos);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);
	
public:
	
	Game(const std::string& config); //constructor takes in game config

	void run();
};