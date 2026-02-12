#include "Game.h"
#include<fstream>
#include<iostream>




Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{
	//TODO : read in config file here
	// use the premade playerconfig, enemyconfig, bulletconfig variables

	std::ifstream fin(path);
	if (fin.is_open())
	{
		std::string input;
		while (fin >> input)
		{
			if (input == "Window" || input == "window")
			{
				fin >> m_windowConfig.WH >> m_windowConfig.WW >> m_windowConfig.FL >> m_windowConfig.FS;

			}

			else if (input == "font" || input == "Font")
			{
				fin >> m_fontConfig.F >> m_fontConfig.S >> m_fontConfig.R >> m_fontConfig.G >> m_fontConfig.B;
				if (!m_font.loadFromFile(RESOURCES_PATH + m_fontConfig.F))
				{
					//some error handling
					std::cout << "failed to load tech font from file";
					exit(-1);
				}
			}

			else if (input == "Player" || input == "player")
			{
				fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR >> m_playerConfig.FG >>m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V;
			}

			else if (input == "Enemy" || input == "enemy")
			{
				fin >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI;
			}

			else if (input == "Bullet" || input == "bullet")
			{
				fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
			}


			else
			{
				std::cout << "invalid input :" << input << std::endl;
			}
		}

		//set up default window parameters
		if (m_windowConfig.FS == 1)
		{
			m_window.create(sf::VideoMode(m_windowConfig.WH, m_windowConfig.WW), "Poly Wars", sf::Style::Fullscreen);
		}
		else
		{
			m_window.create(sf::VideoMode(m_windowConfig.WH, m_windowConfig.WW), "Poly Wars");
		}

		m_window.setFramerateLimit(m_windowConfig.FL);
		m_window.setKeyRepeatEnabled(false);

		//ImGUI setup

		if (!ImGui::SFML::Init(m_window)) {};
		//you can use whatever imgui theme you like!
		//ImGui::StyleColorsDark();				
		//imguiThemes::yellow();
		//imguiThemes::gray();
		imguiThemes::green();
		//imguiThemes::red();
		//imguiThemes::gray();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.FontGlobalScale = 2.f;
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_WindowBg].w = 0.5f;
		//style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;



		m_text.setFont(m_font);
		m_text.setCharacterSize(m_fontConfig.S);
		m_text.setFillColor(sf::Color(m_fontConfig.R, m_fontConfig.G, m_fontConfig.B));
		m_text.setPosition(0, 0);

		spawnPlayer();
	}
	else
	{
		std::cout << "unable to open file" <<std::endl;
	}
}

void Game::run()
{
	//TODO : add pause functionality in here
	//	some systems function while pause (rendering)
	//	some systems shouldn't (movement/input)
	m_running = true;
	srand(time(NULL));
	while (m_running)
	{
		m_entities.update();

		//calculate the delta time
		sf::Time deltaTime = m_deltaClock.restart();
		float deltaTimeSeconds = deltaTime.asSeconds();

		//make sure delta time stays within normal bounds, like between one FPS and zero FPS
		deltaTimeSeconds = std::min(deltaTimeSeconds, 1.f);
		deltaTimeSeconds = std::max(deltaTimeSeconds, 0.f);

		ImGui::SFML::Update(m_window, deltaTime);

		if (!m_paused)  //showed as an example for m_paused may or may not be final implementation
		{
			sEnemySpawner();
			sLifespan();
			sCollision();
			sMovement();
		}

		sUserInput();
		//sGUI();
		sRender();
		m_currentFrame++;
		//increment the current frame
		//may need to be moved when pause implemented
	}
}

void Game::setPaused(bool paused)
{
	m_paused = !paused;
}

std::shared_ptr<Entity> Game::player()
{
	auto& players = m_entities.getEntities("Player");
	return players.front();
}

