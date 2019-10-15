#include "Game.h"
#include <string>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <vector>

const float Game::PI = 3.1415926535;
const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);  //set framerate 60 FPS
const float Game::PlayerSpeed = 400.f;
//onst float Game::BallSpeed = 400.f;


Game::Game() : window(sf::VideoMode(WIDTH, HEIGHT), "-< Breakout >-")
             , mTexture()
             , sprPaddle()
             , sprBrick()
             , bricks()
             , sprBall()
             , sprLoot()
             , mFont()
             , mStatisticsText()
             , scoreFont()
             , txtScore()
             , music()
             , mStatisticsUpdateTime()
             , mStatisticsNumFrames(0)
{
    // window.setFramerateLimit(60);   //not as accurate as timePerFrame / timeSinceLastUpdate
    bricks.clear();
    Loots.clear() ;

    ballSize = 31;
    BallSpeed = 400.f;

    if (!mTexture.loadFromFile("Assets/breakout_sprites.png"))
	{
		printf ("cannot load sprite-sheet (breakout_sprites.png) !!! \n");
	};
	sprPaddle.setTexture(mTexture);
	sprPaddle.setTextureRect(sf::IntRect(0, 200, 96, 24));
    sprPaddle.setPosition(WIDTH / 2 - 96/2, HEIGHT - 24);

    sprBrick.setTexture(mTexture);
    drawBricks();

	sprBall.setTexture(mTexture);
	sprBall.setTextureRect(sf::IntRect(121, 121, ballSize, ballSize));
	sprBall.setOrigin(sf::Vector2f(ballSize /2, ballSize /2));
    sprBall.setPosition(WIDTH /2 , HEIGHT - 24 - 31);
    srand(time(NULL));
    sprBall.setRotation((rand()%180) + 180);

    sprLoot.setTexture(mTexture);
    sprLoot.setTextureRect(sf::IntRect(281,80, 31, 31));
    sprLoot.setOrigin(ballSize / 2 , ballSize / 2);

    if (!scoreFont.loadFromFile("Assets/game_over.ttf"))
        printf ("cannot load score font!\n");

    txtScore.setFont(scoreFont);
    txtScore.setColor(sf::Color::Red);
    txtScore.setCharacterSize(50);
    txtScore.setPosition(WIDTH - 100 , HEIGHT - 60);

    if (!mFont.loadFromFile("Assets/FreeSans.ttf"))
        printf ("cannot load FreeSans font!\n");

	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setColor(sf::Color::White);
	mStatisticsText.setCharacterSize(10);

	//sf::SoundBuffer music;
    if (!music.openFromFile("Assets/throughspace.ogg"))
        printf ("cannot load music file!\n");
    if (!bufBrickHit.loadFromFile("Assets/cork.flac"))
        printf ("cannot load BrickHIt SFX file!\n");
    sndBrickHit.setBuffer(bufBrickHit);
    if (!bufPowerUp.loadFromFile("Assets/rise01.ogg"))
        printf ("cannot load Power-Up SFX file!\n");
    sndPowerUp.setBuffer(bufPowerUp);

	mIsMovingUp = false;
    mIsMovingDown = false;
    mIsMovingLeft = false;
    mIsMovingRight = false;
    quitKey    = false;
    gameOver   = false;
    nScore = 0;
}

void Game::run(bool bMusicFlag, bool bSndFlag)
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    bSoundFlag = bSndFlag;

    if (bMusicFlag)
        music.play();
    while (window.isOpen() && !quitKey && !gameOver)
    {
        processEvents();
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            update(TimePerFrame);
        }
        updateStatistics(elapsedTime);
        render();
    }
}

void Game::processEvents()
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
        case sf::Event::Closed:
            window.close();
            break;
        }
    }
}

