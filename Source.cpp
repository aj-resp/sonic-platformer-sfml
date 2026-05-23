#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <cstdlib> 
#include <ctime>
#include "player.h"
#include"SoundManager.h"
#include "enemi.h"

using namespace sf;
using namespace std;



//The Level class is an abstract base class ( it has pure virtual func so ) from which level1, level2, level3 and level4 ( boss level) are inherited from. it is defining all the common
//properties that each level should have i.e background, layout of map etc. 

class Level
{
protected:
    char** layout;
    Texture backgroundTex;
    Sprite background;
    Clock levelTimer;   //this tracks how long player has been in the levl currently being played 
    int levelNumber;    //describes what level isbeing currently played 


    int width, height;   // for the bg 

    string bgPath;     // file path
    string levelType;
    int ringCount;

    int screen_x = 1200;
    int screen_y = 900;



    Texture wallTex[4], breakableTex[4], platformTex[4];
    Sprite wallSprite[4];
    Sprite breakableSprite, platformSprite;

public:

    int getLevel()
    {
        return levelNumber;
    }

    Level(string bgFile, string type = "normal") : bgPath(bgFile), levelType(type), ringCount(0)
    {
        backgroundTex.loadFromFile(bgPath);
        background.setTexture(backgroundTex);

        int texW = backgroundTex.getSize().x;
        int texH = backgroundTex.getSize().y;

        width = texW / 64;   // tiles are 64*64
        height = texH / 64;

        layout = new char* [height];
        for (int i = 0; i < height; ++i)
            layout[i] = new char[width] {'\0'};

        background.setScale(1, 1);  // scale if needed
    }

    void initGraphics()
    {
        // Load wall textures and assign to sprites
        wallTex[0].loadFromFile("Data/brick1.png");
        wallTex[1].loadFromFile("Data/PP.png");
        wallTex[2].loadFromFile("Data/brick3.png");
        wallTex[3].loadFromFile("Data/brick1 - Copy.png");

        for (int i = 0; i < 4; ++i)
            wallSprite[i].setTexture(wallTex[i]);

        for (int i = 0; i < 4; ++i)
            breakableTex[i].loadFromFile("Data/breakable.png");

        for (int i = 0; i < 4; ++i)
            platformTex[i].loadFromFile("Data/platform.png");
    }

    bool loadLayoutFromFile(const string& filename)
    {

        ifstream file(filename);
        if (!file.is_open())
        {
            cout << "Failed " << filename << endl;
            return false;
        }

        string line;
        for (int i = 0; i < height; ++i)
        {
            if (!getline(file, line))
                break;
            for (int j = 0; j < width && j < (int)line.length(); ++j)
            {
                layout[i][j] = line[j];
            }
        }

        file.close();
        return true;
    }


    void display(RenderWindow& window, const int height, const int width, char** lvl,
        const int cell_size, float offsetX, float offsetY, Level* levelPtr)
    {
        int levelID = levelPtr->getLevel();

        breakableSprite.setTexture(breakableTex[levelID]);
        platformSprite.setTexture(platformTex[levelID]);

        int startRow = offsetY / cell_size;
        int startCol = offsetX / cell_size;
        int visibleRows = screen_y / cell_size + 2;
        int visibleCols = screen_x / cell_size + 2;

        for (int i = startRow; i < startRow + visibleRows && i < height; ++i) {
            for (int j = startCol; j < startCol + visibleCols && j < width; ++j) {
                if (i < 0 || j < 0) continue;

                float posX = j * cell_size - offsetX;
                float posY = i * cell_size - offsetY;

                if (lvl[i][j] == 'w')
                {
                    wallSprite[levelID].setPosition(posX, posY);
                    window.draw(wallSprite[levelID]);
                }
                else if (lvl[i][j] == 'x') {
                    breakableSprite.setPosition(posX, posY);
                    window.draw(breakableSprite);
                }
                else if (lvl[i][j] == 'y') {
                    platformSprite.setPosition(posX, posY);
                    window.draw(platformSprite);
                }
            }
        }
    }

    virtual void buildLevel() = 0;

    char** getLayout()
    {
        return layout;
    }
    Sprite& getBackground()
    {
        return background;
    }
    int getElapsedTime()
    {
        return levelTimer.getElapsedTime().asSeconds();
    }
    string getType()
    {
        return levelType;
    }
    void resetTimer()
    {
        levelTimer.restart();
    }

    // these are requiredd to change the level
    void setRingCount(int r)
    {
        ringCount = r;
    }
    int getRingCount() const
    {
        return ringCount;
    }

    virtual ~Level()
    {
        for (int i = 0; i < height; ++i)
            delete[] layout[i];
        delete[] layout;
    }
};

class Level1 : public Level
{
public:


    Level1() : Level("Data/bgg1.png") { levelNumber = 0; }



    void buildLevel() override
    {
        initGraphics();
        // to identify error of file
        if (!loadLayoutFromFile("Data/level1.txt"))
        {
            cout << "Error" << endl;
            return;
        }

        int count = 0;
        for (int i = 0; i < height; ++i)
            for (int j = 0; j < width; ++j)
                if (layout[i][j] == 'r')
                    count++;

        setRingCount(count);
    }

};

class Level2 : public Level
{
public:

    Level2() : Level("Data/bgg2.png") { levelNumber = 1; }


    void buildLevel() override
    {
        initGraphics();
        if (!loadLayoutFromFile("Data/level2.txt"))
        {
            cout << "eE" << endl;
            return;
        }

        int count = 0;
        for (int i = 0; i < height; ++i)
            for (int j = 0; j < width; ++j)
                if (layout[i][j] == 'r') count++;

        setRingCount(count);
    }

};

class Level3 : public Level {
public:
    Level3() : Level("Data/bg2.png") { levelNumber = 2; }


    void buildLevel() override {
        initGraphics();
        if (!loadLayoutFromFile("Data/level1.txt"))
        {
            cout << "errorr" << endl;
            return;
        }

        int count = 0;
        for (int i = 0; i < height; ++i)
            for (int j = 0; j < width; ++j)
                if (layout[i][j] == 'r') count++;

        setRingCount(count);
    }

};

class Level4 : public Level
{
public:
    Level4() : Level("Data/background_boss.png") { levelNumber = 3; }


    void buildLevel() override {
        initGraphics();
        if (!loadLayoutFromFile("Data/level4.txt")) {
            cout << "error" << endl;
            return;
        }

        int count = 0;
        for (int i = 0; i < height; ++i)
            for (int j = 0; j < width; ++j)
                if (layout[i][j] == 'r') count++;

        setRingCount(count);
    }

};


class Obstacle
{
protected:
    Sprite sprite;
    Texture texture;
    float x, y;  //initiall positions
    int health;
    float speed;
    float hitboxWidth;
    float hitboxHeight;

