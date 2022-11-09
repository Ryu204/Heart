#include <vector>
#include <cassert>
#include <iostream>

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

// Animation parameters
float stroke_offset = 0.f;
int stroke_count = 30;
constexpr int stroke_speed = 10;
int stroke_layer = 15;
float stroke_size = 10.f;
float heart_beat = 0.f;
float heart_beat_speed = 5.f;
float heart_size = 30.f;
float intensity = 0.3f;
float frequency = 10;
// global varibles
RenderWindow win(VideoMode(800, 800), "<33", Style::Fullscreen);
std::vector<std::vector<CircleShape>> stroke(stroke_layer);
constexpr float PI = 3.141592f;
Clock clk;
const Time TPF(sf::seconds(1.f / 60));
Time elapsed_time = sf::Time::Zero;
// Color
std::vector<Color> color_list = { Color(255, 136, 134), Color(255, 168, 181), Color(255, 204, 203), Color(255, 220, 209), Color(255, 104, 99) };

// Utilities
template<typename T>  
void center(T& e);
float lerp(float a, float b, float t);
float nlerp(float a, float b, float t, bool type = true);
float heartbeat(float t = 0.f);

// parametric equation of heart
Vector2f get_coordinate(float t, bool left = true)
{
	return Vector2f((0.5f - left) * 2 * 16 * sin(t) * sin(t) * sin(t), 
		-(13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t)));
}

void init()
{
	sf::View vi = win.getDefaultView();
	vi.setCenter(Vector2f(0, 0));
	win.setView(vi);
}

void buffer_heart()
{
	for (auto& i : stroke)
		i.clear();
	CircleShape c(0, 10);
	
	for (int i = 0; i < stroke_layer; i++)
	{
		int color_id = (int)lerp(0, color_list.size() - 1.f, (stroke_layer - i) * 1.f / stroke_layer);
		c.setRadius(nlerp(0, stroke_size, (i + 1) * 1.f / stroke_layer, false));
		center(c);
		c.setFillColor(color_list[color_id]);
		float scale = heartbeat(heart_beat) * nlerp(0, heart_size, (i + 1) * 1.f / stroke_layer, false);
		int temp_strkc = nlerp(0, stroke_count, (i + 1) * 1.f / stroke_layer, false);
		// float temp_off = stroke_offset / (stroke_layer - i);
		float temp_off = nlerp(0, stroke_offset, (stroke_layer - i) * 1.f / stroke_layer, false);
		for (float t = temp_off; t < PI + temp_off; t += PI / temp_strkc)
		{
			float tmp = fmod(t, PI);
			Color cl = c.getFillColor();
			cl.a = nlerp(0, 255, 1 - i * 1.f / stroke_layer);
			c.setFillColor(cl);
			// c.setFillColor(cl);

			c.setPosition(scale * get_coordinate(tmp, true));
			stroke[i].push_back(c);
			c.setPosition(scale * get_coordinate(tmp, false));
			stroke[i].push_back(c);
		}
	}
}

void update_heart(Time dt)
{
	stroke_offset += stroke_speed * dt.asSeconds();
	heart_beat += heart_beat_speed * dt.asSeconds();
	heart_beat = fmod(heart_beat, 1.f);
	buffer_heart();
}

void render()
{
	win.clear();
	
	for (auto& i : stroke)
		for (auto& j : i)
			win.draw(j);

	win.display();
}

int main()
{
	srand(time(0));
	init();
	while (win.isOpen())
	{
		elapsed_time += clk.restart();

		while (elapsed_time > TPF)
		{
			Event e;
			while (win.pollEvent(e))
			{
				if (e.type == sf::Event::Closed || e.type == sf::Event::KeyPressed)
					win.close();
			}

			elapsed_time -= TPF;
			update_heart(elapsed_time);
		}

		render();
	}
}

// Utilities definition
template<typename T>
void center(T& e)
{
	FloatRect b = e.getLocalBounds();
	e.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
}

float lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

float nlerp(float a, float b, float t, bool type)
{
	if (type)
		return a + (b - a) * (-sin(t * PI / 2 + PI / 2) + 1);
	else
		return a + (b - a) * sin(t * PI / 2);
}

float heartbeat(float t) {
	const float v = atan(sin(t * PI * frequency) * intensity);
	return (v + PI / 2) / PI;
}