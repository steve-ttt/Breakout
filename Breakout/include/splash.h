#ifndef SPLASH_H
#define SPLASH_H
#pragma once
#include "SFML/Graphics.hpp"

#define WIDTH 800
#define HEIGHT 600


class splash
{
    public:
        splash();
        virtual ~splash();
        void run();


    protected:

    private:
        //void update(sf::Time elapsedTime);
        void render();
        //void processEvents();
       // void update(sf::Time elapsedTime);
        //void render();

        sf::Font        mFont;
        sf::Font        bsgFont;
        sf::Text        mText;
        sf::Text        bsgText;
        sf::Sprite      mLogo;
        sf::RenderWindow window;
        sf::Texture     mTexture;
};

#endif // SPLASH_H
