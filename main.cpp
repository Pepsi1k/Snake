#include <iostream>
#include <cstring>
#include <Windows.h>
#include <conio.h>
#include <ctime>
#include <vector>
#include "EvilSnake.h"

using namespace std;


class Field
{
  private:
	HANDLE hConsole;

  public:
	unsigned int Window_width;
	unsigned int Window_height;
	int field_width;
	int field_height;

	Field()
	{
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		GetConsoleScreenBufferInfo(hConsole, &consoleInfo);

		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(hConsole, &cursorInfo);
		cursorInfo.bVisible = false;
		SetConsoleCursorInfo(hConsole, &cursorInfo);

		Window_width = consoleInfo.srWindow.Right;
		Window_height = consoleInfo.srWindow.Bottom;

		field_width = Window_width - 20;
		field_height = Window_height;
	}

	void DrawField()
	{
		COORD position;

		for (int y = 0; y < Window_height; y++)
		{
			for (int x = 0; x < Window_width; x++)
			{
				if (x == 0 || x == field_width - 1 || (y == 0 && x <= field_width - 1) || (y == Window_height - 1 && x <= field_width - 1))
				{
					position.X = x;
					position.Y = y;
					SetConsoleTextAttribute(hConsole, (WORD)(6 << 4));
					SetConsoleCursorPosition(hConsole, position);
					cout << " ";
				}

				if (x == field_width || x == Window_width - 1 || (y == 0 && x >= field_width) || (y == Window_height - 1 && x >= field_width))
				
				{
					position.X = x;
					position.Y = y;
					SetConsoleTextAttribute(hConsole, (WORD)(5 << 4));
					SetConsoleCursorPosition(hConsole, position);
					cout << " ";
				}
			}
		}
	}

};

class Snake
{
  private:
	HANDLE hConsole;

  public:
	char chHead;
	char chTail;
	vector<COORD> body;
	unsigned short int iSizeTail;

	Snake(const int x, const int y)
	{
		COORD Head;

		Head.X = x;
		Head.Y = y;
		iSizeTail = 1;
		body.push_back(Head);

		chHead = char(2);  // face
		chTail = char(42); // *

		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	void Move(const Route &route, Field &field)
	{
		for (int i = iSizeTail - 1; i > 0; i--)
		{
			body[i].X = body[i - 1].X;
			body[i].Y = body[i - 1].Y;
		}

		body[0].X -= (int)route.left;
		body[0].X += (int)route.right;
		body[0].Y -= (int)route.up;
		body[0].Y += (int)route.down;

		if (body[0].X >= field.field_width - 1)
			body[0].X = 1;
		else if (body[0].X <= 0)
			body[0].X = field.field_width - 2;
		else if (body[0].Y >= field.field_height - 1)
			body[0].Y = 1;
		else if (body[0].Y <= 0)
			body[0].Y = field.field_height - 2;
	}

	void ClearPrevious()
	{
		SetConsoleCursorPosition(hConsole, body[iSizeTail - 1]);
		cout << " ";
	}

	void Draw()
	{
		for (int i = 0; i < iSizeTail; i++)
		{
			SetConsoleCursorPosition(hConsole, body[i]);
			if (i == 0)
			{
				SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 10));
				cout << chHead;
			}
			else
			{
				SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));
				cout << chTail;
			}
		}
	}

};

enum BUTTON
{
	LEFT = 'a',
	RIGHT = 'd',
	UP = 'w',
	DOWN = 's',
	QUIT = '`'
};

class Food
{
  public:
	char chFood;
 	bool exist;
	COORD position;

	Food()
	{
		position.X = 1;
		position.Y = 1;
		exist = false;
		chFood = char(3);
	}

	void Spawn(const Field &field, const Snake &snake)
	{
		if (!exist)
		{
			bool EndRand = true;
			do
			{
				EndRand = true;

				position.X = (rand() % (field.field_width - 2)) + 1;
				position.Y = (rand() % (field.field_height - 2)) + 1;

				for (int i = 0; i < snake.iSizeTail; i++)
					if (position.X == snake.body[i].X || position.Y == snake.body[i].Y)
						EndRand = false;

			} while (!EndRand);

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 12));
			SetConsoleCursorPosition(hConsole, position);
			cout << chFood;
			exist = true;
		}
	}

};

void Eat(Snake *snake, Food *food)
{
	snake->body.push_back(snake->body[snake->iSizeTail - 1]);
	snake->iSizeTail++;
	food->exist = false;
}

void DrawProperties(const Snake *snake, const Food *food, const Field *field)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	COORD position;

	// SIZE OF TAIL SNAKE
	position.X = field->field_width + 2;
	position.Y = 2;
	SetConsoleCursorPosition(hConsole, position);

	SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 8));
	cout << "iSizeTail: ";
	SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 7));
	cout << snake->iSizeTail;


	// HEAD OF SNAKE 
	position.Y = 4;
	SetConsoleCursorPosition(hConsole, position);
	SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 8));
	cout << "HeadSnake: ";

	position.X = field->field_width + 2 + strlen("HeadSnake: ");
	SetConsoleCursorPosition(hConsole, position);
	SetConsoleTextAttribute(hConsole, (WORD)(0 << 4));
	for (int i = position.X; i < field->Window_width - 1; i++)
		cout << " ";

	position.X = field->field_width + 2 + strlen("HeadSnake: ");
	SetConsoleCursorPosition(hConsole, position);
	SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 7));
	cout << snake->body[0].X;
	SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 8));
	cout << ",";
	SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 7));
	cout << snake->body[0].Y;

	position.X = field->field_width + 2;
	position.Y = 6;
	SetConsoleCursorPosition(hConsole, position);
	SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 8));
	cout << "Food: ";

	position.X = field->field_width + 2 + strlen("food: ");
	SetConsoleCursorPosition(hConsole, position);
	SetConsoleTextAttribute(hConsole, (WORD)(0 << 4));
	for (int i = position.X; i < field->Window_width - 1; i++)
		cout << " ";

	position.X = field->field_width + 2 + strlen("food: ");
	SetConsoleCursorPosition(hConsole, position);
	SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 7));
	cout << food->position.X;
	SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 8));
	cout << ",";
	SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 7));
	cout << food->position.Y;



}

int main()
{
	srand(time(0));
	


	Snake snake(1, 12);
	snake.Draw();

	Route route;

	Field field;

	field.DrawField();
	Food food;

	bool EndProgram = false;
	while (!EndProgram)
	{
		Sleep(100);
		if (snake.body[0].X == food.position.X && snake.body[0].Y == food.position.Y)
		{
			Eat(&snake, &food);
		}
		snake.ClearPrevious();
		snake.Move(route, field);

		snake.Draw();
		food.Spawn(field, snake);
		DrawProperties(&snake, &food, &field);


		if (_kbhit())
		{
			//system("cls");
			switch (_getch())
			{
			case LEFT: //left
			{
				route = 0;
				route.left = 1;
				break;
			}

			case RIGHT: //right
			{
				route = 0;
				route.right = 1;
				break;
			}
			case UP: //up
			{
				route = 0;
				route.up = 1;
				break;
			}
			case DOWN: //down
			{
				route = 0;
				route.down = 1;
				break;
			}
			case QUIT:
			{
				EndProgram = true;
				break;
			}
			}
		}
	}

	_getch();
	return 0;
}
