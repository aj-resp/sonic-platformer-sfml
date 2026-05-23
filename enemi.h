#pragma once

#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <cstdlib> 
#include <ctime>


using namespace sf;
using namespace std;
class Player;
class Enemy {
protected:
    Sprite sprite;
    Texture texture;
    float x, y;
    float speed;
    int health;
    float hitboxWidth, hitboxHeight;


    // Animation same thing we did in the players class
    int currentFrame;
    int totalFrames;
    int frameWidth, frameHeight;
    float frameDuration;
    Clock frameClock;

public:
    Enemy(string texturePath, float posX, float posY, int fWidth, int fHeight, int fCount, float fTime,
        float spd = 0.0f, int hp = 1)
        : x(posX), y(posY), speed(spd), health(hp),
        frameWidth(fWidth), frameHeight(fHeight),
        totalFrames(fCount), frameDuration(fTime), currentFrame(0)
    {
        texture.loadFromFile(texturePath);
        sprite.setTexture(texture);
        sprite.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
        sprite.setPosition(x, y);
        hitboxWidth = frameWidth;
        hitboxHeight = frameHeight;
    }
    float getX() const
    {
        return x;
    }
    int getHealth() const {
        return health;
    }

    float getY() const { return y; }
    float getHitboxWidth() const { return hitboxWidth; }
    float getHitboxHeight() const { return hitboxHeight; }

    virtual void setLevelReference(char**, int) {}
    virtual void update(float playerX, float playerY, Player& other) = 0;

    virtual void animate()
    {
        if (frameClock.getElapsedTime().asSeconds() > frameDuration) {
            currentFrame = (currentFrame + 1) % totalFrames;
            sprite.setTextureRect(IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
            frameClock.restart();
        }
    }

    virtual void draw(RenderWindow& window, float offsetX, float offsetY)
    {
        sprite.setPosition(x - offsetX, y - offsetY);
        sprite.setScale(1, 1);
        window.draw(sprite);
    }


    //in this function we have basically created an additional 'boxed' area around both the ring sprite and the player and the reason is to properly check for any overlapping collision
//that occurs between the player and the ring, and when it does, the ring is collected. this is done cus if we just depend on the pixels of the sprites for the collision then a lot 
//of times it doesnt collide even tho the collision very clearly occurs so
    virtual bool checkCollision(float px, float py, float pWidth, float pHeight)
    {
        return (x + hitboxWidth > px && x < px + pWidth && y + hitboxHeight > py && y < py + pHeight);
    }

    virtual void takeDamage(int dmg) { health -= dmg; }
    virtual bool isDead() const { return health <= 0; }
    virtual string getName() const = 0;

    virtual ~Enemy() {}
};

class FinalBoss : public Enemy {
private:
    Clock attackClock;
    float attackInterval = 2.5f;

    bool movingRight = true;

    char** levelRef = nullptr;
    int cellSizeRef = 64;

public:
    FinalBoss(float posX, float posY)
        : Enemy("Data/eggstingerbossL.png", posX, posY, 320, 309, 1, 0.2f, 1.2f, 20) // 15 HP
    {
        // No sprite scaling here
    }

    void setLevelReference(char** lvl, int cellSize) override
    {
        levelRef = lvl;
        cellSizeRef = cellSize;
    }


    void attack(float playerX, float playerY, Player& player)
    {
        static bool descending = false;
        static bool ascending = false;
        static float originalY = y;

        float descendSpeed = 4.0f;
        float maxDescendDistance = 100.0f;

        if (!descending && !ascending && attackClock.getElapsedTime().asSeconds() > attackInterval)
        {
            descending = true;
            originalY = y;  // Set baseline to return to
        }

        if (descending)
        {
            if (y < originalY + maxDescendDistance)
            {
                y += descendSpeed;
            }
            else
            {
                destroyBlockBelow();
                descending = false;
                ascending = true;
            }
        }
        else if (ascending)
        {
            if (y > originalY)
            {
                y -= descendSpeed;
                if (y <= originalY)
                {
                    y = originalY;
                    ascending = false;
                    attackClock.restart();  // Restart attack cycle
                }
            }
        }
    }

