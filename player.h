#pragma once

#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <cstdlib> 
#include <ctime>
#include "Character.h"
#include"SoundManager.h"

using namespace sf;
using namespace std;


class Enemy;
class EnemyManager;
class Player
{
private:

    bool isFlying = false;  // Already added
    float flightSpeed = 5.0f;  // How fast Tails can move up/down during flight

    int screen_xx = 1200;
    int screen_yy = 900;
    bool hasSpecialBoost = false;
    SoundManager PlayerSounds;  //sounds will be played when players do various things like flying for tails etc 


    Sprite sprite;
    float x, y;
    float velocityX, velocityY;

    float velocityXMAX, velocityYMAX;
    float gravity;
    float terminalVelocity;  // the player reaches this speed when falling gradually
    float jumpStrength;

    int Pheight, Pwidth; // players dimension
    int hitboxOffsetX, hitboxOffsetY;  // this is basically the little gap we need to keep
    float scaleX, scaleY;
    float moveSpeed;

    // bools 
    bool facingRight = true;
    bool onGround;

    // Character System
    Character* characters[3]; // Sonic, Tails, Knuckles
    int currentCharIndex;
    Character* activeChar;

    Texture* currentAnimTexture = nullptr;


    int total_power;
    // Animation

    string currentAnimationState;

    float posX, posY;     // Player's position
    //int health;           // Current health
    int power;            // Number of power-ups
    //  int currentCharacterIndex;  // Currently selected character


    int frameWidth = 40;
    int frameHeight = 40;

    int currentFrame = 0;

    int totalFrames = 10;  // different for each but minimun this so i just used this for now
    Clock frameClock;
    float frameDuration = 0.08f;





    bool isSuperMode = false;
    Clock superModeClock;
    float superDuration = 15.0f;
    Character* superChar = nullptr;


    Clock specialAbilityClock;
    bool isUsingSpecial = false;
    float specialDuration = 0;
    float originalMoveSpeed;

    bool isInvincible = false;
    Clock invincibilityClock;
    float invincibilityDuration = 1.0f; // 1 second

    bool isPunching = false;


    float acceleration;

    int health = 30;
public:

    Player(string texturePath, float startX, float startY, float scaleX = 2.5, float scaleY = 2.5) :currentCharIndex(0) // so sonic is loaded first
    {


        velocityXMAX = 30;
        velocityYMAX = 30;
        sprite.setScale(scaleX, scaleY);

        x = startX;
        y = startY;

        this->scaleX = scaleX;
        this->scaleY = scaleY;

        int rawWidth = 24;
        int rawHeight = 35;
        Pwidth = rawWidth * scaleX;
        Pheight = rawHeight * scaleY;

        hitboxOffsetX = 8 * scaleX;
        hitboxOffsetY = 5 * scaleY;

        velocityX = 0;
        velocityY = 0;
        gravity = 1;
        terminalVelocity = 20;
        jumpStrength = -20;
        moveSpeed = 10;
        onGround = false;

        characters[0] = new Sonic();

        acceleration = 0.5f;
        characters[1] = new Tails();
        characters[2] = new Knuckles();
        activeChar = characters[0];
        total_power = 100;

        sprite.setTexture(activeChar->getIdleRightTexture());
        updateAnimation(activeChar->getIdleRightTexture());
    }
    Player() : Player("Data/0left_still.png", 560, 100) {}

    void levelUpdating(int levelNum)
    {
        switch (levelNum)
        {
        case 0: // Labyrinth Zone - normal
            moveSpeed = 10;
            velocityXMAX = 30;
            gravity = 1;
            terminalVelocity = 20;
            break;

        case 1: // Ice Cap Zone - slippery, fast
            moveSpeed = 13;
            velocityXMAX = 50;
            gravity = 1;
            terminalVelocity = 20;
            break;

        case 2: // Death Egg Zone - low gravity
            moveSpeed = 10;
            velocityXMAX = 30;
            gravity = 0.5f;
            terminalVelocity = 10;
            break;

        case 3: // Final Boss - no change
            break;
        }
    }
    void takeDamage(int amount)
    {
        if (!isInvincible)
        {
            health -= amount;
            if (health < 0) health = 0;
            isInvincible = true;
            invincibilityClock.restart();
        }
    }

