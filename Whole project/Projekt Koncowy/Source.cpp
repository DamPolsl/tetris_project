// TETRIS KNOCKOFF :)
//KNOWN BUGS
	
//FEATURES TO ADD
	

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Joystick.hpp>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
using namespace sf;

//debug mode
	bool debug_mode = false;

//constants
	const int WIDTH = 10, HEIGHT = 20;
	const float screen_width = 40, screen_height = 22.5;
	const int CELL_SIZE = 40;
	const int block_width = 4, block_height = 4;
	const int rotation_ammount = 4;

//tablica play_area, aka plansza
int play_area[HEIGHT][WIDTH] = { 0 };

//4d table containing all the different pieces and their rotations
int tetrinos[rotation_ammount][7][block_height][block_width] =
{
//rotation 0
	//O
	0,0,0,0,
	0,1,1,0,
	0,2,2,0,
	0,0,0,0,

	//I
	0,0,0,0,
	2,2,2,2,
	0,0,0,0,
	0,0,0,0,

	//Z
	0,0,0,0,
	2,1,0,0,
	0,2,2,0,
	0,0,0,0,

	//S
	0,0,0,0,
	0,1,2,0,
	2,2,0,0,
	0,0,0,0,

	//J
	0,0,0,0,
	1,0,0,0,
	2,2,2,0,
	0,0,0,0,

	//L
	0,0,0,0,
	0,0,1,0,
	2,2,2,0,
	0,0,0,0,

	//T
	0,0,0,0,
	0,1,0,0,
	2,2,2,0,
	0,0,0,0,
//rotation 1
	//O
	0,0,0,0,
	0,1,1,0,
	0,2,2,0,
	0,0,0,0,

	//I
	0,0,1,0,
	0,0,1,0,
	0,0,1,0,
	0,0,2,0,

	//Z
	0,0,0,0,
	0,0,1,0,
	0,1,2,0,
	0,2,0,0,

	//S
	0,0,0,0,
	0,1,0,0,
	0,2,1,0,
	0,0,2,0,

	//J
	0,0,0,0,
	0,1,2,0,
	0,2,0,0,
	0,2,0,0,

	//L
	0,0,0,0,
	0,1,0,0,
	0,1,0,0,
	0,2,2,0,

	//T
	0,0,0,0,
	0,1,0,0,
	0,1,2,0,
	0,2,0,0,
//rotation 2
	//O
	0,0,0,0,
	0,1,1,0,
	0,2,2,0,
	0,0,0,0,

	//I
	0,0,0,0,
	0,0,0,0,
	2,2,2,2,
	0,0,0,0,

	//Z
	0,0,0,0,
	0,0,0,0,
	2,1,0,0,
	0,2,2,0,

	//S
	0,0,0,0,
	0,0,0,0,
	0,1,2,0,
	2,2,0,0,

	//J
	0,0,0,0,
	0,0,0,0,
	2,2,1,0,
	0,0,2,0,

	//L
	0,0,0,0,
	0,0,0,0,
	1,2,2,0,
	2,0,0,0,

	//T
	0,0,0,0,
	0,0,0,0,
	2,1,2,0,
	0,2,0,0,
//rotation 3
	//O
	0,0,0,0,
	0,1,1,0,
	0,2,2,0,
	0,0,0,0,

	//I
	0,1,0,0,
	0,1,0,0,
	0,1,0,0,
	0,2,0,0,

	//Z
	0,0,0,0,
	0,1,0,0,
	1,2,0,0,
	2,0,0,0,

	//S
	0,0,0,0,
	1,0,0,0,
	2,1,0,0,
	0,2,0,0,

	//J
	0,0,0,0,
	0,1,0,0,
	0,1,0,0,
	2,2,0,0,

	//L
	0,0,0,0,
	2,1,0,0,
	0,1,0,0,
	0,2,0,0,

	//T
	0,0,0,0,
	0,1,0,0,
	2,1,0,0,
	0,2,0,0,
};

//function that hides the console
void hide_console()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}
//function that shows the console
void show_console()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

