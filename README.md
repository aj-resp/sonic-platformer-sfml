# Sonic Platformer SFML Game

A 2D Sonic-inspired platformer game developed in C++ using SFML.  
The game features multiple levels, enemy AI, collectibles, boss fights, power ups, sound effects, and different environments inspired by classic Sonic games.

Note: Some large assets and extra files were not uploaded because of GitHub space limitations. Only the main required files for the project are included here.

---

## Features

- Multiple playable levels
- Sonic, Tails, and Knuckles characters
- Enemy AI and moving enemies
- Boss fight level
- Ring collection system (green for health, black for powerup, "p" for count of power ups)
- Health and score system
- Power ups and collectibles
- Different themed maps and environments
- Sound effects and background music
- Platformer movement and collision system
- knuckles(breaks walls)
- Sonic (fast movement)
- Tails (fly)  
---

## Screenshots

### Main Screen

![Main Screen](images/SonicMainScreen.png)

---

### Level 1

![Level 1](images/SonicLevel1.png)

---

### Level 2

![Level 2](images/SonicLevel2.png)

---

### Level 3

![Level 3](images/SonicLevel3.png)

---

### Boss Level

![Boss Level](images/SonicBossLevel.png)

---

### Level Transition Screen

![Transition Screen](images/SonicLevelChangingScreen.png)

---

### Debug / Console Output

![Terminal](images/SonicTerminal.png)

---

## Controls

- Up Arrow → Jump
- Down Arrow → Crouch
- Z → Change character
- X → Use character special ability
- Collect rings and power ups
- Avoid spikes and enemies

---

## Requirements

- Visual Studio
- SFML Library
- Windows OS

---

## How To Run

1. Install SFML library
2. Open `Sonic.vcxproj` in Visual Studio
3. Build the project
4. Run the game

Make sure the `Data` folder remains in the same directory so textures, sounds, and assets load properly.

---

## Project Structure

```text
sonic-platformer-sfml/
│
├── Data/
├── images/
│
├── Source.cpp
├── Character.h
├── player.h
├── enemi.h
├── SoundManager.h
│
├── Sonic.vcxproj
├── Sonic.vcxproj.filters
│
├── README.md
├── LICENSE
└── .gitignore
