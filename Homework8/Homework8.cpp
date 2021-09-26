#include <iostream>
#include <random>
#include <stdlib.h>
#include <chrono>

using namespace std;

enum Cell
{
	CROSS = 'X',
	ZERO = 'O',
	EMPTY = '_'
};

enum GameProgress 
{
	IN_PROGRESS,
	WON_HUMAN,
	WON_AI,
	DRAW
};

#pragma pack(push,1)
struct Game
{
	Cell** ppField = nullptr;
	const size_t SIZE = 3;
	GameProgress progress = IN_PROGRESS;
	Cell ai;
	Cell human;
	size_t turn = 0;
};
#pragma pack(pop)

struct Coord 
{
	size_t y;
	size_t x;
};

inline void clearScr()
{
	system("cls");
}

int32_t getRandomNum(int32_t min, int32_t max)
{
	const static auto seed = chrono::system_clock::now().time_since_epoch().count();
	static mt19937_64 generator(seed);
	uniform_int_distribution<int32_t> dis(min, max);
	return dis(generator);
};

void StartGame(Game& g) 
{
	g.ppField = new Cell * [g.SIZE];

	for (size_t i = 0; i < g.SIZE; i++)
	{
		g.ppField[i] = new Cell[g.SIZE];	
	}

	for (size_t y = 0; y < g.SIZE; y++)
	{
		for (size_t x = 0; x < g.SIZE; x++)
		{
			g.ppField[y][x] = EMPTY;
		}
	}

	if (getRandomNum(0, 10000) > 5000) //Чей ход
	{
		g.ai = CROSS;
		g.human = ZERO;
		g.turn = 1;
	}
	else
	{
		g.human = CROSS;
		g.ai = ZERO;
		g.turn = 0;
	}
}

void EndGame(Game& g)
{
	for (size_t i = 0; i < g.SIZE; i++)
	{
		delete[] g.ppField[i];
	}
	delete[] g.ppField;
	g.ppField = nullptr;
}

void printField(const Game& g)
{
	cout << "    ";

	for (size_t i = 0; i < g.SIZE; i++)
	{
		cout << i + 1 << "   ";
	}
	cout << endl;

	for (size_t y = 0; y < g.SIZE; y++)
	{
		cout << y + 1 << " | ";

		for (size_t x = 0; x < g.SIZE; x++)
		{
			cout<< (char)g.ppField[y][x]<<" | ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "Human playing: " << (char)g.human << endl;
	cout << endl;
	cout << "Ai playing: " << (char)g.ai << endl<<endl;
}

GameProgress __fastcall WhoWon(Game& g)
{
	for (size_t y = 0; y < g.SIZE; y++)
	{
		if (g.ppField[y][0] == g.ppField[y][1] && g.ppField[y][0] == g.ppField[y][2])
		{
			if (g.ppField[y][0] == g.human)
			{
				return WON_HUMAN;
			}
			else if (g.ppField[y][0] == g.ai)
			{
				return WON_AI;
			}
		}
	}
	for (size_t x = 0; x < g.SIZE; x++)
	{
		if (g.ppField[0][x] == g.ppField[1][x] && g.ppField[0][x] == g.ppField[2][x])
		{
			if (g.ppField[0][x] == g.human)
			{
				return WON_HUMAN;
			}
			else if (g.ppField[0][x] == g.ai)
			{
				return WON_AI;
			}
		}
	}
	if (g.ppField[0][0] == g.ppField[1][1] && g.ppField[1][1] == g.ppField[2][2])
	{
		if (g.ppField[0][0] == g.human)
		{
			return WON_HUMAN;
		}
		else if (g.ppField[0][0] == g.ai)
		{
			return WON_AI;
		}
	}
	if (g.ppField[0][2] == g.ppField[1][1] && g.ppField[1][1] == g.ppField[2][0])
	{
		if (g.ppField[1][1] == g.human)
		{
			return WON_HUMAN;
		}
		else if (g.ppField[1][1] == g.ai)
		{
			return WON_AI;
		}

	}

	bool draw = true;

	for (size_t y = 0; y < g.SIZE; y++)
	{
		for (size_t x = 0; x < g.SIZE; x++)
		{
			if (g.ppField[y][x] == EMPTY)
			{
				draw = false;
				break;
			}
		}
		if (!draw)
			break;
	}
	if (draw)
	{
		return DRAW;
	}

  return IN_PROGRESS;
}

Coord getHumanMove(Game& g)
{
	Coord c;

	do
	{
		cout << "Enter X coord ";
		cin >> c.x;

		cout << "Enter Y coord ";
		cin >> c.y;
	
	}
	while (c.y == 0 || c.y > 3 || c.x == 0 || c.x > 3 || g.ppField[c.y - 1][c.x - 1] != EMPTY);

	c.y--;

	c.x--;

	return c;
}

Coord getAiMove(Game& g)
{
	Coord c;

	// Приоритетный ход

	if (g.ppField[1][1] == EMPTY)
	{
		return { 1,1 };
	}

	//Попытка победить

	for (size_t y = 0; y < g.SIZE; y++)
	{
		for (size_t x = 0; x < g.SIZE; x++)
		{
			if (g.ppField[y][x] == EMPTY)
			{
				g.ppField[y][x] = g.ai;
				if (WhoWon(g) == WON_AI)
				{
					g.ppField[y][x] = EMPTY;
					return {y,x};
				}
				g.ppField[y][x] = EMPTY;
			}
		}
	}

	//Попытка помешать

	for (size_t y = 0; y < g.SIZE; y++)
	{
		for (size_t x = 0; x < g.SIZE; x++)
		{
			if (g.ppField[y][x] == EMPTY)
			{
				g.ppField[y][x] = g.human;
				if (WhoWon(g) == WON_HUMAN)
				{
					g.ppField[y][x] = EMPTY;
					return {y,x};
				}
				g.ppField[y][x] = EMPTY;
			}
		}
	}

	//ход компьютера в обычных условиях

	do 
	{
		c.x = getRandomNum(0, 2);
		c.y = getRandomNum(0, 2);
		
	}while (g.ppField[c.y][c.x] != EMPTY);
		    
	return c;
}

void printWinner(const Game& g)
{
	if (g.progress == WON_HUMAN)
	{
		cout << "Human Won -_-" << endl;
	}
	else if (g.progress == WON_AI)
	{
		cout << "Skynet Won ^_^" << endl;
	}
	else if (g.progress == DRAW)
	{
		cout << "This time the forces are equal, there is no winner" << endl;
	}
}

//============================================================

int main()
{
	Game g;

	Coord c;

	StartGame(g);

	printField(g);

	do
	{
		if (g.turn % 2 == 0)
		{
		    c = getHumanMove(g);
			g.ppField[c.y][c.x] = g.human;
		}
		else
		{
			c = getAiMove(g);
			g.ppField[c.y][c.x] = g.ai;
		}

		clearScr();

		printField(g);

		g.turn++;

		g.progress = WhoWon(g);

	} 
	while (g.progress == IN_PROGRESS);
	
	printWinner(g);

	EndGame(g);

	return 0;
}