    void activateSpecialBoost()
    {
        hasSpecialBoost = true;
    }
    void setPosition(float x, float y) {
        posX = x;
        posY = y;
    }

    void setHealth(int h) {
        health = h;
    }

    void setPower(int p) {
        power = p;
    }
    void setCharacterIndex(int index) {
        currentCharIndex = index;
        activeChar = characters[index];
    }


    void resetSpecialBoost() {
        hasSpecialBoost = false;
    }

    bool isSpecialBoostActive() const {
        return hasSpecialBoost;
    }

    void resetPosition(float newX, float newY)
    {
        x = newX;
        y = newY;
        velocityX = 0;
        velocityY = 0;
        onGround = false;
        isFlying = false;
        isUsingSpecial = false;
        sprite.setPosition(x, y);  // optional but clean
    }

    int getHealth() const
    {
        return health;
    }
    void bounceUp()
    {
        velocityY = -12; // small jump when bouncing on enemy
        onGround = false;
    }

    void usingPower()
    {
        total_power--;
        if (total_power < 0)
        {
            total_power = 0;
        }
    }
    int getPower()
    {
        return total_power;
    }
    void powerINC()
    {
        total_power++;
    }

    void healthINC()
    {
        health++;
    }
    // this was used for the collision purpose 
    float getWidth() const
    {
        return Pwidth;
    }
    float getHeight()const
    {
        return Pheight;
    }

    bool levelTrigger()
    {

        if (x >= 6100 && x <= 6400 && y >= 0 && y <= 3 * Pheight)
        {
            return true;
        }
        else
            return false;
    }
    //used in camera settings
    float getCameraOffsetX() const
    {
        return x -
            screen_xx / 2;
    }
    float getCameraOffsetY() const
    {
        return y - screen_yy / 2;
    }

    bool isInvincibleNow() const {
        return isInvincible;
    }


    void updateInvincibility(int n)
    {
        if (!isUsingSpecial)
        {
            if (isInvincible && invincibilityClock.getElapsedTime().asSeconds() > n) // one
            {
                isInvincible = false;
            }
        }
        else
        {
            if (activeChar->getName() == "Knuckles")
            {
                if (isInvincible && invincibilityClock.getElapsedTime().asSeconds() > 15) // one
                {
                    isInvincible = false;
                }

            }
        }
    }

    void switchCharacter()
    {

        currentCharIndex = (currentCharIndex + 1) % 3;
        cout << characters[currentCharIndex]->getName() << endl;
        activeChar = characters[currentCharIndex];
        currentFrame = 0;
        frameClock.restart();
        sprite.setTexture(activeChar->getIdleRightTexture());
    }


    void activateSuperMode(EnemyManager* enemyManager);



    // This runs when the player presses X
    void useSpecial()
    {
        if (isUsingSpecial || total_power <= 0)
            return;

        string name = activeChar->getName();

        if (name == "Sonic")
        {
            originalMoveSpeed = moveSpeed;
            moveSpeed += 15;
            velocityXMAX = moveSpeed;
            acceleration = 2.0f;

            specialDuration = 5.0f;
        }
        else if (name == "Tails")
        {
            isFlying = true;
            PlayerSounds.PlayFlyingSound();
            specialDuration = 7.0f;
        }
        else if (name == "Knuckles")
        {
            specialDuration = 3.0f;
        }

        specialAbilityClock.restart();
        isUsingSpecial = true;
        usingPower(); // reduce power count
    }

    void useSpecialBoost()
    {
        if (isUsingSpecial)
            return;

        string name = activeChar->getName();

        if (name == "Sonic")
        {
            originalMoveSpeed = moveSpeed;
            moveSpeed += 15;
            velocityXMAX = moveSpeed;
            acceleration = 2.0f;
            specialDuration = 8.0f;
        }
        else if (name == "Tails")
        {
            isFlying = true;
            PlayerSounds.PlayFlyingSound();
            specialDuration = 10.0f;
        }
        else if (name == "Knuckles")
        {
            isInvincible = true;
            invincibilityClock.restart();
            specialDuration = 15.0f;
        }

        specialAbilityClock.restart();
        isUsingSpecial = true;
        resetSpecialBoost();  // Only reset here
    }



