#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Game.h"
#include "Titlescreen.h"
#include "Gameplay.h"


int main()
{
	Game& game = Game::getInstance();
	game.init();
	game.loop();
	game.quit();

	return EXIT_SUCCESS;
}

void Game::init()
{
	srand(mClock.getElapsedTime().asSeconds());

	mWindow.create(sf::VideoMode(600, 400), "Raven Catch!", sf::Style::Close | sf::Style::Resize);
	mCamera = mWindow.getDefaultView();
	mFullscreen = false;

	mLoopCount = 0;
	mLoopLimit = 10;
	mFrameLimit = 1.0f / 60.0f;
	mFrameTime = 0.0f;
	mDeltaTime = 0.0f;
	mFrameTotal = 0.0f;

	mGameMode = GameState::TitleScreen;
	mGameState = new GamePlayState();
}

void Game::loop()
{
	while(mWindow.isOpen())
	{
		mFrameTime = mClock.restart().asSeconds();

		while(mWindow.pollEvent(mEvent))
		{
			switch(mEvent.type)
			{
				case sf::Event::Closed:
					mWindow.close();
					break;
				default:
					break;
			}
		}

		if(mWindow.hasFocus())
		{
			mLoopCount = 0;
			mFrameTotal += mFrameTime;

			while(mFrameTotal > mFrameLimit && mLoopCount < mLoopLimit)
			{
				if(mGameState->mGameMode != mGameMode)
				{
					switch(mGameMode)
					{
						case GameState::TitleScreen :
							mGameState = new TitleScreenState();
							break;
						case GameState::GamePlay :
							mGameState = new GamePlayState();
							break;
						case GameState::CreditScreen :
							break;
						default :
							break;
					}

					mGameState->init();
				}


				mGameState->loop();

				mFrameTotal -= mFrameLimit;
				++mLoopCount;
			}

			mDeltaTime = mFrameTotal / mFrameLimit;
		}

		mWindow.clear(sf::Color::Cyan);
		mWindow.draw(*mGameState);
		mWindow.display();
	}
}

void Game::quit()
{
	mWindow.close();
}