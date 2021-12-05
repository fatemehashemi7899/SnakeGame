#include <iostream>
#include <queue>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <ctime>
#include <conio.h>
#include <math.h>

using namespace std;
const int N = 530;
const int Gx = 15;
const int Gy = 35;
int v[N][5];
int sze[N];
int par[N];
int par2[N];
int ground[Gx][Gy];
struct coordinate {
	int x, y;
};
struct Snake {
	coordinate length[525];
	int L=4;
};

void HideCursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}
void clearScreen() {
	system("cls");
}
void setTextColor(int textColor, int backColor = 0)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	int colorAttribute = backColor << 4 | textColor;
	SetConsoleTextAttribute(consoleHandle, colorAttribute);
}
void gotoxy(int x, int y) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursorCoord;
	cursorCoord.X = y;
	cursorCoord.Y = x; SetConsoleCursorPosition(consoleHandle, cursorCoord);
}
void sleep(unsigned int mseconds)
{
	clock_t goal = mseconds + clock();
	while (goal > clock());
}
int randomx() {
	return(rand() % 12 + 1);
}
int randomy() {
	return(rand() % 32 + 1);
}
int table() {
	setTextColor(3, 0);
	int i;
	for (i = 0; i <= 34; i++) {
		gotoxy(0, i);
		printf("-");
		gotoxy(14, i);
		printf("-");
	}
	for (i = 0; i <= 14; i++) {
		gotoxy(i, 0);
		printf("|");
		gotoxy(i, 34);
		printf("|");
	}
	return 0;
}
void shift(Snake &s) {
	int i;
	for (i = 524; i >= 1; i--) {
		s.length[i].x = s.length[i - 1].x;
		s.length[i].y = s.length[i - 1].y;
	}
}
int die(Snake mine) {
	int i;
	for (i = 1; i <= mine.L; i++) {
		if (mine.length[0].x == mine.length[i].x && mine.length[0].y == mine.length[i].y) return 0;
	}
	if (mine.length[0].x <= 0) return 0;
	if (mine.length[0].y <= 0)  return 0;
	if (mine.length[0].y >= 34) return 0;
	if (mine.length[0].x >= 14) return 0;
	return 1;
}
int newDie(Snake one, Snake two, int &whoDie) {
	int i;
	for (i = 1; i <= one.L; i++) {
		if (one.length[0].x == one.length[i].x && one.length[0].y == one.length[i].y) {
			whoDie = 1;
			return 0;
		}
	}
	if (one.length[0].x <= 0) {
		whoDie = 1;
		return 0;
	}
	if (one.length[0].y <= 0) {
		whoDie = 1;
		return 0;
	}
	if (one.length[0].y >= 34) {
		whoDie = 1;
		return 0;
	}
	if (one.length[0].x >= 14) {
		whoDie = 1;
		return 0;
	}
	for (i = 1; i <= two.L; i++) {
		if (two.length[0].x == two.length[i].x && two.length[0].y == two.length[i].y) {
			whoDie = 2;
			return 0;
		}
	}
	if (two.length[0].x <= 0) {
		whoDie = 2;
		return 0;
	}
	if (two.length[0].y <= 0) {
		whoDie = 2;
		return 0;
	}
	if (two.length[0].y >= 34) {
		whoDie = 2;
		return 0;
	}
	if (two.length[0].x >= 14) {
		whoDie = 2;
		return 0;
	}
	for (i = 1; i <= two.L; i++) {
		if (one.length[0].x == two.length[i].x && one.length[0].y == two.length[i].y) {
			whoDie = 1;
			return 0;
		}
	}
	for (i = 1; i <= one.L; i++) {
		if (two.length[0].x == one.length[i].x && two.length[0].y == one.length[i].y) {
			whoDie = 2;
			return 0;
		}
	}
	if (one.length[0].x == two.length[0].x && one.length[0].y == two.length[0].y) {
		whoDie = 3;
		return 0;
	}
	return 1;
}
void Apple(Snake s1, int &xa, int &ya) {
	int sw, m;
	do {
		sw = 0;
		xa = randomx();
		ya = randomy();
		for (m = 0; m < s1.L; m++) {
			if (s1.length[m].x == xa && s1.length[m].y == ya) sw = 1;
		}
	} while (sw == 1);
	gotoxy(xa, ya);
	setTextColor(4, 0);
	printf("@");
}
void Apple2(Snake s1, Snake s2, int &xa, int &ya) {
	int sw, m;
	do {
		sw = 0;
		xa = randomx();
		ya = randomy();
		for (m = 0; m < s1.L; m++) {
			if (s1.length[m].x == xa && s1.length[m].y == ya) sw = 1;
			if (s2.length[m].x == xa && s2.length[m].y == ya) sw = 1;
		}
	} while (sw == 1);
	gotoxy(xa, ya);
	setTextColor(4, 0);
	printf("@");
}
void printScore(Snake s1) {
	gotoxy(16, 1);
	setTextColor(8, 0);
	printf("Snake one: %d", s1.L - 4);
}
void printScore2(Snake s1, Snake s2) {
	gotoxy(16, 1);
	setTextColor(8, 0);
	printf("Snake one: %d", s1.L - 4);
	gotoxy(16, 22);
	setTextColor(1, 0);
	printf("Snake two: %d", s2.L - 4);
}

