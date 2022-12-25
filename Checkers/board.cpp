#include "board.h"

Board::Board()
{ 
	HasJumped = false;

	Dead_Red = new string[12]; 
	Dead_Black = new string[12]; 

	DeadRed = 0;
	DeadBlack = 0;

	indexR = -1;
	indexB = -1;

	for (short i = 0; i < 12; i++)
	{
		*(Dead_Red + i) = " ";
		*(Dead_Black + i) = " ";
	}

	for (short Y = 0; Y < 9; ++Y)
	{
		for (short X = 0; X < 9; ++X)
		{
			if ((X + Y) % 2 != 0 || X == 0 || Y == 0)
			{
				Position[Y][X] = new Piece(NUTE, EMPTY);
			}
			else if (Y > 0 && Y < 4 && (X + Y) % 2 == 0)
			{
				Position[Y][X] = new Piece(RED, REGULAR);
			}
			else if (Y > 5 && Y < 9 && (X + Y) % 2 == 0)
			{
				Position[Y][X] = new Piece(BLACK, REGULAR);
			}
			else
			{
				Position[Y][X] = new Piece(NUTE, EMPTY);
			}
		}
	}
} 

Board::~Board()
 {
	for (short Y = 1; Y < 9; ++Y)
	{
		for(short X = 1; X < 9; ++X)
		{
			delete Position[Y][X];
			Position[Y][X] = NULL;
		}
	}
 }
 
bool Board::Move_Is_Illegal(short X1, short Y1, short X2, short Y2)
{
	if (X2 < 1 || X2 > 8 || Y2 > 8 || Y2 < 1 || Y2 > (Y1 + 2) || (X2 + Y2) % 2 != 0 || Position[Y2][X2]->GetClass() != EMPTY || Y1 == Y2 || X1 == X2)
	{
		return true;
	}

	bool is_illegal;

	HasJumped = false;

	switch (Position[Y1][X1]->GetClass())
	{
	case REGULAR:	is_illegal = Regular_Illegal(X1, Y1, X2, Y2);
					break;
	case KING:	is_illegal = King_Illegal(X1, Y1, X2, Y2);
				break;
	default:	is_illegal = true;
				break;
	}

		return is_illegal;
}

bool Board::Regular_Illegal(short X1, short Y1, short X2, short Y2)
{
	switch (Position[Y1][X1]->GetTeam())
	{

	case RED:

				if (Y2 < Y1)
				{
					return true;
				}

				else if (Y2 == (Y1 + 1) && X2 == (X1 + 1) || Y2 == (Y1 + 1) && X2 == (X1 - 1))
				{
					return false;
				}

				else if (Y2 == Y1 + 2 && (X2 == X1 + 2 || X2 == X1 - 2))
				{
					if (Position[Y1 + 1][(X1 + X2) / 2]->GetTeam() == BLACK)
					{
						HasJumped = true;
						return false;
					}

					else return true;
				}

				else return true;

					break;


	case BLACK:

				if (Y2 > Y1)
				{
					return true;
				}

				else if (Y2 == (Y1 - 1) && X2 == (X1 + 1) || Y2 == (Y1 - 1) && X2 == (X1 - 1))
				{
					return false;
				}

				else if (Y2 == Y1 - 2 && (X2 == X1 + 2 || X2 == X1 - 2))
				{
					if (Position[Y1 - 1][(X1 + X2) / 2]->GetTeam() == RED)
					{
						HasJumped = true;
						return false;
					}

					else return true;
				}

				else return true;

					break;

	default: return true;
	}	
}

bool Board::King_Illegal(short X1, short Y1, short X2, short Y2)
{
	char op_team = Op_Team((Position[Y1][X1]->GetTeam()));

	if ((Y2 == (Y1 + 1) && X2 == (X1 + 1)) || (Y2 == (Y1 + 1) && X2 == (X1 - 1)) 
		|| (Y2 == (Y1 - 1) && X2 == (X1 + 1)) || (Y2 == (Y1 - 1) && X2 == (X1 - 1)))
	{
		return false;
	}

	else if (X2 == (X1 - 2) || X2 == (X1 + 2))
	{
		if (Y2 == (Y1 + 2) || Y2 == (Y1 - 2))
		{
			if (Position[((Y1 + Y2) / 2)][((X1 + X2) / 2)]->GetTeam() == op_team)
			{
				HasJumped = true;
				return false;
			}
		}	
	}

		return true;
}

