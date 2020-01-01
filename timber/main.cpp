
#include <sstream>
#include <SFML/Graphics.hpp>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

int randInt(int timer, int range);

void updateBranches(int seed);
const int NUM_BRANCHES = 6;
sf::Sprite branches[NUM_BRANCHES];
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

int main()
{
    sf::VideoMode vm(WINDOW_WIDTH, WINDOW_HEIGHT);
    sf::RenderWindow window(vm, "Timber!!!");
    sf::View view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    window.setView(view);

    sf::Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");
    sf::Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);

    sf::Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    sf::Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    sf::Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    sf::Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);
    bool beeActive = false;
    float beeSpeed = 0.0f;

    sf::Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");
    sf::Sprite spriteCloud1;
    sf::Sprite spriteCloud2;
    sf::Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    int cloudWidth = 400;
    spriteCloud1.setPosition(randInt(10, WINDOW_WIDTH) - cloudWidth, 0);
    spriteCloud2.setPosition(randInt(20, WINDOW_WIDTH) - cloudWidth, 250);
    spriteCloud3.setPosition(randInt(30, WINDOW_WIDTH) - cloudWidth, 500);

    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    int score = 0;
    sf::Text messageText;
    sf::Text scoreText;
    sf::Font font;
    font.loadFromFile("fonts/KOMIKAP.ttf");
    messageText.setFont(font);
    scoreText.setFont(font);
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    messageText.setFillColor(sf::Color::White);
    scoreText.setFillColor(sf::Color::White);

    sf::FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f,
                          textRect.top + textRect.height / 2.0f);
    messageText.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
    scoreText.setPosition(20, 20);

    sf::Clock clock;

    // Time bar
    sf::RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(sf::Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(sf::Color::Red);
    timeBar.setPosition((WINDOW_WIDTH / 2) - timeBarStartWidth / 2, 980);

    sf::Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    bool paused = true;

    sf::Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        branches[i].setOrigin(220, 20);
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                paused = false;

                // Reset the time and the score
                score = 0;
                timeRemaining = 5;
            }
        }

        if (!paused)
        {
        /**
         * Update the scene
         */
        sf::Time dt = clock.restart();

        timeRemaining -= dt.asSeconds();
        timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
        if (timeRemaining <= 0.0f)
        {
            // Pause the game
            paused = true;

            // Change the message shown to the player
            messageText.setString("Out of time!!");

            // Reposition the text based on its new size
            sf::FloatRect textRect = messageText.getLocalBounds();
            messageText.setOrigin(textRect.left + textRect.width / 2.0f,
                                  textRect.top + textRect.height / 2.0f);
            messageText.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
        }

        // manage bee
        if (!beeActive)
        {
            srand((int)time(0) * 10);
            beeSpeed = (rand() % 200) + 200;

            srand((int)time(0) * 10);
            float height = (rand() % 500) + 500;
            spriteBee.setPosition(2000, height);
            beeActive = true;
        }
        else
        {
            spriteBee.setPosition(
                spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
                spriteBee.getPosition().y
            );

            if (spriteBee.getPosition().x < -100)
            {
                beeActive = false;
            }
        }

        // manage cloud
        if (!cloud1Active)
        {
            cloud1Speed = randInt(10, 200) + 1;
            float height = randInt(10, 150);
            spriteCloud1.setPosition(spriteCloud1.getPosition().x, height);
            cloud1Active = true;
        }
        else
        {
            spriteCloud1.setPosition(
                spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()),
                spriteCloud1.getPosition().y
            );
            if (spriteCloud1.getPosition().x > WINDOW_WIDTH)
            {
                spriteCloud1.setPosition(
                    -cloudWidth,
                    spriteCloud1.getPosition().y
                );
                cloud1Active = false;
            }
        }

        if (!cloud2Active)
        {
            cloud2Speed = randInt(20, 200) + 1;
            float height = randInt(20, 300) + 150;
            spriteCloud2.setPosition(spriteCloud2.getPosition().x, height);
            cloud2Active = true;
        }
        else
        {
            spriteCloud2.setPosition(
                spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()),
                spriteCloud2.getPosition().y
            );
            if (spriteCloud2.getPosition().x > WINDOW_WIDTH)
            {
                spriteCloud2.setPosition(
                    -cloudWidth,
                    spriteCloud2.getPosition().y
                );
                cloud2Active = false;
            }
        }

        if (!cloud3Active)
        {
            cloud3Speed = randInt(30, 200) + 1;
            float height = randInt(30, 450) + 150;
            spriteCloud3.setPosition(spriteCloud3.getPosition().x, height);
            cloud3Active = true;
        }
        else
        {
            spriteCloud3.setPosition(
                spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()),
                spriteCloud3.getPosition().y
            );
            if (spriteCloud3.getPosition().x > WINDOW_WIDTH)
            {
                spriteCloud3.setPosition(
                    -cloudWidth,
                    spriteCloud3.getPosition().y
                );
                cloud3Active = false;
            }
        }

        // Update the score text
        std::stringstream ss;
        ss << "Score = " << score;
        scoreText.setString(ss.str());

        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            float height = i * 150;
            if (branchPositions[i] == side::LEFT)
            {
                branches[i].setPosition(610, height);
                branches[i].setRotation(180);
            }
            else if (branchPositions[i] == side::RIGHT)
            {
                branches[i].setPosition(1330, height);
                branches[i].setRotation(0);
            }
            else
            {
                // hide the branch
                branches[i].setPosition(3000, height);
            }
        }

        } // End of paused

        window.clear();

        window.draw(spriteBackground);

        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        window.draw(spriteTree);

        window.draw(spriteBee);

        window.draw(scoreText);
        window.draw(timeBar);

        if (paused)
        {
            window.draw(messageText);
        }

        window.display();
    }

    return 0;
}

// harmful name haha...
int randInt(int timer, int range)
{
    if (range == 0) return 0;
    srand((int) time(0) * timer);
    return rand() % range;
}

void updateBranches(int seed)
{
    for (int j = NUM_BRANCHES - 1; j > 0; j--)
    {
        branchPositions[j] = branchPositions[j - 1];
    }

    srand((int) time(0) + seed);
    int r = (rand() % 5);
    switch(r)
    {
    case 0:
        branchPositions[0] = side::LEFT;
        break;
    case 1:
        branchPositions[0] = side::RIGHT;
        break;
    default:
        branchPositions[0] = side::NONE;
    }
}