    int screen_x = 1200;
    int screen_y = 900;


public:
    Obstacle(string texturePath, float posX, float posY, int hp = 1, float spd = 0) : x(posX), y(posY), health(hp), speed(spd)
    {
        texture.loadFromFile(texturePath);
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        hitboxWidth = static_cast<float>(texture.getSize().x);    // this is the inbuilt function to know that size of the box
        hitboxHeight = static_cast<float>(texture.getSize().y);

    }



    // WE COULD JUST HAVE MADE A FUNCTION OF CHECKING COLLSION HERE 

    virtual void update(float playerX) = 0;
    virtual void draw(RenderWindow& window)
    {
        sprite.setPosition(x, y);
        window.draw(sprite);
    }

    virtual void takeDamage(int amount) {
        health -= amount;
        if (health < 0)
            health = 0;
    }

    virtual bool isDead() const
    {
        if (health <= 0)
            return 1;
        else
            return 0;
    }



    virtual ~Obstacle() {}
};




class PowerUps : public Obstacle
{
private:
    bool collected;
    int currentFrame;
    int totalFrames;
    int frameWidth, frameHeight;
    Clock frameClock;
    float frameDuration;

public:
    PowerUps(float posX = 0, float posY = 0) : Obstacle("Data/power.png", posX, posY, 1, 0), collected(false), currentFrame(0), totalFrames(4), frameDuration(0.1f) {

        frameWidth = 16;   // Actual frame width based on sprite (64 / 4)
        frameHeight = 16;  // Full height of sprite
        hitboxWidth = frameWidth;
        hitboxHeight = frameHeight;



        sprite.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));  // this is the initial thing that we have to do (frame)
    }


    void update(float) override // just for making sure that exact above function
    {
        if (!collected && frameClock.getElapsedTime().asSeconds() > frameDuration)
        {
            currentFrame = (currentFrame + 1) % totalFrames;
            sprite.setTextureRect(IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
            frameClock.restart();
        }
    }


    bool checkCollision(float px, float py, float pWidth, float pHeight)
    {
        if (collected)   // as their is no ring 
            return false;

        //in this function we have basically created an additional 'boxed' area around both the ring sprite and the player and the reason is to properly check for any overlapping collision
       //that occurs between the player and the ring, and when it does, the ring is collected. this is done cus if we just depend on the pixels of the sprites for the collision then a lot 
       //of times it doesnt collide even tho the collision very clearly occurs so


        float ringLeft = x;
        float ringRight = x + hitboxWidth;
        float ringTop = y;
        float ringBottom = y + hitboxHeight;

        float playerLeft = px;
        float playerRight = px + pWidth;
        float playerTop = py;
        float playerBottom = py + pHeight;

        if (ringRight > playerLeft && ringLeft < playerRight && ringBottom > playerTop && ringTop < playerBottom)
        {
            collected = true;
            return true;
        }

        return false;
    }

    void draw(RenderWindow& window, float offsetX, float offsetY)
    {
        if (!collected)
        {
            sprite.setPosition(x - offsetX, y - offsetY);
            sprite.setScale(3, 3);  // must scaling as size was way too small
            window.draw(sprite);
        }
    }



    bool isCollected() const
    {
        return collected;
    }
};



class SpecialPowerUps : public Obstacle
{
private:
    bool collected;
    int currentFrame;
    int totalFrames;
    int frameWidth, frameHeight;
    Clock frameClock;
    float frameDuration;

public:
    SpecialPowerUps(float posX = 0, float posY = 0)
        : Obstacle("Data/specialBoost.png", posX, posY, 1, 0),
        collected(false), currentFrame(0), totalFrames(4), frameDuration(0.1f)
    {
        frameWidth = 16;
        frameHeight = 16;
        hitboxWidth = frameWidth;
        hitboxHeight = frameHeight;

        sprite.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
    }

    void update(float) override
    {
        if (!collected && frameClock.getElapsedTime().asSeconds() > frameDuration)
        {
            currentFrame = (currentFrame + 1) % totalFrames;
            sprite.setTextureRect(IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
            frameClock.restart();
        }
    }

    bool checkCollision(float px, float py, float pWidth, float pHeight)
    {
        if (collected)
            return false;

        float left = x;
        float right = x + hitboxWidth;
        float top = y;
        float bottom = y + hitboxHeight;

        float playerLeft = px;
        float playerRight = px + pWidth;
        float playerTop = py;
        float playerBottom = py + pHeight;

        if (right > playerLeft && left < playerRight && bottom > playerTop && top < playerBottom)
        {
            collected = true;
            return true;
        }

        return false;
    }

    void draw(RenderWindow& window, float offsetX, float offsetY)
    {
        if (!collected)
        {
            sprite.setPosition(x - offsetX, y - offsetY);
            sprite.setScale(3, 3);
            window.draw(sprite);
        }
    }

    bool isCollected() const
    {
        return collected;
    }
};


class Energy : public Obstacle
{
private:
    bool collected;
    int currentFrame;
    int totalFrames;
    int frameWidth, frameHeight;
    Clock frameClock;
    float frameDuration;

public:
    Energy(float posX = 0, float posY = 0)
        : Obstacle("Data/ring - Copy.png", posX, posY, 1, 0), collected(false), currentFrame(0), totalFrames(4), frameDuration(0.1f) {

        frameWidth = 16;   // Actual frame width based on sprite (64 / 4)
        frameHeight = 16;  // Full height of sprite
        hitboxWidth = frameWidth;
        hitboxHeight = frameHeight;

        //  sprite.setOrigin(frameWidth / 2, frameHeight / 2);

        sprite.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
    }


    void update(float) override // just for compatibility
    {
        if (!collected && frameClock.getElapsedTime().asSeconds() > frameDuration)
        {
            currentFrame = (currentFrame + 1) % totalFrames;
            sprite.setTextureRect(IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
            frameClock.restart();
        }
    }


    bool checkCollision(float px, float py, float pWidth, float pHeight)
    {
        if (collected)   // as their is no ring 
            return false;

        float ringLeft = x;
        float ringRight = x + hitboxWidth;
        float ringTop = y;
        float ringBottom = y + hitboxHeight;

        float playerLeft = px;
        float playerRight = px + pWidth;
        float playerTop = py;
        float playerBottom = py + pHeight;

        if (ringRight > playerLeft && ringLeft < playerRight && ringBottom > playerTop && ringTop < playerBottom)
        {
            collected = true;
            return true;
        }

        return false;
    }

    void draw(RenderWindow& window, float offsetX, float offsetY)
    {
        if (!collected)
        {
            sprite.setPosition(x - offsetX, y - offsetY);
            sprite.setScale(3, 3);  // Optional scaling
            window.draw(sprite);
        }
    }



    bool isCollected() const
    {
        return collected;
    }
};


class Ring : public Obstacle
{
private:
    bool collected;
    int currentFrame;
    int totalFrames;
    int frameWidth, frameHeight;
    Clock frameClock;
    float frameDuration;

public:
    Ring(float posX = 0, float posY = 0)
        : Obstacle("Data/ring.png", posX, posY, 1, 0), collected(false), currentFrame(0), totalFrames(4), frameDuration(0.1f) {

        frameWidth = 16;   // Actual frame width based on sprite (64 / 4)
        frameHeight = 16;  // Full height of sprite
        hitboxWidth = frameWidth;
        hitboxHeight = frameHeight;

        //  sprite.setOrigin(frameWidth / 2, frameHeight / 2);

        sprite.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
    }


