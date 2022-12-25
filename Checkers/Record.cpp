#include <iostream>
#include <string>
#include "record.h"

	using namespace std;

//Move class, function bodies>
Move::Move(string name, char X1, short Y1, char X2, short Y2, Piece* positions[9][9]):

	 mpNext(0),
	 mpPrev(0),
	 player(name),
	 X1(X1),
	 Y1(Y1),
	 X2(X2),
	 Y2(Y2),
	 X3(' '),
	 Y3(0),
	 piece1(0),
	 piece2(0)
{
	if (Y2 - Y1 == 2 || Y1 - Y2 == 2)
	{
		piece1 = new Piece();
		*piece1 = *positions[(Y1 + Y2) / 2][(GetX(1) + GetX(2)) / 2];
	}
}

Move::~Move()
{
	delete piece1;
	delete piece2;

	piece1 = 0;
	piece2 = 0;
	mpNext = 0;
	mpPrev = 0;
}

Move* Move::GetNext() const
{
	return mpNext;
}

Move* Move::GetPrev() const
{
	return mpPrev;
}

void Move::SetPrev(Move* prev)
{
	mpPrev = prev;
}

void Move::SetNext(Move* next)
{
	mpNext = next;
}

short Move::GetX(short num)
{
	if (num == 1) return X1 - 96;
	else if (num == 2) return X2 - 96;
	else if (num == 3) return X3 - 96;
	else return -1;
}

short Move::GetY(short num)
{
	if (num == 1) return Y1;
	else if (num == 2) return Y2;
	else if (num == 3) return Y3;
	else return -1;
}

inline void Move::Set_DoubleJump(char x, short y, Piece* positions[9][9])
{
	X3 = x;
	Y3 = y;

	piece2 = new Piece();
	*piece2 = *positions[(Y2 + Y3) / 2][(GetX(2) + GetX(3)) / 2];
}

ostream& operator<<(ostream& os, const Move& move)
{
		os<<move.player<<":\t"<<move.X1<<move.Y1<<" to "<<move.X2<<move.Y2;
		if (move.Y3 != 0) cout<<" to "<<move.X3<<move.Y3;
		cout<<endl<<endl;

	return os;
}



//Record class, function bodies
Record::Record():
	mpHead(0),
	mpTail(0)
{}

Record::~Record()
{
	while (mpHead != 0)
	{
		Move* temp = mpHead;
		mpHead = mpHead->GetNext();
		delete temp;
	}
}

void Record::Add(string name, char X1, short Y1, char X2, short Y2, Piece* positions[9][9])
{
	Move* move = new Move(name, X1, Y1, X2, Y2, positions);

	if (mpHead == 0)
	{
		mpHead = move;
		mpTail = mpHead;

	}
	else
	{
		mpTail->SetNext(move);
		move->SetPrev(mpTail);
		mpTail = move;
	}
}

void Record::Add_Double(char x, short y, Piece* positions[9][9])
{
	mpTail->Set_DoubleJump(x, y, positions);
}

void Record::Clear()
{
	this->~Record();
}

void Record::Show()
{
	cout<<(*this);
}

void Record::Delete_Record()
{
	Move* temp = mpTail;
	mpTail = mpTail->GetPrev();
		
	delete temp;
	temp = 0;
	mpTail->SetNext(0);
}

void Record::Undo(Board* board)
{
	if (mpTail != mpHead)
	{
		cout<<"\n\n\aUndoing last move...\n\n";

		if (mpTail->piece1 == 0)
		{
			*board->Position[mpTail->GetY(1)][mpTail->GetX(1)] = *board->Position[mpTail->GetY(2)][mpTail->GetX(2)];

			delete board->Position[mpTail->GetY(2)][mpTail->GetX(2)];
			board->Position[mpTail->GetY(2)][mpTail->GetX(2)] = new Piece();
		}
		else
		{
			short pos1 = 1, pos2 = 2;
			Piece* piece = mpTail->piece1;

			if (mpTail->GetY(3) != 0)
			{
				pos1 = 2;
				pos2 = 3;
				piece = mpTail->piece2;
			}

			while (pos1 >= 1)
			{
				*board->Position[mpTail->GetY(pos1)][mpTail->GetX(pos1)] = *board->Position[mpTail->GetY(pos2)][mpTail->GetX(pos2)];

				delete board->Position[mpTail->GetY(pos2)][mpTail->GetX(pos2)];
				board->Position[mpTail->GetY(pos2)][mpTail->GetX(pos2)] = new Piece();

				*board->Position[(mpTail->GetY(pos1) + mpTail->GetY(pos2)) / 2][(mpTail->GetX(pos1) + mpTail->GetX(pos2)) / 2] = *piece;

				board->RemoveDeadPcs(piece->GetTeam());
				--pos1;
				--pos2;
				piece = mpTail->piece1;
			}
		}
		
		Delete_Record();
	}	
	
	else
	{
		cout<<"\n\n\a\a\aYou have reached the starting positions - You can not undo any further!\n\n";

			throw 1;
	}
}

ostream& operator<<(ostream& os, const Record& record)
{
	cout<<endl<<"<RECORD BEGIN>\n\n";

	for (Move* iter = record.mpHead->GetNext(); iter != 0; iter = iter->GetNext())
	{
		os<<(*iter);
	}

	return os;
}