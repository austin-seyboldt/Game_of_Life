#include "pch.h"
#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include <cstdlib>
#include <ctime>
using namespace sf;

const int GAME_WIDTH = 100,
			GAME_HEIGHT = GAME_WIDTH;
const float boxScaleFactor = 800 / GAME_WIDTH;

void populateCells(int cells[GAME_HEIGHT][GAME_WIDTH]);
int getRandState();
void updateCells(int cells[GAME_HEIGHT][GAME_WIDTH], int secondCells[GAME_HEIGHT][GAME_WIDTH]);
int getDesiredCellState(int cells[GAME_HEIGHT][GAME_WIDTH], int row, int col);
void placeCells(int cells[GAME_HEIGHT][GAME_WIDTH], Vector2i mousePosition);
void clearCells(int cells[GAME_HEIGHT][GAME_WIDTH], int secondCells[GAME_HEIGHT][GAME_WIDTH]);


int main()
{
	srand(time(0));

	sf::RenderWindow window(sf::VideoMode(GAME_WIDTH*boxScaleFactor, GAME_HEIGHT*boxScaleFactor), "Conway's Game of Life", sf::Style::Default);
	window.setFramerateLimit(10);

	// The boxes of the game
	int cells[GAME_HEIGHT][GAME_WIDTH];
	int secondCells[GAME_HEIGHT][GAME_WIDTH];

	// Make empty cells
	for (int i = 0; i < GAME_HEIGHT; i++)
		for (int j = 0; j < GAME_WIDTH; j++)
		{
			cells[i][j] = 0;
			secondCells[i][j] = cells[i][j];
		}
	
	// The cells that will be displayed on screen
	std::cout << "Creating cells..." << std::endl;
	RectangleShape* rectCells[GAME_HEIGHT][GAME_WIDTH];
	for (int row = 0; row < GAME_HEIGHT; row++)
	{
		for (int col = 0; col < GAME_WIDTH; col++)
		{
			rectCells[row][col] = new RectangleShape(Vector2f(boxScaleFactor, boxScaleFactor));
			rectCells[row][col]->setPosition(Vector2f(row*boxScaleFactor, col*boxScaleFactor));
			rectCells[row][col]->setFillColor(Color(11,110,186));
		}
	}
	
	bool switchCells = true;
	int loop = 0;

	bool startGame = false;
	
	// Main game loop
	std::cout << "Beginning main loop" << std::endl;
	while (window.isOpen())
	{
		std::cout << "Loop number: " << ++loop << std::endl;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == Keyboard::Escape)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == Keyboard::Space)
				startGame = !startGame;
		}
		
		// Update cells, pause game 
		while (!startGame && window.isOpen())
		{
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::KeyPressed && event.key.code == Keyboard::Escape)
					window.close();
				if (event.type == sf::Event::KeyPressed && event.key.code == Keyboard::Space)
					startGame = !startGame;
				if (event.type == sf::Event::KeyPressed && event.key.code == Keyboard::C)
					clearCells(cells, secondCells);
				if (event.type == sf::Event::MouseButtonPressed)
				{
					Vector2i mousePosition = Mouse::getPosition(window);
					if (switchCells)
					{
						placeCells(cells, mousePosition);

						// Update second array - kinda inefficient - updating all cells
						for (int i = 0; i < GAME_HEIGHT; i++)
							for (int j = 0; j < GAME_WIDTH; j++)
							{
								secondCells[i][j] = cells[i][j];
							}
					}
					else
					{
						placeCells(secondCells, mousePosition);

						// Update second array - kinda inefficient - updating all cells
						for (int i = 0; i < GAME_HEIGHT; i++)
							for (int j = 0; j < GAME_WIDTH; j++)
							{
								cells[i][j] = secondCells[i][j];
							}
					}
				}
			
			}

			// Display cells
			if (switchCells)
			{
				window.clear(Color(63, 207, 255, 0));
				for (int row = 0; row < GAME_HEIGHT; row++)
				{
					for (int col = 0; col < GAME_WIDTH; col++)
					{

						if (cells[row][col])
							window.draw(*(rectCells[row][col]));
					}
				}
			}
			else
			{
				window.clear(Color(63, 207, 255, 0));
				for (int row = 0; row < GAME_HEIGHT; row++)
				{
					for (int col = 0; col < GAME_WIDTH; col++)
					{

						if (secondCells[row][col])
							window.draw(*(rectCells[row][col]));
					}
				}
			}
			
			window.display();
		}
		
		// Update stuff
		if (switchCells)
			updateCells(cells, secondCells);
		else
			updateCells(secondCells, cells);
		

		window.clear(Color(63,207,255,0));
		
		//Draw stuff
		if (switchCells)
		{
			for (int row = 0; row < GAME_HEIGHT; row++)
			{
				for (int col = 0; col < GAME_WIDTH; col++)
				{

					if (cells[row][col])
						window.draw(*(rectCells[row][col]));
				}
			}
		}
		else
		{
			for (int row = 0; row < GAME_HEIGHT; row++)
			{
				for (int col = 0; col < GAME_WIDTH; col++)
				{

					if (secondCells[row][col])
						window.draw(*(rectCells[row][col]));
				}
			}
		}
		
		window.display();
		if (startGame)
			switchCells = !switchCells;
	}

	for (int row = 0; row < GAME_HEIGHT; row++)
	{
		for (int col = 0; col < GAME_WIDTH; col++)
		{
			delete rectCells[row][col];
			rectCells[row][col] = nullptr;
		}
	}
	return 0;
}