    void update(float) override // just for compatibility
    {
        if (!collected && frameClock.getElapsedTime().asSeconds() > frameDuration)
        {
            currentFrame = (currentFrame + 1) % totalFrames;
            sprite.setTextureRect(IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
            frameClock.restart();
        }
    }

    bool checkCollision(float px, float py, float pWidth, float pHeight)
    {
        if (collected)   // as their is no ring 
            return false;

        float ringLeft = x;
        float ringRight = x + hitboxWidth;
        float ringTop = y;
        float ringBottom = y + hitboxHeight;

        float playerLeft = px;
        float playerRight = px + pWidth;
        float playerTop = py;
        float playerBottom = py + pHeight;

        if (ringRight > playerLeft && ringLeft < playerRight && ringBottom > playerTop && ringTop < playerBottom)  // jsut checking the opposites basically
        {
            collected = true;
            return true;
        }

        return false;
    }

    void draw(RenderWindow& window, float offsetX, float offsetY)
    {
        if (!collected)
        {
            sprite.setPosition(x - offsetX, y - offsetY);
            sprite.setScale(3, 3);  // Optional scaling
            window.draw(sprite);
        }
    }



    bool isCollected() const
    {
        return collected;
    }
};

class Spike : public Obstacle
{
public:
    Spike(float posX, float posY) : Obstacle("Data/spike.png", posX, posY, 1, 0)
    {
    }

    void draw(RenderWindow& window, float offsetX, float offsetY)
    {

        sprite.setPosition(x - offsetX, y - offsetY);
        sprite.setScale(1, 1);  // Optional scaling
        window.draw(sprite);
    }
    void update(float playerX) override {}


    // copy paste from rings logic
    bool checkCollision(float px, float py, float pWidth, float pHeight)
    {
        float spikeLeft = x;
        float spikeRight = x + hitboxWidth;
        float spikeTop = y;
        float spikeBottom = y + hitboxHeight;

        float playerLeft = px;
        float playerRight = px + pWidth;
        float playerTop = py;
        float playerBottom = py + pHeight;

        if (spikeRight > playerLeft && spikeLeft < playerRight && spikeBottom > playerTop && spikeTop < playerBottom)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
};



class Menu
{
public:
    bool musicOn;
    bool soundOn;
    Music lvlMus;
    int leaderboardEntries = 10;

    string* leaderboard;
    int leaderboardCount;

    Menu()
    {
        musicOn = false;
        soundOn = true;
        leaderboardCount = 0;

        leaderboard = new string[leaderboardEntries];
        for (int i = 0; i < leaderboardEntries; i++)
        {
            leaderboard[i] = "";
        }
    }
    bool getSound()
    {
        return soundOn;
    }
    void initMusic() {
        if (!lvlMus.openFromFile("Data/beepboop.ogg"))  // music not loaded
        {
            cout << "Music not loaded" << endl;
            return;
        }
        lvlMus.setVolume(100);
        lvlMus.setLoop(true);
        if (musicOn)
            lvlMus.play();
    }

    void toggleMusic()
    {
        musicOn = !musicOn;

        if (!musicOn)
        {
            lvlMus.stop();
        }
        else
        {
            // Game handles playing appropriate music
        }

        cout << (musicOn ? "Music On" : "Music Off") << endl;
    }


    void toggleSound()
    {
        soundOn = !soundOn;
    }

    // displaying of menu here
    void display(RenderWindow& window, Text menuTexts[], int selectedItem)
    {
        for (int i = 0; i < 5; i++)// if selected item blue else it will be white
        {
            if (i == selectedItem)
            {
                menuTexts[i].setFillColor(Color::Blue);
            }
            else
            {
                menuTexts[i].setFillColor(Color::Black);
            }

            window.draw(menuTexts[i]);
        }

    }

    void displayOptions(RenderWindow& window, Text optionsTexts[], int selectedItem)
    {
        for (int i = 0; i < 3; ++i)
        {
            if (i == selectedItem)
            {
                optionsTexts[i].setFillColor(Color::Blue);
            }
            else
            {
                optionsTexts[i].setFillColor(Color::White);
            }

            window.draw(optionsTexts[i]);
        }
    }
};

class Companion
{
private:

    int screen_x = 1200;
    int screen_y = 900;

    Sprite sprite;
    Character* character;
    Texture* currentAnimTexture = nullptr;
    bool facingRight;  // no need for facing left as if not facing left it would be facing right obvi


    float x, y;
    float scaleX, scaleY;

    int frameWidth = 40;
    int frameHeight = 40;
    int currentFrame = 0;
    int totalFrames = 10;
    float frameDuration = 0.09f;
    Clock frameClock;

public:
    Companion(Character* charPtr, float startX, float startY, float sX = 2.5f, float sY = 2.5f)
        : character(charPtr), x(startX), y(startY), scaleX(sX), scaleY(sY)
    {
        //basically through this we are getting which character is currently playing and so the current animation 
        //texture of both the companions is set as this character
        currentAnimTexture = &character->getIdleRightTexture();
        sprite.setTexture(*currentAnimTexture);
        sprite.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
        sprite.setScale(scaleX, scaleY);
    }

    void follow(float leaderX, float leaderY, float xOffset, bool isFacingRight)
    {
        facingRight = isFacingRight;

        float targetX = leaderX + xOffset;
        float targetY = leaderY;

        x += (targetX - x) * 0.1f;
        y += (targetY - y) * 0.1f;

        sprite.setPosition(x, y);
    }