int main()
{
	if (debug_mode)
		show_console();
	else
		hide_console();
//rendering window and all necessary things
	RenderWindow window(VideoMode( (CELL_SIZE * screen_width), (CELL_SIZE * screen_height)), "Tetris", Style::Close);
	Texture texture, background_texture_top, background_texture_bottom, starting_screen_texture, pause_screen_texture, game_over_texture;
	Sprite s(texture), bg_top(background_texture_top), bg_bottom(background_texture_bottom), starting_screen(starting_screen_texture), pause_screen(pause_screen_texture), game_over_screen(game_over_texture);
	Clock clock, mvmt_clock, landing_clock, rotation_clock;
	Font font;
	Image icon;

	window.setFramerateLimit(60);
	texture.loadFromFile("img/tiles_big.png");
	starting_screen_texture.loadFromFile("img/start_screen.png");
	background_texture_bottom.loadFromFile("img/bg_bottom.png");
	background_texture_top.loadFromFile("img/bg_top.png");
	pause_screen_texture.loadFromFile("img/pause_screen.png");
	game_over_texture.loadFromFile("img/game_over_screen.png");
	font.loadFromFile("nyala.ttf");
	icon.loadFromFile("img/icon_square.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

//variables
	int type, current_x, current_y, current_rotation;
	int next_piece, held_piece = -1, temp_piece = -1;
	int held_color = -1;
	int x_offset = 15;
	int y_offset = 1;
	long hi_score{};
	long score = 0;
	int lines = 0;
	int temp_lines = 0;
	int level = 0;
	float timer = 0, delay = 725 * pow(0.85, level);
	float mvmt_timer = 0, mvmt_delay = 100 - level;
	float landing_timer = 0, landing_delay = (700 + level*50); 
	float rotation_timer = 0, rotation_delay = 20;
	bool switched = false;
	bool hard_drop = false;
	bool game_started = false;
	bool game_over = false;
	bool game_paused = false;
	
//read current hi_score
	std::ifstream infile;
	infile.open("hi_score.txt");
	infile >> hi_score;

//update all the on-screen items around the board
	auto update_dashboard = [&]() {
		Text text_score, text_hi_score, text_lines, text_level;
	//displays current score
		auto score_string = std::to_string(score);
			text_score.setFont(font);
			int score_length = score_string.length();
			text_score.setString(score_string);
			text_score.setCharacterSize(35);
			text_score.setPosition(((screen_width+WIDTH)*CELL_SIZE/2 + 85 - score_length*5), 480);
	//displays current high score
		auto hi_score_string = std::to_string(hi_score);
			text_hi_score.setFont(font);
			int hi_score_length = hi_score_string.length();
			text_hi_score.setString(hi_score_string);
			text_hi_score.setCharacterSize(35);
			text_hi_score.setPosition(((screen_width + WIDTH) * CELL_SIZE / 2 + 85 - hi_score_length * 5), 340);
	//displays current ammount of lines cleared
		auto lines_string = std::to_string(temp_lines);
			text_lines.setFont(font);
			int lines_length = lines_string.length();
			text_lines.setString(lines_string);
			text_lines.setCharacterSize(35);
			text_lines.setPosition(((screen_width - WIDTH) * CELL_SIZE/2 - lines_length*5 - 90), 335);
	//displays current level
		auto level_string = std::to_string(level);
			text_level.setFont(font);
			int level_length = level_string.length();
			text_level.setString(level_string);
			text_level.setCharacterSize(35);
			text_level.setPosition(((screen_width + WIDTH) * CELL_SIZE / 2 + 85 - level_length*5), 615);
	//different ammount of points for different ammount of lines cleared
		if (lines - temp_lines == 1) {
			temp_lines = lines;
			score += (40*(level + 1));
		}
		else if (lines - temp_lines == 2) {
			temp_lines = lines;
			score += (100 * (level + 1));

		}
		else if (lines - temp_lines == 3) {
			temp_lines = lines;
			score += (300 * (level + 1));
		}
		else if (lines - temp_lines == 4) {
			temp_lines = lines;
			score += (1200 * (level + 1));
		}
	//if the ammount of lines is a multiplication of 10, increase level
		if ((temp_lines >= (level + 1) * 10) && level < 30)
			level++;
	//if level increases, the speed of falling down also increases
		delay = 725 * pow(0.85, level);
		mvmt_delay = 100 - level;
		landing_delay = (700 + level * 50);
	//draw texts onto the screen
		window.draw(text_hi_score);
		window.draw(text_score);
		window.draw(text_lines);
		window.draw(text_level);
	//draws the next piece on the screen
		int color = CELL_SIZE * next_piece;
		for (int y = 0; y < block_height; y++) for (int x = 0; x < block_width; x++) {
			if (tetrinos[0][next_piece][y][x]) {
				s.setTextureRect(IntRect(color, 0, CELL_SIZE, CELL_SIZE));
				s.setPosition(Vector2f(((x) * CELL_SIZE + 1008), ((y) * CELL_SIZE + 105)));
				window.draw(s);
			}
		}
	//draws the held piece on the screen
		if(switched)
			held_color = CELL_SIZE * 7;
		else {
			held_color = CELL_SIZE * temp_piece;
		}
		for (int y = 0; y < block_height; y++) for (int x = 0; x < block_width; x++) {
			if (tetrinos[0][held_piece][y][x]) {
				s.setTextureRect(IntRect(held_color, 0, CELL_SIZE, CELL_SIZE));
				s.setPosition(Vector2f(((x)*CELL_SIZE + 428), ((y)*CELL_SIZE + 105)));
				window.draw(s);
			}
		}
	};
//function that generates starting piece and the following ones
	auto new_block = [&]() {
		switched = false;
		srand(time(NULL));
		if (!game_started) {
			next_piece = rand() % 7;
		}
		current_x = (WIDTH - block_width) / 2;
		current_y = -3;
		current_rotation = 0;
		type = next_piece;
		next_piece = rand() % 7;
		update_dashboard();
	};
	new_block();
//function that checks if the block can move in a certain direction
	auto check_block = [&](int dir) {
		for (int y = 0; y < block_height; y++) for (int x = 0; x<block_width; x++)
		{
			if (tetrinos[type][y][x] == 0) continue;
		//check if the block can move to the left
			if (dir == 1) {
				if (tetrinos[current_rotation][type][y][x]) {
					if (play_area[current_y + y][current_x + x - 1])
					{
						return false;
					}
					if (current_x + x <= 0) {
						return false;
					}
				}
			}
		//check if the block can move down
			if (dir == 2) {
				if (tetrinos[current_rotation][type][y][x] == 2) {
					if (play_area[current_y + y + 1][current_x + x] != 0)
					{
						return false;
					}
					if (y + current_y + 2 > HEIGHT) {
						return false;
					}
				}
			}
		//check if the block can move to the right
			if (dir == 3) {
				if (tetrinos[current_rotation][type][y][x]) {
					if (play_area[current_y + y][current_x + x + 1])
					{
						return false;
					}
					if (current_x + x + 1 >= WIDTH) {
						return false;
					}
				}
			}
		}
		return true;
	};
//function that adds current block to the play_area array, and draws the updated play_area, checks if the game is over
	auto place_block = [&]() {
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				if (play_area[y][x]) {
					int color = (play_area[y][x] - 1)*CELL_SIZE;
					s.setTextureRect(IntRect(color, 0, CELL_SIZE, CELL_SIZE));
					s.setPosition(Vector2f((x+x_offset) * CELL_SIZE, (y+y_offset) * CELL_SIZE));
					window.draw(s);
				}
			//if a block is above the allowed area, end the game
				if (play_area[-1][x])
				{
					game_over = true;
				}
			}
		}
	};