// Populate the cells randomly at game start
void populateCells(int cells[GAME_HEIGHT][GAME_WIDTH])
{
	for (int row = 0; row < GAME_HEIGHT; row++)
	{
		for (int col = 0; col < GAME_WIDTH; col++)
		{
			cells[row][col] = getRandState();
		}
	}
}

// Return a random bool
int getRandState()
{
	return rand() % 2;
}

// Update each cell according to game rules
void updateCells(int cells[GAME_HEIGHT][GAME_WIDTH], int secondCells[GAME_HEIGHT][GAME_WIDTH])
{
	for (int row = 0; row < GAME_HEIGHT; row++)
	{
		for (int col = 0; col < GAME_WIDTH; col++)
		{
			// Update the cells
			cells[row][col] = getDesiredCellState(secondCells, row, col);
		}
	}
	
}

// Return the the cell's updated state
int getDesiredCellState(int cells[GAME_HEIGHT][GAME_WIDTH], int row, int col)
{
	
	int numNeighbors = 0;

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			
			numNeighbors += cells[(row + i + GAME_HEIGHT) % GAME_HEIGHT][(col + j + GAME_WIDTH) % GAME_WIDTH];
		}
	}
	numNeighbors -= cells[row][col];
	if (numNeighbors < 2 || numNeighbors > 3)
		return 0;
	if (numNeighbors == 3)
		return 1;
	if (cells[row][col] == 1 && numNeighbors == 2)
		return 1;
	else
		return 0;
	
	
}

// Place or delete a cell at the coordinates
void placeCells(int cells[GAME_HEIGHT][GAME_WIDTH], Vector2i mousePosition)
{
	if (cells[mousePosition.x / static_cast<int>(boxScaleFactor)][mousePosition.y / static_cast<int>(boxScaleFactor)] == 0)
		cells[mousePosition.x / static_cast<int>(boxScaleFactor)][mousePosition.y / static_cast<int>(boxScaleFactor)] = 1;
	else
		cells[mousePosition.x / static_cast<int>(boxScaleFactor)][mousePosition.y / static_cast<int>(boxScaleFactor)] = 0;

	std::cout << mousePosition.x << " " << mousePosition.y << std::endl;
	std::cout << mousePosition.x / static_cast<int>(boxScaleFactor) << " " << mousePosition.y / static_cast<int>(boxScaleFactor) << std::endl;
	std::cout << cells[mousePosition.x / static_cast<int>(boxScaleFactor)][mousePosition.y / static_cast<int>(boxScaleFactor)] << std::endl;
}

// Clear all cells
void clearCells(int cells[GAME_HEIGHT][GAME_WIDTH], int secondCells[GAME_HEIGHT][GAME_WIDTH])
{
	for (int row = 0; row < GAME_HEIGHT; row++)
		for (int col = 0; col < GAME_WIDTH; col++)
		{
			cells[row][col] = 0;
			secondCells[row][col] = 0;
		}
}