    void updateAnimation(Texture& animTexture, bool isAnimated)
    {
        if (&animTexture != currentAnimTexture)
        {
            currentAnimTexture = &animTexture;
            sprite.setTexture(animTexture);
            currentFrame = 0;
            frameClock.restart();

            // Set first frame
            sprite.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
        }

        if (isAnimated && frameClock.getElapsedTime().asSeconds() > frameDuration) // we are only taking frames if isAnimated is true which mean the character is moving and we initialized it down at update animation part that it is true or false
        {
            currentFrame = (currentFrame + 1) % totalFrames;
            frameClock.restart();

            sprite.setTextureRect(IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
        }

        sprite.setScale(scaleX, scaleY);
    }


    //basically here we have used conditional statements to see what is the current state of the main chracter, if the main character is moving then companions are also 
  //set to moving and if main character is idle then so are companions etc

    void updateTexture(bool facingRight, bool isRolling = false, bool isMoving = false)
    {
        if (isRolling)
        {
            if (facingRight)
            {
                updateAnimation(character->getRollRightTexture(), true);
            }
            else
            {
                updateAnimation(character->getRollLeftTexture(), true);
            }
        }
        else if (isMoving)
        {
            if (facingRight)
            {
                updateAnimation(character->getRunRightTexture(), true);
            }
            else
            {
                updateAnimation(character->getRunLeftTexture(), true);
            }
        }
        else
        {
            if (facingRight)
            {
                updateAnimation(character->getIdleRightTexture(), false);
            }
            else
            {
                updateAnimation(character->getIdleLeftTexture(), false);
            }
        }
    }



    void draw(RenderWindow& window, float offsetX, float offsetY)
    {
        sprite.setPosition(x - offsetX, y - offsetY);
        window.draw(sprite);
    }
};


class Game
{

public:
    bool isRunning;
    Menu menu;
    Music levelMusic;
    int Score;
    EnemyManager* enemyManager;
    SoundManager sounds;    //will be composed here so that there are different sound effects that are played upon various events like ring collection etc
    Player player;
    Level* levels[4];
    int currentLevel;




    float player_x = 100, player_y = 100, velocityX = 0, velocityY = 0, gravity = 1;
    float terminal_Velocity = 20, jumpStrength = -20, scale_x = 2.5, scale_y = 2.5;
    bool onGround = false;
    int raw_img_x = 24, raw_img_y = 35;
    int Pheight = raw_img_y * scale_y;
    int Pwidth = raw_img_x * scale_x;
    int hit_box_factor_x = 8 * scale_x;
    int hit_box_factor_y = 5 * scale_y;
    float offset_x = 0, offset_y = 0;

    Companion* companions[2];



    string levelMusicPaths[4] = {
    "Data/orchestral2.ogg",      // Level 1
    "Data/labrynth.ogg",      // Level 2
    "Data/orchestral3.ogg",      // Level 3
    "Data/boss_music.ogg"     // Level 4
    };

    int compIndex;
    Game()
    {
        isRunning = false;
        enemyManager = new EnemyManager(); // initialize once
        levels[0] = new Level1();
        levels[1] = new Level2();
        levels[2] = new Level3();
        levels[3] = new Level4();

        companions[0] = nullptr;
        companions[1] = nullptr;

        currentLevel = 0;
    }

    ~Game()
    {
        delete enemyManager;

    }

    int getScore()
    {
        return Score;
    }

    void startNewGame()
    {
        isRunning = true;
        Score = 0;
    }

    void savingName(const string& playerName)
    {
        ofstream file("name.txt");
        if (file.is_open())
        {
            file << playerName;
            file.close();
        }
        else
        {
            cout << "FailINGGGGG" << endl;
        }
    }

