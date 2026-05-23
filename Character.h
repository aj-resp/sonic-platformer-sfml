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
// Character system 
class Character
{
public:
    // we made this pure virtual as we dont need them here so yeah
    virtual Texture& getIdleLeftTexture() = 0;
    virtual Texture& getIdleRightTexture() = 0;
    virtual Texture& getRunLeftTexture() = 0;
    virtual Texture& getRunRightTexture() = 0;
    virtual Texture& getJumpTexture() = 0;
    virtual Texture& getRollLeftTexture() = 0;
    virtual Texture& getRollRightTexture() = 0;
    /*   virtual Texture& getFlyL() {
           static Texture dummy;
           return dummy;
       }

       virtual Texture& getFlyR() {
           static Texture dummy;
           return dummy;
       }
       */



    virtual string getName() = 0;
    virtual ~Character() {}
};

class Sonic : public Character
{
private:
    Texture idleLeftTex, idleRightTex;
    Texture runLeftTex, runRightTex;
    Texture jumpTex;
    Texture rollLeftTex, rollRightTex;

public:
    Sonic()
    {
        idleLeftTex.loadFromFile("Data/0left_still.png");
        idleRightTex.loadFromFile("Data/0right_still.png");
        runLeftTex.loadFromFile("Data/0jog_left.png");
        runRightTex.loadFromFile("Data/0jog_right.png");
        jumpTex.loadFromFile("Data/0upL.png");
        rollLeftTex.loadFromFile("Data/0upL.png");
        rollRightTex.loadFromFile("Data/0upR.png");
    }


    // getters as they are needed ahead
    string getName() override
    {
        return "Sonic";
    }


    Texture& getIdleLeftTexture() override
    {
        return idleLeftTex;
    }
    Texture& getIdleRightTexture() override
    {
        return idleRightTex;
    }
    Texture& getRunLeftTexture() override
    {
        return runLeftTex;
    }
    Texture& getRunRightTexture() override { return runRightTex; }
    Texture& getJumpTexture() override { return jumpTex; }
    Texture& getRollLeftTexture() override { return rollLeftTex; }
    Texture& getRollRightTexture() override { return rollRightTex; }
};












class Tails : public Character
{
    Texture idleLeftTex, idleRightTex;
    Texture runLeftTex, runRightTex;
    Texture jumpTex;
    Texture rollLeftTex, rollRightTex;
    Texture flyl, flyr;

public:
    // every thing similar to the sonic just different path

    Tails()
    {
        idleLeftTex.loadFromFile("Data/tli.png");
        idleRightTex.loadFromFile("Data/tRi.png");
        runLeftTex.loadFromFile("Data/left.png");
        runRightTex.loadFromFile("Data/right.png");
        jumpTex.loadFromFile("Data/TR.png");
        rollLeftTex.loadFromFile("Data/TR.png");
        rollRightTex.loadFromFile("Data/TR.png");
        flyl.loadFromFile("Data/tailsflyl.png");
        flyr.loadFromFile("Data/tailsflyr.png");



    }


    string getName() override { return "Tails"; }
    Texture& getIdleLeftTexture() override { return idleLeftTex; }
    Texture& getIdleRightTexture() override { return idleRightTex; }
    Texture& getRunLeftTexture() override { return runLeftTex; }
    Texture& getRunRightTexture() override { return runRightTex; }
    Texture& getJumpTexture() override { return jumpTex; }
    Texture& getRollLeftTexture() override { return rollLeftTex; }
    Texture& getRollRightTexture() override { return rollRightTex; }
    /*Texture& getFlyL() override {return flyl;}
    Texture& getFlyR() override {return flyr;}*/

};


class Knuckles : public Character {
    Texture idleLeftTex, idleRightTex;
    Texture runLeftTex, runRightTex;
    Texture jumpTex;
    Texture rollLeftTex, rollRightTex;

public:
    // same as above
    Knuckles()
    {
        idleLeftTex.loadFromFile("Data/2_idleL.png");
        idleRightTex.loadFromFile("Data/2_idleR.png");
        runLeftTex.loadFromFile("Data/KLJ.png");
        runRightTex.loadFromFile("Data/KRJ.png");
        jumpTex.loadFromFile("Data/KR.png");
        rollLeftTex.loadFromFile("Data/KR.png");
        rollRightTex.loadFromFile("Data/KR.png");
    }

    string getName() override { return "Knuckles"; }

    Texture& getIdleLeftTexture() override { return idleLeftTex; }
    Texture& getIdleRightTexture() override { return idleRightTex; }
    Texture& getRunLeftTexture() override { return runLeftTex; }
    Texture& getRunRightTexture() override { return runRightTex; }
    Texture& getJumpTexture() override { return jumpTex; }
    Texture& getRollLeftTexture() override { return rollLeftTex; }
    Texture& getRollRightTexture() override { return rollRightTex; }
};


class SuperCharacter : public Character {
private:
    Texture idleLeftTex, idleRightTex;
    Texture runLeftTex, runRightTex;
    Texture jumpTex;
    Texture rollLeftTex, rollRightTex;

public:
    SuperCharacter()
    {
        idleLeftTex.loadFromFile("Data/SuperSonicIdleL.png");
        idleRightTex.loadFromFile("Data/SuperSonicIdleR.png");
        runLeftTex.loadFromFile("Data/SuperSonicMoveL.png");
        runRightTex.loadFromFile("Data/SuperSonicMoveR.png");
        jumpTex.loadFromFile("Data/SuperSonicRollL.png");
        rollLeftTex.loadFromFile("Data/SuperSonicRollR.png");
        rollRightTex.loadFromFile("Data/SuperSonicRollr.png");
    }

    string getName() override { return "SuperCharacter"; }

    Texture& getIdleLeftTexture() override { return idleLeftTex; }
    Texture& getIdleRightTexture() override { return idleRightTex; }
    Texture& getRunLeftTexture() override { return runLeftTex; }
    Texture& getRunRightTexture() override { return runRightTex; }
    Texture& getJumpTexture() override { return jumpTex; }
    Texture& getRollLeftTexture() override { return rollLeftTex; }
    Texture& getRollRightTexture() override { return rollRightTex; }
};

