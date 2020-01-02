#include <SFML/Graphics.hpp>
#include <sstream>
#include "ZombieArena.h"
#include "Player.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"

using namespace sf;

int main()
{
    TextureHolder holder;
    enum class State
    {
        PAUSED, LEVELING_UP, GAME_OVER, PLAYING
    };
    State state = State::GAME_OVER;

    Vector2f resolution;
    resolution.x = VideoMode::getDesktopMode().width;
    resolution.y = VideoMode::getDesktopMode().height;

    RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen);

    View mainView(FloatRect(0, 0, resolution.x, resolution.y));

    Clock clock;
    Time gameTimeTotal;

    Vector2f mouseWorldPosition;
    Vector2i mouseScreenPosition;

    Player player;
    IntRect arena;

    VertexArray background;
    Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

    int numZombies;
    int numZombiesAlive;
    Zombie* zombies = nullptr;

    const int BULLET_ARRAY_SIZE = 100;
    Bullet bullets[BULLET_ARRAY_SIZE];
    int currentBullet = 0;
    int bulletsSpare = 24;
    int bulletsInClip = 6;
    int clipSize = 6;
    float fireRate = 1;
    Time lastPressed;

    window.setMouseCursorVisible(false);
    Sprite spriteCrosshair;
    Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
    spriteCrosshair.setTexture(textureCrosshair);
    spriteCrosshair.setOrigin(25, 25);

    Pickup healthPickup(1);
    Pickup ammoPickup(2);

    int score = 0;
    int hiScore = 0;

    Sprite spriteGameOver;
    Texture textureGameOver = TextureHolder::GetTexture("graphics/bakcground.png");
    spriteGameOver.setTexture(textureGameOver);
    spriteGameOver.setPosition(0, 0);

    View hubView(sf::FloatRect(0, 0, resolution.x, resolution.y));

    Sprite spriteAmmoIcon;
    Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
    spriteAmmoIcon.setTexture(textureAmmoIcon);
    spriteAmmoIcon.setPosition(28, 620);

    Font font;
    font.loadFromFile("fonts/zombiecontrol.ttf");

    Text pausedText;
    pausedText.setFont(font);
    pausedText.setCharacterSize(85);
    pausedText.setFillColor(Color::White);
    pausedText.setPosition(350, 200);
    pausedText.setString("Press Enter \nto continue");

    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(80);
    gameOverText.setFillColor(Color::White);
    gameOverText.setPosition(280, 540);
    gameOverText.setString("Press Enter to play");

    Text hiScoreText;
    hiScoreText.setFont(font);
    hiScoreText.setCharacterSize(50);
    hiScoreText.setFillColor(Color::White);
    hiScoreText.setPosition(980, 0);
    std::stringstream s;
    s << "Hi Score: " << hiScore;
    hiScoreText.setString(s.str());

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(500, 0);
    scoreText.setString("Hello world");

    Text zombiesRemainingText;
    zombiesRemainingText.setFont(font);
    zombiesRemainingText.setCharacterSize(50);
    zombiesRemainingText.setFillColor(Color::White);
    zombiesRemainingText.setPosition(975, 620);
    zombiesRemainingText.setString("Zombies: 100");

    int wave = 0;
    Text waveNumberText;
    waveNumberText.setFont(font);
    waveNumberText.setCharacterSize(50);
    waveNumberText.setFillColor(Color::White);
    waveNumberText.setPosition(750, 620);
    waveNumberText.setString("Wave: 0");

    Text ammoText;
    ammoText.setFont(font);
    ammoText.setCharacterSize(50);
    ammoText.setFillColor(Color::White);
    ammoText.setPosition(100, 620);
    ammoText.setString("ammo");

    RectangleShape healthBar;
    healthBar.setFillColor(Color::Red);
    healthBar.setPosition(300, 620);

    int framesSinceLastHUDUpdate = 0;
    int fpsMeasurementFrameInterval = 1000;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Return &&
                    state == State::PLAYING)
                {
                    state = State::PAUSED;
                }
                else if (event.key.code == Keyboard::Return &&
                        state == State::PAUSED)
                {
                    state = State::PLAYING;
                    clock.restart();
                }
                else if (event.key.code == Keyboard::Return &&
                         state == State::GAME_OVER)
                {
                    state = State::LEVELING_UP;
                }

                if (state == State::PLAYING)
                {
                    // Reloading
                    if (event.key.code == Keyboard::R)
                    {
                        if (bulletsSpare >= clipSize)
                        {
                            bulletsInClip = clipSize;
                            bulletsSpare -= clipSize;
                        }
                        else if (bulletsSpare > 0)
                        {
                            bulletsInClip = bulletsSpare;
                            bulletsSpare = 0;
                        }
                        else
                        {
                        }
                    }
                }
            }
        }// End event polling

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        if (state == State::PLAYING)
        {
            if (Keyboard::isKeyPressed(Keyboard::W))
            {
                player.moveUp();
            }
            else
            {
                player.stopUp();
            }

            if (Keyboard::isKeyPressed(Keyboard::S))
            {
                player.moveDown();
            }
            else
            {
                player.stopDown();
            }

            if (Keyboard::isKeyPressed(Keyboard::A))
            {
                player.moveLeft();
            }
            else
            {
                player.stopLeft();
            }
            if (Keyboard::isKeyPressed(Keyboard::D))
            {
                player.moveRight();
            }
            else
            {
                player.stopRight();
            }

            if (Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (gameTimeTotal.asMicroseconds() - lastPressed.asMicroseconds() > 1000 / fireRate &&
                    bulletsInClip > 0)
                {
                    bullets[currentBullet].shoot(
                        player.getCenter().x, player.getCenter().y,
                        mouseWorldPosition.x, mouseWorldPosition.y);

                    currentBullet++;
                    if (currentBullet >= BULLET_ARRAY_SIZE)
                    {
                        currentBullet = 0;
                    }
                    lastPressed = gameTimeTotal;
                    bulletsInClip--;
                }
            }
        }// End WASD while playing

        if (state == State::LEVELING_UP)
        {
            if (event.key.code == Keyboard::Num1)
            {
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num2)
            {
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num3)
            {
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num4)
            {
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num5)
            {
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num6)
            {
                state = State::PLAYING;
            }

            if (state == State::PLAYING)
            {
                arena.width = 500;
                arena.height = 500;
                arena.left = 0;
                arena.top = 0;

                int tileSize = createBackground(background, arena);
                player.spawn(arena, resolution, tileSize);

                healthPickup.setArena(arena);
                ammoPickup.setArena(arena);

                numZombies = 10;
                delete[] zombies;
                zombies = createHorde(numZombies, arena);
                numZombiesAlive = numZombies;

                clock.restart();
            }
        }// End levelling up

        // Update the frame
        if (state == State::PLAYING)
        {
            Time dt = clock.restart();
            gameTimeTotal += dt;
            float dtAsSeconds = dt.asSeconds();
            mouseScreenPosition = Mouse::getPosition();
            mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);
            spriteCrosshair.setPosition(mouseWorldPosition);

            player.update(dtAsSeconds, Mouse::getPosition());

            Vector2f playerPosition(player.getCenter());
            mainView.setCenter(player.getCenter());

            for (int i = 0; i < numZombies; i++)
            {
                if (zombies[i].isAlive())
                {
                    zombies[i].update(dt.asSeconds(), playerPosition);
                }
            }

            for (int i = 0; i < BULLET_ARRAY_SIZE; i++)
            {
                if (bullets[i].isInFlight())
                {
                    bullets[i].update(dtAsSeconds);
                }
            }

            healthPickup.update(dtAsSeconds);
            ammoPickup.update(dtAsSeconds);

            for (int i = 0; i < BULLET_ARRAY_SIZE; i++)
            {
                for (int j = 0; j < numZombies; j++)
                {
                    if (bullets[i].isInFlight() &&
                        zombies[j].isAlive())
                    {
                        if (bullets[i].getPosition().intersects(zombies[j].getPosition()))
                        {
                            bullets[i].stop();
                            if (zombies[j].hit())
                            {
                                score += 10;
                                if (score >= hiScore)
                                {
                                    hiScore = score;
                                }
                                numZombiesAlive--;

                                if (numZombiesAlive == 0)
                                {
                                    state = State::LEVELING_UP;
                                }
                            }
                        }
                    }
                }
            }// End zombie being shot

            for (int i = 0; i < numZombies; i++)
            {
                if (player.getPosition().intersects(zombies[i].getPosition()) &&
                    zombies[i].isAlive())
                {
                    if (player.hit(gameTimeTotal))
                    {
                        // More here later
                    }
                    if (player.getHealth() <= 0)
                    {
                        state = State::GAME_OVER;
                    }
                }
            } // End player touched

            // Has the player touched health pickup
            if (player.getPosition().intersects(healthPickup.getPosition()) &&
                healthPickup.isSpawned())
            {
                player.increaseHealthLevel(healthPickup.gotIt());
            }

            // Has the player touched ammo pickup
            if (player.getPosition().intersects(ammoPickup.getPosition()) &&
                ammoPickup.isSpawned())
            {
                bulletsSpare += ammoPickup.gotIt();
            }

            healthBar.setSize(Vector2f(player.getHealth() * 3, 70));

            framesSinceLastHUDUpdate++;
            if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
            {
                std::stringstream ssAmmo;
                std::stringstream ssScore;
                std::stringstream ssHiScore;
                std::stringstream ssWave;
                std::stringstream ssZombiesAlive;

                ssAmmo << bulletsInClip << "/" << bulletsSpare;
                ammoText.setString(ssAmmo.str());

                ssScore << "Score: " << score;
                scoreText.setString(ssScore.str());

                ssHiScore << "Hi Score: " << hiScore;
                hiScoreText.setString(ssHiScore.str());

                ssWave << "Wave: " << wave;
                waveNumberText.setString(ssWave.str());

                ssZombiesAlive << "Zombies:" << numZombiesAlive;
                zombiesRemainingText.setString(ssZombiesAlive.str());

                framesSinceLastHUDUpdate = 0;
            }

        }// End updating the scene

        // Draw the scene
        if (state == State::PLAYING)
        {
            window.clear();

            window.setView(mainView);

            window.draw(background, &textureBackground);

            for (int i = 0; i < numZombies; i++)
            {
                window.draw(zombies[i].getSprite());
            }

            for (int i = 0; i < BULLET_ARRAY_SIZE; i++)
            {
                if (bullets[i].isInFlight())
                {
                    window.draw(bullets[i].getShape());
                }
            }

            window.draw(player.getSprite());

            if (ammoPickup.isSpawned())
            {
                window.draw(ammoPickup.getSprite());
            }
            if (healthPickup.isSpawned())
            {
                window.draw(healthPickup.getSprite());
            }

            window.draw(spriteCrosshair);

            window.setView(hubView);

            window.draw(spriteAmmoIcon);
            window.draw(ammoText);
            window.draw(scoreText);
            window.draw(hiScoreText);
            window.draw(healthBar);
            window.draw(waveNumberText);
            window.draw(zombiesRemainingText);
        }

        if (state == State::LEVELING_UP)
        {
            window.draw(spriteGameOver);
            //window.draw(levelUpText);
        }

        if (state == State::PAUSED)
        {
            window.draw(pausedText);
        }

        if (state == State::GAME_OVER)
        {
            window.draw(spriteGameOver);
            window.draw(gameOverText);
            window.draw(scoreText);
            window.draw(hiScoreText);
        }

        window.display();
    } // End of game loop

    //
    delete[] zombies;

    return 0;
}