
#include<iostream>
#include <cstdlib>
#include<ctime>
#include<iomanip>
#include<windows.h>
#include<conio.h>
#include<cstring>
#include<time.h>
#include<fstream>
const char ROWS = 8;
const char COL = 8;
#define BLACK 0
#define BRWON 6
#define WHITE 15
#define GREEN 2
#define RED 4
#define LBLUE 9
#define ORANGE 166
#define MAGENTA 200
#define PINK 65

struct cell
{
	char val;
	char mine;
	char opened;
	char flag;

};
struct record
{
	char games;
	char wins ;
	char losses;
	char time ;
};
struct User
{
	char username[20];
	char password[20];
	record rec;
	bool islogged;
};
void displayuser(User user)
{
	std::cout << "Your Stats: " << std::endl;
	std::cout << "------------" << std::endl;
	std::cout << std::setw(20) << "Username: " << user.username << std::endl;
	std::cout << std::setw(20) << "Games Played: " << static_cast<int>(user.rec.games) << std::endl;
	std::cout << std::setw(20) << "Wins: " << static_cast<int>(user.rec.wins) << std::endl;
	std::cout << std::setw(20) << "Losses: " << static_cast<int>(user.rec.losses) << std::endl;
	std::cout << std::setw(20) << "Best Time: " << static_cast<int>(user.rec.time) << " seconds" << std::endl;
}
void bubblesort(User users[], char size)
{
	for (int i=0; i < size - 1; i++)
	{
		for (int j = 0; j < size - i - 1; j++)
		{
			if (users[j].rec.time > users[j + 1].rec.time)
			{
				std::swap(users[j], users[j + 1]);
			}
		}
	}
}
void leadership(User users[], char size)
{
	std::cout << "LEADERBOARD" << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << std::setw(5) << "Sr.";
	std::cout << std::setw(20) << "Username";
	std::cout << std::setw(15) << "Time";
	std::cout << std::setw(10) << "Games";
	std::cout << std::setw(10) << "Wins";
	std::cout << std::setw(10) << "Losses" << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	for (int i = 0; i < size; ++i) {
		std::cout << std::setw(5) << i + 1;
		std::cout << std::setw(20) << users[i].username;
		std::cout << std::setw(15) << static_cast<int>(users[i].rec.time) << " seconds";
		std::cout << std::setw(10) << static_cast<int>(users[i].rec.games);
		std::cout << std::setw(10) << static_cast<int>(users[i].rec.wins);
		std::cout << std::setw(10) << static_cast<int>(users[i].rec.losses) << std::endl;
	}
}
bool signup(const char* file, User& acc)
{
	std::ofstream writer(file, std::ios::binary | std::ios::app);
	if (!writer)
	{
		std::cout << "enable to write" << std::endl;
		return false;
	}
	writer.write((char*)&acc, sizeof(acc));
	return true;
	
}

bool login(const char* file, User& acc, const char username[], const char password[])
{
	std::ifstream reader(file, std::ios::binary);
		if(!reader)
	    {
			std::cout << "couldnt read"<<std::endl;
			return false;
	    }
	User temp;
	while (reader.read((char*)&temp, sizeof(temp)))
	{
		if (strcmp(temp.username, username) == 0 && strcmp(temp.password, password) == 0)
		{
			acc = temp;
			return true;
		}
	}

}