    void run(RenderWindow& window, bool isContinuing = false)
    {

        window.setFramerateLimit(60);
        string cheatBuffer = "";

        if (!isContinuing)
            startNewGame();  // Only start fresh game if not         //  This ensures the game actually starts

        Font font;
        font.loadFromFile("Data/NiseSegaSonic.ttf");


        Sprite mBGS1;
        Texture mBG1;
        mBG1.loadFromFile("Data/notmainbg.jpg");
        mBGS1.setTexture(mBG1);



        LoadingScreen(window);
        string playerName = getPlayerName(window, font);
        LoadingScreen(window);


        string menuItems[5] = { "Continue", "New Game", "Options", "Leaderboard", "Exit" };


        // text 3 options so three objects of text class
        Text menuTexts[5];
        int selectedItem = 0;

        for (int i = 0; i < 5; i++)
        {
            menuTexts[i].setFont(font);
            menuTexts[i].setString(menuItems[i]);
            menuTexts[i].setCharacterSize(45);
            menuTexts[i].setPosition(400, 300 + i * 100);
        }

        // Options menu setup  - similar to what i did above
        string optionsItems[3] = { "Toggle Music", "Toggle Sound", "Back" };
        Text optionsTexts[3];
        int selectedOptionItem = 0;

        // spikes
        const int MAX_SPIKES = 50;
        Spike* spikes[MAX_SPIKES];
        int spikeCount = 0;


        const int MAX_POWERS = 50;
        PowerUps* powerUps[MAX_POWERS];
        int powerUpCount = 0;


        // ringss
        const int MAX_RINGS = 50;
        Ring* rings[MAX_RINGS];  // we will have separate array for each obstacle , basically number fof object
        int ringCount = 0;
        int ringsCollected = 0;


        // energy
        const int MAX_ENERGY = 50;
        Energy* energy[MAX_ENERGY];
        int energyCount = 0;
        int energyCollected = 0;


        //special powerup
        const int MAX_SPECIALS = 10;
        SpecialPowerUps* specialPowerUps[MAX_SPECIALS];
        int specialPowerCount = 0;


        for (int i = 0; i < MAX_SPECIALS; ++i)
            specialPowerUps[i] = nullptr;
        specialPowerCount = 0;

        // Clear old rings
        for (int i = 0; i < MAX_RINGS; ++i)
        {
            rings[i] = nullptr;
            spikes[i] = nullptr;
            energy[i] = nullptr;

        }



        for (int i = 0; i < 3; ++i)
        {
            optionsTexts[i].setFont(font);
            optionsTexts[i].setString(optionsItems[i]);
            optionsTexts[i].setCharacterSize(50);
            optionsTexts[i].setPosition(450, 300 + i * 80);
        }

        // Game environment setup
        const int cell_size = 64;
        const int width = 100;
        const int height = 56;

        char** lvl = new char* [height];
        for (int i = 0; i < height; i++)
            lvl[i] = new char[width] {'\0'};
        char** lvls = levels[currentLevel]->getLayout(); // replaces old lvl

        levels[currentLevel]->buildLevel();
        levels[currentLevel]->resetTimer();






        spawnEntities(lvls, height, width, cell_size, rings, ringCount, MAX_RINGS, energy, energyCount, MAX_ENERGY, spikes, spikeCount, MAX_SPIKES, powerUps, powerUpCount, MAX_POWERS, specialPowerUps, specialPowerCount, MAX_SPECIALS, enemyManager);



        for (int i = 0; i < 3; ++i)
        {
            if (i != player.getCurrentCharIndex())
            {
                companions[compIndex++] = new Companion(player.getCharacter(i), player.getX() - 100 * (compIndex + 1), player.getY());
            }
        }


        Clock enemySpawnClock;
        float enemySpawnInterval = 10.0f;  // seconds

        // State flags
        bool inMenu = true;
        bool inOptions = false;
        bool inLeaderboard = false;

        Event ev;
        while (window.isOpen())
        {

            while (window.pollEvent(ev))
            {
                if (ev.type == Event::Closed)
                    window.close();

                // Main menu
                if (inMenu && ev.type == Event::KeyPressed)
                {
                    if (ev.key.code == Keyboard::Up)
                    {
                        selectedItem--;
                        if (selectedItem < 0) selectedItem = 4;  // back to the last one 
                    }
                    else if (ev.key.code == Keyboard::Down)
                    {
                        selectedItem++;
                        if (selectedItem > 4) selectedItem = 0;
                    }
                    else if (ev.key.code == Keyboard::Enter)
                    {

                        if (selectedItem == 0) {
                            string loadedName;
                            int currentLevel, score, health, power, charIndex, elapsedTime;
                            float x, y;

                            if (loadGameState(loadedName, currentLevel, score, health, power, x, y, charIndex, elapsedTime)) {
                                inMenu = false;
                                player.setPosition(x, y);
                                player.setHealth(health);
                                player.setPower(power);
                                player.setCharacterIndex(charIndex);
                                Score = score;

                                levels[currentLevel]->buildLevel();
                                levels[currentLevel]->resetTimer();
                                lvls = levels[currentLevel]->getLayout();

                                // Spawn all enemies, rings, powerups etc.
                                spawnEntities(lvls, height, width, cell_size, rings, ringCount, MAX_RINGS,
                                    energy, energyCount, MAX_ENERGY, spikes, spikeCount, MAX_SPIKES,
                                    powerUps, powerUpCount, MAX_POWERS, specialPowerUps, specialPowerCount, MAX_SPECIALS, enemyManager);

                                // Setup companions
                                updateCompanions(player, companions);

                                // Play background music
                                playLevelMusic(currentLevel);
                            }
                            else
                            {
                                cout << "No saved game found. Starting new game." << endl;
                                selectedItem = 1;  // fallback to new game 
                            }
                        }

                        else if (selectedItem == 1) {
                            // startNewGame();
                            inMenu = false;
                            menu.lvlMus.stop();
                            playLevelMusic(currentLevel);
                        }
                        else if (selectedItem == 2)
                        {
                            inOptions = true;
                            inMenu = false;
                            inLeaderboard = false;
                            continue;
                        }

                        else if (selectedItem == 3)
                        {
                            inMenu = false;
                            inOptions = false;
                            inLeaderboard = true;

                            bool viewingLeaderboard = true;
                            while (viewingLeaderboard)
                            {
                                Event leaderboardEvent;
                                while (window.pollEvent(leaderboardEvent))
                                {
                                    if (leaderboardEvent.type == Event::Closed)
                                        window.close();
                                    else if (leaderboardEvent.type == Event::KeyPressed || leaderboardEvent.type == Event::MouseButtonPressed)
                                        viewingLeaderboard = false;
                                }

                                window.clear();
                                showLeaderboard(window, font);
                                window.display();
                            }

                            inMenu = true;  // Go back to main menu after viewing
                            continue;
                        }


                        else if (selectedItem == 4) // exit selected so babyeee 
                        {
                            window.close();
                        }
                    }
                }

                // Options menu
                if (inOptions && ev.type == Event::KeyPressed)
                {
                    if (ev.key.code == Keyboard::Up)
                    {
                        selectedOptionItem--;
                        if (selectedOptionItem < 0)
                            selectedOptionItem = 2;
                    }
                    else if (ev.key.code == Keyboard::Down)
                    {
                        selectedOptionItem++;
                        if (selectedOptionItem > 2)
                            selectedOptionItem = 0;
                    }
                    else if (ev.key.code == Keyboard::Enter)
                    {
                        if (selectedOptionItem == 0)
                            menu.toggleMusic();

                        // Stop or play current level music accordingly
                        if (menu.musicOn)
                            playLevelMusic(currentLevel);
                        else
                            levelMusic.stop();
                        if (selectedOptionItem == 1)
                        {
                            menu.toggleSound();
                            sounds.setSoundEnabled(menu.getSound());  // this connects Menu to SoundManager
                        }

                        if (selectedOptionItem == 2)
                        {
                            inOptions = false;
                            inMenu = true;  // going back to the main menu easily
                        }
                    }

                }

                else if (inLeaderboard && ev.type == Event::KeyPressed)
                {
                    inLeaderboard = false;
                    inMenu = true;
                }

                if (!inMenu && !inOptions && !inLeaderboard && ev.type == Event::KeyPressed) {
                    if (ev.key.code == Keyboard::Z)
                    {
                        player.switchCharacter();
                        updateCompanions(player, companions);

                    }
                    else if (ev.key.code == Keyboard::X)
                    {

                        player.usingPower();
                        if (player.getPower() > 0)
                        {
                            player.useSpecial();
                        }
                    }
                }
                //if (!inMenu && !inOptions && !inLeaderboard && ev.type == Event::TextEntered)
                //{
                //    char c = ev.text.unicode;  // Read the character typed

                //    // Only accept the cheat letters: O, P, S
                //    if (c == 'O' || c == 'P' || c == 'S')
                //    {
                //        // Shift the previous characters left to make space for the new one
                //        cheatBuffer[0] = cheatBuffer[1];
                //        cheatBuffer[1] = cheatBuffer[2];
                //        cheatBuffer[2] = cheatBuffer[3];
                //        cheatBuffer[3] = c;  // Add the new character at the end

                //        // Check if the current 4 letter sequence is the cheat code
                //        if (cheatBuffer[0] == 'O' &&
                //            cheatBuffer[1] == 'O' &&
                //            cheatBuffer[2] == 'P' &&
                //            cheatBuffer[3] == 'S')
                //        {
                //            cout << "Hacker modeeee" << endl;
                //            player.activateSuperMode(enemyManager);  // Turn on cheat
                //            // Reset the cheat buffer so player must retype it again
                //            cheatBuffer[0] = cheatBuffer[1] = cheatBuffer[2] = cheatBuffer[3] = '\0';
                //        }
                //    }
                //}
                if (!inMenu && !inOptions && !inLeaderboard && ev.type == Event::TextEntered)
                {
                    char c = ev.text.unicode;

                    // Only accept uppercase O, P, S
                    if ((c == 'O' || c == 'P' || c == 'S'))
                    {
                        cheatBuffer += c;

                        // Keep only last 4 characters
                        if (cheatBuffer.length() > 4)
                        {
                            // Shift characters to the left manually
                            cheatBuffer[0] = cheatBuffer[1];
                            cheatBuffer[1] = cheatBuffer[2];
                            cheatBuffer[2] = cheatBuffer[3];
                            cheatBuffer[3] = c;
                            cheatBuffer = cheatBuffer.substr(0, 4);  // trimming 
                        }

                        if (cheatBuffer == "OOPS")
                        {
                            cout << "CHEAT ACTIVATEDDDDDDD LOLOLOL HACKERRRRRRRRRRRRRRRRRRRRR" << endl;
                            player.activateSuperMode(enemyManager);
                            cheatBuffer = "";  // Reset
                        }
                    }
                }


            }


            window.clear();            // Clear the screen each frame
            window.draw(mBGS1);
            if (inMenu)
            {
                menu.display(window, menuTexts, selectedItem);
            }
            else if (inOptions)
            {
                menu.displayOptions(window, optionsTexts, selectedOptionItem);
            }
            else if (inLeaderboard)
            {

            }

            else
            {
                float offsetX = player.getCameraOffsetX();
                float offsetY = player.getCameraOffsetY();

                player.update(lvls, cell_size);


                if (Keyboard::isKeyPressed(Keyboard::Escape))
                {
                    saveGameState(player, currentLevel, playerName, Score, levels[currentLevel]->getElapsedTime());
                    window.close();
                }
                update();
                window.draw(levels[currentLevel]->getBackground());

                // Timer check
                if (levels[currentLevel]->getElapsedTime() >= 180)
                {

                    saveScoreToFile(Score, playerName);  // Save the score

                    //cout << "Out of TME" << endl;
                    isRunning = false;

                    return;
                }

                // Health check
                if (player.getHealth() <= 0)

                {
                    saveScoreToFile(Score, playerName);  //  Save the score

                    //cout << "Out of h" << endl;
                    isRunning = false;
                    //    run();  // restart the menu/game loop
                    return;
                }



                if (player.getY() > 3600)
                {
                    saveScoreToFile(Score, playerName);
                    isRunning = false;

                    return;
                }



                bool trigger = player.levelTrigger();
                // Ring check
                if (ringsCollected >= levels[currentLevel]->getRingCount() && trigger)
                {
                    Sprite trans;
                    Texture transt;
                    transt.loadFromFile("Data/transition.png");
                    trans.setTexture(transt);

                    Clock k;
                    while (k.getElapsedTime().asSeconds() < 4)
                    {
                        // we did this so if in between closed no issue occurs
                        Event ev;
                        while (window.pollEvent(ev))
                        {
                            if (ev.type == Event::Closed)
                                window.close();
                        }

                        window.clear();
                        window.draw(trans);
                        window.display();
                    }


                    if (currentLevel == 3)
                    {
                        saveScoreToFile(Score, playerName);
                        gameOver(window);
                        isRunning = false;
                        //  run();  // restart the menu/game loop

                        return;
                    }
                    else
                    {
                        currentLevel++;
                        levels[currentLevel]->buildLevel();
                        levels[currentLevel]->resetTimer();
                        levelMusic.stop();
                        playLevelMusic(currentLevel);
                        lvls = levels[currentLevel]->getLayout();
                        player.resetPosition(560, 100);
                        enemyManager->clearAll();

                        // Clear and recreate rings
                        for (int i = 0; i < MAX_RINGS; ++i)
                            rings[i] = nullptr;
                        ringCount = 0;
                        ringsCollected = 0;
                        for (int i = 0; i < MAX_POWERS; ++i)
                            powerUps[i] = nullptr;
                        powerUpCount = 0;

                        for (int i = 0; i < MAX_SPIKES; ++i)
                            spikes[i] = nullptr;
                        spikeCount = 0;
                        ringsCollected = 0;
                        spawnEntities(lvls, height, width, cell_size, rings, ringCount, MAX_RINGS,
                            energy, energyCount, MAX_ENERGY, spikes, spikeCount, MAX_SPIKES,
                            powerUps, powerUpCount, MAX_POWERS, specialPowerUps, specialPowerCount, MAX_SPECIALS, enemyManager);


                    }
                }

                //drawing background
                Sprite& bg = levels[currentLevel]->getBackground();
                bg.setPosition(-player.getCameraOffsetX(), -player.getCameraOffsetY());
                window.draw(bg);

                Text uiText;
                uiText.setFont(font);
                uiText.setCharacterSize(30);
                uiText.setFillColor(Color::White);
                uiText.setString("Health: " + to_string(player.getHealth()) + "   Time: " + to_string(180 - levels[currentLevel]->getElapsedTime()) + "  Power Ups : " + to_string(player.getPower()) + "     Score: " + to_string(Score));
                uiText.setPosition(20, 20);

                for (int i = 0; i < ringCount; ++i)
                {

                    rings[i]->update(player.getX());

                    if (!rings[i]->isCollected() && rings[i]->checkCollision(player.getX(), player.getY(), player.getWidth(), player.getHeight()))
                    {
                        Score += 10;
                        ringsCollected++;
                        sounds.PlayRingSound();
                        cout << "Ring collected!" << endl;
                    }
                    rings[i]->draw(window, offsetX, offsetY);
                }
                for (int i = 0; i < energyCount; ++i)
                {
                    energy[i]->update(player.getX());

                    if (!energy[i]->isCollected() &&
                        energy[i]->checkCollision(player.getX(), player.getY(), player.getWidth(), player.getHeight()))
                    {

                        energyCollected++;
                        player.healthINC();
                        cout << "Energy collected!" << endl;
                    }

                    energy[i]->draw(window, offsetX, offsetY);
                }

                for (int i = 0; i < spikeCount; ++i)
                {
                    if (spikes[i]->checkCollision(player.getX(), player.getY(), player.getWidth(), player.getHeight()))
                    {
                        if (!player.isInvincibleNow())
                        {

                            player.takeDamage(1);
                            sounds.PlaySpikeSound();
                            cout << "Player hit a spike! Health reduced." << endl;
                        }
                    }
                }
                for (int i = 0; i < powerUpCount; ++i)
                {
                    powerUps[i]->update(player.getX());
                    if (powerUps[i]->checkCollision(player.getX(), player.getY(), player.getWidth(), player.getHeight()))
                    {
                        player.powerINC();
                        cout << "Power up sued" << endl;
                    }
                    powerUps[i]->draw(window, offsetX, offsetY);
                }





                for (int i = 0; i < spikeCount; ++i)
                {
                    spikes[i]->draw(window, offsetX, offsetY);
                }
                bool isRolling = Keyboard::isKeyPressed(Keyboard::Up);
                bool isMoving = Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Right);
                bool facingRight = Keyboard::isKeyPressed(Keyboard::Right);
                for (int i = 0; i < 2; ++i)
                {

                    float offset = (i + 1) * 100.0f;
                    if (player.isFacingRight())
                        offset = -offset;  // companions trail behind
                    else
                        offset = offset;   // companions trail to the right

                    companions[i]->follow(player.getX(), player.getY(), offset, player.isFacingRight());
                    companions[i]->updateTexture(player.isFacingRight(), isRolling, isMoving);
                    companions[i]->draw(window, player.getCameraOffsetX(), player.getCameraOffsetY());

                }


                player.draw(window, player.getCameraOffsetX(), player.getCameraOffsetY());
                player.updateInvincibility(1);
                enemyManager->updateAll(player.getX(), player.getY(), lvls, cell_size, player);

                enemyManager->checkPlayerCollision(player);


                levels[currentLevel]->display(window, height, width, lvls, cell_size, player.getCameraOffsetX(), player.getCameraOffsetY(), levels[currentLevel]);


                window.draw(uiText);

                enemyManager->drawAll(window, player.getCameraOffsetX(), player.getCameraOffsetY());
                for (int i = 0; i < specialPowerCount; ++i)
                {
                    specialPowerUps[i]->update(player.getX());

                    // Check collision only if not collected
                    if (!specialPowerUps[i]->isCollected() && specialPowerUps[i]->checkCollision(player.getX(), player.getY(), player.getWidth(), player.getHeight()))
                    {
                        player.useSpecialBoost();
                    }

                    specialPowerUps[i]->draw(window, offsetX, offsetY);
                }


                render(window);
            }

            window.display();
        }
    }