void Board::Adjust_Board(short X1, short Y1, short X2, short Y2)
{
	Position[Y2][X2]->SetClass(Position[Y1][X1]->GetClass());
	Position[Y2][X2]->SetTeam(Position[Y1][X1]->GetTeam());

	Delete_Piece(X1,Y1);

	if (Y2 == (Y1 + 2) || Y2 == (Y1 - 2))
	{
		short Y = (Y1 + Y2) / 2;
		short X = (X1 + X2) / 2;

		Delete_Piece(X,Y);

		AddDeadPcs(Position[Y2][X2]->GetTeam());
	}

	if (Y2 == 8 && Position[Y2][X2]->GetClass() == REGULAR && Position[Y2][X2]->GetTeam() == RED)
	{
		King_Me(X2, Y2, Position[Y2][X2]->GetTeam());
	}
	else if(Y2 == 1 && Position[Y2][X2]->GetClass() == REGULAR && Position[Y2][X2]->GetTeam() == BLACK)
	{
		King_Me(X2, Y2, Position[Y2][X2]->GetTeam());
	}
}

void Board::Show_Board()
{
	cout<<" |-------------------------------|\n";
	cout<<"8|   |"<<(*Position[8][2])<<"|   |"<<(*Position[8][4])<<"|   |"<<(*Position[8][6])<<"|   |"<<(*Position[8][8])<<"|\n";
	cout<<" |-------------------------------|\t\t"<<*(Dead_Red + 0)<<*(Dead_Red + 1)<<*(Dead_Red + 2)<<*(Dead_Red + 3)<<*(Dead_Red + 4)<<*(Dead_Red + 5)<<"\n";
	cout<<"7|"<<(*Position[7][1])<<"|   |"<<(*Position[7][3])<<"|   |"<<(*Position[7][5])<<"|   |"<<(*Position[7][7])<<"|   |\n";
	cout<<" |-------------------------------|\t\t"<<*(Dead_Red + 6)<<*(Dead_Red + 7)<<*(Dead_Red + 8)<<*(Dead_Red + 9)<<*(Dead_Red + 10)<<*(Dead_Red + 11)<<"\n";
	cout<<"6|   |"<<(*Position[6][2])<<"|   |"<<(*Position[6][4])<<"|   |"<<(*Position[6][6])<<"|   |"<<(*Position[6][8])<<"|\n";
	cout<<" |-------------------------------|\n";
	cout<<"5|"<<(*Position[5][1])<<"|   |"<<(*Position[5][3])<<"|   |"<<(*Position[5][5])<<"|   |"<<(*Position[5][7])<<"|   |\n";
	cout<<" |-------------------------------|\n";
	cout<<"4|   |"<<(*Position[4][2])<<"|   |"<<(*Position[4][4])<<"|   |"<<(*Position[4][6])<<"|   |"<<(*Position[4][8])<<"|\n";
	cout<<" |-------------------------------|\n";
	cout<<"3|"<<(*Position[3][1])<<"|   |"<<(*Position[3][3])<<"|   |"<<(*Position[3][5])<<"|   |"<<(*Position[3][7])<<"|   |\n";
	cout<<" |-------------------------------|\t\t"<<*(Dead_Black + 6)<<*(Dead_Black + 7)<<*(Dead_Black + 8)<<*(Dead_Black + 9)<<*(Dead_Black + 10)<<*(Dead_Black + 11)<<"\n";
	cout<<"2|   |"<<(*Position[2][2])<<"|   |"<<(*Position[2][4])<<"|   |"<<(*Position[2][6])<<"|   |"<<(*Position[2][8])<<"|\n";
	cout<<" |-------------------------------|\t\t"<<*(Dead_Black + 0)<<*(Dead_Black + 1)<<*(Dead_Black + 2)<<*(Dead_Black + 3)<<*(Dead_Black + 4)<<*(Dead_Black + 5)<<"\n";
	cout<<"1|"<<(*Position[1][1])<<"|   |"<<(*Position[1][3])<<"|   |"<<(*Position[1][5])<<"|   |"<<(*Position[1][7])<<"|   |\n";
	cout<<" |-------------------------------|\n";
	cout<<"   a   b   c   d   e   f   g   h  \n";
}

void Board::Delete_Piece(short X, short Y)
{
	Position[Y][X]->SetClass(EMPTY);
	Position[Y][X]->SetTeam(NUTE);
}

void Board::King_Me(short X, short Y, char Z)
{
	Position[Y][X]->SetClass(KING);
}