void Game::sMovement()
{
	//TODO : implement all entity movement in this function
	// you should read the m_player->cInput component to determine if the player is moving
	//m_player->cTransform->velocity = Vec2(0, 0);
	player()->get<CTransform>().velocity = Vec2(0, 0);

	//if (m_player->cInput->up)
	//{
	//	m_player->cTransform->velocity.y = -5.0f;
	//}
	//if (m_player->cInput->left)
	//{
	//	m_player->cTransform->velocity.x = -5.0f;
	//}
	//if (m_player->cInput->down)
	//{
	//	m_player->cTransform->velocity.y = 5.0f;
	//}
	//if (m_player->cInput->right)
	//{
	//	m_player->cTransform->velocity.x = 5.0f;
	//}
	
	if (player()->get<CInput>().up)
	{
		player()->get<CTransform>().velocity.y = -5.0f;
	}
	if (player()->get<CInput>().left)
	{
		player()->get<CTransform>().velocity.x = -5.0f;
	}
	if (player()->get<CInput>().down)
	{
		player()->get<CTransform>().velocity.y = 5.0f;
	}
	if (player()->get<CInput>().right)
	{
		player()->get<CTransform>().velocity.x = 5.0f;
	}

	//Sample movement speed update
	/*m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
	m_player->cTransform->pos.y += m_player->cTransform->velocity.y;

	for (auto bullets : m_entities.getEntities("Bullet"))
	{
		bullets->cTransform->pos += bullets->cTransform->velocity;
	}*/
	

	//std::cout << m_player->cShape->circle.getPosition().x << "," << m_player->cShape->circle.getPosition().y<<std::endl;

	//m_player->cShape->circle.ge



	//for (auto e : m_entities.getEntities())
	//{
	//	if ((m_player->cShape->circle.getGlobalBounds().top < 0 && m_player->cTransform->velocity.y < 0) || (m_player->cShape->circle.getGlobalBounds().top + m_player->cShape->circle.getGlobalBounds().height >= 720 && m_player->cTransform->velocity.y > 0))
	//	{
	//		m_player->cTransform->velocity.y = 0;
	//	}

	//	if ((m_player->cShape->circle.getGlobalBounds().left < 0 && m_player->cTransform->velocity.x < 0) || (m_player->cShape->circle.getGlobalBounds().left + m_player->cShape->circle.getGlobalBounds().width >= 1280 && m_player->cTransform->velocity.x > 0))
	//	{
	//		m_player->cTransform->velocity.x = 0;
	//	}
	//	
	//	e->cTransform->pos += e->cTransform->velocity;
	//}
	for (auto& e : m_entities.getEntities())
	{
		if ((player()->get<CShape>().circle.getGlobalBounds().top < 0 && player()->get<CTransform>().velocity.y < 0) || (player()->get<CShape>().circle.getGlobalBounds().top + player()->get<CShape>().circle.getGlobalBounds().height >= 720 && player()->get<CTransform>().velocity.y > 0))
		{
			player()->get<CTransform>().velocity.y = 0;
		}

		if ((player()->get<CShape>().circle.getGlobalBounds().left < 0 && player()->get<CTransform>().velocity.x < 0) || (player()->get<CShape>().circle.getGlobalBounds().left + player()->get<CShape>().circle.getGlobalBounds().width >= 1280 && player()->get<CTransform>().velocity.x > 0))
		{
			player()->get<CTransform>().velocity.x = 0;
		}
		
		e->get<CTransform>().pos += e->get<CTransform>().velocity;
	}
}

void Game::sUserInput()
{
	//TODO : handle user input here

	//note that you should only be setting the player's input component variables here
	// you should not implement the player's movement logic here
	// the movement system will read the variables you set in this function

	sf::Event event;
	while (m_window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(m_window, event);
		
		//this event triggers when the window is closed
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
			m_window.close();
		}
		else if (event.type == sf::Event::Resized)
		{
			// Adjust the viewport when the window is resized
			sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			m_window.setView(sf::View(visibleArea));
		}

		//this event is triggered when a key is pressed 
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W key pressed\n";
				//m_player->cInput->up = true;
				player()->get<CInput>().up = true;
				break;
			case sf::Keyboard::A:
				std::cout << "A key pressed\n";
				//m_player->cInput->left = true;
				player()->get<CInput>().left = true;
				break;
			case sf::Keyboard::S:
				std::cout << "S key pressed\n";
				//m_player->cInput->down = true;
				player()->get<CInput>().down = true;
				break;
			case sf::Keyboard::D:
				std::cout << "D key pressed\n";
				//m_player->cInput->right = true;
				player()->get<CInput>().right = true;
				break;
			case sf::Keyboard::P:
				std::cout << "P key pressed\n";
				setPaused(m_paused);
				break;
			case sf::Keyboard::Escape:
				std::cout << "Esc key pressed\n";
				m_running = false;
				break;
			default: break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W key released\n";
				//m_player->cInput->up = false;
				player()->get<CInput>().up = false;
				break;
			case sf::Keyboard::A:
				std::cout << "A key released\n";
				//m_player->cInput->left = false;
				player()->get<CInput>().left = false;
				break;
			case sf::Keyboard::S:
				std::cout << "S key released\n";
				//m_player->cInput->down = false;
				player()->get<CInput>().down = false;
				break;
			case sf::Keyboard::D:
				std::cout << "D key released\n";
				//m_player->cInput->right = false;
				player()->get<CInput>().right = false;
				break;
			default: break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed && !m_paused)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::cout << "Left mouse button clicked at (" << event.mouseButton.x << "," << event.mouseButton.y << ")\n";
				//spawnBullet(m_player,Vec2(event.mouseButton.x,event.mouseButton.y));
				spawnBullet(player(), Vec2(event.mouseButton.x, event.mouseButton.y));
			}

			if (event.mouseButton.button == sf::Mouse::Right)
			{
				std::cout << "Right mouse button clicked at (" << event.mouseButton.x << "," << event.mouseButton.y << ")\n";
				//call special weapon here
			}
		}
	}
}

