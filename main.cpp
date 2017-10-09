#include "DigitalCanvas2D.h"
#include "RGBColors.h"
#include <iostream>
#include <Windows.h>
#include <stdlib.h>

using namespace std;
using namespace glm;
int width = 1024;
int height = 768;
DigitalCanvas2D my_canvas("Asteroid_Shooting_Game", 1024, 768); // Canvas : (-1.0, -1.0) x (1.0, 1.0)

double randomDouble(void) {
	return (float)rand() / (float)RAND_MAX * 2.0f - 1.0f;
}
double randomDouble2(void) {
	return (float)rand() / (float)RAND_MAX;
}
class Player
{
public:
	vec3 center_ = vec3(0.0, -1.0, 1.0);
	vec3 direction_;
	float radius_ = 0.25;
	int life = 1;

	void draw()
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.drawFilledBox(RGBColors::green, 0.25, 0.05);
		my_canvas.translate(0.0, -0.05);
		my_canvas.drawFilledBox(RGBColors::green, 0.05, 0.25);
		my_canvas.translate(0.0, -0.10);
		my_canvas.drawFilledBox(RGBColors::green, 0.15, 0.03);
		my_canvas.endTransformation();
	}
};

class Blackout {
public:
	vec3 center_;
	
	void draw()
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.drawFilledBox(RGBColors::black, 3.0, 2.0);
		my_canvas.endTransformation();
	}
};

class MyBullet
{
public:
	vec3 center_;
	vec3 velocity_;
	float radius_ = 0.02;
	int life = 1;
	
	void draw()
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.drawFilledCircle(RGBColors::black, radius_, 8);
		my_canvas.endTransformation();
	}

	
	void update(const float& dt)
	{
		center_ += velocity_*dt;
	}

};

class Asteroid
{
public:
	vec3 center_;
	vec3 velocity_;
	float radius_ = 0.1;
	int life = 1;
	void draw()
	{
		my_canvas.beginTransformation();
		my_canvas.translate(center_.x, center_.y);
		my_canvas.drawFilledCircle(RGBColors::gray, radius_, 6);
		my_canvas.endTransformation();
	}
	void update(const float& dt)
	{
		center_ += velocity_*dt;
	}
};

bool Score(Asteroid *a, MyBullet *b) {
	return (b->center_.x - a->center_.x)*(b->center_.x - a->center_.x) +
		(b->center_.y - a->center_.y)*(b->center_.y - a->center_.y)<
		(a->radius_ + b->radius_)*(a->radius_ + b->radius_);
}

bool Crash(Asteroid *a, Player b) {
	return (b.center_.x - a->center_.x)*(b.center_.x - a->center_.x) +
		(b.center_.y - a->center_.y)*(b.center_.y - a->center_.y)<
		(a->radius_ + b.radius_)*(a->radius_ + b.radius_);
}



int main(void)
{
	float time = 0.0;
	int score = 0;
	int i = 0;
	Player player;
	Blackout blackout;
	int startTime = 0;
	int currentTime = 0;
	int delay = 1;
	int num = 0;

	vector<MyBullet *> bullets;
	vector<Asteroid *> asteroids;

	my_canvas.show([&]
	{
		// update

		// move player
		if (my_canvas.isKeyPressed(GLFW_KEY_LEFT))
			if (player.center_.x >= -1.0)
				player.center_.x -= 0.01;
		if (my_canvas.isKeyPressed(GLFW_KEY_UP))
			if (player.center_.y <= 1.0)
				player.center_.y += 0.01;
		if (my_canvas.isKeyPressed(GLFW_KEY_RIGHT))
			if (player.center_.x <= 1.0)
				player.center_.x += 0.01;
		if (my_canvas.isKeyPressed(GLFW_KEY_DOWN))
			if (player.center_.y >= -1.0)
				player.center_.y -= 0.01;

		// shoot a bullet
		if (my_canvas.isKeyPressed(GLFW_KEY_SPACE))
		{
			++currentTime;
			if (currentTime - startTime > delay) {
				//Reset
				startTime = currentTime;
				currentTime = 0;
				bullets.push_back(new MyBullet());
				bullets.push_back(new MyBullet());

				int size = bullets.size();
				bullets[size - 2]->center_ = player.center_;
				bullets[size - 2]->center_.x -= 0.1;
				bullets[size - 2]->center_.y += 0.1;
				bullets[size - 2]->velocity_ = vec3(0.0, 2.0, 0.0);
				bullets[size - 1]->center_ = player.center_;
				bullets[size - 1]->center_.x += 0.1;
				bullets[size - 1]->center_.y += 0.1;
				bullets[size - 1]->velocity_ = vec3(0.0, 2.0, 0.0);

			}
		}

		if (rand() % 150 == 0) {
			asteroids.push_back(new Asteroid());
			int size = asteroids.size();
			asteroids[size - 1]->center_.x = randomDouble();
			asteroids[size - 1]->center_.y = randomDouble2();
			asteroids[size - 1]->velocity_ = vec3(randomDouble(), randomDouble(), 0.0);

		}


		for (int j = 0; j < asteroids.size(); j++) {
			if (asteroids[j] == nullptr) continue;
			if (asteroids[j] != nullptr) asteroids[j]->update(0.1 / 60.0f);
			if (asteroids[j] != nullptr) asteroids[j]->draw();
			if (asteroids[j]->life == 0) asteroids[j] = nullptr;
		}

		if (player.life != 0) {
			player.draw();

			for (int i = 0; i < bullets.size(); i++)
			{
				if (bullets[i] == nullptr)continue;

				if (bullets[i] != nullptr) bullets[i]->update(0.1 / 60.0f);
				if (bullets[i] != nullptr) bullets[i]->draw();
				if (bullets[i]->life == 0) bullets[i] = nullptr;
			}
		}
		else {
			blackout.draw();
		}

		for (int j = 0; j < asteroids.size(); j++) {
			for (int i = 0; i < bullets.size(); i++) {
				if (asteroids[j] != nullptr && bullets[i] != nullptr) {
					if (Score(asteroids[j], bullets[i]) == 1) {
						asteroids[j]->life = 0;
						bullets[i]->life = 0;
					}
				}
			}
			if (asteroids[j] != nullptr) {
				if (Crash(asteroids[j], player) == 1) {
					player.life = 0;
				}
			}
		}

		time += 1 / 60.0;


	}
	);

	return 0;

}
