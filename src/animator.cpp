#include "animator.hpp"

using namespace sf;

void animate(Sprite &sprite, const Texture texture, float animationSpeed, Time elapsedTime)
{
	int frameWidth = 32;
	int frames = (texture.getSize().x) / frameWidth;
	int currentFrame = (int)(elapsedTime.asSeconds() * animationSpeed) % frames;
	sprite.setTextureRect({ IntRect({currentFrame * frameWidth, 0}, {frameWidth, frameWidth}) });
}