#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Game.h"
#include "Player.h"
#include "NonPlayer.h"
#include "Cage.h"
#include "Raven.h"
#include "resource_path.hpp"


struct GamePlayState : public GameState
{
	void init()
	{
		mLevel = 1;

		mCamera = Game::getInstance().mCamera;
		mCamera.setSize(sf::Vector2f(200, 150));

		mGameMode = GameState::GameModes::GamePlay;

		mBackgroundTexture.loadFromFile(resourcePath() + "background1.png");
		mBackground.setTexture(mBackgroundTexture);

		mEdgeTexture.loadFromFile(resourcePath() + "edge.png");
		mEdge.setTexture(mEdgeTexture);
		mEdge.setColor(sf::Color(255, 255, 255, 200));

		mSpriteTexture.loadFromFile(resourcePath() + "spritesheet.png");
		mPlayer.init(mSpriteTexture);
		mNonPlayer.init(mSpriteTexture);

		mHouse.setTexture(mSpriteTexture);
		mHouse.setTextureRect(sf::IntRect(128, 32, 128, 96));
		mHouse.setOrigin(64, 48);
		mHouse.setPosition(300, 222);

		mRoof.setTexture(mSpriteTexture);
		mRoof.setTextureRect(sf::IntRect(128, 0, 128, 32));
		mRoof.setOrigin(64, 16);
		mRoof.setPosition(300, 161);
		
		for(unsigned int i = 0; i < 8; i++)
		{
			Raven raven;
			raven.init(mSpriteTexture);
			mRavenList.push_back(raven);
		}
		
		//Initialize wall shape to be used
		sf::RectangleShape collider;
		collider.setFillColor(sf::Color::Transparent);
		collider.setOutlineColor(sf::Color::Red);
		collider.setOutlineThickness(1.0f);

		//Set house wall collider
		collider.setSize(sf::Vector2f(123, 52));
		collider.setPosition(238, 175);
		mColliderList.push_back(collider);

		//Top border collider
		collider.setSize(sf::Vector2f(600, 10.0f));
		collider.setPosition(0, -5);
		mColliderList.push_back(collider);

		//Bottom border collider
		collider.setPosition(0, 395);
		mColliderList.push_back(collider);

		//Left border collider
		collider.setSize(sf::Vector2f(10.0f, 400.0f));
		collider.setPosition(-5, 0);
		mColliderList.push_back(collider);

		//Right border collider
		collider.setPosition(595.0f, 0);
		mColliderList.push_back(collider);

		mBackgroundMusic.openFromFile(resourcePath() + "raven_play.wav");
		mBackgroundMusic.setLoop(true);
		mBackgroundMusic.setVolume(5.0f);
		mBackgroundMusic.play();

		mSound.setBuffer(mSoundBuffer);
	}

	void loop()
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			//Delay or whatever thing
			Game::getInstance().mGameMode = GameState::GameModes::TitleScreen;
			mBackgroundMusic.stop();
		}
		
		if(mPlayer.mDelay >= 30.0f && sf::Keyboard::isKeyPressed(sf::Keyboard::C))
		{
			Cage cage;
			cage.init(mSpriteTexture);
			cage.mDirection = mPlayer.mDirection;
			cage.mCollider.setPosition(mPlayer.mCollider.getPosition());

			mCageList.push_back(cage);

			mPlayer.mDelay = 0.0f;
		}

		mNonPlayer.loop();
		mPlayer.loop();

		for(unsigned int cageIndex = 0; cageIndex < mCageList.size(); cageIndex++)
		{
			if(mCageList[cageIndex].mAction != Actions::Dead)
				mCageList[cageIndex].loop();
			else
				mCageList.erase(mCageList.begin() + cageIndex);
		}

		//Check collision
		for(auto& collider : mColliderList)
		{
			mPlayer.collision(collider);

			for(auto& cage : mCageList)
			{
				if(cage.collision(collider))
				{
					cage.mAction = Actions::Idle;
				}
			}
		}

		for(unsigned int ravenIndex = 0; ravenIndex < mRavenList.size(); ravenIndex++)
		{
			if(mRavenList[ravenIndex].mAction == ActionB && mRavenList[ravenIndex].collision(mPlayer.mCollider))
			{
				mSoundBuffer.loadFromFile(resourcePath() + "pickup.wav");
				mSound.play();
				mRavenList.erase(mRavenList.begin() + ravenIndex);
			}
			else
			{
				mRavenList[ravenIndex].loop();
				mRavenList[ravenIndex].checkPlayerDistance(mPlayer.mCollider.getPosition());


				for(auto& collider : mColliderList)
					mRavenList[ravenIndex].collision(collider);

				for(auto& cage : mCageList)
				{
					if(mRavenList[ravenIndex].mAction != Actions::ActionB && cage.mAction != Actions::Idle && mRavenList[ravenIndex].collision(cage.mCollider))
					{
						cage.mAction = Actions::Dead;
						mRavenList[ravenIndex].mAction = Actions::ActionB;

						mSoundBuffer.loadFromFile(resourcePath() + "bird_catch.wav");
						mSound.play();
					}
				}

				if(mRavenList[ravenIndex].mAction == Actions::ActionA && mSound.getStatus() != sf::Sound::Playing)
				{
					mSoundBuffer.loadFromFile(resourcePath() + "bird_fly.wav");
					mSound.play();
				}
			}
		}

		mCamera.setCenter(mPlayer.mCollider.getPosition());
		sf::Vector2f campos = mCamera.getCenter();

		if(campos.x < mCamera.getSize().x / 2)
			campos.x = mCamera.getSize().x / 2;
		if(campos.y < mCamera.getSize().y / 2)
			campos.y = mCamera.getSize().y / 2;

		if(campos.x > 600 - mCamera.getSize().x / 2)
			campos.x = 600 - mCamera.getSize().x / 2;
		if(campos.y > 400 - mCamera.getSize().y / 2)
			campos.y = 400 - mCamera.getSize().y / 2;

		mCamera.setCenter(campos);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.setView(mCamera);

		target.draw(mBackground);
		target.draw(mHouse);
		target.draw(mNonPlayer);

		target.draw(mPlayer);

		for(auto& raven : mRavenList)
			target.draw(raven);

		for(auto& snare : mCageList)
			target.draw(snare);

		target.draw(mRoof);
		target.draw(mEdge);

		//for(auto& collider : mColliderList)
			//target.draw(collider);
	}

	sf::Texture mBackgroundTexture;
	sf::Texture mEdgeTexture;
	sf::Texture mSpriteTexture;

	Player mPlayer;
	NonPlayer mNonPlayer;

	sf::Sprite mBackground;
	sf::Sprite mHouse;
	sf::Sprite mRoof;
	sf::Sprite mEdge;

	std::vector<Raven> mRavenList;
	std::vector<Cage> mCageList;

	std::vector<sf::RectangleShape> mColliderList;

	sf::Music mBackgroundMusic;
	sf::SoundBuffer mSoundBuffer;
	sf::Sound mSound;
	unsigned int mLevel;
};


#endif