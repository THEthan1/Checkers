#include <iostream>
#include <string>
#include "record.h"
#include "board.h"

	using namespace std;

class Player
{
public:
	friend ostream& operator<<(ostream& os, Player& player);

	Player(string name, char team);
	
	virtual void GetMove(Board* board, Record& record) = 0;

	string GetName();

	char GetTeam();

protected:
	string m_Name;
	const char m_Team;
};

Player::Player(string name, char team):
	m_Name(name),
	m_Team(team)
{}

string Player::GetName()
{
	return m_Name;
}

char Player::GetTeam()
{
	return m_Team;
}



class HumanPlayer : public Player
{
public:
	HumanPlayer(string name, char team);

	virtual void GetMove(Board* board, Record& record);

	void Do_DoubleJump(short X, short Y, Board* board, Record& record);

	void SetName();

	void Options();

	void Quit();

	void New_Game();

private:
	short Convert_To_Int(char position);
	char Convert_To_Char(short position);
};

HumanPlayer::HumanPlayer(string name, char team):
	Player(name, team)
{}

void HumanPlayer::GetMove(Board* board, Record& record)
{
	short X1 = 0,Y1 = 0, X2 = 0 ,Y2 = 0;
	char X_Position1, X_Position2;

	bool invalid = false;

	board->HasJumped = false;

	try
	{
	do
	{
		if (invalid)
		{
			cout<<"\n\a\a\aThat is an invalid move!";
		}

		do
		{
			cout<<"\n\nIn which position is the piece that you wish to move?\n\n(X/Y):\t";
			cin>>X_Position1;
			cin>>Y1;

			if (X_Position1 == 'x' && Y1 == 1) 
			{
				Options();
			}

			X1 = Convert_To_Int(X_Position1);

			if ((board->Position[Y1][X1])->GetTeam() != m_Team)
			{
				if ((board->Position[Y1][X1])->GetClass() == EMPTY)
				{
					cout<<"\n\n\n\a\a\aThat space is empty! \nPlease choose a space that has a '("<< m_Team <<")' symbol in it.\n";
				}
				else
				{
					cout<<"\n\n\n\a\a\aThat piece is not your color! \nPlease choose a piece that has a/an "<< m_Team <<" on it.\n";
				}
			}

		}while((board->Position[Y1][X1])->GetTeam() != m_Team);

		cout<<"\n\nTo which position would you like to move this piece to?\n\n(X/Y):\t";
		cin>>X_Position2;
		cin>>Y2;

		X2 = Convert_To_Int(X_Position2);

		invalid = true;
	}while (board->Move_Is_Illegal(X1,Y1, X2,Y2));

	record.Add(m_Name, X_Position1, Y1, X_Position2, Y2, board->Position);

	board->Adjust_Board(X1,Y1, X2,Y2);

	if ((Y2 == Y1 + 2 || Y2 == Y1 - 2) && board->DoubleJump_Available(X2, Y2)) Do_DoubleJump(X2, Y2, board, record);
	}

	catch(char)
	{
		record.Undo(board);
		record.Undo(board);

			throw -1;
	}
}

inline short HumanPlayer::Convert_To_Int(char position)
{
	short x = position - 96;

		return x;
}

inline char HumanPlayer::Convert_To_Char(short position)
{
	char c = position + 96;

		return c;
}

void HumanPlayer::Do_DoubleJump(short X, short Y, Board* board, Record& record)
{
	short X1 = X, Y1 = Y, Y2, X2 = 0;
	char X_Position2, cX1 = Convert_To_Char(X);

	do
	{
		if (X2 != 0) cout<<"\a\a\aThat is an invalid move!";

		cout<<"\n\nTo where would you like to move the piece in position "<< cX1 << Y1 <<" ?\n\n(X/Y):\t";
		cin>>X_Position2;
		cin>>Y2;

		X2 = Convert_To_Int(X_Position2);

	}while (board->Move_Is_Illegal(X1,Y1, X2,Y2) || board->HasJumped == false);
	
	record.Add_Double(X_Position2, Y2, board->Position);

	board->Adjust_Board(X1, Y1, X2, Y2);

	if (board->DoubleJump_Available(X2, Y2)) Do_DoubleJump(X2, Y2, board, record);
}

void HumanPlayer::SetName()
{
	cout<<"\n\n"<<m_Name<<", what would you like to change your name to?\t";
	cin>>m_Name;

	cout<<"\n\nOK, your name has been changed to "<<m_Name<<".\n\n";

		throw -1;
}

void HumanPlayer::Options()
{
	int command;

	do
	{
		cout<<"\n\nInput the number of the command you wish to select:";
		cout<<"\n\n1) Undo Move.";
		cout<<"\n2) Edit Name.";
		cout<<"\n3) Start New Game.";
		cout<<"\n4) Quit Game.";
		cout<<"\n5) Exit Options.";
		cout<<"\n\n[1-5]:\t";
		cin>>command;
	}while (command < 1 || command > 4);

	switch (command)
	{
	case 1:		throw 'U';
					break;

	case 2:		SetName();
					break;

	case 3:		New_Game();
					break;

	case 4:		Quit();
					break;

	case 5:		throw -1;	
					break;

	default:	break;
	}
}

void HumanPlayer::Quit()
{
	char ans;
	cout<<"\n\n\a\aAre you sure you want to quit? [Y/N]:\t";
	cin>> ans;

	string quit = "Quit";
	if (ans == 'y' || ans == 'Y') throw quit;

	else Options();
}

void HumanPlayer::New_Game()
{
	char ans;
	cout<<"\n\n\a\aAre you sure you want to start a new game? [Y/N]:\t";
	cin>> ans;

	string new_game = "New Game";
	if (ans == 'y' || ans == 'Y') throw new_game;

	else Options();
}