    void setIdleTexture()
    {
        Texture& idleTex = facingRight ? activeChar->getIdleRightTexture() : activeChar->getIdleLeftTexture();

        // Only set if texture actually changes mtlb leeft to right roll etc
        if (&idleTex != currentAnimTexture)
        {
            currentAnimTexture = &idleTex;
            sprite.setTexture(idleTex);

            sprite.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));  //  so only the required state is called  , basically we are giving the coordinates of the png


            sprite.setScale(scaleX, scaleY);

        }
    }


    //basically we enter this function through the updateTexture function below and what this function does is simply just set the companion sprites to the current 
 //state that was decided by the updateTexture function. Also, here "isAnimated" basically checks whether the current sprite has frames or is just a single sprite
 // and if it has frames then we use intRect on it otherwise we just use it as normal
    void updateAnimation(Texture& animTexture)
    {
        if (&animTexture != currentAnimTexture)
        {
            currentAnimTexture = &animTexture;
            sprite.setTexture(animTexture);
            currentFrame = 0;
            frameClock.restart();


            // this is the initial thing that we have to do where we select the 0th frame of the texture which has a number of 
     //parts that we will use for animation. The update function below will be called into main loop which is going to go from 0 to 4 then again 0 to 4 and so on and basically all that
     //with frameWidth = 16 so 0*16 = 0, 1*16 = 1, 2*16 = 32, 3*16 = 48, 4*16 = 64 which will be the selected x-coordinate of the sprite from which we want to draw it. The y-coordinate
     //stays the same and thus by calling update function continously, the sprite will be put into animation
            sprite.setTextureRect(IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
        }

        if (frameClock.getElapsedTime().asSeconds() > frameDuration)
        {
            currentFrame = (currentFrame + 1) % totalFrames; // rotation of array kinda
            frameClock.restart();
            sprite.setTextureRect(IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
        }
        sprite.setScale(scaleX, scaleY);
    }
    void update(char** lvl, int cellSize)
    {
        if (isSuperMode && superModeClock.getElapsedTime().asSeconds() > superDuration) {
            isSuperMode = false;
            isInvincible = false;
            moveSpeed -= 10;
            velocityXMAX -= 10;

            delete superChar;
            superChar = nullptr;

            switchCharacter(); // back to previous normal character
        }

        if (isFlying) //  Tails Flight Mode Active
        {
            flying(lvl, cellSize);

            if (specialAbilityClock.getElapsedTime().asSeconds() > specialDuration)
            {
                isFlying = false;
                isUsingSpecial = false;
                cout << "Tails Flight Ended!" << endl;
            }
            return;
        }

        // Normal movement code here
        normalMovementUpdate(lvl, cellSize);

    }


    void normalMovementUpdate(char** lvl, int cellSize)
    {
        //  float acceleration = 0.5f;       // how fast the character speeds up
        float friction = 0.3f;           // how fast the character slows down
        float maxSpeed = moveSpeed;      // use current moveSpeed 
        bool isMoving = false;

        // Handle special power timeout
        if (isUsingSpecial)
        {
            if ((isSuperMode && superModeClock.getElapsedTime().asSeconds() > superDuration) ||
                (!isSuperMode && specialAbilityClock.getElapsedTime().asSeconds() > specialDuration))
            {
                if (activeChar->getName() == "Sonic")
                {
                    moveSpeed = originalMoveSpeed;
                    velocityXMAX = moveSpeed;
                    acceleration = 0.5f;
                    cout << "Sonic Speed Boost Ended.\n";
                }
                isUsingSpecial = false;
            }

        }

        // Movement input
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            velocityX -= acceleration;
            if (velocityX < -maxSpeed) velocityX = -maxSpeed;

            facingRight = false;
            isMoving = true;

            if (Keyboard::isKeyPressed(Keyboard::Up))
                updateAnimation(activeChar->getRollLeftTexture());
            else
                updateAnimation(activeChar->getRunLeftTexture());
        }
        else if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            velocityX += acceleration;
            if (velocityX > maxSpeed) velocityX = maxSpeed;

            facingRight = true;
            isMoving = true;

            if (Keyboard::isKeyPressed(Keyboard::Up))
                updateAnimation(activeChar->getRollRightTexture());
            else
                updateAnimation(activeChar->getRunRightTexture());
        }
        else
        {
            // Apply friction if no left/right keys pressed
            if (velocityX > 0)
            {
                velocityX -= friction;
                if (velocityX < 0) velocityX = 0;
            }
            else if (velocityX < 0)
            {
                velocityX += friction;
                if (velocityX > 0) velocityX = 0;
            }
        }

        // Jump
        if (Keyboard::isKeyPressed(Keyboard::Up) && onGround)
        {
            velocityY = jumpStrength;
            onGround = false;
            isMoving = true;
            updateAnimation(activeChar->getJumpTexture());
        }

        // Idle animation
        if (!isMoving && onGround)
        {
            setIdleTexture();
        }

        // Collision and movement
        handleHorizontalCollision(lvl, cellSize);
        handleVerticalCollision(lvl, cellSize);
    }





    void flying(char** lvl, int cellSize)
    {
        float acceleration = 0.5f;
        float maxSpeed = 6.0f;
        bool isMoving = false;

        // Horizontal Movement
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            velocityX -= acceleration;
            if (velocityX < -maxSpeed)
                velocityX = -maxSpeed;
            facingRight = false;
            isMoving = true;

            if (currentAnimTexture != &activeChar->getRunLeftTexture())
                updateAnimation(activeChar->getRunLeftTexture());
        }
        else if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            velocityX += acceleration;
            if (velocityX > maxSpeed)
                velocityX = maxSpeed;
            facingRight = true;
            isMoving = true;

            if (currentAnimTexture != &activeChar->getRunRightTexture())
                updateAnimation(activeChar->getRunRightTexture());
        }
        else
        {
            velocityX *= 0.9f; // Slow down gradually
        }


        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            velocityY -= acceleration;
            if (velocityY < -maxSpeed)
                velocityY = -maxSpeed;
            isMoving = true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            velocityY += acceleration;
            if (velocityY > maxSpeed)
                velocityY = maxSpeed;
            isMoving = true;
        }
        else
        {
            velocityY *= 0.9f; // Slow down gradually
        }

        if (!isMoving)
        {
            setIdleTexture(); // if not moving, go idle
        }

        // Always animate frames
        updateAnimation(*currentAnimTexture);
        //handleHorizontalCollision(lvl, cellSize);
        //handleVerticalCollision(lvl, cellSize);
        //// Update position
        float nextX = x + velocityX;
        float nextY = y + velocityY;

        bool canMoveX = (nextX >= frameWidth * 2 && nextX <= 6400 - Pwidth);   // checks if it is inside the map or not
        bool canMoveY = (nextY >= frameHeight);




        int topRow = (nextY + hitboxOffsetY) / cellSize;
        int bottomRow = (nextY + hitboxOffsetY + Pheight - 1) / cellSize;
        int leftCol = (nextX + hitboxOffsetX) / cellSize;
        int rightCol = (nextX + hitboxOffsetX + Pwidth - 1) / cellSize;

        bool noWallCollision = true;
        if (topRow >= 0 && topRow < 56 && bottomRow >= 0 && bottomRow < 56 && leftCol >= 0 && leftCol < 100 && rightCol >= 0 && rightCol < 100)
        {

            if (lvl[topRow][leftCol] == 'w' || lvl[topRow][rightCol] == 'w' || lvl[bottomRow][leftCol] == 'w' || lvl[bottomRow][rightCol] == 'w' || lvl[bottomRow][rightCol] == 'x'
                || lvl[topRow][rightCol] == 'x' || lvl[bottomRow][leftCol] == 'x' || lvl[topRow][leftCol] == 'x' || lvl[bottomRow][rightCol] == 'y' || lvl[topRow][rightCol] == 'y'
                || lvl[bottomRow][leftCol] == 'y' || lvl[topRow][leftCol] == 'y')
            {
                noWallCollision = false;
            }
        }

        if (canMoveX && noWallCollision)
            x = nextX;
        if (canMoveY && noWallCollision)
            y = nextY;


        sprite.setScale(scaleX, scaleY);

    }

    void handleHorizontalCollision(char** lvl, int cellSize)
    {
        float nextX = x + velocityX;
        if (velocityX >= velocityXMAX)
        {
            velocityX = velocityXMAX;
        }
        int rowTop = (y + hitboxOffsetY) / cellSize;
        int rowBottom = (y + hitboxOffsetY + Pheight - 1) / cellSize;
        int rowMid = (y + hitboxOffsetY + Pheight / 2) / cellSize;

        bool canMoveX = (nextX >= 0 && nextX <= 6400 - Pwidth);   // checks if it is inside the map or not
        if (canMoveX)
        {
            if (velocityX > 0) // Moving right
            {
                int colRight = (nextX + hitboxOffsetX + Pwidth) / cellSize;
                if (colRight >= 100)
                    return;

                bool hitWallRight = (lvl[rowTop][colRight] == 'w' || lvl[rowMid][colRight] == 'w' || lvl[rowBottom][colRight] == 'w');
                bool hitBreakableRight = (lvl[rowTop][colRight] == 'x' || lvl[rowMid][colRight] == 'x' || lvl[rowBottom][colRight] == 'x');

                if (hitWallRight || hitBreakableRight)
                {
                    if ((isUsingSpecial && characters[currentCharIndex]->getName() == "Knuckles") ||
                        (isSuperMode && activeChar->getName() == "Super"))

                    {
                        if (lvl[rowTop][colRight] == 'x') lvl[rowTop][colRight] = ' ';
                        if (lvl[rowMid][colRight] == 'x') lvl[rowMid][colRight] = ' ';
                        if (lvl[rowBottom][colRight] == 'x') lvl[rowBottom][colRight] = ' ';
                        x = nextX;
                    }
                    else
                    {
                        velocityX = 0;
                        x = colRight * cellSize - hitboxOffsetX - Pwidth;
                    }
                }
                else
                {
                    x = nextX;
                }
            }
            else if (velocityX < 0) // Moving left
            {
                int colLeft = (nextX + hitboxOffsetX) / cellSize;
                if (colLeft < 0)
                    return;

                bool hitWallLeft = (lvl[rowTop][colLeft] == 'w' || lvl[rowMid][colLeft] == 'w' || lvl[rowBottom][colLeft] == 'w');
                bool hitBreakableLeft = (lvl[rowTop][colLeft] == 'x' || lvl[rowMid][colLeft] == 'x' || lvl[rowBottom][colLeft] == 'x');

                if (hitWallLeft || hitBreakableLeft)
                {
                    if (isUsingSpecial && characters[currentCharIndex]->getName() == "Knuckles")
                    {
                        if (lvl[rowTop][colLeft] == 'x') lvl[rowTop][colLeft] = ' ';
                        if (lvl[rowMid][colLeft] == 'x') lvl[rowMid][colLeft] = ' ';
                        if (lvl[rowBottom][colLeft] == 'x') lvl[rowBottom][colLeft] = ' ';
                        x = nextX;
                    }
                    else
                    {
                        velocityX = 0;
                        x = (colLeft + 1) * cellSize - hitboxOffsetX;
                    }
                }
                else
                {
                    x = nextX;
                }
            }
        }
    }
    void handleVerticalCollision(char** lvl, int cellSize)
    {
        float nextY = y + velocityY;
        int colLeft = (x + hitboxOffsetX) / cellSize;
        int colRight = (x + hitboxOffsetX + Pwidth - 1) / cellSize;
        int colMid = (x + hitboxOffsetX + Pwidth / 2) / cellSize;

        if (velocityY > 0) // Falling down
        {
            int rowBelow = (nextY + hitboxOffsetY + Pheight) / cellSize;
            float playerBottom = y + hitboxOffsetY + Pheight;
            float tileTop = rowBelow * cellSize;

            if (rowBelow >= 56) // 56 rows
            {
                y = nextY;

                return;
            }

            bool block = false;

            if ((lvl[rowBelow][colLeft] == 'w' || lvl[rowBelow][colLeft] == 'x' ||
                (lvl[rowBelow][colLeft] == 'y' && playerBottom <= tileTop + 5)))
                block = true;

            if ((lvl[rowBelow][colMid] == 'w' || lvl[rowBelow][colMid] == 'x' ||
                (lvl[rowBelow][colMid] == 'y' && playerBottom <= tileTop + 5)))
                block = true;

            if ((lvl[rowBelow][colRight] == 'w' || lvl[rowBelow][colRight] == 'x' ||
                (lvl[rowBelow][colRight] == 'y' && playerBottom <= tileTop + 5)))
                block = true;

            if (block)
            {
                onGround = true;
                velocityY = 0;
                y = tileTop - hitboxOffsetY - Pheight;
            }
            else
            {
                y = nextY;
                onGround = false;
                velocityY += gravity;
                if (velocityY > terminalVelocity)
                    velocityY = terminalVelocity;
            }
        }
        else if (velocityY < 0) // Jumping up  // moving up the grid basically
        {
            int rowAbove = (int)(nextY + hitboxOffsetY) / cellSize;
            if (rowAbove < 0)
            {
                velocityY = 0;
                y = -hitboxOffsetY;
                return;
            }

            if ((lvl[rowAbove][colLeft] == 'w' || lvl[rowAbove][colLeft] == 'x') ||
                (lvl[rowAbove][colMid] == 'w' || lvl[rowAbove][colMid] == 'x') ||
                (lvl[rowAbove][colRight] == 'w' || lvl[rowAbove][colRight] == 'x'))
            {
                velocityY = 0;
                y = (rowAbove + 1) * cellSize - hitboxOffsetY;
            }
            else
            {
                y = nextY;
                velocityY += gravity;
            }
        }
        else
        {
            y = nextY;
            velocityY += gravity;
        }
    }

    void drawCompanion(RenderWindow& window, float offsetX, float offsetY)
    {
        float followDistance = 60.0f; // Distance between companions and player
        int index = getCurrentCharIndex();

        for (int i = 0; i < 3; ++i)
        {
            if (i == index) continue;  // Skip active character

            // Set companion position based on active player's position
            float companionX = x - offsetX - followDistance * (i + 1);
            float companionY = y - offsetY;

            Sprite companionSprite;
            companionSprite.setTexture(characters[i]->getIdleRightTexture()); // You can customize animation
            companionSprite.setScale(scaleX, scaleY);
            companionSprite.setPosition(companionX, companionY);

            // Optionally make them semi-transparent for visual distinction
            companionSprite.setColor(Color(255, 255, 255, 255));

            window.draw(companionSprite);
        }
    }

    void draw(RenderWindow& window, float offsetX, float offsetY)
    {

        sprite.setPosition(x - offsetX, y - offsetY);

        if (isInvincible)
        {
            // Set transparency (alpha) to make it look "blinking" or "invisible"
            sprite.setColor(Color(255, 255, 255, 128));  // 128 = half transparent
        }
        else {
            sprite.setColor(Color(255, 255, 255, 255));  // Normal
        }

        window.draw(sprite);
    }

    float getX()
    {
        return x;
    }
    float getY()
    {
        return y;
    }
    // used in the comapnion class
    Character* getCharacter(int index) const
    {
        return characters[index];
    }
    int getCurrentCharIndex() const
    {
        return currentCharIndex;
    }
    bool isFacingRight() const
    {
        return facingRight;
    }

    ~Player()
    {
        for (int i = 0; i < 3; ++i)
        {
            delete characters[i];
        }
    }
};




