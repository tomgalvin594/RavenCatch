#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include <iostream>
#include <cmath>
#include <cstdio>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Game.h"
#include "resource_path.hpp"

struct TitleScreenState : public GameState
{
	void init()
	{
		mFont.loadFromFile(resourcePath() + "Chewy.ttf");

		mText;
		mText.setFont(mFont);
		mText.setColor(sf::Color::Black);

		mText.setString("Play");
		mText.setColor(sf::Color(255,153,153,255));
		mText.setOrigin(sf::Vector2f(mText.getGlobalBounds().width / 2, mText.getGlobalBounds().height / 2));
		mText.setPosition(sf::Vector2f(300, 325));
		mMenuList.push_back(mText);

		mText.setString("Fullscreen");
		mText.setOrigin(sf::Vector2f(mText.getGlobalBounds().width / 2, mText.getGlobalBounds().height / 2));
		mText.move(sf::Vector2f(0, 25));
		mMenuList.push_back(mText);

		mText.setString("Quit");
		mText.setOrigin(sf::Vector2f(mText.getGlobalBounds().width / 2, mText.getGlobalBounds().height / 2));
		mText.move(sf::Vector2f(0, 25));
		mMenuList.push_back(mText);

		mText.setString("Raven\nCatch!");
		mText.setColor(sf::Color(255,153,153,255));
		mText.setOrigin(100, 50);
		mText.setPosition(sf::Vector2f(300, 50));
		mText.setCharacterSize(84);

		mMenuCursor = 0;
		mDelay = 0.0f;

		mBackgroundTexture.loadFromFile(resourcePath() + "titlebg_1.png");
		mBackgroundTexture.setRepeated(true);
		mBackground.setTexture(mBackgroundTexture);
		

		mForegroundTexture.loadFromFile(resourcePath() + "titlebg_2.png");
		mForeground.setTexture(mForegroundTexture);
		
		mBackgroundMusic.openFromFile(resourcePath() + "raven_title.wav");
		mBackgroundMusic.setLoop(true);
		mBackgroundMusic.setVolume(5.0f);
		mBackgroundMusic.play();

		mSoundBuffer.loadFromFile(resourcePath() + "pickup.wav");
		mSoundEffect.setBuffer(mSoundBuffer);


		mCamera = Game::getInstance().mCamera;
		mGameMode = GameState::TitleScreen;
	}

	void loop()
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && mDelay >= 10.0f)
		{
			mMenuCursor = mMenuCursor > 0 ? mMenuCursor - 1 : mMenuList.size() - 1;
			mDelay = 0.0f;
			mSoundEffect.play();
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && mDelay >= 10.0f)
		{
			mMenuCursor = mMenuCursor < mMenuList.size() - 1 ? mMenuCursor + 1 : 0;
			mDelay = 0.0f;
			mSoundEffect.play();
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			switch(mMenuCursor)
			{
				case 0 :
					Game::getInstance().mGameMode = GameState::GamePlay;
					mBackgroundMusic.stop();
					break;
				case 1 :
					Game::getInstance().mFullscreen = !Game::getInstance().mFullscreen;
					Game::getInstance().mWindow.create(sf::VideoMode(600, 400), "Raven Catch!", Game::getInstance().mFullscreen ? sf::Style::Fullscreen : sf::Style::Close | sf::Style::Resize);
					break;
				case 2 :
					Game::getInstance().mWindow.close();
					break;
				default : break;
			}
		}

		for(unsigned int i = 0; i < mMenuList.size(); i++)
		{
			if(i == mMenuCursor)
			{
				mMenuList[i].setRotation(5 * sinf(5 * AnimClock::asSeconds()));
				mMenuList[i].setScale(1.0f, 0.5 + fabs(sinf(5 * AnimClock::asSeconds())) / 2);
				mMenuList[i].setColor(sf::Color(255,153,51,255));
			}
			else
			{
				mMenuList[i].setRotation(0);
				mMenuList[i].setScale(1.0f, 1.0f);
				mMenuList[i].setColor(sf::Color(255,153,153,255));
			}
		}

		mBackground.setTextureRect(sf::IntRect(mBackground.getTextureRect().left + 5, 0, 600, 400));
		mForeground.setPosition(0.0f, 25 + 25 * sinf(5 * AnimClock::asSeconds()));

		if(mDelay < 10.0f)
			mDelay += 1.0f;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.setView(mCamera);

		target.draw(mBackground);
		target.draw(mForeground);

		for(unsigned int i = 0; i < mMenuList.size(); i++)
		{
			if(i != mMenuCursor)
				target.draw(mMenuList[i]);
		}

		target.draw(mMenuList[mMenuCursor]);

		target.draw(mText);
	}

	unsigned int mMenuCursor;
	float mDelay;

	sf::Font mFont;
	sf::Text mText;
	sf::SoundBuffer mSoundBuffer;
	sf::Music mBackgroundMusic;
	sf::Sound mSoundEffect;
	sf::Sprite mBackground;
	sf::Sprite mForeground;
	sf::Texture mBackgroundTexture;
	sf::Texture mForegroundTexture;
	std::vector<sf::Text> mMenuList;
};


#endif