void Game::sLifespan()
{
	//TODO: implement all lifespan functionality
	for (auto e : m_entities.getEntities())
	{
		//if (!e->cLifespan) { continue; }
		//e->cLifespan->remaining--;
		//if (e->cLifespan->remaining > 0 && e->isActive())
		//{
		//	auto ogFillColor = e->cShape->circle.getFillColor();
		//	auto ogBorderColor = e->cShape->circle.getOutlineColor();
		//	auto newFillColor = sf::Color(ogFillColor.r, ogFillColor.g, ogFillColor.b,(((float)e->cLifespan->remaining / e->cLifespan->total) * 255));
		//	auto newBorderColor = sf::Color(ogBorderColor.r, ogBorderColor.g, ogBorderColor.b, (((float)e->cLifespan->remaining / e->cLifespan->total) * 255));
		//	//std::cout << "rt = "<< newColor.a<< std::endl;
		//	e->cShape->circle.setFillColor(newFillColor);
		//	e->cShape->circle.setOutlineColor(newBorderColor);
		//}
		//if (e->cLifespan->remaining <= 15  && e->isActive())
		//{
		//	e->destroy();
		//}
		if (!e->get<CLifespan>().exists) { continue; }
		e->get<CLifespan>().remaining--;
		if (e->get<CLifespan>().remaining > 0 && e->isActive())
		{
			auto ogFillColor = e->get<CShape>().circle.getFillColor();
			auto ogBorderColor = e->get<CShape>().circle.getOutlineColor();
			auto newFillColor = sf::Color(ogFillColor.r, ogFillColor.g, ogFillColor.b,(((float)e->get<CLifespan>().remaining / e->get<CLifespan>().total) * 255));
			auto newBorderColor = sf::Color(ogBorderColor.r, ogBorderColor.g, ogBorderColor.b, (((float)e->get<CLifespan>().remaining / e->get<CLifespan>().total) * 255));
			//std::cout << "rt = "<< newColor.a<< std::endl;
			e->get<CShape>().circle.setFillColor(newFillColor);
			e->get<CShape>().circle.setOutlineColor(newBorderColor);
		}
		if (e->get<CLifespan>().remaining <= 15  && e->isActive())
		{
			e->destroy();
		}
		
	}

	//if entity has > 0 remaining lifespan , subtract 1
	// if it has lifespan and is alive
	//		scale its alpha channel properly
	// if it has lifespan and its time is up
	//		destroy the entity
}

