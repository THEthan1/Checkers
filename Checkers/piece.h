#include <iostream>
#ifndef PIECE_H
#define PIECE_H

	using namespace std;

	
	const char RED = 'R';
	const char BLACK = 'B';
	const char NUTE = ' ';

	enum PieceClass {EMPTY, REGULAR, KING};

class Piece
{			
public:
	Piece(char team = NUTE, PieceClass type = EMPTY);

	PieceClass GetClass();

	void SetClass(PieceClass type);

	void SetTeam(char team);

	char GetTeam();

	Piece& Piece::operator=(const Piece& piece);

	friend ostream& operator<<(ostream& os, Piece& piece);

protected:
	char m_Team;
	PieceClass m_Class;
};

#endif