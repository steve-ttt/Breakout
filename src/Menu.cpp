#include "Menu.h"
//#include <Vector2.hpp>end


Menu::Menu(): window(sf::VideoMode(WIDTH, HEIGHT), "-< Menu >-")
    ,txtMusic()
    ,txtPlay()
    ,txtQuit()
    ,txtSound()
    ,lastClickTime()
    ,currentTime()
    ,clock()
{
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);

    if (!titleFont.loadFromFile("Assets/bsg.ttf"))
        printf ("cannot load menu font!\n");
    txtTitle.setFont(titleFont);
    txtTitle.setColor(sf::Color::Red);
    txtTitle.setCharacterSize(75);
    txtTitle.setPosition(40 / 2, 40);
    txtTitle.setString("  -Breakout -\n or Arkanoids?" );


    if (!mFont.loadFromFile("Assets/game_over.ttf"))
        printf ("cannot load menu font!\n");
    txtPlay.setFont(mFont);
    txtPlay.setColor(sf::Color::Cyan);
    txtPlay.setCharacterSize(50);
    txtPlay.setPosition(20, HEIGHT / 2);
    txtPlay.setString("Play Game (p key)" );

    txtQuit.setFont(mFont);
    txtQuit.setColor(sf::Color::Cyan);
    txtQuit.setCharacterSize(50);
    txtQuit.setPosition(20, HEIGHT / 2 + 25);
    txtQuit.setString("Quit Game (q key)" );

    txtSound.setFont(mFont);
    txtSound.setColor(sf::Color::Cyan);
    txtSound.setCharacterSize(50);
    txtSound.setPosition(20, HEIGHT / 2 +50);
    txtSound.setString("Sound off (s key)" );

    txtMusic.setFont(mFont);
    txtMusic.setColor(sf::Color::Cyan);
    txtMusic.setCharacterSize(50);
    txtMusic.setPosition(20, HEIGHT / 2 + 75);
    txtMusic.setString("Music off (m key)" );

    if (!pTexture.loadFromFile("Assets/pitrizzo.png"))
			printf ("cannot load menu sprite pointer !!! \n");
    if (!wpTexture.loadFromFile("Assets/wallp.jpg"))
        printf ("cannot load menu wallpaper !!! \n");

	sprPointer.setTexture(pTexture);
    sprPointer.setPosition(WIDTH / 2 , HEIGHT /2);
    sprPointer.setRotation(-45.f);

    sprWallp.setTexture(wpTexture);
    sprWallp.setPosition(0,0);

    mItems.play = false;
    mItems.quit = false;
    mItems.music = false;
    mItems.sound = false;

    quitKey    = false;
    playKey    = false;
    soundKey    = false;
    musicKey    = false;
    lmbKey      = false;

}


Menu::~Menu()
{
    //dtor
}



void Menu::run()
{
    while ( !mItems.quit )
    {
        processEvents();
        update();
        render();
        if(mItems.play)
            break;
    }
}

void Menu::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::KeyPressed:
            handlePlayerInput(event.key.code, true);
            break;
        case sf::Event::KeyReleased:
            handlePlayerInput(event.key.code, false);
            break;
        case sf::Event::MouseButtonPressed:
            handlePlayerInput(event.key.code, true);
            break;
        case sf::Event::MouseButtonReleased:
            handlePlayerInput(event.key.code, false);
            break;
        case sf::Event::Closed:
            window.close();
            break;
        }
    }
}

void Menu::update()
{
    currentTime = clock.getElapsedTime();
    sprPointer.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))); //set sprite to mouse position

    if (mItems.sound)
        txtSound.setColor(sf::Color::Red);
    if (!mItems.sound)
        txtSound.setColor(sf::Color::Cyan);
    if (mItems.music)
        txtMusic.setColor(sf::Color::Red);
    if (!mItems.music)
        txtMusic.setColor(sf::Color::Cyan);
    txtQuit.setColor(sf::Color::Cyan);
    txtPlay.setColor(sf::Color::Cyan);
    checkMouseColl();

    if(playKey)
    {
      mItems.play = true;
    }

    if(quitKey)
    {
        mItems.quit = true;
    }

    if(soundKey)
    {
        mItems.sound = !mItems.sound;
        if (mItems.sound)
            txtSound.setString("Sound on (s key)");
        else
            txtSound.setString("Sound off (s key)");
        soundKey = false;
    }
    if(musicKey)
    {
        mItems.music = !mItems.music;
        if (mItems.music)
            txtMusic.setString("Music on (m key)");

        else
            txtMusic.setString("Music off (m key)");
        musicKey = false;
    }



}

void Menu::render()
{
    window.clear();
    window.draw(sprWallp);
    window.draw(txtPlay);
    window.draw(txtQuit);
    window.draw(txtSound);
    window.draw(txtMusic);
    window.draw(txtTitle);
    window.draw(sprPointer);
    window.display();
}


void Menu::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
    if (key == sf::Keyboard::Q)
        quitKey = isPressed;
    if (key == sf::Keyboard::P)
        playKey = isPressed;
    if (key == sf::Keyboard::S)
        soundKey = isPressed;
    if (key == sf::Keyboard::M)
        musicKey = isPressed;
    if (key == sf::Mouse::Left)
        lmbKey = isPressed;
}


void Menu::checkMouseColl()
{
        if (txtMusic.getGlobalBounds().contains( static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))) )
                {
                txtMusic.setColor(sf::Color::Red);
                if(lmbKey)
                    musicKey = true;
                    lmbKey = false;
                }
        if (txtSound.getGlobalBounds().contains( static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))) )
                {
                    txtSound.setColor(sf::Color::Red);
                    if(lmbKey)
                        soundKey = true;
                        lmbKey = false;
                }
        if (txtQuit.getGlobalBounds().contains( static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))) )
            {
                txtQuit.setColor(sf::Color::Red);
                if(lmbKey)
                    quitKey = true;
                    lmbKey = false;
            }
        if (txtPlay.getGlobalBounds().contains( static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))) )
            {
                txtPlay.setColor(sf::Color::Red);
                if(lmbKey)
                    playKey = true;
                    lmbKey = false;
            }
}


bool Menu::getPlayState()
{
    return mItems.play;
}
bool Menu::getQuitState()
{
    return mItems.quit;
}
bool Menu::getSoundState()
{
    return mItems.sound;
}
bool Menu::getMusicState()
{
    return mItems.music;
}
bool Menu::setPlayState(bool b)
{
    mItems.play = b;
}
bool Menu::setQuitState(bool b)
{
    mItems.quit = b;
}
