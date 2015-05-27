#ifndef RAVEN_H
#define RAVEN_H

#include <SFML/Graphics.hpp>
#include "Game.h"

struct Raven : public Entity
{
	void init(sf::Texture& texture)
	{
		mType = Entity::Types::Raven;
		mAction = Actions::Idle;
		mDirection = Directions::Down;
		
		mSprite.setTexture(texture);
		mSprite.setTextureRect(sf::IntRect(64, 0, 32, 32));
		mSprite.setOrigin(16, 16);
		mSprite.setPosition(rand() % 550, rand() % 350);

		mCollider.setFillColor(sf::Color::Transparent);
		mCollider.setOutlineColor(sf::Color::Magenta);
		mCollider.setOutlineThickness(1.0f);
		mCollider.setPosition(mSprite.getPosition());
		mCollider.setSize(sf::Vector2f(32, 16));
		mCollider.setOrigin(mSprite.getOrigin().x, mSprite.getOrigin().y - 16);

		mSpeed = sf::Vector2f(2.5f, 2.5f);
		mDelay = 0.0f;
	}

	void loop()
	{
		int actionChoice = mAction;
		int directionChoice = mDirection;

		if(mDelay <= 0.0f)
		{
			if(mAction != Actions::ActionB)
				actionChoice = rand() % 2;

			switch(actionChoice)
			{
				case 0 : mAction = Actions::Idle; break;
				case 1 : mAction = Actions::Moving; break;
				default : break;
			}

			int directionChoice = rand() % 4;

			switch(directionChoice)
			{
				case 0 : mDirection = Directions::Up; break;
				case 1 : mDirection = Directions::Down; break;
				case 2 : mDirection = Directions::Left; break;
				case 3 : mDirection = Directions::Right; break;
				default : break;
			}
			
			mDelay = 20.0f + rand() % 10;
		}

		mSprite.setTextureRect(sf::IntRect(64, 0, 32, 32));

		switch(mAction)
		{
			case Actions::Idle :
				mSprite.setRotation(0.0f);
				break;
			case Actions::Moving :
				mSpeed = sf::Vector2f(2.5, 2.5);
				if(mDirection == Directions::Up)
					mCollider.move(0.0f, -mSpeed.y);
				if(mDirection == Directions::Down)
					mCollider.move(0.0f, mSpeed.y); 
				if(mDirection == Directions::Left)
					mCollider.move(-mSpeed.x, 0.0f); 
				if(mDirection == Directions::Right)
					mCollider.move(mSpeed.x, 0.0f);
				
				mSprite.setRotation(5 * sinf(50 * AnimClock::asSeconds()));
				break;
			case Actions::ActionA :
				mSpeed = sf::Vector2f(5.0, 5.0);

				if(mDirection == Directions::Up)
					mCollider.move(0.0f, -mSpeed.y);
				if(mDirection == Directions::Down)
					mCollider.move(0.0f, mSpeed.y); 
				if(mDirection == Directions::Left)
					mCollider.move(-mSpeed.x, 0.0f); 
				if(mDirection == Directions::Right)
					mCollider.move(mSpeed.x, 0.0f);

				mSprite.setTextureRect(sf::IntRect(64, 32, 32, 32));
				mSprite.setRotation(10 * sinf(50 * AnimClock::asSeconds()));
				break;
			case Actions::ActionB :
				mSprite.setTextureRect(sf::IntRect(64, 64, 32, 32));
				mSprite.setRotation(5 * sinf(20 * AnimClock::asSeconds()));
				break;
			default : break;
		}

		if(mDirection == Directions::Left)
			mSprite.setScale(-1.0f, 1.0f);
		if(mDirection == Directions::Right)
			mSprite.setScale(1.0f, 1.0f);

		mDelay -= 1.0f;
	
		mSprite.setPosition(mCollider.getPosition());
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(mSprite);
		//target.draw(mCollider);
	}

	bool collision(sf::RectangleShape other)
	{
		sf::FloatRect boxA = mCollider.getGlobalBounds();
		sf::FloatRect boxB = other.getGlobalBounds();
		sf::FloatRect overlap;

		if(boxA.intersects(boxB, overlap))
		{
			if(boxA.left + (boxA.width / 2) <= boxB.left + (boxB.width / 2) && overlap.width < overlap.height)
			{
				mCollider.move(-overlap.width, 0.0f);
				mDirection = Directions::Up;
			}
			if(boxA.top + (boxA.height / 2) <= boxB.top + (boxB.height / 2) && overlap.width > overlap.height)
			{
				mCollider.move(0.0f, -overlap.height);
				mDirection = Directions::Left;
			}
			if(boxA.left + (boxA.width / 2) >= boxB.left + (boxB.width / 2) && overlap.width < overlap.height)
			{
				mCollider.move(overlap.width, 0.0f);
				mDirection = Directions::Down;
			}
			if(boxA.top + (boxA.height / 2) >= boxB.top + (boxB.height / 2) && overlap.width > overlap.height)
			{
				mCollider.move(0.0f, overlap.height);
				mDirection = Directions::Right;
			}

			return true;
		}

		return false;
	}

	bool checkPlayerDistance(sf::Vector2f other)
	{
		sf::Vector2f position = mCollider.getPosition();

		float x = std::max(other.x, position.x) - std::min(other.x, position.x);
		float y = std::max(other.y, position.y) - std::min(other.y, position.y);

		if(std::sqrt((x * x) + (y * y)) < 75 && mAction != Actions::ActionB)
		{
			mAction = Actions::ActionA;
			return true;
		}

		return false;
	}

	float mDelay;
};

#endif