#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace sf;

class SoundManager
{
	SoundBuffer RingBuffer;
	Sound RingSound;

	SoundBuffer FlyBuffer;
	Sound FlyActivateSound;

	SoundBuffer DestroyBuffer;
	Sound DestroyWallSound;

	SoundBuffer SpikeBuffer;
	Sound SpikeDamage;

	SoundBuffer DestroyEnemyBuffer;
	Sound DestroyEnemy;

	bool soundEnabled = true;

public:

	SoundManager()
	{
		if (!RingBuffer.loadFromFile("Data/Ring.wav"))
			cout << "Ring sound didn't load\n";
		else
			RingSound.setBuffer(RingBuffer);

		if (!FlyBuffer.loadFromFile("Data/Flying.wav"))
			cout << "Fly sound didn't load\n";
		else
			FlyActivateSound.setBuffer(FlyBuffer);

		if (!DestroyBuffer.loadFromFile("Data/Destroy.wav"))
			cout << "Wall destroy sound didn't load\n";
		else
			DestroyWallSound.setBuffer(DestroyBuffer);

		if (!SpikeBuffer.loadFromFile("Data/Spikes.wav"))
			cout << "Spike damage sound didn't load\n";
		else
			SpikeDamage.setBuffer(SpikeBuffer);

		if (!DestroyEnemyBuffer.loadFromFile("Data/Destroy.wav"))
			cout << "Enemy destroy sound didn't load\n";
		else
			DestroyEnemy.setBuffer(DestroyEnemyBuffer);
	}

	void setSoundEnabled(bool enabled)
	{
		soundEnabled = enabled;
	}

	void PlayRingSound()
	{
		if (!soundEnabled) return;

		// Fix overlapping issue
		if (RingSound.getStatus() == Sound::Playing)
			RingSound.stop();
		RingSound.play();
	}

	void PlayFlyingSound()
	{
		if (!soundEnabled) return;

		FlyActivateSound.play();
	}

	void PlayDestroyWallSound()
	{
		if (!soundEnabled) return;

		DestroyWallSound.play();
	}

	void PlaySpikeSound()
	{
		if (!soundEnabled) return;

		SpikeDamage.play();
	}

	void PlayDestroyEnemySound()
	{
		if (!soundEnabled) return;

		DestroyEnemy.play();
	}
};
