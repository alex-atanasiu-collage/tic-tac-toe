/*
 * Author: Atanasiu Alexandru-Marian, email: alexandru.atanasiu@outlook.com
 * File: main.cpp
 * Description: entry point for TicTacToe game
 */

#include "constants.h"
#include <SFML/Graphics.hpp>
#include <list>

/* current game state */
GameState current_state;

/* game grid */
Cell game_matrix[3][3];

/* shapes to be drawn on the screen */
std::list<sf::CircleShape*> shapes;

/* function that initializes the game */
void initialize_game(){
	/* clean the shape list */
	for(sf::CircleShape* crt : shapes){
		delete crt;
	}
	shapes.clear();

	/* set the current state */
	current_state = X_TURN_STATE;	

	/* empty the game grid */
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			game_matrix[i][j] = EMPTY_CELL;
		}
	}
}

/* function that make a move for the given player at the given coordonates
   it returns true if the move succeeded
   it returns false if the move is illegal */
bool make_move(unsigned char player, unsigned char x, unsigned char y){
	/* checks if the move is legal */
	if(game_matrix[x][y] != EMPTY_CELL){
		return false;
	}
	
	/* places move */
	game_matrix[x][y] = player;
	sf::CircleShape *shape = new sf::CircleShape(95);
	shape->setOrigin(95, 95);
	shape->setPosition(x * (WINDOW_WIDTH / 3) + 105, y * (WINDOW_HEIGHT / 3) + 105);
	if(player == X_TURN_STATE){
		shape->setFillColor(sf::Color(255,0,0));	
	} else {
		shape->setFillColor(sf::Color(0,0,255));	
	}

	shapes.push_back(shape);
	return true;
}

/* struct used for line configuration */
struct config{
	Cell c1, c2, c3;
	config(Cell c1, Cell c2, Cell c3){
		this->c1 = c1;
		this->c2 = c2;
		this->c3 = c3;
	}
};

/* function that returns true if either of the players has won */
bool game_ended(){
	std::list<config> conf;
	conf.push_back(config(game_matrix[0][0],game_matrix[0][1],game_matrix[0][2]));
	conf.push_back(config(game_matrix[1][0],game_matrix[1][1],game_matrix[1][2]));
	conf.push_back(config(game_matrix[2][0],game_matrix[2][1],game_matrix[2][2]));
	conf.push_back(config(game_matrix[0][0],game_matrix[1][0],game_matrix[2][0]));
	conf.push_back(config(game_matrix[0][1],game_matrix[1][1],game_matrix[2][1]));
	conf.push_back(config(game_matrix[0][2],game_matrix[1][2],game_matrix[2][2]));
	conf.push_back(config(game_matrix[0][0],game_matrix[1][1],game_matrix[2][2]));
	conf.push_back(config(game_matrix[0][2],game_matrix[1][1],game_matrix[2][0]));

	for(config cfg : conf){
		if(cfg.c1 == cfg.c2 && cfg.c2 == cfg.c3 && cfg.c1 != EMPTY_CELL){
			return true;
		}
	}
	return false;	
}

/* function that cleans the remaining resources */
void clean_game(){
	for(sf::CircleShape* crt : shapes){
		delete crt;
	}
}

/* main function */
int main() {
	/* Video mode construction */
	sf::VideoMode video_mode(WINDOW_WIDTH, WINDOW_HEIGHT, COLOR_DEPTH);

	/* Window creation */
    sf::RenderWindow window(video_mode, "TIC TAC TOE");
	window.setVerticalSyncEnabled(true);


	/* placement of grid lines */
	sf::RectangleShape horiz_line1;
	horiz_line1.setSize(sf::Vector2f(WINDOW_WIDTH,10));
	horiz_line1.setFillColor(sf::Color(0,0,0));
	horiz_line1.setPosition(sf::Vector2f(0, WINDOW_HEIGHT / 3));

	sf::RectangleShape horiz_line2;
	horiz_line2.setSize(sf::Vector2f(WINDOW_WIDTH,10));
	horiz_line2.setFillColor(sf::Color(0,0,0));
	horiz_line2.setPosition(sf::Vector2f(0, 2 * WINDOW_HEIGHT / 3));

	sf::RectangleShape vert_line1;
	vert_line1.setSize(sf::Vector2f(10,WINDOW_HEIGHT));
	vert_line1.setFillColor(sf::Color(0,0,0));
	vert_line1.setPosition(sf::Vector2f(WINDOW_WIDTH / 3, 0));

	sf::RectangleShape vert_line2;
	vert_line2.setSize(sf::Vector2f(10,WINDOW_HEIGHT));
	vert_line2.setFillColor(sf::Color(0,0,0));
	vert_line2.setPosition(sf::Vector2f(2 * WINDOW_WIDTH / 3, 0));



	initialize_game();
	/* Main Loop */
    while (window.isOpen()) {
		/* Event parsing */
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
			} else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::R){
					initialize_game();	
				}
			}
        }
		
		if(current_state != GAME_FINISHED){
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				sf::Vector2i position = sf::Mouse::getPosition(window);
				
				int x = position.x / (WINDOW_WIDTH / 3);
				int y = position.y / (WINDOW_HEIGHT / 3);

				if(make_move(current_state, x, y)){
					if(!game_ended()){
						current_state = 3 - current_state;
					} else {
						current_state = GAME_FINISHED;
					}
				}
			}
		}

		/* clear screen buffer */
		window.clear(sf::Color(255,255,255));

		/* draw the grid */
		window.draw(horiz_line1);
		window.draw(horiz_line2);
		window.draw(vert_line1);
		window.draw(vert_line2);

		/* draw shapes */
		for(sf::CircleShape* crt : shapes) {
			window.draw(*crt);
		}        


        window.display();
    }

    return 0;
}