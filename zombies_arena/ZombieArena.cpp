#include <SFML/Graphics.hpp>
#include "ZombieArena.h"
#include "Player.h"
#include "TextureHolder.h"
#include "Bullet.h"

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
        }

        if (state == State::LEVELING_UP)
        {
        }

        if (state == State::PAUSED)
        {
        }

        if (state == State::GAME_OVER)
        {
        }

        window.display();
    } // End of game loop

    //
    delete[] zombies;

    return 0;
}