void getRowColbyLeftClick(char& rpos, char& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(char rpos, char cpos)
{
	COORD scrn;
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOutput, scrn);
}
void setClr(char tcl, char bcl=7)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (tcl + (bcl * 16)));
}
void boarder(char row,char col,char celllength,char cellwidth,char color)
{
	char r;
	char c;
	setClr(color);
	r = row * celllength;
	c = col * cellwidth;
	gotoRowCol(r, c);
	for (char width = 0; width < cellwidth; width++)
	{
		std::cout << char(-37);
	}
	for (char length = 0; length < cellwidth; length++)
	{
		gotoRowCol(r + length, c);
		std::cout << char(-37);

	}
	r += celllength;
	gotoRowCol(r, c);
	for (char width = 0; width <= cellwidth; width++)
	{
		std::cout << char(-37);
	}
	r -= celllength;
	c += cellwidth;
	for (char length = 0; length < cellwidth; length++)
	{
		gotoRowCol(r + length, c);
		std::cout << char(-37);

	}

}
void grid(char (*arr)[COL], char mines)
{


	for (char i = 0; i < ROWS; i++)
	{
		for (char j = 0; j < COL; j++)
		{

			*(*(arr + i) + j) = '0';
		}
	}



	for (unsigned char i = 0; i < mines; i++)
	{

		char rows = rand() % ROWS;
		char cols = rand() % COL;
		if (*(*(arr + rows) + cols) != 'X')
		{
			*(*(arr + rows) + cols) = 'X';
		}
		else
		{
			i--;
		}
	}
	for (char i = 0; i < ROWS; i++)
	{
		for (char j = 0; j < COL; j++)
		{
			if (*(*(arr + i) + j) == 'X')
			{
				for (char x = -1; x <= 1; x++)
				{
					for (char y = -1; y <= 1; y++)
					{
						if (x == 0 && y == 0) continue;
						char Row1 = i + x;
						char Col1 = j + y;
						if (Row1 >= 0 && Row1 < ROWS && Col1 >= 0 && Col1 < COL && arr[Row1][Col1] != 'X') {
							(*(*(arr + Row1) + Col1))++;
						}
					}
				}
			}
		}
	}

}