//function that clears the full line and moves everything above 1 down
	auto clear_line = [&](int row) {
		for (int x = 0; x < WIDTH; x++)
		{
			play_area[row][x] = 0;
		}
		for (int y = row; y >= 0; y--) for (int x = 0; x < WIDTH; x++)
		{
			play_area[y][x] = play_area[y-1][x];
		}
	};
//function that checks if a line has been cleared
	auto check_line = [&]() {
		for (int y = 0; y < HEIGHT; y++)
		{
			bool is_cleared = true;
			for (int x = 0; x < WIDTH; x++) {
				if (play_area[y][x] == 0) {
					is_cleared = false;
				}
			}
			if (is_cleared) 
			{
				lines++;
				clear_line(y);
			}
		}
	};
//function that draws current movable block
	auto create_block = [&]() {
		int color = CELL_SIZE * type;
		for (int y = 0; y < block_height; y++) for (int x = 0; x < block_width; x++) {
			if (tetrinos[current_rotation][type][y][x]) {
				s.setTextureRect(IntRect(color, 0, CELL_SIZE, CELL_SIZE));
				s.setPosition(Vector2f((current_x + x_offset + x) * CELL_SIZE, (current_y + y + y_offset) * CELL_SIZE));
				window.draw(s);
			}
		}
		//update screen pt. 2
		window.draw(bg_top);
		update_dashboard();
	};

