#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <bitset>
#include "Game.h"

struct Player : public Entity
{
	void init(sf::Texture& texture)
	{
		mType = Entity::Types::Player;
		mAction = Actions::Idle;
		mDirection = Directions::Up;

		mSprite.setTexture(texture);
		mSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
		mSprite.setOrigin(16, 16);
		mSprite.setPosition(300, 300);

		mCollider.setFillColor(sf::Color::Transparent);
		mCollider.setOutlineColor(sf::Color::Magenta);
		mCollider.setOutlineThickness(1.0f);
		mCollider.setPosition(mSprite.getPosition());
		mCollider.setSize(sf::Vector2f(32, 16));
		mCollider.setOrigin(mSprite.getOrigin().x, mSprite.getOrigin().y - 16);

		mSpeed = sf::Vector2f(2.0f, 2.0f);
		mDelay = 0.0f;
		mDirectionKeys = 0;
	}

	void loop()
	{
		mAction = Actions::Idle;

		//Up Key
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			mDirectionKeys |= Directions::Up;

			if((mDirectionKeys) == Directions::Up)
			{
				mDirection = Directions::Up;
				mCollider.move(0.0f, -mSpeed.y);
			}
		}
		else
			mDirectionKeys &= ~Directions::Up;

		//Down Key
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			mDirectionKeys |= Directions::Down;

			if((mDirectionKeys) == Directions::Down)
			{
				mDirection = Directions::Down;
				mCollider.move(0.0f, mSpeed.y); 
			}
		}
		else
			mDirectionKeys &= ~Directions::Down;

		//Left Key
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			mDirectionKeys |= Directions::Left;

			if(mDirectionKeys == Directions::Left)
			{
				mDirection = Directions::Left;
				mCollider.move(-mSpeed.x, 0.0f); 
			}
		}
		else
			mDirectionKeys &= ~Directions::Left;

		//Right Key
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			mDirectionKeys |= Directions::Right;

			if(mDirectionKeys == Directions::Right)
			{
				mDirection = Directions::Right;
				mCollider.move(mSpeed.x, 0.0f); 
			}
		}
		else
			mDirectionKeys &= ~Directions::Right;

		if(mDirectionKeys)
			mAction = Actions::Moving;

		switch(mAction)
		{
			case Actions::Idle :
				mSprite.setRotation(0.0f);
				break;
			case Actions::Moving :
				mSprite.setRotation(5 * sinf(20 * AnimClock::asSeconds()));
				break;
			default : break;
		}

		if(mDirection == Directions::Left)
			mSprite.setScale(-1.0f, 1.0f);
		if(mDirection == Directions::Right)
			mSprite.setScale(1.0f, 1.0f);

		if(mDelay < 30.0f)
			mDelay += 1.0f;

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

	sf::SoundBuffer mSoundBuffer;
	sf::Sound mSound;
	sf::Uint8 mDirectionKeys;
};

#endif