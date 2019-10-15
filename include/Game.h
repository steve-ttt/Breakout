#ifndef GAME_H
#define GAME_H
#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#define WIDTH 800
#define HEIGHT 600

class Game
{
public:
    Game();
    void run(bool bMusicFlag, bool bSoundFlag);

private:

    static const float      PI;
    static const sf::Time	TimePerFrame;
    static const float      PlayerSpeed;
 //   static const float      BallSpeed;
    int     ballSize;
    int     nScore;

    void processEvents();
    void update(sf::Time elapsedTime);
    void render();
    void updateStatistics(sf::Time elapsedTime);
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
    void testWallCollision();
    bool testPaddleCollision(sf::FloatRect r);
    bool testBrickCollision(sf::FloatRect ball);
    void drawBricks();
    void dropLoot(float x, float y);
    void updateLoot(sf::Time elapsedTime);
    void resetLevel();

    float mapFrange(float value, float start1, float stop1, float start2, float stop2);

    sf::Font        mFont;
    sf::Font        scoreFont;
    sf::Text		mStatisticsText;
    sf::Text		txtScore;
    sf::Time	    mStatisticsUpdateTime;
	std::size_t	    mStatisticsNumFrames;
	sf::Music       music;
	sf::SoundBuffer bufBrickHit;
    sf::Sound sndBrickHit;
    sf::SoundBuffer bufPowerUp;
    sf::Sound       sndPowerUp;

    sf::RenderWindow window;
    //sf::CircleShape mPlayer;
    sf::Texture     mTexture;
    sf::Sprite      sprPaddle;
    sf::Sprite      sprBall;
    sf::Sprite      sprBrick;
    sf::Sprite      sprLoot;
    std::vector<sf::Sprite> bricks;
    std::vector<sf::Sprite> Loots;

    float           BallSpeed;
    bool            gameOver;
    bool            bSoundFlag;
    bool            quitKey;
    bool            mIsMovingUp;
    bool            mIsMovingDown;
    bool            mIsMovingLeft;
    bool            mIsMovingRight;
};

#endif // GAME_H
