#include <iostream>
#include <string>
#include "piece.h"
#include "board.h"
#ifndef RECORD_H
#define RECORD_H

	using namespace std;

class Move
{
	friend ostream& operator<<(ostream& os, const Move& aMove);

public:
	Move(string name, char X1, short Y1, char X2, short Y2, Piece* positions[9][9]);
	~Move();
	Move* GetNext() const;
	Move* GetPrev() const;
	void SetPrev(Move* prev);
	void SetNext(Move* next);
	void Set_DoubleJump(char x, short y, Piece* positions[9][9]);
	short GetX(short num);
	short GetY(short num);
	Piece* piece1;
	Piece* piece2;

private:
	Move* mpNext;
	Move* mpPrev;
	string player;
	char X1;
	short Y1;
	char X2;
	short Y2;
	char X3;
	short Y3;
};

class Record
{
	friend ostream& operator<<(ostream& os, const Record& aRecord);

public:
	Record();
	~Record();
	void Add(string name, char X1, short Y1, char X2, short Y2, Piece* positions[9][9]);
	void Add_Double(char x, short y, Piece* positions[9][9]);
	void Clear();
	void Show();
	void Delete_Record();
	void Undo(Board* board);
	
protected:
	Move* mpHead;
	Move* mpTail;
	Move* mpMost_Recent;
};

#endif