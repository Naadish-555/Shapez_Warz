#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"
#include "Vec2.h"






int main()
{
	Game g(RESOURCES_PATH "config.txt");
	g.run();
	std::cout << "Output on console" << std::endl;

	std::cout << std::endl << "=====================================================================================";

	ImGui::SFML::Shutdown();

	return 0;
}