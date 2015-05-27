#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

struct AnimClock
{
	static float asSeconds()
	{
		static sf::Clock mAnimClock;
		return mAnimClock.getElapsedTime().asSeconds();
	}
};

enum Directions
{
	Up = 1 << 0,
	Down = 1 << 1,
	Left = 1 << 2,
	Right = 1 << 3
};

enum Actions
{
	Idle,
	Moving,
	ActionA,
	ActionB,
	Dead
};

struct Entity : public sf::Drawable
{
	enum Types
	{
		Player,
		NonPlayer,
		Raven,
		Cage,
		Pickup,
	};

	virtual void init(sf::Texture& texture) {};
	virtual void loop() {};
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {};
	virtual bool collision(sf::RectangleShape other) {return false;};

	sf::RectangleShape mCollider;
	sf::Sprite mSprite;

	sf::Vector2f mSpeed;
	float mDelay;

	Types mType;
	Actions mAction;
	Directions mDirection;
};

struct GameState : public sf::Drawable
{
	enum GameModes
	{
		TitleScreen,
		GamePlay,
		CreditScreen,
		None
	};

	GameState() : mGameMode(GameModes::None) {};
	virtual void init() {};
	virtual void loop() {};
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {};

	sf::View mCamera;
	GameModes mGameMode;
};

struct Game
{
	static Game& getInstance()
	{
		static Game instance;
		return instance;
	}

	void init();
	void loop();
	void quit();

	sf::RenderWindow mWindow;
	sf::View mCamera;
	sf::Event mEvent;

	sf::Clock mClock;
	int mLoopCount;
	int mLoopLimit;
	float mFrameLimit;
	float mFrameTime;
	float mDeltaTime;
	float mFrameTotal;

	bool mFullscreen;

	GameState::GameModes mGameMode;
	GameState* mGameState;
};


#endif