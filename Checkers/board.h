#include <iostream>
#include <string>
#include "piece.h"
#ifndef BOARD_H
#define BOARD_H

	using namespace std;

class Board
{
public:
	Board();

	~Board();

	void Adjust_Board(short X1, short Y1, short X2, short Y2);

	Piece* Position[9][9];

	bool Move_Is_Illegal(short X1, short Y1, short X2, short Y2);

	bool Regular_Illegal(short X1, short Y1, short X2, short Y2);

	bool King_Illegal(short X1, short Y1, short X2, short Y2);

	void Show_Board();

	void Delete_Piece(short X, short Y);

	void King_Me(short X, short Y, char Z);

	bool DoubleJump_Available(short X, short Y);

	bool Not_Empty();

	char Op_Team(char team);

	void AddDeadPcs(char jumpING_team);

	void RemoveDeadPcs(char team);

	string GetWinner();

	Board& Board::operator=(const Board& board);


	bool HasJumped;

private:
	 string Winning_Team;
	 string* Dead_Red;
	 string* Dead_Black;
	 short indexR;
	 short indexB;
	 short DeadRed;
	 short DeadBlack;
};
#endif