void print(char (*arr)[COL], cell(*cells)[COL])
{
	const int cellLength = 8; 
	const int cellWidth = 8;

	std::cout << std::endl;
	for (char i = 0; i < ROWS; i++)
	{
		for (char j = 0; j < COL; j++)
		{
			char centerRow = i * cellLength + (cellLength / 2)+4;
			char centerCol = j * cellWidth + (cellWidth / 2)+4;
			boarder(i+2, j+2, cellLength, cellWidth, ORANGE);

			gotoRowCol(10+centerRow,10+ centerCol);
			if (*((char*)(&(cells[i][j])) + sizeof(char) * 3))
			{
				setClr(WHITE, BLACK);
				std::cout << "F";
			}
			else if (!(*((char*)(&(cells[i][j])) + sizeof(char) * 2)))
			{
				setClr(WHITE, BLACK);
				std::cout << " " <<char(-37)<<char(-37)<<" ";
			}
			else
			{
				if ((*(*(arr + i) + j)) == 'X' || (*(*(arr + i) + j)) == '3')
				{
					setClr(RED, BLACK);
				}
				else if ((*(*(arr + i) + j)) == '0' || (*(*(arr + i) + j)) == '4')
				{
					setClr(LBLUE, BLACK);
				}
				else if ((*(*(arr + i) + j)) == '2' || (*(*(arr + i) + j)) == '7' || (*(*(arr + i) + j)) == '8' || (*(*(arr + i) + j)) == '6')
				{
					setClr(BRWON, BLACK);
				}
				else if ((*(*(arr + i) + j)) == '1' || (*(*(arr + i) + j)) == '5')
				{
					setClr(GREEN, BLACK);
				}
				else
				{
					setClr(BLACK, BLACK);
				}
				std::cout << "  " << (*(*(arr + i) + j)) << " ";
			}



		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
}
//void grid1(int totalrows, int totalcols)
//{
//	setClr(LBLUE);
//	for (int i = 0; i < totalrows; i++)
//	{
//		for (int j = 0; j < totalcols; j++)
//		{
//			boarder(i, j, 8, 8,LBLUE);
//		}
//	}
//}


void rightclick(char r, char c, cell(*cells)[COL])
{
	if (!*((char*)(&(cells[r][c])) + sizeof(char) * 2))
	{
		*((char*)(&(cells[r][c])) + sizeof(char) * 3) = !(*((char*)(&(cells[r][c])) + sizeof(char) * 3));
	}
	else if (*((char*)(&(cells[r][c])) + sizeof(char) * 3))
	{
		*((char*)(&(cells[r][c])) + sizeof(char) * 3) = false;
	}
}
void floodfill(char r, char c, char (*arr)[COL], cell (*cells)[COL], char mines)
{
	char dX[] = { 0, 0, 1, -1, 1, -1, 1, -1 };
	char dY[] = { 1, -1, 0, 0, 1, -1, -1, 1 };

	char Row[ROWS * COL];
	char Col[ROWS * COL];
	char Index = 0;

	Row[Index] = r;
	Col[Index] = c;
	Index++;

	while (Index > 0)
	{
		char r = Row[Index - 1];
		char c = Col[Index - 1];
		Index--;

		for (char i = 0; i < 8; ++i)
		{
			char newRow = r + dX[i];
			char newCol = c + dY[i];

			if (newRow >= 0 && newRow < ROWS && newCol >= 0 && newCol < COL && !cells[newRow][newCol].opened)
			{
				cells[newRow][newCol].opened = true;
				if (*(*(arr + newRow) + newCol) == '0')
				{
					Row[Index] = newRow;
					Col[Index] = newCol;
					Index++;
				}
			}
		}
	}
}





	
void leftClick(char r, char c, char(*arr)[COL], cell(*cells)[COL], char mines,bool& gameover,time_t& starttime) {
	if (!(*((char*)(&(cells[r][c])) + sizeof(char) * 3)) && !(*((char*)(&(cells[r][c])) + sizeof(char) * 2)))
	{
		*((char*)(&(cells[r][c])) + sizeof(char) * 2) = true;

		if (*(*(arr + r) + c) == 'X')
		{
			gameover = true;
			std::cout << std::endl;
			std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << "                                  Game Over! You clicked on a mine." << std::endl;
		}
		
		else if (*(*(arr + r) + c) == '0') {
			
			floodfill(r, c, arr, cells,mines);
		}
	}
}


bool getRowColbyClick(char& rpos, char& cpos) {
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do {
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.EventType == MOUSE_EVENT) {
			if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
				cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
				rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
				return true; // Left click
			}
			else if (InputRecord.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
				cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
				rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
				return false; // Right click
			}
		}
	} while (true);
}


void cellclick(char celllength,char cellwidth,char (*arr)[COL],cell(*cells)[COL],char mines,bool& gameover,time_t& starttime)
{
	char r, c;
	bool isleftclick =getRowColbyClick(r, c);
	
	char ri = (r-15) / celllength;
	char ci = (c-15) / cellwidth;
	if (!gameover)
	{
		if (ri >= 0 && ri < ROWS && ci >= 0 && ci < COL)
		{
			if (isleftclick)
			{
				leftClick(ri, ci, arr, cells, mines,gameover,starttime);
			}
			else {
				rightclick(ri, ci, cells);
			}
		}
		print(arr, cells);
	}
	

}
void header(char r, char c, char cwidth,char cheight, char color, const char*message)
{
	setClr(color, BLACK);
	gotoRowCol(r, c);
	for (char width = 0; width < cwidth; width++)
	{
		std::cout << char(-37);
	}

	for (char height = 1; height < cheight-1; height++)
	{
		gotoRowCol(r+height, c);
		std::cout << char(-37);
		for (char width = 1; width < cwidth-1; width++)
		{
			std::cout << " ";
		}
		std::cout << char(-37);
	}
	gotoRowCol(r+cheight-1, c);
	for (char width = 0; width < cwidth; width++)
	{
		std::cout << char(-37);
	}
	char messageLength = strlen(message);
	char messageStartCol = r + (cwidth - messageLength) / 2;
	gotoRowCol(r + (cheight / 2), messageStartCol);
	std::cout << message;
}




