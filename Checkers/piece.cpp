#include "piece.h"


Piece::Piece(char team, PieceClass type):
	m_Team(team),
	m_Class(type)
{}

PieceClass Piece::GetClass()
{
	return m_Class;
}

char Piece::GetTeam()
{
	return m_Team;
}

void Piece::SetClass(PieceClass type)
{
	m_Class = type;
}

void Piece::SetTeam(char team)
{
	m_Team = team;
}

Piece& Piece::operator=(const Piece& piece)
{
	if (this != &piece)
	{
		this->m_Class = piece.m_Class;
		this->m_Team = piece.m_Team;
	}

	return *this;
}

ostream& operator<<(ostream& os, Piece& piece)
{
	if (piece.GetClass() == REGULAR)
	{
		os<<"("<< piece.GetTeam() <<")";
	}
	else if (piece.GetClass() == KING)
	{
		os<<"{"<< piece.GetTeam() <<"}";
	}
	else
	{
		os<<"   ";
	}

	return os;
}