bool Board::DoubleJump_Available(short X, short Y)
{
	char op_team = Op_Team((Position[Y][X]->GetTeam()));

	bool Jump_Available = false;

	switch (Position[Y][X]->GetClass())
	{
	case REGULAR:	switch (Position[Y][X]->GetTeam())
					{
					case RED:	  if ((Y + 2 < 9) && (X + 2 < 9) && Position[Y + 2][X + 2]->GetClass() == EMPTY && Jump_Available == false)
								  {
									  if (Position[Y + 1][X + 1]->GetTeam() == BLACK) 
										  Jump_Available = true;
								  }
								  if ((Y + 2 < 9) && (X - 2 > 0) && Position[Y + 2][X - 2]->GetClass() == EMPTY && Jump_Available == false)
								  {
									  if (Position[Y + 1][X - 1]->GetTeam() == BLACK)
									      Jump_Available = true;
								  }
									  	  break;

					case BLACK:	  if ((Y - 2 > 0) && (X + 2 < 9) && Position[Y - 2][X + 2]->GetClass() == EMPTY && Jump_Available == false)	
								  {
									  if (Position[Y - 1][X + 1]->GetTeam() == RED)
										  Jump_Available = true;
								  }
								  if ((Y - 2 > 0) && (X - 2 > 0) && Position[Y - 2][X - 2]->GetClass() == EMPTY && Jump_Available == false)
								  {
									  if (Position[Y - 1][X - 1]->GetTeam() == RED)
										  Jump_Available = true;
								  }
										  break;
					}
							break;

	case KING:		if ((Y + 2 < 9) && (X + 2 < 9) && Position[Y + 2][X + 2]->GetClass() == EMPTY && Jump_Available == false)
					{
						if (Position[Y + 1][X + 1]->GetTeam() == op_team)
							Jump_Available = true;
					}
					if ((Y + 2 < 9) && (X - 2 > 0) && Position[Y + 2][X - 2]->GetClass() == EMPTY && Jump_Available ==false)
					{
						if (Position[Y + 1][X - 1]->GetTeam() == op_team)
							Jump_Available = true;
					}
					if((Y - 2 > 0) && (X + 2 < 9) && Position[Y - 2][X + 2]->GetClass() == EMPTY && Jump_Available == false)
					{
						if (Position[Y - 1][X + 1]->GetTeam() == op_team)
							Jump_Available = true;
					}
					if((Y - 2 > 0) && (X - 2 > 0) && Position[Y - 2][X - 2]->GetClass() == EMPTY && Jump_Available == false)
					{
						if (Position[Y - 1][X - 1]->GetTeam() == op_team)
							Jump_Available = true;
					}
					        break;
	}

	if (Jump_Available)
	{
		Show_Board();

		char ans;
		cout<<"\n\n\a\aThere is a double-jump available. Would you like to take it?\n\n(Y/N):\t";
		cin>>ans;

		if (ans == 'y' || ans == 'Y') return true;

		else return false;
	}

	else return false;

}

bool Board::Not_Empty()
{
	if (DeadRed == 12 || DeadBlack == 12)
	{
		if (DeadBlack > DeadRed)
		{
			Winning_Team = "Red";
		}
		else if (DeadRed > DeadBlack)
		{
			Winning_Team = "Black";
		}
			return false;
	}
	else
	{
		return true;
	}
}

string Board::GetWinner()
{
	return Winning_Team;
}

char Board::Op_Team(char team)
{
	if (team == RED)
	{
		return BLACK;
	}
	else
	{
		return RED;
	}
}

void Board::AddDeadPcs(char jumpING_Team)
{
	string* DeadPcs;
	short* index;

	if (jumpING_Team == BLACK)
	{
		DeadPcs = Dead_Red;
		index = &indexR;

		++DeadRed;
	}
	else
	{
		DeadPcs = Dead_Black;
		index = &indexB;

		++DeadBlack;
	}

	if (*index < 11) ++(*index);

	char jumpED_Team = Op_Team(jumpING_Team);

	string str = " (";
	str += jumpED_Team;
	str += ")";

	*(DeadPcs + *index) = str;
}

void Board::RemoveDeadPcs(char team)
{
	string* DeadPcs;
	short* index;

	if (team == RED)
	{
		DeadPcs = Dead_Red;
		index = &indexR;

		--DeadRed;
	}
	else 
	{
		DeadPcs = Dead_Black;
		index = &indexB;

		--DeadBlack;
	}

	*(DeadPcs + *index) = " "; 

	if (*index >= 0) --(*index); 
}

Board& Board::operator=(const Board& board)
{
	if (this != &board)
	{
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				this->Position[i][j] = board.Position[i][j];
	}
		return *this;
}