void Game::sRender()
{
	//TODO : changfe the code below to draw all of the entities
	//	sample drawing of the player entity that we have created
	m_window.clear();

	//set the position of the shape based on the entity's transform->pos
	//m_player->cShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);

	//set the rotation of the shaped based on the entity's transform->angle
	//m_player->cTransform->angle += 1.0f;
	//m_player->cShape->circle.setRotation(m_player->cTransform->angle);

	//draw the entity's sf::circleShape
	//m_window.draw(m_player->cShape->circle);

	//what will be actually implemented , mentioned in the video 
	/*for (auto e : m_entities.getEntities())
	{
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
		e->cTransform->angle += 1.0f * e->cShape->circle.getPointCount();
		e->cShape->circle.setRotation(e->cTransform->angle);
		
		m_window.draw(e->cShape->circle);
	}*/
	for (auto e : m_entities.getEntities())
	{
		e->get<CShape>().circle.setPosition(e->get<CTransform>().pos.x, e->get<CTransform>().pos.y);
		e->get<CTransform>().angle += 1.0f * e->get<CShape>().circle.getPointCount();
		e->get<CShape>().circle.setRotation(e->get<CTransform>().angle);
		
		m_window.draw(e->get<CShape>().circle);
	}


	std::string score = "Score : " + std::to_string(m_score); 
	std::cout << score << std::endl;
	m_text.setString(score);
	m_window.draw(m_text);
	

	ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
	ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	ImGui::PopStyleColor(2);



	ImGui::Begin("Debug menu");
	ImGui::SeparatorText("Performance");

	// %.1f means "floating point with 1 decimal place"
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Enemies spawned : %d ", m_entities.getEntities("Enemy").size());
	ImGui::End();
	ImGui::End();


	ImGui::SFML::Render(m_window);



	m_window.display();
}

void Game::sEnemySpawner()
{
	//TODO : code which implements enemy spawning should go here
	if (m_currentFrame - m_lastEnemySpawnTime > m_enemyConfig.SI)
	{
		spawnEnemy();
	}
	//		use(m_currentFrame - m_lastEnemySpawnTime) to determine
	//		how long it has been since the last enemy spawned
	
}

void Game::sCollision()
{

	
	//TODO: implement all proper collisiion between entities
	// be sure to use the collisioin radius , not the shape radius
	/*for (auto e : m_entities.getEntities())
	{
		if (e->tag() == "Enemy" || e->tag() == "SmallEnemy")
		{
			for (auto b : m_entities.getEntities("Bullet"))
			{
				float diff = e->cTransform->pos.dist(b->cTransform->pos);
				if (diff < ((b->cCollision->radius + e->cCollision->radius) * (b->cCollision->radius + e->cCollision->radius)) && e->isActive())
				{
					e->destroy();
					b->destroy();
					m_score += e->cScore->score;
					if (e->tag() == "Enemy")
					{
						spawnSmallEnemies(e);
					}

				}
			}

			if (m_player->isActive() && e-> isActive())
			{
				float diff = e->cTransform->pos.dist(m_player->cTransform->pos);
				if (diff < ((m_player->cCollision->radius + e->cCollision->radius) * (m_player->cCollision->radius + e->cCollision->radius)))
				{
					m_player->destroy();
					e->destroy();
					spawnPlayer();
				}
			}
		}
		

		if (e->tag() == "Enemy")
		{
			if ((e->cShape->circle.getGlobalBounds().top < 0 && e->cTransform->velocity.y < 0) || (e->cShape->circle.getGlobalBounds().top + e->cShape->circle.getGlobalBounds().height >= m_windowConfig.WW && e->cTransform->velocity.y > 0))
			{
				e->cTransform->velocity.y = -(e->cTransform->velocity.y);
			}

			if ((e->cShape->circle.getGlobalBounds().left < 0 && e->cTransform->velocity.x < 0) || (e->cShape->circle.getGlobalBounds().left + e->cShape->circle.getGlobalBounds().width >= m_windowConfig.WH && e->cTransform->velocity.x > 0))
			{
				e->cTransform->velocity.x = -(e->cTransform->velocity.x);
			}
			}
		}*/
	for (auto e : m_entities.getEntities())
	{
		if (e->tag() == "Enemy" || e->tag() == "SmallEnemy")
		{
			for (auto b : m_entities.getEntities("Bullet"))
			{
				float diff = e->get<CTransform>().pos.dist(b->get<CTransform>().pos);
				if (diff < ((b->get<CCollision>().radius + e->get<CCollision>().radius) * (b->get<CCollision>().radius + e->get<CCollision>().radius)) && e->isActive())
				{
					e->destroy();
					b->destroy();
					m_score += e->get<CScore>().score;
					if (e->tag() == "Enemy")
					{
						spawnSmallEnemies(e);
					}

				}
			}

			if (player()->isActive() && e->isActive())
			{
				float diff = e->get<CTransform>().pos.dist(player()->get<CTransform>().pos);
				if (diff < ((player()->get<CCollision>().radius + e->get<CCollision>().radius) * (player()->get<CCollision>().radius + e->get<CCollision>().radius)))
				{
					player()->destroy();
					e->destroy();
					spawnPlayer();
				}
			}
		}
		

		if (e->tag() == "Enemy")
		{
			if ((e->get<CShape>().circle.getGlobalBounds().top < 0 && e->get<CTransform>().velocity.y < 0) || (e->get<CShape>().circle.getGlobalBounds().top + e->get<CShape>().circle.getGlobalBounds().height >= m_windowConfig.WW && e->get<CTransform>().velocity.y > 0))
			{
				e->get<CTransform>().velocity.y = -(e->get<CTransform>().velocity.y);
			}

			if ((e->get<CShape>().circle.getGlobalBounds().left < 0 && e->get<CTransform>().velocity.x < 0) || (e->get<CShape>().circle.getGlobalBounds().left + e->get<CShape>().circle.getGlobalBounds().width >= m_windowConfig.WH && e->get<CTransform>().velocity.x > 0))
			{
				e->get<CTransform>().velocity.x = -(e->get<CTransform>().velocity.x);
			}
		}
		

	}



}

