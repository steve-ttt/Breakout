#include "splash.h"
#include <time.h>
#include <string>



splash::splash(): window(sf::VideoMode(WIDTH, HEIGHT), "-< Breakout >-")
             , mTexture()
             , mLogo()
             , mText()
             , mFont()
             , bsgFont()
{
    //ctor
    if (!mFont.loadFromFile("Assets/FreeSans.ttf"))
    {
        printf ("splash cannot load FreeSans font!\n");
    };
    mText.setFont(mFont);
    mText.setCharacterSize(20);
	mText.setPosition(WIDTH / 2 - 100, HEIGHT - 25);
	mText.setColor(sf::Color::White);
	mText.setString("c 2019 -- and all that jazz");

    if (!bsgFont.loadFromFile("Assets/bsg.ttf"))
    {
        printf ("splash cannot load bsg font!\n");
    };
    bsgText.setFont(bsgFont);
    bsgText.setCharacterSize(100);
    bsgText.setPosition(50, HEIGHT /2 -100);
	bsgText.setColor(sf::Color::White);
	bsgText.setString(" ROUGH AS\n    GUTZ");
}

splash::~splash()
{
    //dtor
}

void splash::run()
{

        render();
        sf::sleep(sf::seconds(1.0f));
        window.close();

}

void splash::render()
{
    window.clear(sf::Color::Black);
    window.draw(mText);
    window.draw(bsgText);
    window.display();
}
