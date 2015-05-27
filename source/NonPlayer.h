#ifndef NONPLAYER_H
#define NONPLAYER_H

#include <SFML/Graphics.hpp>
#include "Game.h"


struct NonPlayer : public Entity
{
	void init(sf::Texture& texture)
	{
		mType = Entity::Types::NonPlayer;
		mSprite.setTexture(texture);
		mSprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
		mSprite.setOrigin(16, 16);
		mSprite.setPosition(300, 230);

		mCollider.setFillColor(sf::Color::Transparent);
		mCollider.setOutlineColor(sf::Color::Magenta);
		mCollider.setOutlineThickness(1.0f);
		mCollider.setPosition(mSprite.getPosition());
		mCollider.setSize(sf::Vector2f(32, 16));
		mCollider.setOrigin(mSprite.getOrigin().x, mSprite.getOrigin().y - 16);

		mSpeed = sf::Vector2f(0.0f, 0.0f);
	}

	void loop()
	{
		mSprite.setRotation(5 * sinf(20 * AnimClock::asSeconds()));
		mCollider.move(0.0f, -sinf(100 * AnimClock::asSeconds()));
		
		mSprite.setPosition(mCollider.getPosition());
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(mSprite);
		//target.draw(mCollider);
	}
};

#endif