void Game::update(sf::Time elapsedTime)
{
    sf::Vector2f movement(0.f, 0.f);

    /*
        if (mIsMovingUp)
            movement.y -= PlayerSpeed;
        if (mIsMovingDown)
            movement.y += PlayerSpeed;
    */
    if (mIsMovingLeft)
        movement.x -= PlayerSpeed;
    if (mIsMovingRight)
        movement.x += PlayerSpeed;

    testWallCollision();
    testBrickCollision(sprBall.getGlobalBounds() );
 /*   if( testBrickCollision(sprBall.getGlobalBounds() ) )
        {
            sprBall.setRotation(sprBall.getRotation() - 180.0f ); //todo: find a way to do this better
            nScore++;
        }
*/

    sf::Vector2f ballDirection(std::cos(sprBall.getRotation() * PI /180), std::sin(sprBall.getRotation() * PI /180));
    sprBall.move( ballDirection * BallSpeed * elapsedTime.asSeconds() );

    updateLoot(elapsedTime);

    sprPaddle.move(movement * elapsedTime.asSeconds());
    if(testPaddleCollision(sprBall.getGlobalBounds()) )
    {
        //paddle  is 96 long
        //if(sprBall.getPosition().x + sprBall.getOrigin().x <= sprPaddle.getPosition().x + 96/2)
        if(sprBall.getPosition().x  <= sprPaddle.getPosition().x + 96/2)
        {
            float x =  (sprBall.getPosition().x  -  (sprPaddle.getPosition().x + 96/2)) * -1;
            float newRx = mapFrange(x, 96.0f, 0.0f / 2, 180.0f, 270.0f);
            sprBall.setRotation( newRx );
        }
        if(sprBall.getPosition().x  >= sprPaddle.getPosition().x + 96/2)
        {
            float x =  (sprBall.getPosition().x + 31/2) - (sprPaddle.getPosition().x + 96/2) ;
            float newRx = mapFrange(x , 0, 96.0f , 270.0f, 360.0f);
            sprBall.setRotation(newRx );
        }
    }
}

void Game::render()
{
    window.clear(sf::Color::Black);
    window.draw(sprPaddle);
    window.draw(sprBall);
    for (auto &b : bricks)
        window.draw(b);
    for (auto &l : Loots)
        window.draw(l);
    window.draw(mStatisticsText);
    window.draw(txtScore);
    window.display();
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
    if (key == sf::Keyboard::W)
        mIsMovingUp = isPressed;
    else if (key == sf::Keyboard::S)
        mIsMovingDown = isPressed;
    else if (key == sf::Keyboard::A || key == sf::Keyboard::Left)
        mIsMovingLeft = isPressed;
    else if (key == sf::Keyboard::D || key == sf::Keyboard::Right)
        mIsMovingRight = isPressed;
    else if (key == sf::Keyboard::Q)
        quitKey = isPressed;


}