    void destroyBlockBelow()
    {
        if (!levelRef) return;

        int col = static_cast<int>((x + hitboxWidth / 2) / cellSizeRef);

        // Start just below the boss's bottom
        int startRow = static_cast<int>((y + hitboxHeight) / cellSizeRef);

        // Look 5 blocks down at most
        for (int i = 1; i <= 10; ++i)
        {
            int row = startRow + i;
            if (row >= 56 || col < 0 || col >= 100)  // bounds check
                break;

            char& tile = levelRef[row][col];
            if (tile == 'w' || tile == 'x' || tile == 'y')  // solid blocks
            {
                tile = ' ';  // Remove it
                break;
            }
        }
    }

    void update(float playerX, float playerY, Player& player) override {
        // Follow player horizontally
        if (playerX > x)
            x += speed;
        else if (playerX < x)
            x -= speed;




        attack(playerX, playerY, player);  // 

        // Animate sprite frames
        animate();
    }


    string getName() const override {
        return "FinalBoss";
    }
};


class BatBrain : public Enemy {
private:
    char** lvlRef;
    int cellSizeRef;
    bool movingRight = true;

public:
    BatBrain(float posX, float posY)
        : Enemy("Data/batbrainR.png", posX, posY, 80, 64, 4, 0.2f, 0.8f, 3) {
        sprite.setScale(1.5, 1.5);
    }

    void setLevelReference(char** lvl, int cellSize)
    {
        lvlRef = lvl;
        cellSizeRef = cellSize;
    }

    void handleCollision(float playerX)
    {
        int row = int((y + hitboxHeight / 2) / cellSizeRef);
        int colAhead;
        if (movingRight)
            colAhead = int((x + hitboxWidth + 1) / cellSizeRef);
        else
            colAhead = int((x - 1) / cellSizeRef);

        if (lvlRef[row][colAhead] == 'w' || lvlRef[row][colAhead] == 'x') {

            return;
        }

        // Follow logic like Motobug
        if (playerX > x)
            x += speed;
        else if (playerX < x)
            x -= speed;
    }

    void update(float playerX, float playerY, Player& other) override
    {
        // Direction update (for flipping)
        movingRight = playerX > x;



        // Vertical hovering
        if (playerY < y)
            y -= speed * 0.5f;
        else
            y += speed * 0.5f;
        if (playerX > x)
        {
            movingRight = true;
            texture.loadFromFile("Data/batbrainL.png");
        }
        else
        {
            movingRight = false;
            texture.loadFromFile("Data/batbrainR.png");
        }

        sprite.setTexture(texture);

        handleCollision(playerX);
        animate();
    }

    string getName() const override { return "BatBrain"; }
};

class BeeBot : public Enemy {
private:
    Clock shootClock;
    float angle = 0;
    bool movingRight = true;

    Texture bulletTextureLeft;
    Texture bulletTextureRight;


    // Inside private or protected section of BeeBot:
    char** levelRef = nullptr;
    int cellSizeRef = 64;

    struct Bullet
    {
        Sprite sprite;
        float x, y;
        float vx, vy;
        float startX, startY;
        float targetX, targetY;
        bool active;
        float bwidth = 31.0f;
        float bheight = 33.0f;
        Bullet() : x(0), y(0), vx(0), vy(0), startX(0), startY(0), targetX(0), targetY(0), active(false) {}

        bool Bcollision(Player& other)
        {
            float bulletLeft = x;
            float bulletTop = y;
            float bulletRight = x + bwidth;
            float bulletBottom = y + bheight;

            float playerLeft = other.getX();
            float playerTop = other.getY();
            float playerRight = playerLeft + other.getWidth();
            float playerBottom = playerTop + other.getHeight();

            // Simple AABB collision
            return !(bulletRight < playerLeft || bulletLeft > playerRight || bulletBottom < playerTop || bulletTop > playerBottom);
        }

    };


