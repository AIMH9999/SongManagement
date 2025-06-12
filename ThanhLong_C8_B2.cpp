#include "stdio.h"
#include "conio.h"
#include "time.h"
#include "math.h"
#include "CType.h"
#include "string.h"
#include "Windows.h"
#include <stdlib.h>

int nunberOfUser;
int currentUser = -1;

struct Date {
	int day = 0;
	int month = 0;
	int year = 0;
};

struct Genre {
	int id;
	char* name = new char[255];
	int status = 1;
};

struct GenreDTO {
	int id;
	char* name;
};

struct Song {
	int id;
	int idGenre;
	char* name = new char[256];
	Date releaseDate;
	int status = 1;
};

struct Database {
	Song* songs = new Song[100];
	int songsLength = 0;
	Genre* genres = new Genre[10];
	int genresLength = 0;
};

const int max = 30;

struct Account
{
	int id;
	char* username = new char[max];
	char* password = new char[max];
};

struct listAccount
{
	Account* accounts;
	int nunberOfAccount;
};

Database Data;
listAccount lstAccount;

int optionControl(int& position, int maxOption)
{
	while (1) {
		if (_kbhit()) {
			char key = _getche();
			switch (key) {
			case 72:
				if (position > 1)
					position -= 1;
				return 0;
			case 80:
				if (position < maxOption)
					position += 1;
				return 0;
			case 13:
				return position;
			case 27:
				return -1;
			default:
				return 0;
			}
		}
	}
}

int isExistGenre(char* inputWord)
{
	for (int i = 0; i < Data.genresLength; i++)
	{
		if (strcmp(inputWord, Data.genres[i].name) == 0)
		{
			return 1;
		}
	}
	return 0;
}

int isExistSong(char* inputWord)
{
	for (int i = 0; i < Data.songsLength; i++)
	{
		if (strcmp(inputWord, Data.songs[i].name) == 0)
		{
			return 1;
		}
	}
	return 0;
}

int getIndexSong(char* inputWord)
{
	for (int i = 0; i < Data.songsLength; i++)
	{
		if (strcmp(inputWord, Data.songs[i].name) == 0)
		{
			return i;
		}
	}
	return -1;
}

