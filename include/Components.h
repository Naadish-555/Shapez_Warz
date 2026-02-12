#pragma once

#include "Vec2.h"
#include <SFML\Graphics.hpp>

class Component
{
public:
	bool exists = false;	
};

class CTransform : public Component
{
public:
	Vec2 pos		= { 0.0, 0.0 };
	Vec2 velocity	= { 0.0, 0.0 };
	float angle		= 0;

	CTransform() = default;
	CTransform(const Vec2& p, const Vec2& v, float a)
		:pos(p), velocity(v), angle(a) {}
};

class CShape : public Component
{
public:
	sf::CircleShape circle;

	CShape() = default;
	CShape(float radius, int points, const sf::Color& fill, const sf::Color& outline, float thickness)
		: circle(radius, points)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);
	}		

};

class CCollision : public Component
{
public:
	float radius = 0;

	CCollision() = default;
	CCollision(float r)
		: radius(r) {}
};

class CScore : public Component
{
	//for assigning points to the given enemy e.g 100,200,etc when it dies 
public:
	int score = 0;

	CScore() = default;
	CScore(int s)
		: score(s) {}
};

class CLifespan : public Component
{
public:
	int remaining = 0; //lifespan remaing on entity
	int total	  = 0; //total amount of lifespan

	CLifespan() = default;
	CLifespan(int total)
		:remaining(total), total(total){}
};

class CInput : public Component
{
public:
	bool up = false;
	bool left = false;
	bool right = false;
	bool down = false;
	bool shoot = false;
	
	CInput() = default;
};