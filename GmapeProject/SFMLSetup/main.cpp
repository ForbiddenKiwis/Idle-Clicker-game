#include <iostream>

#include "Game.h"

using namespace std;
//using namespace sf;

int main()
{

    // Init Random
    srand(static_cast<unsigned>(time(NULL)));

    // Init Game Engine
    Game game;

    //Game Loop
    while (game.isRunning() && !game.getEndGame())
    {
        
        //Update
        game.update();
        
        //Render      
        game.render();
        
    }

    //End of app
    return 0;
}