// Create the main window
	while (window.isOpen())
	{
	// code that is responsible for falling of the blocks
		float time = clock.getElapsedTime().asMilliseconds();
		clock.restart();
		if(game_over == false && game_started && !game_paused)
			timer += time;
	//if timer has higher value than the current delay, drop the block 1 down
		if (timer > delay)
		{
		//if a block can move down, move it. otherwise start the landing timer
			if (check_block(2)) {
				landing_timer = 0;
				landing_clock.restart();
				current_y++;
			}
			else {
				float landing_time = landing_clock.getElapsedTime().asMilliseconds();
				landing_timer += landing_time;
				landing_clock.restart();
				landing_timer += landing_time;
				if (landing_timer > landing_delay) {
					current_y--;
					for (int y = 0; y < block_height; y++) for (int x = 0; x < block_width; x++)
					{
						if (tetrinos[current_rotation][type][y][x]) {
							play_area[current_y + y + 1][current_x + x] = type + 1;
						}
					}
					landing_timer = 0;
					place_block();
					new_block();
					hard_drop = false;
				}
			}
			timer = 0;
		}

	//movement++
		if (game_started && !game_over) {
			//toggle pause
			if (Keyboard::isKeyPressed(Keyboard::P) || (Joystick::isButtonPressed(0, 7))) {
				float time = rotation_clock.getElapsedTime().asMilliseconds();
				rotation_clock.restart();
				rotation_timer += time;
				if (rotation_timer > rotation_delay) {
					if (!game_paused)
						game_paused = true;
					else
						game_paused = false;
				}
				rotation_timer = 0;
			}
			if (!game_paused) {
				if (!hard_drop) {
					//if comma (X) was pressed, rotate the block counter-clockwise
					if (Keyboard::isKeyPressed(Keyboard::Comma) || Joystick::isButtonPressed(0, 0)) {
						float time = rotation_clock.getElapsedTime().asMilliseconds();
						rotation_clock.restart();
						rotation_timer += time;
						if (rotation_timer > rotation_delay) {
							if (current_rotation > 0)
								current_rotation--;
							else
								current_rotation = 3;
							//check if the block can rotate in a wanted way
							for (int y = 0; y < block_height; y++) for (int x = 0; x < block_width; x++)
							{
								if (tetrinos[current_rotation][type][y][x]) {
									if (play_area[current_y + y][current_x + x])
									{
										if (current_rotation < 3)
										{
											current_rotation++;
										}
										else
											current_rotation = 0;
									}
									else {
										if (current_y + y >= HEIGHT)
										{
											current_y--;
										}
										if (current_x + x >= WIDTH || current_x + x < 0)
										{
											current_rotation++;
										}
									}
								}
							}
						}
						rotation_timer = 0;
					}
					//if period was pressed, rotate the block clockwise
					if (Keyboard::isKeyPressed(Keyboard::Period) || Joystick::isButtonPressed(0, 1)) {
						float time = rotation_clock.getElapsedTime().asMilliseconds();
						rotation_clock.restart();
						rotation_timer += time;
						if (rotation_timer > rotation_delay) {
							if (current_rotation < 3)
								current_rotation++;
							else
								current_rotation = 0;
							for (int y = 0; y < block_height; y++) for (int x = 0; x < block_width; x++)
							{
								//check if the block can rotate in a wanted way
								if (tetrinos[current_rotation][type][y][x]) {
									if (play_area[current_y + y][current_x + x])
									{
										if (current_rotation > 0)
										{
											current_rotation--;
										}
										else
											current_rotation = 3;
									}
									else {
										if (current_y + y >= HEIGHT)
										{
											current_y--;
										}
										if (current_x + x >= WIDTH || current_x + x < 0)
										{
											if (current_rotation > 0)
											{
												current_rotation--;
											}
											else
												current_rotation = 3;
										}
									}
								}
							}
						}
						rotation_timer = 0;
					}
					//moving the block
					if (Keyboard::isKeyPressed(Keyboard::A) || (Joystick::getAxisPosition(0, Joystick::PovX)) < -70)
					{
						float time = mvmt_clock.getElapsedTime().asMilliseconds();
						mvmt_clock.restart();
						mvmt_timer += time;
						if (mvmt_timer > mvmt_delay) {
							if (check_block(1))
							{
								current_x--;
							}
							mvmt_timer = 0;
						}
					}
					if (Keyboard::isKeyPressed(Keyboard::D) || (Joystick::getAxisPosition(0, Joystick::PovX)) > 70) {
						float time = mvmt_clock.getElapsedTime().asMilliseconds();
						mvmt_clock.restart();
						mvmt_timer += time;
						if (mvmt_timer > mvmt_delay) {
							if (check_block(3)) {
								current_x++;
							}
							mvmt_timer = 0;
						}
					}
				}
				//soft drop
				if (Keyboard::isKeyPressed(Keyboard::S) || (Joystick::getAxisPosition(0, Joystick::PovY)) < -70) {
					float time = mvmt_clock.getElapsedTime().asMilliseconds();
					float mvmt_delay_down = 100 * pow(0.9, level);
					mvmt_clock.restart();
					mvmt_timer += time;
					if (mvmt_timer > mvmt_delay_down) {
						if (check_block(2)) {
							score++;
							landing_timer = 0;
							landing_clock.restart();
							current_y++;
						}
						else {
							float landing_time = landing_clock.getElapsedTime().asMilliseconds();
							landing_timer += landing_time;
							landing_clock.restart();
							if (landing_timer > landing_delay) {
								current_y--;
								for (int y = 0; y < block_height; y++) for (int x = 0; x < block_width; x++)
								{
									if (tetrinos[current_rotation][type][y][x]) {
										play_area[current_y + y + 1][current_x + x] = type + 1;
									}
								}
								landing_timer = 0;
								place_block();
								new_block();
							}
						}
						mvmt_timer = 0;
					}
				}
				//hard drop
				if (Keyboard::isKeyPressed(Keyboard::W) || (Joystick::getAxisPosition(0, Joystick::PovY)) > 70) {
					float time = rotation_clock.getElapsedTime().asMilliseconds();
					rotation_clock.restart();
					rotation_timer += time;
					if (rotation_timer > rotation_delay) {
						for (int i = 0; i <= HEIGHT; i++)
						{
							hard_drop = true;
							if (check_block(2)) {
								current_y++;
								score += 2;
							}
							else {
								break;
							}
						}
					}
					rotation_timer = 0;
				}
				//hold functionality
				if ((Keyboard::isKeyPressed(Keyboard::LShift) || (Joystick::isButtonPressed(0, 4))) && !switched) {
					if (held_piece == -1) {
						temp_piece = type;
						type = next_piece;
						held_piece = temp_piece;
					}
					else if (held_piece != -1) {
						temp_piece = type;
						type = held_piece;
						held_piece = temp_piece;
					}
					current_rotation = 0;
					current_x = (WIDTH - block_width) / 2;
					current_y = -3;
					switched = true;
					create_block();
				}
			}
			else {
				//while mid-pause, exit the game
				if (((Keyboard::isKeyPressed(Keyboard::Escape)) || Joystick::isButtonPressed(0, 1)) && game_paused) {
					return EXIT_SUCCESS;
				}
			}
		}
		else if (!game_started || game_over) {
			if ((Keyboard::isKeyPressed(Keyboard::Comma) || (Joystick::getAxisPosition(0, Joystick::PovX)) < -70) && level > 0) {
				float time = mvmt_clock.getElapsedTime().asMilliseconds();
				mvmt_clock.restart();
				mvmt_timer += time;
				if (mvmt_timer > mvmt_delay) {
					level--;
					mvmt_timer = 0;
				}
			}
			if ((Keyboard::isKeyPressed(Keyboard::Period) || (Joystick::getAxisPosition(0, Joystick::PovX)) > 70) && level < 30) {
				float time = mvmt_clock.getElapsedTime().asMilliseconds();
				mvmt_clock.restart();
				mvmt_timer += time;
				if (mvmt_timer > mvmt_delay) {
					level++;
					mvmt_timer = 0;
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Space) || Joystick::isButtonPressed(0, 0))
				game_started = true;
			if (Keyboard::isKeyPressed(Keyboard::Escape) || Joystick::isButtonPressed(0, 1))
				return EXIT_SUCCESS;
		}

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed /*|| Joystick::isButtonPressed*/)
			{	
			//bindings for debugging purposes
				if (debug_mode) {
					if (event.key.code == Keyboard::I) {
						std::cout << game_over << std::endl;
					}
					if (event.key.code == Keyboard::X)
						for (int y = 0; y < HEIGHT; y++)
						{
							std::cout << y << ". ";
							for (int x = 0; x < WIDTH; x++) {
								std::cout << play_area[y][x];
							}
							std::cout << std::endl;
						}
					if (event.key.code == Keyboard::C)
					{
						std::cout << "Current x: " << current_x << " Current y: " << current_y << "Current rot: " << current_rotation << std::endl;
						for (int y = 0; y < block_height; y++)
						{
							for (int x = 0; x < block_width; x++)
								std::cout << tetrinos[current_rotation][type][y][x] << " ";
							std::cout << std::endl;
						}
						std::cout << "Next piece: " << next_piece << std::endl;
					}
				}
			}
		}
		window.clear(Color(50,50,50));
	//drawing background
		if (game_started && !game_paused && !game_over) {
			bg_bottom.setPosition(0, 0);
			bg_bottom.setTextureRect(IntRect(0, 0, 1600, 900));
			bg_top.setPosition(0, 0);
			bg_top.setTextureRect(IntRect(0, 0, 1600, 900));
		// Update the window
		// A bit of updating the screen takes place in create_block for esthetical reasons
			window.draw(bg_bottom);
			place_block();
			check_line();
			create_block();
		}
		if (game_started && game_paused && !game_over) {
			pause_screen.setPosition(0, 0);
			pause_screen.setTextureRect(IntRect(0, 0, 1600, 900));
			window.draw(pause_screen);
		}

	//starting screen that shows controlls
		if (!game_started) {
			starting_screen.setPosition(0,0);
			starting_screen.setTextureRect(IntRect(0, 0, 1600, 900));
			Text text_level;
				auto level_string = std::to_string(level);
				text_level.setFont(font);
				int level_length = level_string.length();
				text_level.setString(level_string);
				text_level.setCharacterSize(70);
				text_level.setPosition(((screen_width+WIDTH)*CELL_SIZE/2 - 80), 700);
			window.draw(starting_screen);
			window.draw(text_level);
		}
	//if the game is over, show game-over screen
		if (game_over)
		{
			game_over_screen.setPosition(0, 0);
			game_over_screen.setTextureRect(IntRect(0, 0, 1600, 900));
			window.draw(game_over_screen);
			Text text_score;
		//if the score was higher than current high score, write the new high score in the file
			if (score > hi_score)
			{
				auto score_string = "New High Score: " + std::to_string(score);
				text_score.setFont(font);
				int score_length = score_string.length();
				text_score.setString(score_string);
				text_score.setCharacterSize(75);
				text_score.setPosition(((screen_width)*CELL_SIZE) / 2 - score_length * 15, screen_height* CELL_SIZE / 2 - 75);
				window.draw(text_score);
			//saves new high score to hi_score.txt
				std::ofstream outfile;
				outfile.open("hi_score.txt");
				outfile << score;
			}
			else
			{
				auto score_string = "Score: " + std::to_string(score);
				text_score.setFont(font);
				int score_length = score_string.length();
				text_score.setString(score_string);
				text_score.setCharacterSize(75);
				text_score.setPosition(((screen_width)* CELL_SIZE) / 2 - score_length*15, screen_height* CELL_SIZE / 2 - 75);
				window.draw(text_score);
			}
		}
		window.display();
	}
	return EXIT_SUCCESS;
}