    // Inside private:
    Clock verticalClock;
    bool movingUp = true;


    static const int MAX_BULLETS = 40;
    Bullet bullets[MAX_BULLETS];
    int bulletCount = 0;
public:
    BeeBot(float posX, float posY) : Enemy("Data/beebotR.png", posX, posY, 54, 34, 6, 0.15f, 0.0f, 5) {
        sprite.setScale(1.5f, 1.5f);
        bulletTextureLeft.loadFromFile("Data/projectileL.png");
        bulletTextureRight.loadFromFile("Data/projectileR.png");
    }
    void setLevelReference(char** lvl, int cellSize)
    {
        levelRef = lvl;
        cellSizeRef = cellSize;
    }

    void shoot(float targetX, float targetY)
    {
        float startX = x + hitboxWidth / 2;
        float startY = y + hitboxHeight / 2;

        float dx = targetX - startX;
        float dy = targetY - startY;
        float length = sqrt(dx * dx + dy * dy);
        if (length == 0)
            return;

        // calculating the rise and run - slope so we know how much to add in the componenet of the bullet

        float speed = 5.0f;
        float vx = (dx / length) * speed;
        float vy = (dy / length) * speed;

        // Choose the correct texture directly (no pointer)
        Texture& tex = bulletTextureLeft;
        if (targetX > startX)
        {
            tex = bulletTextureRight;
        }


        // Look for an inactive bullet to reuse
        for (int i = 0; i < MAX_BULLETS; ++i)
        {
            if (!bullets[i].active)
            {
                bullets[i].x = startX;
                bullets[i].y = startY;
                bullets[i].startX = startX;
                bullets[i].startY = startY;
                bullets[i].vx = vx;
                bullets[i].vy = vy;
                bullets[i].targetX = targetX;
                bullets[i].targetY = targetY;
                bullets[i].active = true;
                bullets[i].sprite.setTexture(tex);  // Use reference here
                bullets[i].sprite.setPosition(startX, startY);
                break;
            }
        }

    }




    void update(float playerX, float playerY, Player& other) override
    {
        // Set direction toward player
        if (playerX > x)
        {
            movingRight = true;
        }
        else
        {
            movingRight = false;
        }

        if (verticalClock.getElapsedTime().asMilliseconds() > 100) // every 0.1s
        {
            if (movingUp)
                y -= 5;
            else
                y += 5;

            if (y <= 150) movingUp = false;
            if (y >= 250) movingUp = true;

            verticalClock.restart();
        }

        // Predict next position
        float nextX;
        if (movingRight)
        {

            nextX = x + 2.0f;
        }
        else
        {
            nextX = x - 2.0f;
        }




        int row = static_cast<int>((y + hitboxHeight / 2) / cellSizeRef);
        int colAhead;
        if (movingRight)
        {
            colAhead = static_cast<int>((x + hitboxWidth + 1) / cellSizeRef);
        }
        else {
            colAhead = static_cast<int>((x - 1) / cellSizeRef);
        }

        // Check for wall
        bool blocked = false;
        if (levelRef[row][colAhead] == 'w' || levelRef[row][colAhead] == 'x' || levelRef[row][colAhead] == 'y')
        {
            blocked = true;
        }

        // Move if not blocked
        if (!blocked)
        {
            x = nextX;
        }

        // Set correct texture
        if (movingRight)
        {
            texture.loadFromFile("Data/beebotR.png");
        }
        else
        {
            texture.loadFromFile("Data/beebotL.png");
        }
        sprite.setTexture(texture);

        // Shoot toward player
        if (shootClock.getElapsedTime().asSeconds() > 3.0f) {
            shootClock.restart();
            shoot(playerX, playerY);
        }

        //
        for (int i = 0; i < MAX_BULLETS; ++i)
        {
            if (bullets[i].active)
            {
                bullets[i].x += bullets[i].vx;
                bullets[i].y += bullets[i].vy;
                bullets[i].sprite.setPosition(bullets[i].x, bullets[i].y);
                if (bullets[i].Bcollision(other)) {
                    bullets[i].active = false;
                    if (!other.isInvincibleNow()) {
                        other.takeDamage(1);
                    }
                    continue; // Skip rest if already collided
                }



                // Kill bullet if it flies too far
                float dx = bullets[i].x - bullets[i].startX;
                float dy = bullets[i].y - bullets[i].startY;
                float distSq = dx * dx + dy * dy;
                if (distSq > 500 * 500) // traveled more than 500px
                {
                    bullets[i].active = false;

                }
            }
        }

        animate();
    }