void Game::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;
    txtScore.setString("Score: " + std::to_string(nScore) );
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString(
			"Frames / Second = " + std::to_string(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Game::testWallCollision()
{
    if(sprBall.getPosition().x -15.5f <= 0)
    {
        if(sprBall.getRotation() >= 180.0f && sprBall.getRotation() <= 270.0f)
            sprBall.setRotation(360 - (sprBall.getRotation() - 180.f) ) ;
        if (sprBall.getRotation() <= 180.0f && sprBall.getRotation() >= 90.0f)
            sprBall.setRotation( 180 - sprBall.getRotation() );
    }
    if(sprBall.getPosition().x + 15.5f >= WIDTH)
    {
        if(sprBall.getRotation() >= 270.0f && sprBall.getRotation() <= 360.0f)
            sprBall.setRotation( (360 - sprBall.getRotation() ) + 180.f );
        if (sprBall.getRotation() >= 0.0f && sprBall.getRotation() <= 90.0f)
            sprBall.setRotation( 180 - sprBall.getRotation() );
    }
    if(sprBall.getPosition().y - 15.5f <= 0)
    {
        if(sprBall.getRotation() >= 270.0f && sprBall.getRotation() <= 360.0f)
            sprBall.setRotation(90 + (270 - sprBall.getRotation() ) );
        if (sprBall.getRotation() >= 180.0f && sprBall.getRotation() <= 270.0f)
            sprBall.setRotation(90 + (270- sprBall.getRotation() ));

    }
    if(sprBall.getPosition().y - 40.f >= HEIGHT)
    {
        printf("fall's out  \n");
        gameOver = true;
    }
    if(sprPaddle.getPosition().x <= 0)
        sprPaddle.setPosition(0, sprPaddle.getPosition().y);
    if(sprPaddle.getPosition().x + 96 >= WIDTH)
        sprPaddle.setPosition(WIDTH - 96, sprPaddle.getPosition().y);

}


bool Game::testPaddleCollision(sf::FloatRect r)
{
    if(sprPaddle.getGlobalBounds().intersects(r) )
        return true;
}

void Game::drawBricks()
{
    for(int j=1; j <= 4; j++)
    {
        for(int i=0; i <=  25; i++)
        {
            sprBrick.setPosition(i *  32, j * 32);
            int rb = rand()%7 * 40;
            sprBrick.setTextureRect(sf::IntRect(rb, 0, 32, 32));
            bricks.push_back({sprBrick});
        }
    }

}


bool Game::testBrickCollision(sf::FloatRect ball)
{
    bool collisionFlag = false;
    for( auto &b : bricks)
    {
        if( b.getGlobalBounds().intersects(ball) )
        {
            collisionFlag = true;
//randomly decide if it drops loot 1 in 10 chance
            if(rand()%10 == 5 )
                dropLoot(b.getPosition().x, b.getPosition().y);
            float result = atan2 (((sprBall.getPosition().y + ballSize /2) - (b.getPosition().y + 32)), ( (sprBall.getPosition().x + ballSize /2) - (b.getPosition().x + 32) ) ) * 180 / PI; // ### ( * 180/ PI to convert rad to deg )
            sprBall.setRotation(result);
            nScore++;
            if (bSoundFlag)
                sndBrickHit.play();
            b.setPosition(-50.0f, -50.0f); //move it off the screen so we can delete it from the vector AFTER we loop through the vector
            break;
        }
    }
    if (bricks.size() > 0)
    {
        auto i = remove_if(bricks.begin(), bricks.end(), [](auto &o)
                            {
                                return o.getPosition().y <= 0.f;
                            }
                );
        if (i != bricks.end())
                bricks.erase(i);
    }
    if (bricks.size() == 0)
        resetLevel();

    return collisionFlag;

}

void Game::dropLoot(float x, float y)
{
    sprLoot.setPosition(x, y);
    Loots.push_back({sprLoot});
}

void Game::updateLoot(sf::Time elapsedTime)
{
    // Erase anything off screen
    if (Loots.size() > 0)
        {
            auto i = remove_if(Loots.begin(), Loots.end(), [](auto &o)
                                {
                                    return o.getPosition().y >= HEIGHT;
                                }
                    );
            if (i != Loots.end())
                    Loots.erase(i);
        }

     // update loot Position
     if (Loots.size() > 0)
     {
        for (auto &l : Loots)
        {
            float looty = l.getPosition().y;
            l.move(0.f, (BallSpeed / 2 ) * elapsedTime.asSeconds() );
            if (testPaddleCollision(l.getGlobalBounds()) )
                {
                    if (bSoundFlag)
                        sndPowerUp.play();
                    nScore = nScore + 5;
                    l.setPosition(WIDTH, HEIGHT +5);
                }
        }
     }
}

void Game::resetLevel()
{
    bricks.clear();
    Loots.clear() ;
    drawBricks();
    BallSpeed = BallSpeed * 1.2;

}

float Game::mapFrange(float value, float start1, float stop1, float start2, float stop2) {
    float outgoing = start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
    return outgoing;
}
