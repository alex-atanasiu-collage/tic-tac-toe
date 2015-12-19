/*
 * Author: Atanasiu Alexandru-Marian, email: alexandru.atanasiu@outlook.com
 * File: main.cpp
 * Description: entry point for TicTacToe game
 */

#include "constants.h"
#include <SFML/Graphics.hpp>
#include <list>
#include <Windows.h>

/* Texturi */
sf::Texture x_texture, o_texture;

/* current game state */
GameState current_state;

/* ai player */
GameState AI_player = 0;

/* game winner */
Cell winner;

/* game grid */
Cell game_matrix[3][3];

/* shapes to be drawn on the screen */
std::list<sf::Sprite*> sprites;

/* function that initializes the game */
void initialize_game(){
	if(AI_player == 0){
		AI_player = 1;
	} else {
		AI_player = 3 - AI_player;
	}

	/* clean the shape list */
	for(sf::Sprite* crt : sprites){
		delete crt;
	}
	sprites.clear();

	/* set the current state */
	current_state = X_TURN_STATE;	
	winner = 0;
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
	if(x < 0 || x >= 3 || y < 0 || y >= 3){
		return false;
	}

	/* checks if the move is legal */
	if(game_matrix[x][y] != EMPTY_CELL){
		return false;
	}
	
	/* places move */
	game_matrix[x][y] = player;

	sf::Sprite *sprite = NULL;
	if(player == X_TURN_STATE){
		sprite = new sf::Sprite(x_texture);
	} else {
		sprite = new sf::Sprite(o_texture);
	}

	sprite->setOrigin(95, 95);
	sprite->setPosition((float)(x * (WINDOW_WIDTH / 3) + 105),(float)(y * (WINDOW_HEIGHT / 3) + 105));
	
	sprites.push_back(sprite);

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
			winner = cfg.c1;
			return true;
		}
	}

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			if(game_matrix[i][j] == EMPTY_CELL){
				return false;
			}
		}
	}

	winner = 3;
	return true;	
}

/* function that cleans the remaining resources */
void clean_game(){
	for(sf::Sprite* crt : sprites){
		delete crt;
	}
}



/***********************************************************************************/
#define WIN_GAIN 1
#define DRAW_GAIN 0
#define LOSE_GAIN -1

unsigned char who_won(Cell crt_matrix[3][3]){
	std::list<config> conf;
	conf.push_back(config(crt_matrix[0][0],crt_matrix[0][1],crt_matrix[0][2]));
	conf.push_back(config(crt_matrix[1][0],crt_matrix[1][1],crt_matrix[1][2]));
	conf.push_back(config(crt_matrix[2][0],crt_matrix[2][1],crt_matrix[2][2]));
	conf.push_back(config(crt_matrix[0][0],crt_matrix[1][0],crt_matrix[2][0]));
	conf.push_back(config(crt_matrix[0][1],crt_matrix[1][1],crt_matrix[2][1]));
	conf.push_back(config(crt_matrix[0][2],crt_matrix[1][2],crt_matrix[2][2]));
	conf.push_back(config(crt_matrix[0][0],crt_matrix[1][1],crt_matrix[2][2]));
	conf.push_back(config(crt_matrix[0][2],crt_matrix[1][1],crt_matrix[2][0]));

	for(config cfg : conf){
		if(cfg.c1 == cfg.c2 && cfg.c2 == cfg.c3 && cfg.c1 != EMPTY_CELL){
			return cfg.c1;
		}
	}

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			if(crt_matrix[i][j] == EMPTY_CELL){
				return -1;
			}
		}
	}

	return 0;
}


int minmax(unsigned char& x, unsigned char& y, Cell crt_matrix[3][3], GameState crt_player){
	unsigned char crt_winner = who_won(crt_matrix);
	if(crt_winner == AI_player){
		return WIN_GAIN;
	} else if(crt_winner == 3 - AI_player){
		return LOSE_GAIN;
	} else if(crt_winner == 0){
		return DRAW_GAIN;
	} 

	int gain = -5;
	for(int i=0; i < 3; i++){
		for(int j=0; j < 3; j++){
			if(crt_matrix[i][j] == EMPTY_CELL){
				Cell new_matrix[3][3];
				for(int ii=0; ii < 3; ii++){
					for(int jj=0; jj < 3; jj++){
						new_matrix[ii][jj] = crt_matrix[ii][jj];
					}
				}
				new_matrix[i][j] = crt_player;
				
				unsigned char bestx = i, besty = j;
				int crt_gain = minmax(bestx, besty, new_matrix, 3 - crt_player);
				if(gain < crt_gain){
					gain = crt_gain;
					x = bestx;
					y = besty;
				}

				if(gain == WIN_GAIN){
					return WIN_GAIN;
				}
			}	
		}
	}

	return gain;
} 