    void draw(RenderWindow& window, float offsetX, float offsetY) override
    {
        sprite.setPosition(x - offsetX, y - offsetY);
        window.draw(sprite);

        for (int i = 0; i < MAX_BULLETS; ++i)
        {
            if (bullets[i].active)
            {
                bullets[i].sprite.setPosition(bullets[i].x - offsetX, bullets[i].y - offsetY);
                window.draw(bullets[i].sprite);
            }
        }
    }


    string getName() const override { return "BeeBot"; }
};

class CrabMeat : public Enemy {
public:
    char** lvlRef;
    int cellSizeRef;
    bool movingRight = true;
    Clock directionClock;
    float directionChangeInterval = 3.0f;

    CrabMeat(float posX, float posY)
        : Enemy("Data/crabmeatR.png", posX, posY, 67, 50, 7, 0.1f, 1.0f, 4)
    {
        sprite.setScale(4.0f, 4.0f);
        sprite.setTextureRect(IntRect(0, 0, 68, 50));
    }

    void setLevelReference(char** lvl, int cellSize) {
        lvlRef = lvl;
        cellSizeRef = cellSize;
    }


    void handleCollision()
    {
        int row = (y + hitboxHeight) / cellSizeRef;
        int colAhead;

        if (movingRight)
        {
            colAhead = (x + hitboxWidth + 1) / cellSizeRef;
        }
        else
        {
            colAhead = (x - 1) / cellSizeRef;
        }


        // If wall or breakable block ahead, reverse direction
        if (lvlRef[row][colAhead] == 'w' || lvlRef[row][colAhead] == 'x') {
            movingRight = !movingRight;
        }
    }

    void update(float, float, Player& other) override
    {
        if (directionClock.getElapsedTime().asSeconds() >= directionChangeInterval)
        {
            movingRight = rand() % 2;
            directionClock.restart();
        }

        speed = (rand() % 200 + 100) / 100.0f;  // Between 1.0 and 3.0

        handleCollision();

        if (movingRight)
        {
            x += speed;
            sprite.setScale(4.0f, 4.0f); // Facing right
        }
        else
        {
            x -= speed;
            sprite.setScale(-4.0f, 4.0f); // Flip horizontally (facing left)
        }

        animate();
    }

    string getName() const override { return "CrabMeat"; }
};






class Motobug : public Enemy {
public:
    char** lvlRef;      // reference to level layout
    int cellSizeRef;
    bool movingRight = true; //also needed for patrol logic

    void setLevelReference(char** lvl, int cellSize)
    {
        lvlRef = lvl;
        cellSizeRef = cellSize;
    }


    void handleCollision(float playerX)
    {
        int row = static_cast<int>((y + hitboxHeight) / cellSizeRef);
        int colAhead;

        if (movingRight)
        {
            colAhead = static_cast<int>((x + hitboxWidth + 1) / cellSizeRef);
        }
        else
        {
            colAhead = static_cast<int>((x - 1) / cellSizeRef);
        }


        // Wall check → just stop if blocked
        if (lvlRef[row][colAhead] == 'w' || lvlRef[row][colAhead] == 'x')
        {

            return;
        }

        //  follow player
        if (playerX > x)
            x += speed;
        else if (playerX < x)
            x -= speed;
    }


    Motobug(float posX, float posY)
        : Enemy("Data/motoR.png", posX, posY, 78, 59, 4, 0.15f, 1.2f, 2) {
        sprite.setScale(1, 1);
    }