int main()
{
	//----logincode--
	const char* file = "users.bin";
	User user;

	
	char choice;
	std::cout << "Welcome to Minesweeper!\n";
	std::cout << "1. Login\n";
	std::cout << "2. Sign up\n";
	std::cin >> choice;

	if (choice == '1') {
		char username[20];
		char password[20];
		std::cout << "Enter username: ";
		std::cin >> username;
		std::cout << "Enter password: ";
		std::cin >> password;
		if (!login(file, user, username, password)) {
			std::cout << "Login failed. Invalid username or password.\n";
			return 1;
		}
	}
	else if (choice == '2') {
		std::cout << "Enter username: ";
		std::cin >> user.username;
		std::cout << "Enter password: ";
		std::cin >> user.password;
		user.rec.games;
		user.rec.wins;
		user.rec.losses;
		user.rec.time;
		user.islogged = true;
		if (!signup(file, user)) {
			std::cout << "Sign up failed.\n";
			return 1;
		}
	}
	else {
		std::cout << "Invalid choice.\n";
		return 1;
	}

	//-------gamecode------
	srand(time(nullptr));
	
	char a;
	char mines;
	std::cout << std::endl << "ENTER GAME LEVEL :" << std::endl << std::endl << std::endl;
	std::cout << std::endl << "1. Easy ( E ) " << std::endl;
	std::cout << std::endl << "2. Medium ( M ) " << std::endl;
	std::cout << std::endl << "3. Difficult ( D ) " << std::endl;

	std::cin >> a;
	if (a == 'd' || a == 'D')
	{
		std::cout << std::endl << "you chose DIFFICULT level" << std::endl;
		mines = 11;


	}
	else   if (a == 'm' || a == 'M')
	{
		std::cout << std::endl << "you chose MEDIUM level" << std::endl;
		mines = 8;


	}
	else   if (a == 'E' || a == 'e')
	{
		std::cout << std::endl << "you chose EASY level" << std::endl;
		mines = 5;


	}

	system("cls");
	char arr[ROWS][COL
	];
	cell cells[ROWS][COL];
	for (char i = 0; i < ROWS; i++)
	{
		for (char j = 0; j < COL; j++)
		{
			*((char*)(&(cells[i][j])) + sizeof(char) * 0) = { '0' };
			*((char*)(&(cells[i][j])) + sizeof(char) * 3) = false;
			*((char*)(&(cells[i][j])) + sizeof(char) * 1) = false;
			*((char*)(&(cells[i][j])) + sizeof(char) * 2) = false;


		}
	}
	const char* message = "MINESWEEPER";
	header(5, 10, 30, 5, BRWON, message);
	grid(arr, mines);
	print(arr,cells);
	setClr(WHITE, BLACK);
	time_t starttime = time(nullptr);
	bool gameover = false;
	if (!gameover)
	{
		while (true)
		{
			cellclick(8, 8, arr, cells, mines, gameover,starttime);
			if (gameover)
			{
				break;
			}
		}
	}
	time_t endtime = time(nullptr);
	user.rec.time = endtime;
	user.rec.games++;
	double final = difftime(endtime, starttime);
	std::cout << std::endl << std::endl  << "Final Time: " << final << "s" << std::endl;
	_getch();

	//----leadreship---
	system("cls");
	
	displayuser(user);

	const int maxUsers = 100; 
	User users[maxUsers];
	int numUsers = 0;

	std::ifstream fileReader(file, std::ios::binary);
	if (!fileReader) {
		std::cerr << "Error: Unable to open file for reading\n";
		return 1;
	}

	while (fileReader.read(reinterpret_cast<char*>(&users[numUsers]), sizeof(User))) {
		numUsers++;
	}
	fileReader.close();

	bubblesort(users, numUsers);
	leadership(users, numUsers);


    return 0;
}