    void updateCompanions(Player& player, Companion* companions[2])
    {
        int compIndex = 0;
        for (int i = 0; i < 3; ++i) {
            if (i != player.getCurrentCharIndex())
            {
                if (companions[compIndex])
                {
                    delete companions[compIndex]; // Clean up old
                }
                companions[compIndex] = new Companion(player.getCharacter(i), player.getX() - 100 * (compIndex + 1), player.getY());
                compIndex++;
            }
        }
    }




    void continueGame()
    {

    }
    // removed this from main so main is more clear 

    void spawnEntities(char** lvls, int height, int width, int cell_size,
        Ring* rings[], int& ringCount, const int MAX_RINGS,
        Energy* energy[], int& energyCount, const int MAX_ENERGY,
        Spike* spikes[], int& spikeCount, const int MAX_SPIKES,
        PowerUps* power[], int& power_up, const int MAX_POWERS,
        SpecialPowerUps* specialPowerUps[], int& specialPowerCount, const int MAX_SPECIALS,
        EnemyManager* enemyManager)
    {
        ringCount = energyCount = spikeCount = power_up = specialPowerCount = 0;
        enemyManager->clearAll();

        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                float x = j * cell_size;
                float y = i * cell_size;

                switch (lvls[i][j])
                {
                case 'r':
                    if (ringCount < MAX_RINGS)
                        rings[ringCount++] = new Ring(x, y);
                    break;
                case 'e':
                    if (energyCount < MAX_ENERGY)
                        energy[energyCount++] = new Energy(x, y);
                    break;
                case 's':
                    if (spikeCount < MAX_SPIKES)
                        spikes[spikeCount++] = new Spike(x, y);
                    break;
                case 'p':
                    if (power_up < MAX_POWERS)
                        power[power_up++] = new PowerUps(x, y);
                    break;
                case 'h':
                    if (specialPowerCount < MAX_SPECIALS)
                        specialPowerUps[specialPowerCount++] = new SpecialPowerUps(x, y);
                    break;
                case 'm':
                {
                    Motobug* m = new Motobug(x, y);
                    m->setLevelReference(lvls, cell_size);
                    enemyManager->addEnemy(m);
                    break;
                }
                case 'c':
                {
                    CrabMeat* crab = new CrabMeat(x, y);
                    crab->setLevelReference(lvls, cell_size);
                    enemyManager->addEnemy(crab);
                    break;
                }
                case 'b':
                {
                    BatBrain* bat = new BatBrain(x, y);
                    bat->setLevelReference(lvls, cell_size);
                    enemyManager->addEnemy(bat);
                    break;
                }
                case 'B':
                {
                    BeeBot* bee = new BeeBot(x, y);
                    bee->setLevelReference(lvls, cell_size);
                    enemyManager->addEnemy(bee);
                    break;
                }
                case 'f':
                {
                    FinalBoss* boss = new FinalBoss(x, y);
                    boss->setLevelReference(lvls, cell_size);
                    enemyManager->addEnemy(boss);
                    break;
                }
                }

                // Clear special spawns from layout
                if (lvls[i][j] == 'm' || lvls[i][j] == 'c' || lvls[i][j] == 'b' ||
                    lvls[i][j] == 'B' || lvls[i][j] == 'f' || lvls[i][j] == 'h')
                {
                    lvls[i][j] = ' ';
                }
            }
        }
    }

