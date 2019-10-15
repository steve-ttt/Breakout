#ifndef MENU_H
#define MENU_H
#include "SFML/Graphics.hpp"

#define WIDTH 800
#define HEIGHT 600

class Menu
{
    public:
        Menu();
        virtual ~Menu();
        void run();
        bool getPlayState();
        bool getQuitState();
        bool getSoundState();
        bool getMusicState();
        bool setPlayState(bool b);
        bool setQuitState(bool b);

    protected:

    private:

        void processEvents();
        void update();
        void render();
        void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
        void checkMouseColl();


        sf::RenderWindow window;
        sf::CircleShape mPlayer;

        struct MenuItems {
          bool play;
          bool quit;
          bool music;
          bool sound;
        } ;


        sf::Font        mFont;
        sf::Font        titleFont;
        sf::Text		txtPlay;
        sf::Text		txtQuit;
        sf::Text		txtSound;
        sf::Text		txtMusic;
        sf::Text		txtTitle;
        sf::Texture     pTexture;
        sf::Sprite      sprPointer;
        sf::Texture     wpTexture;
        sf::Sprite      sprWallp;
        sf::Mouse       mousePos;

        MenuItems       mItems ;
        bool            soundKey;
        bool            musicKey;
        bool            quitKey;
        bool            playKey;
        bool            lmbKey;
        sf::Time        lastClickTime;
        sf::Time        currentTime;
        sf::Clock       clock;
};

#endif // MENU_H

