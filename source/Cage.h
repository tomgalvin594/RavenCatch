#ifndef CAGE_H
#define CAGE_H

#include <SFML/Graphics.hpp>
#include "Game.h"


struct Cage : public Entity
{
	void init(sf::Texture& texture)
	{
		mType = Entity::Types::Cage;
		mAction = Actions::Moving;
		mDirection = Directions::Up;

		mSprite.setTexture(texture);
		mSprite.setTextureRect(sf::IntRect(0, 32, 32, 32));
		mSprite.setOrigin(16, 16);

		mCollider.setFillColor(sf::Color::Transparent);
		mCollider.setOutlineColor(sf::Color::Magenta);
		mCollider.setOutlineThickness(1.0f);
		mCollider.setSize(sf::Vector2f(mSprite.getGlobalBounds().width, mSprite.getGlobalBounds().height / 2));
		mCollider.setOrigin(mSprite.getOrigin().x, mSprite.getOrigin().y - mSprite.getGlobalBounds().height / 2);

		mSpeed = sf::Vector2f(0.0f, 0.0f);
		mDelay = 15.0f;
	}

	void loop()
	{
		if(mDelay <= 0.0f && mAction != Actions::Dead)
		{
			switch(mAction)
			{
				case Actions::Idle :
						mDelay = -1.0f;
						mAction = Actions::Dead;
						break;
				case Actions::Moving :
						mDelay = 25.0f;
						mAction = Actions::Idle;
						break;
				default : break;
			}
		}

		switch(mAction)
		{
			case Actions::Idle :
				mSprite.setColor(sf::Color(255, 255, 255, (int)mDelay % 5 == 0 ? 0 : 255));
				mSprite.setColor(sf::Color::Red);
				break;
			case Actions::Moving : 
				if(mDirection == Directions::Up)
					mCollider.move(0.0f, -4.5f);
				if(mDirection == Directions::Down)
					mCollider.move(0.0f, 4.5f);
				if(mDirection == Directions::Left)
					mCollider.move(-4.5f, 0.0f);
				if(mDirection == Directions::Right)
					mCollider.move(4.5f, 0.0f);
				break;
			case Actions::ActionB :
				mSprite.setRotation(5 * sinf(20 * AnimClock::asSeconds()));
				break;
			default : break;
		}

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
				mCollider.move(-overlap.width, 0.0f);
			if(boxA.top + (boxA.height / 2) <= boxB.top + (boxB.height / 2) && overlap.width > overlap.height)
				mCollider.move(0.0f, -overlap.height);
			if(boxA.left + (boxA.width / 2) >= boxB.left + (boxB.width / 2) && overlap.width < overlap.height)
				mCollider.move(overlap.width, 0.0f);
			if(boxA.top + (boxA.height / 2) >= boxB.top + (boxB.height / 2) && overlap.width > overlap.height)
				mCollider.move(0.0f, overlap.height);

			return true;
		}

		return false;
	}

	float mDelay;
};

#endif