    void saveScoreToFile(int score, const string& playerName)
    {
        const int MAX_ENTRIES = 10;
        string names[MAX_ENTRIES + 1];
        int scores[MAX_ENTRIES + 1];
        int count = 0;

        ifstream fin("Data/leaderboard.txt");
        while (fin >> names[count] >> scores[count])
        {
            count++;
            if (count >= MAX_ENTRIES)
                break;
        }
        fin.close();

        // Add current player's data
        names[count] = playerName;
        scores[count] = score;
        count++;




        for (int i = 0; i < count; i++)
        {
            for (int j = 0; j < count - 1; j++)
            {
                if (scores[j] < scores[j + 1])
                {
                    swap(scores[j], scores[j + 1]);
                    swap(names[j], names[j + 1]);
                }
            }
        }
        // Save top 10
        ofstream fout("Data/leaderboard.txt", ios::trunc);
        for (int i = 0; i < count && i < MAX_ENTRIES; ++i)
        {
            fout << names[i] << " " << scores[i] << endl;
        }
        fout.close();
    }


    void showLeaderboard(RenderWindow& window, Font& font)
    {
        const int MAX_ENTRIES = 10;
        string names[MAX_ENTRIES];
        int scores[MAX_ENTRIES];
        int count = 0;

        ifstream file("Data/leaderboard.txt");
        while (file >> names[count] >> scores[count])
        {
            count++;
            if (count >= MAX_ENTRIES)
                break;
        }
        file.close();

        Text heading;
        heading.setFont(font);
        heading.setString("Leaderboard");
        heading.setCharacterSize(50);
        heading.setPosition(400, 100);  // postion on screen
        heading.setFillColor(Color::White);
        window.draw(heading);

        if (count == 0)
        {
            Text empty;
            empty.setFont(font);
            empty.setString("No scores yet!");
            empty.setCharacterSize(30);
            empty.setPosition(420, 200);
            empty.setFillColor(Color::White);
            window.draw(empty);
        }
        else
        {
            for (int i = 0; i < count; ++i)
            {
                Text entry;
                entry.setFont(font);
                entry.setCharacterSize(30);
                entry.setFillColor(Color::White);
                entry.setString(to_string(i + 1) + ". " + names[i] + " - " + to_string(scores[i]));
                entry.setPosition(420, 180 + i * 40);
                window.draw(entry);
            }
        }



        Text back;
        back.setFont(font);
        back.setCharacterSize(25);
        back.setString("Press any key to return...");
        back.setPosition(420, 650);
        back.setFillColor(Color::White);
        window.draw(back);
    }



















    string getPlayerName(RenderWindow& window, Font& font);






    void loadGame()
    {
        cout << "Loading Game..." << endl;
    }

    void update()
    {

    }