/*class Computer : public Player
{
public:
	Computer(char team, int dificulty);

	virtual void GetMove(Board* board, Record& recod);

private:
	void ChoseMove(Board& board, int RecurNum, int X, int Y);
	int Dificulty;
};

Computer::Computer(char team, int dificulty):
	Player("The_Computer", team)
{
	switch (dificulty)
	{
		case 1:		Dificulty = 3;
						break;
		case 2:		Dificulty = 5;
						break;
		case 3:		Dificulty = 7;
						break;
		case 4:		Dificulty = 10
						break;

		default:	Dificulty = 5;
						break;
	}
}

void Computer::GetMove(Board& board)
{

}

void Computer::ChoseMove(Board& board, int RecurNum, int X, int Y)
{
	int MaxRecur = Dificulty;
	if (RecurNum != MaxRecur)
	{
		



	}
}


*/



class Game
{
public:
	Game(short NumPlayers);

	~Game();

	void SwitchTurn();

	void Play();

	Record record;

private:
	Board The_Board;
	Player* Player1;
	Player* Player2;
	Player* CurrentPlayer;

};

Game::Game(short NumPlayers)
{
	string P1_Name, P2_Name;
	char ans;
	int dificulty;

	if (NumPlayers == 1)
	{
		cout<<"\n\nWhat is your name player?\t";
		cin>> P1_Name;

		cout<<"\n\nDo you want to go first? (Y/N):\t";
		cin>> ans;

		cout<<"\n\nSelect the difficulty level:";
		cout<<"\n\n1)Easy";
		cout<<"\n2)Medium";
		cout<<"\n3)Hard";
		cout<<"\n4)Super-Duper Hard";
		cin>>dificulty;

		if (ans == 'y' || ans == 'Y')
		{
			Player1 = new HumanPlayer(P1_Name, RED);
			//Player2 = new Computer(BLACK, dificulty);

			//CurrentPlayer = Player1;
		}
		else
		{
			Player1 = new HumanPlayer(P1_Name, BLACK);
			//Player2 = new Computer(RED, dificulty);

			//CurrentPlayer = Player2;
		}
	}
	else if (NumPlayers == 2)
	{
		cout<<"\n\nPlayer 1, what is your name?\t";
		cin>> P1_Name;

		cout<<"\n\nPlayer 2, what is your name?\t";
		cin>> P2_Name;

		cout<<"\n\n"<<P1_Name<<", do you want to go first? (Y/N):\t";
		cin>> ans;

		cout<<endl;

		if (ans == 'y' || ans == 'Y')
		{
			Player1 = new HumanPlayer(P1_Name, RED);
			Player2 = new HumanPlayer(P2_Name, BLACK);

			CurrentPlayer = Player1;
		}
		else
		{
			Player1 = new HumanPlayer(P1_Name, BLACK);
			Player2 = new HumanPlayer(P2_Name, RED);

			CurrentPlayer = Player2;
		}
	}

	cout<<"\n\nInput 'x1' at the start of your move to enter the options menu.\n\n";
}

Game::~Game()
{
	delete Player1;
	delete Player2;

	Player1 = 0;
	Player2 = 0;
	CurrentPlayer = 0;
}

void Game::SwitchTurn()
{
	if (CurrentPlayer == Player1)
	{
		CurrentPlayer = Player2;
	}
	else if (CurrentPlayer == Player2)
	{
		CurrentPlayer = Player1;
	}
}

void Game::Play()
{
	The_Board.Show_Board();

	record.Add(" ",' ',0,' ',0,The_Board.Position);

	while (The_Board.Not_Empty())
	{	 
		cout<<"\nIt is "<< CurrentPlayer->GetName() <<"'s turn.\n";
		try
		{
			CurrentPlayer->GetMove(&The_Board, record);
		}
		catch(int)
		{
			SwitchTurn();
		}

		The_Board.Show_Board();

		SwitchTurn();
	}

	cout<<"\n\n"<<The_Board.GetWinner()<<" IS THE WINNER!\a\a\a\a\a";

	char ans;
	cout<<"\n\n\nWould you like to see a record of your game?\n(Y/N):\t";
	cin>> ans;

	if (ans == 'y' || ans == 'Y') record.Show();

	record.Clear();
}


short main()
{
	short NumPlayers = 2;
	bool Play_Game = true;
	char ans = 'y';

	cout<<"\t\t\t   WELCOME TO CHECKERS!\n";
	cout<<"\n\t\t The game of chess for the lesser minded.\n";

	while (Play_Game == true)
	{
		do			
		{
			if (NumPlayers < 1 || NumPlayers > 2) cout<<"\n\a\a\aYou can only have 1 or 2 players.\n";		

				cout<<"\n\nDo you want to play a single player game \nor a 2 player game? (1-2):   "; 
				cin>> NumPlayers;
	
		}while (NumPlayers < 1 || NumPlayers > 2);

		Game Checker_Game(NumPlayers);

		try
		{
			Checker_Game.Play();

			cout<<"\n\nWould you like to play another game? (Y/N):\t";
			cin>> ans;

			if (ans == 'y' || ans == 'Y')
			{
				Play_Game = true;
			}
			else
			{
				Play_Game = false;
			}
		}

		catch (string command)
		{
			if (command == "Quit") Play_Game = false;
			
			else if (command == "New Game") Play_Game = true;

			else Play_Game = false;
		}

	}

	cout<<"\n\n\n\a\aGood-Bye!\n\n\a";

	return 0;
}


ostream& operator<<(ostream& os, Player& player)
{
	os<<player.GetName();

	return os;
}