#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <cstdlib>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


using namespace std;
//using namespace sf;
/*
	Class which handles game logic
	Wrapper Class.
*/

class Game
{
private:
	//Variables
	//Window
	sf::RenderWindow* window;
	sf::VideoMode vm;
	sf::Event ev;

	//Mouse Position
	sf::Vector2i mousePosWin;
	sf::Vector2f mousePosView;

	//Resources
	sf::Font font;

	//Text
	sf::Text uiText;

	//Game Logics
	bool endGame;
	int points;
	int health;
	int growthTimer;
	int speedTimer;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	bool mouseHeld;

	//Clocks
	sf::Clock growthClock;
	sf::Clock speedClock;
	sf::Clock dtClock;
	int dt;

	//Game Objects
	vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;


	//Functions
	void initVariables();
	void initWindow();
	void initFonts();
	void initText();
	void initEnemies();
	void spawnBigger();
	void fallFaster();

public:
	
	
	//Constructor
	Game();


	//Destructor
	~Game() = default;

	//Accesors
	const bool isRunning() const;
	const bool getEndGame() const;

	//Functions
	void spawnEnemy();

	void pollEvents();
	void updateMousePos();
	void updateText();
	void updateEnemies();
	void update();

	void renderText(sf::RenderTarget& target);
	void renderEnemies(sf::RenderTarget& targert);
	void render();
};