void Game::spawnPlayer()
{
	//Note: all of the below code is just sample code to show creation of entity with this system, we have to use configuration.txt file for passing all the attributes of players, enemies and other entities
	//TODO : finish adding all properties of the player with the correct values from the config

	//we create every entity by calling EntityManager.addEntity(tag)
	//This returns a std::shared_ptr<Entity>, so we use 'auto' to save typing
	auto entity = m_entities.addEntity("Player");

	//Give this entity a Transform so it spawns at (200,200) with velocity (1,1) and angle 0
	float mx = m_window.getSize().x / 2.0f;
	float my = m_window.getSize().y / 2.0f; 
	//for spawning in the middle of screen logic

	//entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(.0f, .0f), 0.0f);
	entity->add<CTransform> (Vec2(mx, my), Vec2(.0f, .0f), 0.0f);
	//The entity shape will have radius 32, 8 sides, dark grey fill and red oultine of thickness 4
	//entity->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);
	entity->add<CShape>(m_playerConfig.SR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);

	//entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);
	entity->add<CCollision>(m_playerConfig.CR);
	//Add an input component to the player so that we can use inputs
	//entity->cInput = std::make_shared<CInput>();
	entity->add<CInput>();

	//Since we want this entity to be our player , set our Game's player variable to be this Entity
	// This goes slightly against the EntityManager paradigm, but we use the player so much that it's worth it
	//m_player = entity;
}

//spawn an enemy at random position
void Game::spawnEnemy()
{
	
	//TODO : make sure the enemy is spawned properly with the m_enemyConfig variables
	//the enemy must be spawned completely within the bounds of the window
	//view the video for some hints related to logic of spawning within the bounds
	//auto e = m_entityManager.addEntity(args);
	auto enemy = m_entities.addEntity("Enemy");
	//enemy->cShape = std::make_shared<CShape>(m_enemyConfig.SR, m_enemyConfig.VMIN + (rand() % (1 + m_enemyConfig.VMAX - m_enemyConfig.VMIN)), sf::Color(0, 0, 0), sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),m_enemyConfig.OT);
	enemy->add<CShape>(m_enemyConfig.SR, m_enemyConfig.VMIN + (rand() % (1 + m_enemyConfig.VMAX - m_enemyConfig.VMIN)), sf::Color(0, 0, 0), sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);


	//enemy->cShape->circle.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255)); // random color generation
	enemy->get<CShape>().circle.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255)); // random color generation
	//enemy->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
	enemy->add<CCollision>(m_enemyConfig.CR);
	//float r = enemy->cCollision->radius;
	float r = enemy->get<CCollision>().radius;
	//enemy->cScore = std::make_shared<CScore>(50);
	enemy->add<CScore>(50);

	int mx = r + (rand() % (size_t) (1 + (m_window.getSize().x-r) - r));
	int my = r + (rand() % (size_t) (1 + (m_window.getSize().y-r) - r));

	float sx = ((float)rand() / RAND_MAX)* (m_enemyConfig.SMAX - m_enemyConfig.SMIN) + (m_enemyConfig.SMIN); //random float number generation between max and min value
	float sy =  ((float)rand() / RAND_MAX)* (m_enemyConfig.SMAX - m_enemyConfig.SMIN) + (m_enemyConfig.SMIN);

	//std::cout << sx << "," << sy<<std::endl;
	/*if ((mx < m_player->cTransform->pos.x - m_player->cShape->circle.getRadius() && mx > m_player->cTransform->pos.x + m_player->cShape->circle.getRadius()) || (my < m_player->cTransform->pos.y - m_player->cShape->circle.getRadius() && my > m_player->cTransform->pos.y + m_player->cShape->circle.getRadius()))*/
	
	//enemy->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(sx, sy), 0);
	enemy->add<CTransform>(Vec2(mx, my), Vec2(sx, sy), 0);
	

	//record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

