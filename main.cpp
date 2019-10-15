#include <SFML/Graphics.hpp>
#include "Game.h"
#include "splash.h"
#include "Menu.h"

enum class GameStates { MAIN_MENU, EXIT, LEVEL, OPTIONS, SPLASH };


int main()
{

    GameStates gs = GameStates::SPLASH;


    bool snd = false;
    bool mus = false;
    bool play = true;
    bool qt = false;


    while( gs != GameStates::EXIT )
    {
        switch(gs)
        {
        case GameStates::SPLASH:
            {
                splash sp;
                sp.run();
                gs = GameStates::MAIN_MENU;
                break;
            }
        case GameStates::MAIN_MENU:
            {
                Menu m;
                m.run();
                snd = m.getSoundState();
                mus = m.getMusicState();
                play = m.getPlayState();
                qt = m.getQuitState();
                if(qt)
                    gs = GameStates::EXIT;
                if(play)
                {
                    m.setPlayState(false);
                    gs = GameStates::LEVEL;
                }
                break;
            }
        case GameStates::LEVEL:
            Game game;
            game.run(mus, snd);
            gs = GameStates::MAIN_MENU;
            break;

        }

    }



}
