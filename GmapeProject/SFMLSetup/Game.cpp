#include "Game.h"

//Private Functions
void Game::initVariables()
{
	this->window = nullptr;

    //Game Logic
    this->endGame = false;
    this->points = 0;
    this->health = 3;
    this->speedTimer = 0;
    this->growthTimer = 0;
    this->enemySpawnTimerMax = 10.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 10;
    this->mouseHeld = false;
    srand(time(NULL));
}

void Game::initWindow()
{
	this->vm.height = 1000;
	this->vm.width = 1200;
	
    this->window = new sf::RenderWindow(this->vm, "First Game", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(30);
}

void Game::initFonts()
{
    if (
        this->font.loadFromFile("Fonts/Pixeboy-z8XGD.ttf"),
        this->font.loadFromFile("Fonts/PixeloidMono-d94EV.ttf")
        )
    {
        cout << "Error GAME::INITFONTS::Failed to load font!" << endl;
    }
}

void Game::initText()
{
    this->uiText.setFont(this->font);
    this->uiText.setCharacterSize(24);
    this->uiText.setFillColor(sf::Color::White);
    this->uiText.setString("None");
}

void Game::initEnemies()
{
    //Properties of enemy

    this->enemy.setPosition(100.f, 100.f);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
}

//Constructor
Game::Game()
{
	this->initVariables();
	this->initWindow();
    this->initFonts();
    this->initText();
    this->initEnemies();

    this->dtClock.restart();
}

//Accesors
const bool Game::isRunning() const
{
	return this->window->isOpen();
}

const bool Game::getEndGame() const
{
    return this->endGame;
}

//Functions
void Game::spawnEnemy()
{
    /*
        @return void

        Spawn Enemies and set their color and positions.
        - Sets a random x position and y position is always on the top of the screen.
        - Sets a random color.
        -Adds enemy to the vector
    */
    

    this->enemy.setPosition(
        static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
        /*static_cast<float>(rand() % static_cast<int>(this->window->getSize().y - this->enemy.getSize().y)*/
        0.f // Makes them spawn on the top of the screen
    );


    //My part
    int random = rand() % 20;

    if (random > 0 && random <= 5)
        this->enemy.setFillColor(sf::Color::Red);
    else if (random == 6)
        this->enemy.setFillColor(sf::Color::Blue);
    else if (random == 7)
        this->enemy.setFillColor(sf::Color::Yellow);
    else if (random > 7 && random <= 10)
        this->enemy.setFillColor(sf::Color::Magenta);
    else
        this->enemy.setFillColor(sf::Color::Green);

    //Spanw Enemy
    this->enemies.push_back(this->enemy);

    //Remove enmies at the end of the screen


}

// Make the enemies bigger
void Game::spawnBigger()
{
    // Restart the clock
    this->growthClock.restart();

    // Set size to bigger
    this->enemy.setSize(sf::Vector2f(200.f, 200.f));
}


void Game::fallFaster()
{
    this->speedClock.restart();

    // Increase speed if timer is active
    this->enemy.move(0.f, 10.f);   
}

void Game::pollEvents()
{
    //Event polling
    while (this->window->pollEvent(this->ev))
    {
        switch (this->ev.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->ev.key.code == sf::Keyboard::Escape)
                this->window->close();
            break;
        }
    }
}

void Game::updateMousePos()
{
    /*
        @ return void

        update mouse position
        - Mouse positions relative to window
    */

    this->mousePosWin = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWin);
}

void Game::updateText()
{
    stringstream ss;

    ss << "Health: " << this->health;
    ss << "\nPoints: " << this->points;

    // Display growth timer if it's active
    if (this->growthTimer > 0)
        ss << "\nGrowth Timer: " << this->growthTimer;

    // Display speed-up timer if it's active
    if (this->speedTimer > 0)
        ss << "\nSpeed Timer: " << this->speedTimer;

    this->uiText.setString(ss.str());
}