int getIndexGenre(char* inputWord)
{
	for (int i = 0; i < Data.genresLength; i++)
	{
		if (strcmp(inputWord, Data.genres[i].name) == 0)
		{
			return i;
		}
	}
	return -1;
}

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SET_COLOR(int color)
{
	WORD wColor;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		wColor = (csbi.wAttributes & 0xF0) + (color & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
}

int isExistAccount(char* userName, char* passWord, listAccount lstAccount)
{
	for (int i = 0; i < lstAccount.nunberOfAccount; i++)
	{
		if (strcmp(userName, lstAccount.accounts[i].username) == 0 && strcmp(passWord, lstAccount.accounts[i].password) == 0)
		{
			currentUser = i;
			gotoxy(1, 5);
			SET_COLOR(10);
			printf("\nLogin %s successfully. Please wait a moment", lstAccount.accounts[i].username);
			SET_COLOR(7);
			return i;
		}
	}
	gotoxy(1, 5);
	SET_COLOR(4);
	printf("\nUser or password you entered is incorrect");
	SET_COLOR(7);
	return -1;
}

void OpenFile(FILE*& f, const char* fileName, const char* mode)
{
	errno_t err = fopen_s(&f, fileName, mode);
	if (err != 0)
		printf("The file was not opened\n");
}

//load
void loadGenre(Database &Data)
{
	FILE* f;
	const char* fileName = "data/Bai2_Genres.txt";
	const char* modeRead = "rb";
	OpenFile(f, fileName, modeRead);
	fscanf_s(f, "%d", &Data.genresLength);
	for (int i = 0; i < Data.genresLength; i++)
	{
		fscanf_s(f, "%d", &Data.genres[i].status); 
		fscanf_s(f, "%d", &Data.genres[i].id);
		char c = fgetc(f);
		fgets(Data.genres[i].name, 255, f);
		if (i < Data.genresLength - 1) {
			Data.genres[i].name[strcspn(Data.genres[i].name, "\r")] = '\0';
		}
	}
	fclose(f);
}

void loadSong(Database& Data)
{
	FILE* f;
	const char* fileName = "data/Bai2_Songs.txt";
	const char* modeRead = "r";
	OpenFile(f, fileName, modeRead);
	fscanf_s(f, "%d", &Data.songsLength);
	for (int i = 0; i < Data.songsLength; i++)
	{
		fscanf_s(f, "%d", &Data.songs[i].status);
		fscanf_s(f, "%d", &Data.songs[i].id);
		fscanf_s(f, "%d", &Data.songs[i].idGenre);
		fscanf_s(f, "%d", &Data.songs[i].releaseDate.day);
		fscanf_s(f, "%d", &Data.songs[i].releaseDate.month);
		fscanf_s(f, "%d", &Data.songs[i].releaseDate.year);
		char c = fgetc(f);
		fgets(Data.songs[i].name, 255, f);
		if (i < Data.songsLength - 1) {
			Data.songs[i].name[strcspn(Data.songs[i].name, "\n")] = '\0';
		}
	}
	fclose(f);
}

void loadUser(listAccount& lstAccount)
{
	FILE* f;
	const char* fileName = "data/Bai2_Accounts.txt";
	const char* modeRead = "r";
	OpenFile(f, fileName, modeRead);
	fscanf_s(f, "%d", &lstAccount.nunberOfAccount);
	lstAccount.accounts = new Account[lstAccount.nunberOfAccount];
	char * str = new char[30];
	char* temp = new char[30];
	for (int i = 0; i < lstAccount.nunberOfAccount; i++)
	{
		fscanf_s(f, "%d", &lstAccount.accounts[i].id);
		char c = fgetc(f);
		fgets(str, 30, f);
		strcpy_s(lstAccount.accounts[i].username, 30, strtok_s(str, " ", &temp));
		strcpy_s(lstAccount.accounts[i].password, 30, strtok_s(NULL, " ", &temp));
		if (i < lstAccount.nunberOfAccount - 1) {
			lstAccount.accounts[i].password[strcspn(str, "\r")] = '\0';
		}
	}
	fclose(f);
}

void lodaData(Database& Data, listAccount& lstAccount)
{
	loadUser(lstAccount);
	loadGenre(Data);
	loadSong(Data);
}
//load

//VIEW
void loginView(int position, int userLength, int passLength, char* userName)
{
	SET_COLOR(11);
	printf("_____________________________________________LONG MUSIC_____________________________________________");
	SET_COLOR(13);
	printf("\n\n                                              [LOGIN]");
	if (position == 1)
	{
		printf("\n> USERNAME: [%s]", userName);
		printf("\n  PASSWORD: [");
		for (int i = 0; i < passLength; i++)
		{
			printf("*");
		}
		printf("]");
		gotoxy(13 + userLength, 3);
	}
	else
	{
		printf("\n  USERNAME: [%s]", userName);
		printf("\n> PASSWORD: [");
		for (int i = 0; i < passLength; i++)
		{
			printf("*");
		}
		printf("]");
		gotoxy(13 + passLength, 4);
	}
	SET_COLOR(7);
}

void adminView(int position)
{
	const char* options[] = {
		"1.Add new genre",
		"2.Edit genre",
		"3.Change status genre",
		"4.Add new song",
		"5.Edit song",
		"6.Change status song",
		"7.Show song & genre status",
		"8.Show all list",
		"9.Return login page"
	};
	SET_COLOR(11);
	printf("_____________________________________________LONG MUSIC_____________________________________________");
	SET_COLOR(13);
	printf("\n\n                                              [ADMIN]");
	SET_COLOR(7);
	for (int i = 0; i < sizeof(options) / sizeof(options[0]); i++) {
		if (i + 1 == position) {
			SET_COLOR(10);
		}
		printf("\n  %s", options[i]);
		SET_COLOR(7);
	}
	gotoxy(1, 2 + position);
	SET_COLOR(10);
	printf("\b>");
	SET_COLOR(7);
	gotoxy(0, 2 + position);
}

void showAllListView(int position)
{
	const char* options[] = {
		"1.Show list genres",
		"2.Show list songs",
	};
	SET_COLOR(11);
	printf("_____________________________________________LONG MUSIC_____________________________________________");
	SET_COLOR(13);
	printf("\n\n                                              [ADMIN]");
	SET_COLOR(1);
	printf("\n                                           [SHOW ALL LIST]");
	SET_COLOR(7);
	for (int i = 0; i < sizeof(options) / sizeof(options[0]); i++) {
		if (i + 1 == position) {
			SET_COLOR(10);
		}
		printf("\n  %s", options[i]);
		SET_COLOR(7);
	}
	gotoxy(1, 3 + position);
	SET_COLOR(10);
	printf("\b>");
	SET_COLOR(7);
	gotoxy(0, 3 + position);
}

void showOptionListStatusView(int position)
{
	const char* options[] = {
		"1.Show list Active Song",
		"2.Show list Lock songs",
		"3.Show list Lock songs by genre",
		"4.Show All status songs",
		"5.Show All status Genres",
	};
	SET_COLOR(11);
	printf("_____________________________________________LONG MUSIC_____________________________________________");
	SET_COLOR(13);
	printf("\n\n                                              [ADMIN]");
	SET_COLOR(1);
	printf("\n                                           [SHOW ALL LIST]");
	SET_COLOR(7);
	for (int i = 0; i < sizeof(options) / sizeof(options[0]); i++) {
		if (i + 1 == position) {
			SET_COLOR(10);
		}
		printf("\n  %s", options[i]);
		SET_COLOR(7);
	}
	gotoxy(1, 3 + position);
	SET_COLOR(10);
	printf("\b>");
	SET_COLOR(7);
	gotoxy(0, 3 + position);
}

void showListStatusView(int position)
{
	SET_COLOR(11);
	printf("_____________________________________________LONG MUSIC_____________________________________________");
	SET_COLOR(13);
	printf("\n\n                                              [ADMIN]");
	SET_COLOR(1);
	printf("\n                                      [SHOW ALL LIST STATUS]");
	SET_COLOR(7);
	printf("\n");	
	int count = 0;
	if (position != 5)
	{
		SET_COLOR(9);
		printf("ID\tRelease Date\tGenre");
		gotoxy(45, 4);
		printf("Song\n");
		SET_COLOR(7);
		for (int i = 0; i < Data.songsLength; i++)
		{
			if (position == 1)
			{
				if (Data.songs[i].status == 1 && Data.genres[Data.songs[i].idGenre - 1].status == 1)
				{
					printf("%d\t%d-%d-%d\t%s", Data.songs[i].id, Data.songs[i].releaseDate.day, Data.songs[i].releaseDate.month, Data.songs[i].releaseDate.year, Data.genres[Data.songs[i].idGenre - 1].name);
					gotoxy(45, 5 + count);
					printf("%s\n", Data.songs[i].name);
					count++;
				}
			}
			else if (position == 2)
			{
				if (Data.songs[i].status == 0 && Data.genres[Data.songs[i].idGenre - 1].status == 1)
				{
					printf("%d\t%d-%d-%d\t%s", Data.songs[i].id, Data.songs[i].releaseDate.day, Data.songs[i].releaseDate.month, Data.songs[i].releaseDate.year, Data.genres[Data.songs[i].idGenre - 1].name);
					gotoxy(45, 5 + count);
					printf("%s\n", Data.songs[i].name);
					count++;
				}
			}
			else if (position == 3)
			{
				if (Data.genres[Data.songs[i].idGenre - 1].status == 0)
				{
					printf("%d\t%d-%d-%d\t%s", Data.songs[i].id, Data.songs[i].releaseDate.day, Data.songs[i].releaseDate.month, Data.songs[i].releaseDate.year, Data.genres[Data.songs[i].idGenre - 1].name);
					gotoxy(45, 5 + count);
					printf("%s\n", Data.songs[i].name);
					count++;
				}
			}
			else if (position == 4)
			{
				if (Data.songs[i].status == 1 && Data.genres[Data.songs[i].idGenre - 1].status == 1)
					SET_COLOR(10);
				else if (Data.songs[i].status == 0 && Data.genres[Data.songs[i].idGenre - 1].status == 1)
					SET_COLOR(12);
				else if (Data.genres[Data.songs[i].idGenre - 1].status == 0)
					SET_COLOR(4);
				printf("%d\t%d-%d-%d\t%s", Data.songs[i].id, Data.songs[i].releaseDate.day, Data.songs[i].releaseDate.month, Data.songs[i].releaseDate.year, Data.genres[Data.songs[i].idGenre - 1].name);
				gotoxy(45, 5 + count);
				printf("%s\n", Data.songs[i].name);
				count++;
			}
		}
	}
	else 
	{
		SET_COLOR(9);
		printf("Id\tName\n"); 
		SET_COLOR(7);
		for (int i = 0; i < Data.genresLength; i++)
		{
			if (Data.genres[i].status == 1)
				SET_COLOR(10);
			else if (Data.genres[i].status == 0)
				SET_COLOR(4);
			printf("%d\t%s\n", Data.genres[i].id, Data.genres[i].name);
			SET_COLOR(7);
		}
	}
	SET_COLOR(4);
	printf("Press any key to go back");
	SET_COLOR(7);
}

void outputSong(Database Data)
{
	SET_COLOR(10);
	printf("ID\tRelease Date\tGenre");
	gotoxy(45, 4);
	printf("Song\n");
	SET_COLOR(7);
	for (int i = 0; i < Data.songsLength; i++)
	{
		printf("%d\t%d-%d-%d\t%s", Data.songs[i].id, Data.songs[i].releaseDate.day, Data.songs[i].releaseDate.month, Data.songs[i].releaseDate.year, Data.genres[Data.songs[i].idGenre - 1].name);
		gotoxy(45, 5 + i);
		printf("%s\n", Data.songs[i].name);
	}
}

void outputGenre(Database Data)
{
	SET_COLOR(10);
	printf("Id\tName\n");
	SET_COLOR(7);
	for (int i = 0; i < Data.genresLength; i++)
	{
		printf("%d\t%s\n", Data.genres[i].id, Data.genres[i].name);
	}
}

void showListSongsView(Database Data)
{
	SET_COLOR(11);
	printf("_____________________________________________LONG MUSIC_____________________________________________");
	SET_COLOR(13);
	printf("\n\n                                              [ADMIN]");
	SET_COLOR(1);
	printf("\n                                          [LIST OF SONGS]");
	SET_COLOR(7);
	printf("\n");
	outputSong(Data);
	SET_COLOR(4);
	printf("Press any key to go back");
	SET_COLOR(7);
}

void showListGenresView(Database Data)
{
	SET_COLOR(11);
	printf("_____________________________________________LONG MUSIC_____________________________________________");
	SET_COLOR(13);
	printf("\n\n                                              [ADMIN]");
	SET_COLOR(1);
	printf("\n                                          [LIST OF GENRES]");
	SET_COLOR(7);
	printf("\n");
	outputGenre(Data);
	SET_COLOR(4);
	printf("Press any key to go back");
	SET_COLOR(7);
}

void addNewGenreView(int id, char* inputName)
{
	SET_COLOR(11);
	printf("_____________________________________________LONG MUSIC_____________________________________________");
	SET_COLOR(13);
	printf("\n\n                                              [ADMIN]");
	SET_COLOR(1);
	printf("\n                                          [ADD NEW GENRE]");
	SET_COLOR(7);
	printf("\n");
	printf("Genre ID:[%d]\n", id);
	printf("Genre name:[%s]", inputName);
	gotoxy(strlen(inputName) + 12, 5);
}

void addNewSongView(int id, char* inputName, char* Genre, int position, Date inputDate)
{
	SET_COLOR(11);
	printf("_____________________________________________LONG MUSIC_____________________________________________");
	SET_COLOR(13);
	printf("\n\n                                              [ADMIN]");
	SET_COLOR(1);
	printf("\n                                           [ADD NEW SONG]");
	SET_COLOR(7);
	printf("\n");
	printf(" Song ID:[%d]\n", id);
	printf(" Song name:[%s]\n", inputName);
	printf(" Day:[%d]\n", inputDate.day);
	printf(" Month:[%d]\n", inputDate.month);
	printf(" year:[%d]\n", inputDate.year);
	printf(" Genre:[%s]", Genre);
	if (position == 1)
	{
		gotoxy(0, 5);
		SET_COLOR(10);
		printf("\b>");
		gotoxy(strlen(inputName) + 13, 5);
		printf("<");
		gotoxy(strlen(inputName) + 12, 5);
	}
	if (position == 2)
	{
		gotoxy(0, 6);
		SET_COLOR(10);
		printf("\b>");
		if(inputDate.day < 10)
			gotoxy(8, 6);
		else
			gotoxy(9, 6);
		printf("<");
	}
	if (position == 3)
	{
		gotoxy(0, 7);
		SET_COLOR(10);
		printf("\b>");
		if (inputDate.month < 10)
			gotoxy(10, 7);
		else
			gotoxy(11, 7);
		printf("<");
	}
	if (position == 4)
	{
		gotoxy(0, 8);
		SET_COLOR(10);
		printf("\b>");
		gotoxy(12, 8);
		printf("<");
	}
	else if (position == 5)
	{
		gotoxy(0, 9);
		SET_COLOR(10);
		printf("\b>");
		gotoxy(strlen(Genre) + 9, 9);
		printf("<");
		gotoxy(strlen(Genre) + 8, 9);
	}
	SET_COLOR(7);
}

void editSongView(Song inputSong, int process, int position)
{
	SET_COLOR(11);
	printf("_____________________________________________LONG MUSIC_____________________________________________");
	SET_COLOR(13);
	printf("\n\n                                              [ADMIN]");
	SET_COLOR(1);
	printf("\n                                            [EDIT SONG]");
	SET_COLOR(7);
	printf("\n");
	if (process == 1)
	{		
		printf("Enter song name you want to edit:[%s]", inputSong.name);
		gotoxy(strlen(inputSong.name) + 34, 4);
	}
	if (process == 2)
	{
		printf(" SONG ID:[%d]\n", inputSong.id);
		printf(" Edit song name:[%s]\n", inputSong.name);
		printf(" Edit song genre:[%s]\n", Data.genres[inputSong.idGenre - 1].name);
		printf(" Edit day release:[%d]\n", inputSong.releaseDate.day);
		printf(" Edit month release:[%d]\n", inputSong.releaseDate.month);
		printf(" Edit year release:[%d]\n", inputSong.releaseDate.year);
		if (position == 1)
		{		
			gotoxy(0, 5);
			SET_COLOR(10);
			printf("\b>");
			gotoxy(strlen(inputSong.name) + 18, 5);
			printf("<");
			gotoxy(strlen(inputSong.name) + 17, 5);
		}
		if (position == 2)
		{
			gotoxy(0, 6);
			SET_COLOR(10);
			printf("\b>");
			gotoxy(strlen(Data.genres[inputSong.idGenre - 1].name)+19, 6);
			printf("<");
		}
		if (position == 3)
		{
			gotoxy(0, 7);
			SET_COLOR(10);
			printf("\b>");
			if (inputSong.releaseDate.day < 10)
				gotoxy(21, 7);
			else
				gotoxy(22, 7);
			printf("<");
		}
		if (position == 4)
		{
			gotoxy(0, 8);
			SET_COLOR(10);
			printf("\b>");
			if (inputSong.releaseDate.month < 10)
				gotoxy(23, 8);
			else
				gotoxy(24, 8);
			printf("<");
		}
		if (position == 5)
		{
			gotoxy(0, 9);
			SET_COLOR(10);
			printf("\b>");
			gotoxy(25, 9);
			printf("<");
		}
		
	}
		
	
	SET_COLOR(7);
}

void editGenreView(Genre inputGenre, int process)
{
	SET_COLOR(11);
	printf("_____________________________________________LONG MUSIC_____________________________________________");
	SET_COLOR(13);
	printf("\n\n                                              [ADMIN]");
	SET_COLOR(1);
	printf("\n                                            [EDIT GENRE]");
	SET_COLOR(7);
	printf("\n");
	if (process == 1)
	{
		printf("Enter Genre you want to edit:[%s]", inputGenre.name);
		gotoxy(strlen(inputGenre.name) + 30, 4);
	}
	if (process == 2)
	{
		printf(" Genre ID:[%d]\n", inputGenre.id);
		printf(" Edit Genre name:[%s]\n", inputGenre.name);
		gotoxy(0, 5);
		SET_COLOR(10);
		printf("\b>");
		gotoxy(strlen(inputGenre.name) + 19, 5);
		printf("<");
		gotoxy(strlen(inputGenre.name) + 18, 5);
	}
	SET_COLOR(7);
}

void changeStatusGenreView(int position)
{
	SET_COLOR(11);
	printf("_____________________________________________LONG MUSIC_____________________________________________");
	SET_COLOR(13);
	printf("\n\n                                              [ADMIN]");
	SET_COLOR(1);
	printf("\n                                       [CHANGE STATUS GENRE]");
	SET_COLOR(7);
	printf("\n");
	printf("  Id\tName\n");
	for (int i = 0; i < Data.genresLength; i++)
	{
		if(Data.genres[i].status == 1)
			SET_COLOR(10);
		else if (Data.genres[i].status == 0)
			SET_COLOR(4);
		printf("  %d\t%s\n", Data.genres[i].id, Data.genres[i].name);
		SET_COLOR(7);
	}
	gotoxy(0, 4 + position);
	SET_COLOR(13);
	printf("\b>");
	gotoxy(0, 6 + Data.genresLength);
	if (Data.genres[position - 1].status == 1)
	{
		SET_COLOR(4);
		printf("\nPress Enter to lock genre [%d]-[%s]", Data.genres[position - 1].id, Data.genres[position - 1].name);
	}
	else
	{
		SET_COLOR(10);
		printf("\nPress Enter to unlock genre [%d]-[%s]", Data.genres[position - 1].id, Data.genres[position - 1].name);
	}
	SET_COLOR(7);
}

void changeStatusSongView(int position)
{
	SET_COLOR(11);
	printf("_____________________________________________LONG MUSIC_____________________________________________");
	SET_COLOR(13);
	printf("\n\n                                              [ADMIN]");
	SET_COLOR(1);
	printf("\n                                       [CHANGE STATUS SONG]");
	SET_COLOR(7);
	printf("\n");
	printf("  ID\tRelease Date\tGenre");
	gotoxy(45, 4);
	printf("Song\n");
	for (int i = 0; i < Data.songsLength; i++)
	{
		if (Data.songs[i].status == 1 && Data.genres[Data.songs[i].idGenre - 1].status == 1)
			SET_COLOR(10);
		else if (Data.songs[i].status == 0 && Data.genres[Data.songs[i].idGenre - 1].status == 1)
			SET_COLOR(12);			
		else
			SET_COLOR(4);
		printf("  %d\t%d-%d-%d\t%s", Data.songs[i].id, Data.songs[i].releaseDate.day, Data.songs[i].releaseDate.month, Data.songs[i].releaseDate.year, Data.genres[Data.songs[i].idGenre - 1].name);
		gotoxy(45, 5 + i);
		printf("%s\n", Data.songs[i].name);
		SET_COLOR(7);
	}
	gotoxy(0, 4 + position);
	SET_COLOR(13);
	printf("\b>");
	gotoxy(0, 6 + Data.songsLength);
	if (Data.songs[position - 1].status == 1 && Data.genres[Data.songs[position - 1].idGenre - 1].status == 1)
	{
		SET_COLOR(4);
		printf("\nPress Enter to lock song [%d]-[%s]", Data.songs[position - 1].id, Data.songs[position - 1].name);
	}
	else if (Data.songs[position - 1].status == 0 && Data.genres[Data.songs[position - 1].idGenre - 1].status == 1)
	{
		SET_COLOR(10);
		printf("\nPress Enter to unlock song [%d]-[%s]", Data.songs[position - 1].id, Data.songs[position - 1].name);
	}
	else
	{
		SET_COLOR(4);
		printf("\nThis song [%d]-[%s] is locked by genre, you cannot unlock it", Data.songs[position - 1].id, Data.songs[position - 1].name);
	}
	SET_COLOR(7);
}

void userView(int position)
{
	const char* options[] = {
		"1.All songs",
		"2.Show all song list by genre",
		"3.Show all song list by year",
		"4.Return login page"
	};
	SET_COLOR(11);
	printf("_____________________________________________LONG MUSIC_____________________________________________");
	SET_COLOR(13);
	printf("\n\n                                               [USER]");
	SET_COLOR(7);
	for (int i = 0; i < sizeof(options) / sizeof(options[0]); i++) {
		if (i + 1 == position) {
			SET_COLOR(10);
		}
		printf("\n  %s", options[i]);
		SET_COLOR(7);
	}
	gotoxy(1, 2 + position);
	SET_COLOR(10);
	printf("\b>");
	SET_COLOR(7);
	gotoxy(0, 2 + position);
}

void userSongByGenreView(int position, int process)
{
	SET_COLOR(11);
	printf("_____________________________________________LONG MUSIC_____________________________________________");
	SET_COLOR(13);
	printf("\n\n                                              [ADMIN]");
	SET_COLOR(1);
	printf("\n                                       [SONG LIST BY GENRE]\n");
	SET_COLOR(7);
	if (process == 1)
	{
		for (int i = 0; i < Data.genresLength; i++) {
			if (Data.genres[i].status == 1)
				printf("\n  %s", Data.genres[i].name);		
		}
		gotoxy(0, 4 + position);
		SET_COLOR(10);
		printf("\b>");
		SET_COLOR(7);
	}	
	else
	{
		int count = 0;
		SET_COLOR(10);
		printf("ID\tRelease Date\tGenre");
		gotoxy(45, 4);
		printf("Song\n");
		SET_COLOR(7);
		for (int i = 0; i < Data.songsLength; i++)
		{
			if (Data.songs[i].idGenre == position)
			{
				printf("%d\t%d-%d-%d\t%s", Data.songs[i].id, Data.songs[i].releaseDate.day, Data.songs[i].releaseDate.month, Data.songs[i].releaseDate.year, Data.genres[Data.songs[i].idGenre - 1].name);
				gotoxy(45, 5 + count);
				printf("%s\n", Data.songs[i].name);
				count++;
			}
			
		}
		SET_COLOR(4);
		printf("Press any key to go back");
		SET_COLOR(7);
	}
}

void userSongByYearView(char *inputYearStr, int inputYear, int process)
{
	SET_COLOR(11);
	printf("_____________________________________________LONG MUSIC_____________________________________________");
	SET_COLOR(13);
	printf("\n\n                                              [ADMIN]");
	SET_COLOR(1);
	printf("\n                                       [SONG LIST BY GENRE]\n");
	SET_COLOR(7);
	if (process == 1)
	{
		SET_COLOR(10);
		printf("Enter song release year[%s]", inputYearStr);
		gotoxy(24+ strlen(inputYearStr), 4);
		SET_COLOR(7);
	}
	else
	{
		int count = 0;
		SET_COLOR(10);
		printf("ID\tRelease Date\tGenre");
		gotoxy(45, 4);
		printf("Song\n");
		SET_COLOR(7);
		for (int i = 0; i < Data.songsLength; i++)
		{
			if (Data.songs[i].releaseDate.year == inputYear)
			{
				printf("%d\t%d-%d-%d\t%s", Data.songs[i].id, Data.songs[i].releaseDate.day, Data.songs[i].releaseDate.month, Data.songs[i].releaseDate.year, Data.genres[Data.songs[i].idGenre - 1].name);
				gotoxy(45, 5 + count);
				printf("%s\n", Data.songs[i].name);
				count++;
			}

		}
		SET_COLOR(4);
		printf("Press any key to go back");
		SET_COLOR(7);
	}
}

void userShowListSongsView()
{
	SET_COLOR(11);
	printf("_____________________________________________LONG MUSIC_____________________________________________");
	SET_COLOR(13);
	printf("\n\n                                               [USER]");
	SET_COLOR(1);
	printf("\n                                          [LIST OF SONGS]");
	SET_COLOR(7);
	printf("\n");
	SET_COLOR(10);
	printf("ID\tRelease Date\tGenre");
	gotoxy(45, 4);
	printf("Song\n");
	SET_COLOR(7);
	int count = 0;
	for (int i = 0; i < Data.songsLength; i++)
	{
		if (Data.genres[Data.songs[i].idGenre - 1].status == 1 && Data.songs[i].status == 1)
		{
			printf("%d\t%d-%d-%d\t%s", Data.songs[i].id, Data.songs[i].releaseDate.day, Data.songs[i].releaseDate.month, Data.songs[i].releaseDate.year, Data.genres[Data.songs[i].idGenre - 1].name);
			gotoxy(45, 5 + count);
			printf("%s\n", Data.songs[i].name);
			count++;
		}	
	}
	SET_COLOR(4);
	printf("Press any key to go back");
	SET_COLOR(7);
}

//VIEW

int loginInput(listAccount lstAccount)
{
	char* userName;
	char* passWord;
	userName = new char[30] {'\0'};
	passWord = new char[30] {'\0'};
	currentUser = -1;
	int position = 1;
	int userLength = 0;
	int passLength = 0;
	system("cls");
	loginView(position, userLength, passLength, userName);
	while (currentUser == -1) {
		if (_kbhit()) {
			char key = _getche();
			switch (key) {
			case 9:
			case 72:
			case 80:
			case 75:
			case 77:
				if (position == 2)
					position -= 1;
				else if (position == 1)
					position += 1;
				break;
			case '\b':
				if (position == 1 && userLength > 0)
				{
					userName[userLength] = NULL;
					userLength -= 1;
					userName[userLength] = '\0';
				}
				else if (position == 2 && passLength > 0)
				{
					passWord[passLength] = NULL;
					passLength -= 1;
					passWord[passLength] = '\0';
				}
				break;
			case 13:
				currentUser = isExistAccount(userName, passWord, lstAccount);
				Sleep(1000);
				break;
			default:
				if (key != -32 && key != 9 && key != 72 && key != 75 && key != 77 && key != 80) {
					if (position == 1)
					{
						userName[userLength] = key;
						userLength += 1;
						userName[userLength] = '\0';
					}
					else
					{
						passWord[passLength] = key;
						passLength += 1;
						passWord[passLength] = '\0';
					}
				}
				break;
			}
			system("cls");
			loginView(position, userLength, passLength, userName);
		}
	}
	return currentUser;
}

void adminShowAllListControl()
{
	A1:	int position = 1;
	int adminOption = 0;
	system("cls");
	showAllListView(1);
	while (adminOption !=-1)
	{	
		system("cls");
		showAllListView(position);
		adminOption = optionControl(position, 2);
		if (adminOption > 0)
		{
			if (adminOption == 1) {
				system("cls");
				showListGenresView(Data);			
			}
			else
			{
				system("cls");
				showListSongsView(Data);
			}
			char key = _getche();
			adminOption = 0;			
		}
	}
}

void saveNewGenre(int id, char* inputName)
{
	FILE* f;
	const char* fileName = "data/Bai2_Genres.txt";
	const char* mode = "r+";
	OpenFile(f, fileName, mode);
	Data.genres[id - 1].id = id;
	strcpy_s(Data.genres[id - 1].name, 30, inputName);
	fseek(f, 0, SEEK_SET);
	fprintf(f, "%d", id);
	fseek(f, 0, SEEK_END);
	fprintf(f, "\n%d %d %s",1, Data.genres[id - 1].id, inputName);
	fclose(f);
}

void saveNewSong(int id, char* inputName, int genreId, Date inputDate)
{
	FILE* f;
	const char* fileName = "data/Bai2_Songs.txt";
	const char* mode = "r+";
	OpenFile(f, fileName, mode);
	Data.songs[id - 1].id = id;
	strcpy_s(Data.songs[id - 1].name, 30, inputName);
	Data.songs[id - 1].releaseDate.day = inputDate.day;
	Data.songs[id - 1].releaseDate.month = inputDate.month;
	Data.songs[id - 1].releaseDate.year = inputDate.year;
	Data.songs[id - 1].idGenre = genreId;
	fseek(f, 0, SEEK_SET);
	fprintf(f, "%d", id);
	fseek(f, 0, SEEK_END);
	fprintf(f, "\n%d %d %d %d %d %d %s", 1, Data.songs[id - 1].id, genreId, inputDate.day, inputDate.month, inputDate.year, inputName);
	fclose(f);
}

void updateSong(FILE* f)
{
	fprintf(f, "%d\n", Data.songsLength);
	for (int i = 0; i < Data.songsLength; i++)
	{
		if (i < Data.songsLength - 1)
			fprintf(f, "%d %d %d %d %d %d %s\n", Data.songs[i].status, Data.songs[i].id, Data.songs[i].idGenre, Data.songs[i].releaseDate.day, Data.songs[i].releaseDate.month, Data.songs[i].releaseDate.year, Data.songs[i].name);
		else
			fprintf(f, "%d %d %d %d %d %d %s", Data.songs[i].status, Data.songs[i].id, Data.songs[i].idGenre, Data.songs[i].releaseDate.day, Data.songs[i].releaseDate.month, Data.songs[i].releaseDate.year, Data.songs[i].name);
	}
}

void updateGenre(FILE* f)
{
	fprintf(f, "%d\n", Data.genresLength);
	for (int i = 0; i < Data.genresLength; i++)
	{
		if (i < Data.genresLength - 1)
			fprintf(f, "%d %d %s\n", Data.genres[i].status, Data.genres[i].id, Data.genres[i].name);
		else
			fprintf(f, "%d %d %s", Data.genres[i].status, Data.genres[i].id, Data.genres[i].name);
	}
}

void updateAfterEditSong(Song inputSong)
{
	FILE* f;
	const char* fileName = "data/Bai2_Songs.txt";
	const char* mode = "w";
	OpenFile(f, fileName, mode);
	Data.songs[inputSong.id - 1].idGenre = inputSong.idGenre;
	Data.songs[inputSong.id - 1].releaseDate.day = inputSong.releaseDate.day;
	Data.songs[inputSong.id - 1].releaseDate.month = inputSong.releaseDate.month;
	Data.songs[inputSong.id - 1].releaseDate.year = inputSong.releaseDate.year;
	strcpy_s(Data.songs[inputSong.id - 1].name, 255, inputSong.name);
	updateSong(f);
	fclose(f);
}

void updateAfterEditGenre(Genre inputGenre)
{
	FILE* f;
	const char* fileName = "data/Bai2_Genres.txt";
	const char* mode = "w";
	OpenFile(f, fileName, mode);
	Data.genres[inputGenre.id - 1].id = inputGenre.id;
	strcpy_s(Data.genres[inputGenre.id - 1].name, 255, inputGenre.name);
	updateGenre(f);
	fclose(f);
}

void changeStatusGenge(int position)
{
	FILE* f;
	const char* fileName = "data/Bai2_Genres.txt";
	const char* mode = "w";
	OpenFile(f, fileName, mode);
	if (Data.genres[position - 1].status == 1)
		Data.genres[position - 1].status = 0;
	else
		Data.genres[position - 1].status = 1;
	updateGenre(f);
	fclose(f);
}

void changeStatusSong(int position)
{
	FILE* f;
	const char* fileName = "data/Bai2_Songs.txt";
	const char* mode = "w";
	OpenFile(f, fileName, mode);
	if (Data.songs[position - 1].status == 1)
		Data.songs[position - 1].status = 0;
	else
		Data.songs[position - 1].status = 1;
	updateSong(f);
	fclose(f);
}

void adminAddNewGenreControl()
{
	int isBack = 0;
A1:	int id = Data.genresLength + 1;
	char* inputName = new char[255];
	inputName[0] = '\0';
	int inputLength = 0;
	system("cls");
	addNewGenreView(id, inputName);	
	while (!isBack) {
		if (_kbhit()) {
			char key = _getche();
			switch (key) {
			case '\b':
				if (inputLength > 0)
				{
					inputName[inputLength] = NULL;
					inputLength -= 1;
					inputName[inputLength] = '\0';
				}		
				break;
			case 13:
				if (isExistGenre(inputName) || inputLength == 0)
				{
					system("cls");
					addNewGenreView(id, inputName);
					SET_COLOR(4);
					if (isExistGenre(inputName))
						printf("\nGenre [%s] already exists", inputName);
					else
						printf("\nPlease input without leaving blank.");
					SET_COLOR(7);
					Sleep(1500);
				}
				else
				{
					do
					{
						system("cls");
						addNewGenreView(id, inputName);
						SET_COLOR(10);
						printf("\nPress enter to save esc to cancel");
						SET_COLOR(7);
						key = _getche();
						if (key == 13) {
							saveNewGenre(id, inputName);
							Data.genresLength += 1;
							delete[]inputName;
							goto A1;
						}
					} while (key != 27 && key != 13);
				}
				break;
			case 27:
				isBack = 1;
				break;
			default:
				if (key != -32 && key != 9 ) {
					inputName[inputLength] = key;
					inputLength += 1;
					inputName[inputLength] = '\0';
				}
				break;
			}
			system("cls");
			addNewGenreView(id, inputName);
		}
	}
}
/*
void adminAddNewSongControl()
{
	int isBack = 0;
A1:	int position = 1;
	int id = Data.songsLength + 1;
	int genreId;
	char* inputName = new char[255];
	inputName[0] = '\0';
	int inputLength = 0;
	char* inputGenre = new char[255];
	inputGenre = Data.genres[0].name;
	Date inputDate;
	inputDate.day = 1;
	inputDate.month = 1;
	inputDate.year = 1900;
	for (int i = 1; i <= Data.genresLength; i++)
	{
		if (Data.genres[i - 1].status == 1)
		{
			genreId = i;
			break;
		}	
	}
	system("cls");
	addNewSongView(id, inputName, inputGenre, position, inputDate);
	while (!isBack) {
		if (_kbhit()) {
			char key = _getche();
			switch (key) {
			case 72:
				if (position > 1)
					position -= 1;
				break;
			case 80:
				if (position < 5)
					position += 1;
				break;
			case 75:
				if (inputDate.day > 1 && position == 2)
					inputDate.day -= 1;
				else if (inputDate.month > 1 && position == 3)
					inputDate.month -= 1;
				else if (position == 4)
					inputDate.year -= 1;
				else if (genreId > 1 && position == 5)
				{
					if (Data.genres[genreId - 2].status == 0 && genreId > 2)
						genreId -= 1;
					else if (Data.genres[genreId - 2].status == 0 && genreId == 2)
						break;
					genreId -= 1;
				}				
				break;
			case 77:
				if (inputDate.day < 31 && position == 2)
					inputDate.day += 1;
				else if (inputDate.month < 12 && position == 3)
					inputDate.month += 1;
				else if (position == 4)
					inputDate.year += 1;
				else if (genreId < Data.genresLength && position == 5)
				{
					if (Data.genres[genreId].status == 0 && genreId < Data.genresLength - 1)
						genreId += 1;
					else if (Data.genres[genreId].status == 0 && genreId == Data.genresLength - 1)
						break;
					genreId += 1;
				}
				break;
			case '\b':
				if (inputLength > 0)
				{
					inputName[inputLength] = NULL;
					inputLength -= 1;
					inputName[inputLength] = '\0';
				}
				break;
			case 13:
				if (isExistSong(inputName) || inputLength == 0)
				{
					system("cls");
					addNewSongView(id, inputName, inputGenre, position, inputDate);
					SET_COLOR(4);
					gotoxy(0, 9);
					if (isExistSong(inputName))
						printf("\nSong [%s] already exists", inputName);
					else
						printf("\nPlease input without leaving blank.");
					SET_COLOR(7);
					Sleep(1500);
				}
				else
				{
					do
					{
						system("cls");
						addNewSongView(id, inputName, inputGenre, position, inputDate);
						SET_COLOR(10);
						printf("\nPress enter to save esc to cancel");
						SET_COLOR(7);
						key = _getche();
						if (key == 13) {
							saveNewSong(id, inputName, genreId, inputDate);
							Data.genresLength += 1;
							delete[]inputName;
							goto A1;
						}
					} while (key != 27 && key != 13);
				}
				break;
			case 27:
				isBack = 1;
				break;
			default:
				if (position == 1) {
					if (key != -32) {
						inputName[inputLength] = key;
						inputLength += 1;
						inputName[inputLength] = '\0';
					}
				}				
				break;
			}
			inputGenre = Data.genres[genreId-1].name;
			system("cls");
			addNewSongView(id, inputName, inputGenre, position, inputDate);
		}
	}
}
*/

void adminAddNewSongControl() {
	int isBack = 0;
A1:	int position = 1;
	int id = Data.songsLength + 1;
	int genreId = 1;
	char* inputName = new char[255] {};
	int inputLength = 0;
	char* inputGenre;
	Date inputDate = { 1, 1, 1900 };

	for (int i = 1; i <= Data.genresLength; i++) {
		if (Data.genres[i - 1].status == 1) {
			genreId = i;
			break;
		}
	}

	system("cls");
	addNewSongView(id, inputName, Data.genres[genreId - 1].name, position, inputDate);

	while (!isBack) {
		if (_kbhit()) {
			int key = _getch();
			if (key == 0 || key == 224) {
				key = _getch();
				if (key == 72 && position > 1) position--;
				if (key == 80 && position < 5) position++;
				if (key == 75) {
					if (position == 2 && inputDate.day > 1) inputDate.day--;
					if (position == 3 && inputDate.month > 1) inputDate.month--;
					if (position == 4) inputDate.year--;
					if (position == 5 && genreId > 1) {
						if (Data.genres[genreId - 2].status == 1 || genreId > 2) genreId--;
					}
				}
				if (key == 77) {
					if (position == 2 && inputDate.day < 31) inputDate.day++;
					if (position == 3 && inputDate.month < 12) inputDate.month++;
					if (position == 4) inputDate.year++;
					if (position == 5 && genreId < Data.genresLength) {
						if (Data.genres[genreId].status == 1 || genreId < Data.genresLength - 1) genreId++;
					}
				}
			}
			else {
				switch (key) {
				case '\b':
					if (inputLength > 0) inputName[--inputLength] = '\0';
					break;
				case 13:
					if (isExistSong(inputName) || inputLength == 0) {
						system("cls");
						addNewSongView(id, inputName, Data.genres[genreId - 1].name, position, inputDate);
						SET_COLOR(4);
						gotoxy(0, 9);
						printf(isExistSong(inputName) ? "\nSong [%s] already exists" : "\nPlease input without leaving blank.", inputName);
						SET_COLOR(7);
						Sleep(1500);
					}
					else {
						do {
							system("cls");
							addNewSongView(id, inputName, Data.genres[genreId - 1].name, position, inputDate);
							SET_COLOR(10);
							gotoxy(0, 9);
							printf("\nPress enter to save, esc to cancel");
							SET_COLOR(7);
							key = _getche();
							if (key == 13) {
								saveNewSong(id, inputName, genreId, inputDate);
								Data.songsLength += 1;
								delete[] inputName;
								goto A1;
							}
						} while (key != 27);
					}
					break;
				case 27:
					isBack = 1;
					break;
				default:
					if (position == 1 && key >= ' ' && key <= '~') {
						inputName[inputLength++] = key;
						inputName[inputLength] = '\0';
					}
					break;
				}
			}
			inputGenre = Data.genres[genreId - 1].name;
			system("cls");
			addNewSongView(id, inputName, inputGenre, position, inputDate);
		}
	}
}

/*
void adminEditSongControl()
{
	int isBack = 0;
A1:	int position = 1;
	Song inputSong;
	inputSong.name[0] = '\0';
	int inputLength = 0;
	int process = 1;
	system("cls");
	editSongView(inputSong, process, position);
	while (!isBack) {
		if (_kbhit()) {
			char key = _getche();
			switch (key) {		
			case 72:
				if (position > 1)
					position -= 1;		
				break;
			case 80:
				if (position < 5)
					position += 1;
				break;
			case 75:
				if (process == 2)
				{
					if (inputSong.idGenre > 1 && position == 2)
					{
						if (Data.genres[inputSong.idGenre - 2].status == 0 && inputSong.idGenre > 2)
							inputSong.idGenre -= 1;
						else if (Data.genres[inputSong.idGenre - 2].status == 0 && inputSong.idGenre == 2)
							break;
						inputSong.idGenre -= 1;
					}
					else if (inputSong.releaseDate.day > 1 && position == 3)
						inputSong.releaseDate.day -= 1;
					else if (inputSong.releaseDate.month > 1 && position == 4)
						inputSong.releaseDate.month -= 1;
					else if (position == 5)
						inputSong.releaseDate.year -= 1;
				}				
				break;
			case 77:
				if (process == 2)
				{
					if (inputSong.idGenre < Data.genresLength && position == 2)
					{
						if (Data.genres[inputSong.idGenre].status == 0 && inputSong.idGenre < Data.genresLength - 1)
							inputSong.idGenre += 1;
						else if (Data.genres[inputSong.idGenre].status == 0 && inputSong.idGenre == Data.genresLength - 1)
							break;
						inputSong.idGenre += 1;
					}
					else if (inputSong.releaseDate.day < 31 && position == 3)
						inputSong.releaseDate.day += 1;
					else if (inputSong.releaseDate.month < 12 && position == 4)
						inputSong.releaseDate.month += 1;
					else if (position == 5)
						inputSong.releaseDate.year += 1;
				}
				break;
			case '\b':
				if (inputLength > 0)
				{
					inputSong.name[inputLength] = NULL;
					inputLength -= 1;
					inputSong.name[inputLength] = '\0';
				}
				break;
			case 13:
				if (process == 1)
				{
					if ((inputLength == 0 || getIndexSong(inputSong.name) == -1))
					{
						system("cls");
						editSongView(inputSong, process, position);
						SET_COLOR(4);
						gotoxy(0, 4);
						if (inputLength == 0)
							printf("\nPlease input without leaving blank.");
						else
							printf("\nSong [%s] isn't exists", inputSong.name);
						SET_COLOR(7);
						Sleep(1500);
					}
					else
					{
						process = 2;
						inputSong.id = Data.songs[getIndexSong(inputSong.name)].id;
						inputSong.idGenre = Data.songs[getIndexSong(inputSong.name)].idGenre;
						strcpy_s(inputSong.name, 255, Data.songs[getIndexSong(inputSong.name)].name);
						inputSong.releaseDate.day = Data.songs[getIndexSong(inputSong.name)].releaseDate.day;
						inputSong.releaseDate.month = Data.songs[getIndexSong(inputSong.name)].releaseDate.month;
						inputSong.releaseDate.year = Data.songs[getIndexSong(inputSong.name)].releaseDate.year;
					}
				}
				else if (process == 2)
				{
					do
					{
						system("cls");
						editSongView(inputSong, process, position);
						gotoxy(0, 10);
						SET_COLOR(10);
						printf("\nPress enter to save esc to cancel");
						SET_COLOR(7);
						key = _getche();
						if (key == 13) {
							updateAfterEditSong(inputSong);
							delete[]inputSong.name;
							goto A1;
						}
					} while (key != 27 && key != 13);
				}			
				break;
			case 27:
				if(process == 1)
					isBack = 1;
				else if (process > 1)
					process -= 1;
				break;
			default:
			 if (position == 1) {
					if (key != -32) {
						inputSong.name[inputLength] = key;
						inputLength += 1;
						inputSong.name[inputLength] = '\0';
					}
				}
				break;
			}
			system("cls");
			editSongView(inputSong, process, position);
		}
	}
}
*/

void adminEditSongControl() {
	int isBack = 0;
A1:	int position = 1;
	Song inputSong;
	inputSong.name[0] = '\0';
	int inputLength = 0;
	int process = 1;
	system("cls");
	editSongView(inputSong, process, position);
	while (!isBack) {
		if (_kbhit()) {
			int key = _getch();
			if (key == 224) {
				key = _getch();
				switch (key) {
				case 72:
					if (position > 1)
						position -= 1;
					break;
				case 80:
					if (position < 5)
						position += 1;
					break;
				case 75:
					if (process == 2) {
						if (inputSong.idGenre > 1 && position == 2) {
							if (Data.genres[inputSong.idGenre - 2].status == 0 && inputSong.idGenre > 2)
								inputSong.idGenre -= 1;
							else if (Data.genres[inputSong.idGenre - 2].status == 0 && inputSong.idGenre == 2)
								break;
							inputSong.idGenre -= 1;
						}
						else if (inputSong.releaseDate.day > 1 && position == 3)
							inputSong.releaseDate.day -= 1;
						else if (inputSong.releaseDate.month > 1 && position == 4)
							inputSong.releaseDate.month -= 1;
						else if (position == 5)
							inputSong.releaseDate.year -= 1;
					}
					break;
				case 77:
					if (process == 2) {
						if (inputSong.idGenre < Data.genresLength && position == 2) {
							if (Data.genres[inputSong.idGenre].status == 0 && inputSong.idGenre < Data.genresLength - 1)
								inputSong.idGenre += 1;
							else if (Data.genres[inputSong.idGenre].status == 0 && inputSong.idGenre == Data.genresLength - 1)
								break;
							inputSong.idGenre += 1;
						}
						else if (inputSong.releaseDate.day < 31 && position == 3)
							inputSong.releaseDate.day += 1;
						else if (inputSong.releaseDate.month < 12 && position == 4)
							inputSong.releaseDate.month += 1;
						else if (position == 5)
							inputSong.releaseDate.year += 1;
					}
					break;
				}
			}
			else {
				switch (key) {
				case '\b':
					if (inputLength > 0) {
						inputSong.name[inputLength] = '\0';
						inputLength -= 1;
						inputSong.name[inputLength] = '\0';
					}
					break;
				case 13:
					if (process == 1) {
						if ((inputLength == 0 || getIndexSong(inputSong.name) == -1)) {
							system("cls");
							editSongView(inputSong, process, position);
							SET_COLOR(4);
							gotoxy(0, 4);
							if (inputLength == 0)
								printf("\nPlease input without leaving blank.");
							else
								printf("\nSong [%s] isn't exists", inputSong.name);
							SET_COLOR(7);
							Sleep(1500);
						}
						else {
							process = 2;
							int songIndex = getIndexSong(inputSong.name);
							inputSong.id = Data.songs[songIndex].id;
							inputSong.idGenre = Data.songs[songIndex].idGenre;
							strcpy_s(inputSong.name, 255, Data.songs[songIndex].name);
							inputSong.releaseDate.day = Data.songs[songIndex].releaseDate.day;
							inputSong.releaseDate.month = Data.songs[songIndex].releaseDate.month;
							inputSong.releaseDate.year = Data.songs[songIndex].releaseDate.year;
						}
					}
					else if (process == 2) {
						do {
							system("cls");
							editSongView(inputSong, process, position);
							gotoxy(0, 10);
							SET_COLOR(10);
							printf("\nPress enter to save esc to cancel");
							SET_COLOR(7);
							key = _getche();
							if (key == 13) {
								updateAfterEditSong(inputSong);
								goto A1;
							}
						} while (key != 27 && key != 13);
					}
					break;
				case 27:
					if (process == 1)
						isBack = 1;
					else if (process > 1)
						process -= 1;
					break;
				default:
					if (position == 1 && key != -32) {
						inputSong.name[inputLength] = key;
						inputLength += 1;
						inputSong.name[inputLength] = '\0';
					}
					break;
				}
			}
			system("cls");
			editSongView(inputSong, process, position);
		}
	}
}

void adminEditGenreControl()
{
	int isBack = 0;
A1:	Genre inputGenre;
	inputGenre.name[0] = '\0';
	int inputLength = 0;
	int process = 1;
	system("cls");
	editGenreView(inputGenre, process);
	while (!isBack) {
		if (_kbhit()) {
			char key = _getche();
			switch (key) {
			case '\b':
				if (inputLength > 0)
				{
					inputGenre.name[inputLength] = NULL;
					inputLength -= 1;
					inputGenre.name[inputLength] = '\0';
				}
				break;
			case 13:
				if (process == 1)
				{
					if ((inputLength == 0 || getIndexGenre(inputGenre.name) == -1))
					{
						system("cls");
						editGenreView(inputGenre, process);
						SET_COLOR(4);
						gotoxy(0, 4);
						if (inputLength == 0)
							printf("\nPlease input without leaving blank.");
						else
							printf("\nGenre [%s] isn't exists", inputGenre.name);
						SET_COLOR(7);
						Sleep(1500);
					}
					else
					{
						process = 2;
						inputGenre.id = Data.genres[getIndexGenre(inputGenre.name)].id;
						strcpy_s(inputGenre.name, 255, Data.genres[getIndexGenre(inputGenre.name)].name);					
					}
				}
				else if (process == 2)
				{
					do
					{
						system("cls");
						editGenreView(inputGenre, process);
						gotoxy(0, 6);
						SET_COLOR(10);
						printf("\nPress enter to save esc to cancel");
						SET_COLOR(7);
						key = _getche();
						if (key == 13) {
							updateAfterEditGenre(inputGenre);
							delete[]inputGenre.name;
							goto A1;
						}
					} while (key != 27 && key != 13);
				}
				break;
			case 27:
				if (process == 1)
					isBack = 1;
				else if (process > 1)
					process -= 1;
				break;
			default:
				if (key != -32) {
					inputGenre.name[inputLength] = key;
					inputLength += 1;
					inputGenre.name[inputLength] = '\0';
				}
				break;
			}
			system("cls");
			editGenreView(inputGenre, process);
		}
	}
}

void adminChangeStatusGenreControl()
{
	int isBack = 0;
A1:	int position = 1;
	system("cls"); 
	changeStatusGenreView(position);
	while (!isBack) {
		if (_kbhit()) {
			char key = _getche();
			switch (key) {
			case 72:
				if (position > 1)
					position -= 1;
				break;
			case 80:
				if (position < Data.genresLength)
					position += 1;
				break;
			case 13:
				do
				{
					system("cls");
					changeStatusGenreView(position);
					gotoxy(0, 6 + Data.genresLength);
					SET_COLOR(1);
					if(Data.genres[position - 1].status == 1)
						printf("\nPress enter to lock [%s] genre esc to cancel", Data.genres[position - 1].name);
					else
						printf("\nPress enter to unlock [%s] genre esc to cancel", Data.genres[position - 1].name);
					SET_COLOR(7);
					key = _getche();
					if (key == 13) {
						changeStatusGenge(position);
						goto A1;
					}
				} while (key != 27 && key != 13);
				break;
			case 27:
				isBack = 1;
				break;
			default:
				break;
			}
			system("cls");
			changeStatusGenreView(position);
		}
	}
}

void adminChangeStatusSongControl()
{
	int isBack = 0;
A1:	int position = 1;
	system("cls");
	changeStatusSongView(position);
	while (!isBack) {
		if (_kbhit()) {
			char key = _getche();
			switch (key) {
			case 72:
				if (position > 1)
					position -= 1;
				break;
			case 80:
				if (position < Data.songsLength)
					position += 1;
				break;
			case 13:
				if (Data.genres[Data.songs[position - 1].idGenre - 1].status == 1)
				{
					do
					{
						system("cls");
						changeStatusSongView(position);
						gotoxy(0, 6 + Data.songsLength);
						SET_COLOR(1);
						if (Data.songs[position - 1].status == 1)
							printf("\nPress enter to lock [%s] genre esc to cancel", Data.songs[position - 1].name);
						else
							printf("\nPress enter to unlock [%s] genre esc to cancel", Data.songs[position - 1].name);
						SET_COLOR(7);
						key = _getche();
						if (key == 13) {
							changeStatusSong(position);
							goto A1;
						}
					} while (key != 27 && key != 13);
				}			
				break;
			case 27:
				isBack = 1;
				break;
			default:
				break;
			}
			system("cls");
			changeStatusSongView(position);
		}
	}
}

void adminShowListStatus()
{
A1:	int position = 1;
	int adminOption = 0;
	system("cls");
	showOptionListStatusView(position);
	while (adminOption != -1)
	{
		system("cls");
		showOptionListStatusView(position);
		adminOption = optionControl(position, 5);
		if (adminOption > 0)
		{
			system("cls");
			showListStatusView(adminOption);
			char key = _getche();
			adminOption = 0;
		}
	}
}

void adminSelectedOption(int position, int &returnLogin)
{
	char key;
	int check = 0;
	while (!check) {
		switch (position) {
		case 1:
			adminAddNewGenreControl();
			check = 1;
			break;
		case 2:
			adminEditGenreControl();
			check = 1;
			break;
		case 3:
			adminChangeStatusGenreControl();
			check = 1;
			break;
		case 4:
			adminAddNewSongControl();
			check = 1;
			break;
		case 5:
			adminEditSongControl();
			check = 1;
			break;
		case 6:
			adminChangeStatusSongControl();
			check = 1;
			break;
		case 7:
			adminShowListStatus();
			check = 1;
			break;
		case 8:
			adminShowAllListControl();
			check = 1;
			break;
		case 9:
			returnLogin = 1;
			check = 1;
			break;
		case -1:
		case 0:
			check = 1;
			break;
		}
		
	}
}

void adminViewControl()
{
	int position = 1;
	int adminOption = 0;
	int returnLogin = 0;
	while (!returnLogin)
	{
		system("cls");
		adminView(position);
		adminOption = optionControl(position, 9);	
		adminSelectedOption(adminOption, returnLogin);
	}
}

void userShowAllListControl()
{
	system("cls");
	userShowListSongsView();
	char key = _getche();
}

void userSongByGenreControl()
{
A1:	int position = 1;
	int adminOption = 0;
	int genresLength = 0;
	int* indexGenre = new int[Data.genresLength];
	for (int i = 0; i < Data.genresLength; i++)
	{
		if (Data.genres[i].status == 1)
		{
			indexGenre[genresLength] = i + 1;
			genresLength += 1;
		}		
	}
	
	system("cls");
	userSongByGenreView(position, 1);
	while (adminOption != -1)
	{
		system("cls");
		userSongByGenreView(position, 1);
		adminOption = optionControl(position, genresLength);
		if (adminOption > 0)
		{
			system("cls");
			userSongByGenreView(indexGenre[adminOption - 1], 2);
			char key = _getche();
			adminOption = 0;
		}
	}
}

void userSongByYearControl()
{
	char* inputYearStr = new char[10];
	inputYearStr[0] = '\0';
	int inputLength = 0;
	int isBack = 0;
	int inputYear = 0;
	system("cls");
	userSongByYearView(inputYearStr, inputYear, 1);
	while (!isBack) {
		if (_kbhit()) {
			char key = _getche();
			switch (key) {
			case '\b':
				if (inputLength > 0)
				{
					inputYearStr[inputLength] = NULL;
					inputLength -= 1;
					inputYearStr[inputLength] = '\0';
				}
				break;
			case 13:
				inputYear = atoi(inputYearStr);
				system("cls");
				userSongByYearView(inputYearStr, inputYear, 2);
				key = _getche();
				break;
			case 27:
				isBack = 1;
				break;
			default:
					if (key >= 48 && key <= 57) {
						inputYearStr[inputLength] = key;
						inputLength += 1;
						inputYearStr[inputLength] = '\0';
					}
				break;
			}
			system("cls");
			userSongByYearView(inputYearStr, inputYear, 1);
		}
	}
}

void userSelectedOption(int position, int &returnLogin)
{
	char key;
	int check = 0;
	while (!check) {
		switch (position) {
		case 1:
			userShowAllListControl();
			check = 1;
			break;
		case 2:
			userSongByGenreControl();
			check = 1;
			break;
		case 3:
			userSongByYearControl();
			check = 1;
			break;
		case 4:
			returnLogin = 1;
			check = 1;
			break;
		case -1:
		case 0:
			check = 1;
			break;
		}

	}
}

void userViewControl()
{
	int position = 1;
	int userOption = 0;
	int returnLogin = 0;
	while (!returnLogin)
	{
		system("cls");
		userView(position);
		userOption = optionControl(position, 4);
		userSelectedOption(userOption, returnLogin);
	}
}

void Program()
{
	int currentUser = -1;
	while (1) {
		lodaData(Data, lstAccount);
		currentUser = loginInput(lstAccount);
		if (currentUser == 0)
			adminViewControl();
		else if (currentUser > 0)
			userViewControl();
	}
}
	
void main()
{
	Program();
}