    void render(RenderWindow& window)
    {

    }
    void gameOver(RenderWindow& window)
    {

        Texture mBG;
        if (!mBG.loadFromFile("Data/OVER.png"))
        {
            cout << "Failed to load loading screen background!" << endl;
            return;
        }

        Sprite mBGS;
        mBGS.setTexture(mBG);

        Clock clock;
        while (clock.getElapsedTime().asSeconds() < 3)
        {
            Event ev;
            while (window.pollEvent(ev))
            {
                if (ev.type == Event::Closed)
                    window.close();
            }

            window.clear();
            window.draw(mBGS);
            window.display();
        }
    }
    void LoadingScreen(RenderWindow& window)
    {

        Texture mBG;
        if (!mBG.loadFromFile("Data/mainbg.png"))
        {
            cout << "Failed to load loading screen background!" << endl;
            return;
        }

        Sprite mBGS;
        mBGS.setTexture(mBG);

        Clock clock;
        while (clock.getElapsedTime().asSeconds() < 3)
        {
            Event ev;
            while (window.pollEvent(ev))
            {
                if (ev.type == Event::Closed)
                    window.close();
            }

            window.clear();
            window.draw(mBGS);
            window.display();
        }
    }
    void saveGameState(Player& player, int currentLevel, const string& playerName, int score, int elapsedTime)
    {
        ofstream saveFile("Data/savegame.txt");
        if (!saveFile)
        {
            cout << "Failed" << endl;
            return;
        }

        saveFile << playerName << endl;
        saveFile << currentLevel << endl;
        saveFile << score << endl;
        saveFile << player.getHealth() << endl;
        saveFile << player.getPower() << endl;
        saveFile << player.getX() << " " << player.getY() << endl;
        saveFile << player.getCurrentCharIndex() << endl;
        saveFile << elapsedTime << endl;

        saveFile.close();
    }
    bool loadGameState(string& playerName, int& currentLevel, int& score, int& health,
        int& power, float& x, float& y, int& charIndex, int& elapsedTime)
    {
        ifstream loadFile("Data/savegame.txt");
        if (!loadFile)
            return false;

        loadFile >> playerName;
        loadFile >> currentLevel;
        loadFile >> score;
        loadFile >> health;
        loadFile >> power;
        loadFile >> x >> y;
        loadFile >> charIndex;
        loadFile >> elapsedTime;

        loadFile.close();
        return true;
    }

    void playLevelMusic(int levelNum);

};

// Function Prototypes
bool showPlayAgainScreen(RenderWindow& window, Font& font)
{
    Text question;
    question.setFont(font);
    question.setString("Do you want to play again?");
    question.setCharacterSize(50);
    question.setFillColor(Color::White);
    question.setPosition(100, 300);

    Text yesOption, noOption;
    yesOption.setFont(font);
    yesOption.setString("Yes (Y)");
    yesOption.setCharacterSize(40);
    yesOption.setFillColor(Color::Green);
    yesOption.setPosition(350, 400);

    noOption.setFont(font);
    noOption.setString("No (N)");
    noOption.setCharacterSize(40);
    noOption.setFillColor(Color::Red);
    noOption.setPosition(650, 400);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Y)
                    return true;
                else if (event.key.code == Keyboard::N)
                    return false;
            }
        }

        window.clear();
        window.draw(question);
        window.draw(yesOption);
        window.draw(noOption);
        window.display();
    }

    return false;  // Fallback
}


int main()
{

    int screen_x = 1200;
    int screen_y = 900;

    Font font;
    font.loadFromFile("Data/NiseSegaSonic.ttf");
    RenderWindow window(VideoMode(screen_x, screen_y), "AJ game only", Style::Close);

    window.setFramerateLimit(60);
    Game game;
    game.menu.initMusic();

    game.sounds.setSoundEnabled(game.menu.getSound());  // this works —sounds is a member of Game
    game.enemyManager->setSoundEnabled(game.menu.getSound());  // sync enemy sounds with menu toggle


    do
    {
        game.run(window, true);  // Now uses isContinuing flag inside
    } while (showPlayAgainScreen(window, font));


    return 0;
}

string Game::getPlayerName(RenderWindow& window, Font& font)
{

    Text namePrompt("Enter Your Name (press Enter to submit):", font, 20);
    namePrompt.setFillColor(Color::White);
    namePrompt.setPosition(250, 150);

    Text nameInput("", font, 24);
    nameInput.setFillColor(Color::White);
    nameInput.setPosition(260, 210);

    string playerName = "";
    bool nameEntered = false;

    while (!nameEntered && window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            else if (event.type == Event::TextEntered)
            {
                if (event.text.unicode == 8 && !playerName.empty()) // backspace
                {
                    playerName.resize(playerName.size() - 1);
                }
                else if ((event.text.unicode == '\r' || event.text.unicode == '\n') && !playerName.empty()) // enter
                {
                    nameEntered = true;
                    savingName(playerName);

                }
                else if (event.text.unicode >= 32 && event.text.unicode <= 126 && playerName.length() < 15)
                {
                    playerName += static_cast<char>(event.text.unicode);
                }

                nameInput.setString(playerName);
            }
        }

        window.clear();
        window.draw(namePrompt);
        window.draw(nameInput);
        window.display();
    }
    // Optional show feedback after name entry, for good impression 
    Text thanks("Thanks, " + playerName + "!", font, 24);
    thanks.setFillColor(Color::White);
    thanks.setPosition(260, 270);

    Clock pauseClock;
    while (pauseClock.getElapsedTime().asSeconds() < 1.5f) // pause for 1.5 seconds
    {
        window.clear();
        window.draw(namePrompt);
        window.draw(nameInput);
        window.draw(thanks);
        window.display();
    }

    return playerName;

}

void Player::activateSuperMode(EnemyManager* enemyManager)
{
    if (isSuperMode)
        return;

    superChar = new SuperCharacter();
    activeChar = superChar;

    isSuperMode = true;
    isInvincible = true;
    isFlying = true;

    moveSpeed += 10;
    velocityXMAX += 10;
    acceleration = 2.0f;

    specialDuration = superDuration;     //  to keep flying for full duration
    isUsingSpecial = true;

    superModeClock.restart();
    invincibilityClock.restart();
    specialAbilityClock.restart();

    updateAnimation(activeChar->getIdleRightTexture());

    PlayerSounds.PlayFlyingSound();

    for (int i = 0; i < MAX_ENEMIES; ++i)
    {
        Enemy* e = enemyManager->getEnemy(i);
        if (e && !e->isDead())
        {
            if (e->getName() == "FinalBoss")
                e->takeDamage(e->getHealth() / 2);
            else
                e->takeDamage(10000);
        }
    }

    cout << "Super Mode ";
}


void Game::playLevelMusic(int levelNum)
{
    if (!menu.musicOn)
        return; // Don't play if music is toggled off

    if (!levelMusic.openFromFile(levelMusicPaths[levelNum]))
    {
        cout << "Failed to load music for level: " << levelNum << endl;
        return;
    }

    levelMusic.setLoop(true);
    levelMusic.setVolume(100);
    levelMusic.play();
}