//spawns the small enemies when a big one (input entity e ) explodes
void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
	//TODO : spawns small enemies at the location of the input enemy e 

	//when we create the smaller enemy , we have to read the values of the original enemy
	// - spawn a number of small enemies equal to the vertices of the original enemy 
	// - set each small enemy to the same color as the original, half the size
	// - small enemies are worth double points or the original enemy
	//for (int i = 1; i <= e->cShape->circle.getPointCount(); i++)
	for (int i = 1; i <= e->get<CShape>().circle.getPointCount(); i++)
	{
		auto smallEnemy = m_entities.addEntity("SmallEnemy");

		//smallEnemy->cShape = std::make_shared<CShape>(e->cShape->circle.getRadius()/2,e->cShape->circle.getPointCount(), e->cShape->circle.getFillColor(),e->cShape->circle.getOutlineColor(), e->cShape->circle.getOutlineThickness());
		smallEnemy->add<CShape>(e->get<CShape>().circle.getRadius() / 2, e->get<CShape>().circle.getPointCount(), e->get<CShape>().circle.getFillColor(), e->get<CShape>().circle.getOutlineColor(), e->get<CShape>().circle.getOutlineThickness());

		//smallEnemy->cCollision = std::make_shared<CCollision>(e->cCollision->radius/2);
		smallEnemy->add<CCollision>(e->get<CCollision>().radius / 2);

		//smallEnemy->cScore = std::make_shared<CScore>((e->cScore->score) * 2);
		smallEnemy->add<CScore>((e->get<CScore>().score) * 2);

		//smallEnemy->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
		smallEnemy->add<CLifespan>(m_enemyConfig.L);

		//float disperseAngle = ((360 / e->cShape->circle.getPointCount()) * i) * 3.14159 /180;
		float disperseAngle = ((360 / e->get<CShape>().circle.getPointCount()) * i) * 3.14159 /180;

		//smallEnemy->cTransform = std::make_shared<CTransform>(e->cTransform->pos, Vec2(cos(disperseAngle), sin(disperseAngle)) * 2, 0);
		smallEnemy->add<CTransform>(e->get<CTransform>().pos, Vec2(cos(disperseAngle), sin(disperseAngle)) * 2, 0);
	

	}
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
	//TODO: implement the spawning of a bullet which travels towards target
	// - bullet speed is given as a scalar speed
	// - you must set the velocity by using formula in notes
	auto bullet = m_entities.addEntity("Bullet");

	//bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB), sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
	bullet->add<CShape>(m_bulletConfig.SR, m_bulletConfig.V, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB), sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);

	//bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
	bullet->add<CCollision>(m_bulletConfig.CR);
	
	//bullet->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
	bullet->add<CLifespan>(m_bulletConfig.L);
	//implementation without sqrtf
	//Vec2 targetDiff = target - m_player->cTransform->pos;
	Vec2 targetDiff = target - player()->get<CTransform>().pos;
	float shootAngle = atan2f(targetDiff.y, targetDiff.x);

	//bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, Vec2(10 * cos(shootAngle), 10 * sin(shootAngle)), 0.0f);
	bullet->add<CTransform>(entity->get<CTransform>().pos, Vec2(10 * cos(shootAngle), 10 * sin(shootAngle)), 0.0f);

	//implementation with the sqrtf in the normalized function
	/*bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, (target - entity->cTransform->pos).normalize() * 10, 0.0f);
	*/

}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	//TODO : implement your own special weapon
}