void minmax_computer(){
	unsigned char x = -1, y = -1;	
	Cell crt_matrix[3][3];
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			crt_matrix[i][j] = game_matrix[i][j];
		}
	}
	
	minmax(x, y, game_matrix, AI_player);
	if(!make_move(current_state, x, y)){
		MessageBox(NULL, L"AI Invalid move. Termination iminent.", L"AI error", MB_OK | MB_ICONERROR);
		exit(1);
	}
}

void dumb_computer1(){
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			if(make_move(current_state, i, j)){
				return;
			}
		}
	}
}

/***********************************************************************************/

/* main function */
int main() {
	/* Video mode construction */
	sf::VideoMode video_mode(WINDOW_WIDTH, WINDOW_HEIGHT, COLOR_DEPTH);

	/* Window creation */
    sf::RenderWindow window(video_mode, "TIC TAC TOE", sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	if(!x_texture.loadFromFile("x_simbol.png")){
		MessageBox(NULL, L"Could not load game resource! Termination iminent.", L"Resource load error", MB_OK | MB_ICONERROR);
		return 1;
	}

	if(!o_texture.loadFromFile("o_simbol.png")){
		MessageBox(NULL, L"Could not load game resource! Termination iminent.", L"Resource load error", MB_OK | MB_ICONERROR);
		return 1;	
	}


	/* placement of grid lines */
	sf::RectangleShape horiz_line1;
	horiz_line1.setSize(sf::Vector2f(WINDOW_WIDTH,10));
	horiz_line1.setFillColor(sf::Color(255,255,255));
	horiz_line1.setPosition(sf::Vector2f(0, WINDOW_HEIGHT / 3));

	sf::RectangleShape horiz_line2;
	horiz_line2.setSize(sf::Vector2f(WINDOW_WIDTH,10));
	horiz_line2.setFillColor(sf::Color(255,255,255));
	horiz_line2.setPosition(sf::Vector2f(0, 2 * WINDOW_HEIGHT / 3));

	sf::RectangleShape vert_line1;
	vert_line1.setSize(sf::Vector2f(10,WINDOW_HEIGHT));
	vert_line1.setFillColor(sf::Color(255,255,255));
	vert_line1.setPosition(sf::Vector2f(WINDOW_WIDTH / 3, 0));

	sf::RectangleShape vert_line2;
	vert_line2.setSize(sf::Vector2f(10,WINDOW_HEIGHT));
	vert_line2.setFillColor(sf::Color(255,255,255));
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
			if (AI_player == current_state){
				minmax_computer();
				if(!game_ended()){
					current_state = 3 - current_state;
				} else {
					current_state = GAME_FINISHED;
				}
			} else {
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
		}

		/* clear screen buffer */
		window.clear(sf::Color(0,0,0));

		/* draw the grid */
		window.draw(horiz_line1);
		window.draw(horiz_line2);
		window.draw(vert_line1);
		window.draw(vert_line2);

		/* draw shapes */
		for(sf::Sprite* crt : sprites){
			window.draw(*crt);
		}		

        window.display();

		if(current_state == GAME_FINISHED){
			int res;
			if(winner == 1){
				res = MessageBox(NULL, L"X won! Would you like to play another game?", L"Game ended", MB_YESNO);
			} else if(winner == 2){
				res = MessageBox(NULL, L"O won! Would you like to play another game?", L"Game ended", MB_YESNO);
			} else if(winner == 3){
				res = MessageBox(NULL, L"Draw! Would you like to play another game?", L"Game ended", MB_YESNO);	
			}
			if(res == IDYES){
				initialize_game();
			} else {
				window.close();
			}

		}
    }

	clean_game();

    return 0;
}