// AI
void bfs(int source, int dist) {
	for (int i = 0; i<N; i++)
		par[i] = -1;
	queue<int>q;
	q.push(source);
	while (q.size()) {
		int w = q.front(); q.pop();
		for (int i = 0; i< sze[w]; i++) {
			int qw = v[w][i];
			if (qw != source && par[qw] == -1) {
				par[qw] = w;
				q.push(qw);
			}
		}
	}
}
void updateGround(Snake s1) {
	int i, j;
	for (i = 1; i < 14; i++) {
		for (j = 1; j < 34; j++) {
			ground[i][j] = 0;
		}
	}
	for (i = 0; i <= 14; i++) {
		ground[i][0] = 1;
		ground[i][34] = 1;
	}
	for (i = 0; i <= 34; i++) {
		ground[0][i] = 1;
		ground[14][i] = 1;
	}
	for (i = 0; i < s1.L; i++) {
		ground[s1.length[i].x][s1.length[i].y] = 1;
	}
}
void updateGround2(Snake s1, Snake s2) {
	int i, j;
	for (i = 1; i < 14; i++) {
		for (j = 1; j < 34; j++) {
			ground[i][j] = 0;
		}
	}
	for (i = 0; i <= 14; i++) {
		ground[i][0] = 1;
		ground[i][34] = 1;
	}
	for (i = 0; i <= 34; i++) {
		ground[0][i] = 1;
		ground[14][i] = 1;
	}
	for (i = 0; i < s1.L; i++) {
		ground[s1.length[i].x][s1.length[i].y] = 1;
	}
	for (i = 0; i < s2.L; i++) {
		ground[s2.length[i].x][s2.length[i].y] = 1;
	}
}
void updateVArrey() {
	int i, j, counter;
	for (i = 0; i <= 34; i++) {
		for (j = 0; j <= 14; j++) {
			counter = 0;
			if (j >= 1 && ground[j - 1][i] != 1)  v[(j * 35) + i][counter++] = ((j - 1) * 35) + i;
			if (i <= 33 && ground[j][i + 1] != 1) v[(j * 35) + i][counter++] = ((j * 35) + i) + 1;
			if (i >= 1 && ground[j][i - 1] != 1)  v[(j * 35) + i][counter++] = ((j * 35) + i) - 1;
			if (j <= 13 && ground[j + 1][i] != 1) v[(j * 35) + i][counter++] = ((j + 1) * 35) + i;
			sze[(j * 35) + i] = counter;
		}
	}
}
int dist(int xa, int ya) {
	return((xa * 35) + ya);
}
int sourse(Snake s1) {
	return((s1.length[0].x * 35) + s1.length[0].y);
}

int main ()
{
    char answer;
	answer = getch();
}
