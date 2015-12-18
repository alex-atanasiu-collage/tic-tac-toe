/*
 * Author: Atanasiu Alexandru-Marian, email: alexandru.atanasiu@outlook.com
 * File: main.cpp
 * Description: entry point for TicTacToe game
 */

#include <SFML/Graphics.hpp>

/* screen stats */
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define COLOR_DEPTH 32

/* main function */
int main() {
	/* Video mode construction */
	sf::VideoMode video_mode(WINDOW_WIDTH, WINDOW_HEIGHT, COLOR_DEPTH);

	/* Window creation */

    sf::RenderWindow window(video_mode, "TIC TAC TOE");
	window.setVerticalSyncEnabled(true);

	//sf::RectangleShape* lines[4];

/*
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
	shape.setPosition(400, 300);
*/

	/* Main Loop */
    while (window.isOpen())
    {
		/* Event parsing */
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
			}
        }

        window.clear(sf::Color(255,255,255));
        //window.draw(shape);
        window.display();
    }

    return 0;
}