void Game::updateEnemies()
{
    /**
    *   @return void
    * 
    *   Updates the enemy spawn timer and spawn enemies 
    *   When the total amount is smaller than the max
    *   Moves the enemies downwards
    *   Removes the enemies at the edge of the screen
    */


    // Get the elapsed time since the last update in seconds
    float dt = this->dtClock.restart().asSeconds();

    // Update the growth timer
    if (this->growthTimer > 0)
    {
        this->growthTimer -= dt;
        if (this->growthTimer <= 0)
        {
            // Reset enemy size when timer reaches 0
            this->enemy.setSize(sf::Vector2f(100.f, 100.f));
            this->growthTimer = 0; // Ensure the timer doesn't go negative
        }
    }

    // Update the speed timer
    if (this->speedTimer > 0)
    {
        this->speedTimer -= dt;
        if (this->speedTimer <= 0)
        {
            // Reset speed when timer reaches 0
            this->enemy.move(0.f, 5.f);
            this->speedTimer = 0; // Ensure the timer doesn't go negative
        }
    }

    //Updating the timer for enemy spawning 
    if (this->enemies.size() < this->maxEnemies) 
    {
        if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
        {
            //Spawn Enemy and reset timer
            this->spawnEnemy();
            this->enemySpawnTimer = 0.f;
        }
        else
            this->enemySpawnTimer += 1.f;
    }

    //Moving and updating the enemies
    for (int i = 0; i < this->enemies.size(); i++)
    {
        this->enemies[i].move(0.f, 5.f);

        if (this->enemies[i].getPosition().y > this->window->getSize().y )
        {
            if (this->enemies[i].getFillColor() == sf::Color::Green)
            {
                this->health -= 1;
                cout << "Health: " << this->health << endl;
            }
            else if (this->enemies[i].getFillColor() == sf::Color::Red)
            {
                this->points += 2;
                cout << "Red enemy reached the bottom" << endl;
            }
            this->enemies.erase(this->enemies.begin() + i);
        }
    }

    //Check if clicked upon

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (this->mouseHeld == false)
        {
            this->mouseHeld = true;
            bool deleted = false;

            for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
            {
                if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
                {
                    if (enemies[i].getFillColor() == sf::Color::Green)
                    {
                        //Deelte the enemy
                        deleted = true;
                        this->enemies.erase(this->enemies.begin() + i);

                        //Gain Points
                        this->points += 1;
                        cout << "Points: " << this->points << endl;
                    }
                    else if (enemies[i].getFillColor() == sf::Color::Red)
                    {
                        deleted = true;
                        this->enemies.erase(this->enemies.begin() + i);

                        //Lose Points
                        this->points -= points;
                        cout << "Points: " << this->points << endl;
                    }
                    else if (enemies[i].getFillColor() == sf::Color::Blue)
                    {
                        deleted = true;
                        this->enemies.erase(this->enemies.begin() + i);
                        this->growthTimer = 60;
                        cout << "Spawing biggger" << endl;
                        this->spawnBigger();
                        break;
                    }
                    else if (enemies[i].getFillColor() == sf::Color::Yellow)
                    {
                        deleted = true;
                        this->enemies.erase(this->enemies.begin() + i);

                        //Fall Faster
                        this->speedTimer = 60;
                        this->fallFaster();
                        cout << "Falling faster" << endl;
                        break;
                    }
                    else
                    {
                        deleted = true;
                        this->enemies.erase(this->enemies.begin() + i);

                        //lose health
                        this->health -= 1;
                        cout << "Health: " << this->health << endl;
                    }
                }
            }
        }
    }
    else
    {
        this->mouseHeld = false;
    }
}

void Game::update()
{
    this->pollEvents();
    if (!this->endGame)
    {
        this->updateMousePos();
        this->updateText();
        this->updateEnemies();
    }
    
    //End game Conditions
    if (this->health < 1)
    {
        this->endGame = true;
    }
}

void Game::renderText(sf::RenderTarget& target)
{
    target.draw(this->uiText);
}

void Game::renderEnemies(sf::RenderTarget& target)
{
    //Rendering Enemies
    for (auto& e : this->enemies)
    {
        target.draw(e);
    }
}

void Game::render()
{
    /*
        @return void
        
        - Clear old Frame
        - Renders the Game Objects 
        - Displays frame in Window
    */

    this->window->clear();

    //Draw game objects
    this->renderEnemies(*this->window);
    this->renderText(*this->window);
    this->window->display();
}