    void update(float playerX, float, Player& other) override
    {
        if (playerX > x)
        {
            movingRight = true;
            texture.loadFromFile("Data/motoR.png");
        }
        else
        {
            movingRight = false;
            texture.loadFromFile("Data/motoL.png");
        }

        sprite.setTexture(texture);
        handleCollision(playerX);
        animate();
    }



    string getName() const override { return "Motobug"; }
};


const int MAX_ENEMIES = 50;  // so no need to delete it later 

class EnemyManager  // just like a player class
{
private:
    Enemy* enemies[MAX_ENEMIES];  // parent class pointing to drived class and due to virtual we will be able to access each enemy separately
    int enemyCount;
    SoundManager EnemySounds;

public:
    EnemyManager() : enemyCount(0)  // initially zero enemy
    {
        for (int i = 0; i < MAX_ENEMIES; ++i)
            enemies[i] = nullptr;
    }
    Enemy* getEnemy(int index)
    {
        if (index >= 0 && index < MAX_ENEMIES)
            return enemies[index];
        return nullptr;
    }

    void destroyAllEnemies()
    {
        for (int i = 0; i < MAX_ENEMIES; ++i)
        {
            if (enemies[i])
            {
                enemies[i]->takeDamage(999);  // kill all
            }
        }
    }

    void clearAll()   // after each lelel 
    {
        for (int i = 0; i < enemyCount; ++i)
        {
            delete enemies[i];
            enemies[i] = nullptr;
        }
        enemyCount = 0;
    }

    void checkPlayerCollision(Player& player)
    {
        float playerX = player.getX();
        float playerY = player.getY();
        float playerWidth = player.getWidth();
        float playerHeight = player.getHeight();

        for (int i = 0; i < enemyCount; ++i)
        {
            if (enemies[i]->isDead())
            {
                continue;
            }

            float enemyX = enemies[i]->getX();
            float enemyY = enemies[i]->getY();
            float enemyWidth = enemies[i]->getHitboxWidth();
            float enemyHeight = enemies[i]->getHitboxHeight();

            bool overlapX = (playerX < enemyX + enemyWidth) && (playerX + playerWidth > enemyX);
            bool overlapY = (playerY < enemyY + enemyHeight) && (playerY + playerHeight > enemyY);
            bool isTouching = overlapX && overlapY;

            if (isTouching)
            {
                float playerBottom = playerY + playerHeight;
                float enemyTop = enemyY;

                bool stompedFromAbove = (playerBottom - enemyTop <= 15) && (playerY < enemyY);

                if (stompedFromAbove)
                {
                    enemies[i]->takeDamage(1);
                    EnemySounds.PlayDestroyEnemySound();
                    player.bounceUp();
                }
                else
                {
                    if (!player.isInvincibleNow())
                        player.takeDamage(5);
                }
            }
        }
    }

    void addEnemy(Enemy* enemy)
    {
        if (enemyCount < MAX_ENEMIES)
            enemies[enemyCount++] = enemy;
        else
        {
            // i wil not add more than 50 enemies hahaahahahaha
        }
    }
    void updateAll(float playerX, float playerY, char** lvl, int cellSize, Player& other)
    {
        for (int i = 0; i < enemyCount; ++i)
        {
            if (enemies[i] && !enemies[i]->isDead())
            {
                enemies[i]->setLevelReference(lvl, cellSize);
                enemies[i]->update(playerX, playerY, other);
            }
        }
    }




    void drawAll(RenderWindow& window, float offsetX, float offsetY)
    {
        for (int i = 0; i < enemyCount; ++i)
            if (enemies[i] && !enemies[i]->isDead())
                enemies[i]->draw(window, offsetX, offsetY);
    }

    void setSoundEnabled(bool enabled)
    {
        EnemySounds.setSoundEnabled(enabled);
    }

    ~EnemyManager()
    {
        for (int i = 0; i < enemyCount; ++i)
            delete enemies[i];
    }
};

