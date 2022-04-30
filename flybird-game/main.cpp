#include<iostream>
#include<conio.h>
#include<dos.h>
#include<stdlib.h>
#include<string.h>
#include <windows.h>
#include <time.h>
#include <chrono>
#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70
#define MENU_WIDTH 20
#define GAP_SIZE 7
#define PIPE_DIF 45
using namespace std;
using namespace std::chrono;
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

class Node
{
public:
int key;
int data;
Node *next;

Node(int c=0)
	{
	    key = 0;
	    data = c;
	    next = NULL;
	}
};
void insertnode(Node **head , int key ,int data)
{
    Node *newnode= new Node();
    newnode->data=data;
    newnode->key=key;
    Node *temp=*head;
    while(temp->next !=NULL)
    {
        temp=temp->next;
    }
    temp->next= newnode;
}
// function to change data in the list
void changevalue(Node *head,int key,int value)
{
    Node *head_ref = head;
    while(head_ref != NULL)
    {
        if (head_ref->key == key)
        {
            head_ref->data = value;
            break;
        }
        head_ref=head_ref->next;
    }
}
// function to get data from the list
int getdata(Node *head,int key)
{
    Node *head_ref = head;
    while(head_ref != NULL)
    {
        if (head_ref->key == key)
        {
            return head_ref->data ;

        }
        head_ref=head_ref->next;
    }
    return -1;
}
      //  int pipePos[3];   // Position of the pipe
     //   int gapPos[3];    // Position of the gap that the bird pass through it
      //  int pipeFlag[3];  // Barriers that end the game when the bird hits them
        Node PipeFlag;
        Node PipePos;
        Node GapPos;
        Node * pipePos=&PipePos;
        Node * gapPos=&GapPos;
        Node * pipeFlag=&PipeFlag;

//insertnode(&pipeFlagg,2,0);

// array have strings used to draw the bird
        char bird[2][6] = { '/','-','-','o','\\',' ',
        					'|','_','_','_',' ','>' };
int birdPos = 6;   // Position of the bird
int score = 0;     // Initially score is 0



void gotoxy(int x, int y)
{

	CursorPosition.X = x;  // position of x axis
	CursorPosition.Y = y;  // position of y axis
	// To set the position of the cursor
	SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size)
{ // to show the statistics after playing
	if (size == 0)
		size = 20;

	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}
void drawBorder()
 {
	// set up and down boarder for the gameplay
	for (int i = 0; i < SCREEN_WIDTH; i++) {
		gotoxy(i, 0); cout << "±";
		gotoxy(i, SCREEN_HEIGHT); cout << "±";
	}

	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		gotoxy(0, i); cout << "±";
		gotoxy(SCREEN_WIDTH, i); cout << "±";
	}
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		gotoxy(WIN_WIDTH, i); cout << "±";
	}
}
void genPipe(int ind)
{
	// every time will generate gap at random position
	//gapPos[ind] = 3 + rand() % 14;
	changevalue(gapPos,ind,(3 + rand() % 14));
}
void drawPipe(int ind) //draws the pipes in the game
{
	if (getdata(pipeFlag,ind)) {
		// draw the up part of gap
		for (int i = 0; i < getdata(gapPos,ind); i++) {
			gotoxy(WIN_WIDTH - getdata(pipePos,ind), i + 1); cout << "*";
		}
		// draw the down part of gap
		for (int i = getdata(gapPos,ind) + GAP_SIZE; i < SCREEN_HEIGHT - 1; i++) {
			gotoxy(WIN_WIDTH - getdata(pipePos,ind), i + 1); cout << "*";
		}
	}
}
void erasePipe(int ind)
 {
	if (getdata(pipeFlag,ind)) {
		// erase the up part of gap to adjust their pos
		for (int i = 0; i < getdata(gapPos,ind); i++) {
			gotoxy(WIN_WIDTH - getdata(pipePos,ind), i + 1); cout << "   ";
		}
		// erase the down part of gap
		for (int i = getdata(gapPos,ind) + GAP_SIZE; i < SCREEN_HEIGHT - 1; i++) {
			gotoxy(WIN_WIDTH - getdata(pipePos,ind), i + 1); cout << "   ";
		}
	}
}

void drawBird()
 {
	// no. of rows is 2 --> length
	for (int i = 0; i < 2; i++) {
		// no. of column is 6  --> width
		for (int j = 0; j < 6; j++) {
			// set position of printing the character and print it at screen
			gotoxy(j + 2, i + birdPos); cout << bird[i][j];
		}
	}
}
void eraseBird()
{
	// function to erase the bird to draw it again after erasing it
	// same for loop for drawing it
	// to erase the old bird position
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 6; j++) {
			gotoxy(j + 2, i + birdPos); cout << " ";
		}
	}
}

int collision()
 {
	// if the bird hits the barriers
	if (getdata(pipePos,0) >= 61) {
		if (birdPos<getdata(gapPos,0) || birdPos >getdata(gapPos,0) + GAP_SIZE) {
			// print hit and wait to get any character to end the game
			cout << " HIT ";
			getch();
			return 1;
		}
	}
	return 0;
}
/*
void debug(){
//	gotoxy(SCREEN_WIDTH + 3, 4); cout<<"Pipe Pos: "<<pipePos[0];

}*/
void gameover(int duration)
 {
	system("cls"); 	// to clear the screen
	cout << endl;
	cout << "\t\t--------------------------" << endl;
	cout << "\t\t-------- Game Over -------" << endl;
	cout << "\t\t--------------------------" << endl << endl;
	cout << "\t\tThe last round took: " << duration << " seconds" << endl;// print the time taken from start to end
	cout << "\t\tYour score = " << score << endl;
	cout << "\t\tPress any key to go back to menu.";
	getch();
}
void updateScore()
{
	// to go to the position of printint the score on the screen and update it
	// pass to it the position
	gotoxy(WIN_WIDTH + 7, 5); cout << "Score: " << score << endl;
}

void instructions()
 {

	system("cls"); // to clear the screen
	cout<<"Instructions";
	cout<<"\n----------------";
	cout<<"\n Press spacebar to make bird fly";
	cout<<"\n\nPress any key to go back to menu";
// to quit from the instruction
	getch();
}

void play()
{
	auto start = high_resolution_clock::now();// calculate the time of beginning the game
	// position of the bird at row 6 at begging of game
	birdPos = 6;
	// initially make score =0
	score = 0;
	changevalue(pipeFlag,0,1) ;  // set it to be 1 --> used later in drawpip
	changevalue(pipeFlag,1, 0);  // set it to be 0 --> used later in drawpip
	// initially start at width 66 at the end of boundary
	changevalue(pipePos,0, getdata(pipePos,1) );

	system("cls");
	// draw the border of the game
	drawBorder();
	// generate Barrier
	genPipe(0);
	// at first --> score=0
	updateScore();
	// print the name of game and score and many instructions at the screen while the user is playing
	gotoxy(WIN_WIDTH + 5, 2); cout << "FLAPPY BIRD";
	gotoxy(WIN_WIDTH + 6, 4); cout << "----------";
	gotoxy(WIN_WIDTH + 6, 6); cout << "----------";
	gotoxy(WIN_WIDTH + 7, 12); cout << "Control ";
	gotoxy(WIN_WIDTH + 7, 13); cout << "-------- ";
	gotoxy(WIN_WIDTH + 2, 14); cout << " Spacebar = jump";

	gotoxy(10, 5); cout << "Press any key to start";
	// any char will start the game
	getch();
	// to clear the press text to start the game
	gotoxy(10, 5); cout << "                      ";

	while (1) {
		//kbhit --> returns a nonzero value if a key has been pressed. Otherwise, it returns 0.
		if (kbhit()) {
			char ch = getch();
			// if the character is the space button
			if (ch == 32) {
				// if the position greater than 3 the bird will jump
				// so the position will decrease  by 3
				if (birdPos > 3)
					birdPos -= 3;
			}
			// if the character is the escape button
			if (ch == 27) {
				break;
			}
		}
		// to draw the bird in the new position after jumping
		drawBird();
		// to draw the Barrier shape
		// i draw two Barrier on the screen
		drawPipe(0);
		drawPipe(1);
		//debug();
		if (collision() == 1) {
			// if the bird hit the barriers --> end and print game over
			auto stop = high_resolution_clock::now();// calculate the time which taken before collision
			auto duration = duration_cast<seconds>(stop - start);// store the difference in time in auto duration
			gameover(duration.count());// print time elappsed on screen
			return;
		}
		// The sleep () function causes the program or the process in which it is called, to suspend its execution temporarily for a period
		Sleep(100);  // in (Windows.h)
		eraseBird();   // erase the bird from the screen
		// erase the pipe from the screen
		erasePipe(0);
		erasePipe(1);
		// make bird move down
		birdPos += 1;

		if (birdPos > SCREEN_HEIGHT - 2) {
			// if the bird hits the boundary of the screen --> end game
			auto stop = high_resolution_clock::now();// calculate the time which taken before hit the boundary
			auto duration = duration_cast<seconds>(stop - start);// store the difference in time in duration
			gameover(duration.count());// print the time elappsed
			return;
		}
		// move the flag left on the screen
		if (getdata(pipeFlag,0) == 1)
			changevalue(pipePos,0,getdata(pipePos,0) + 2);

		if (getdata(pipeFlag,1) == 1)
			changevalue(pipePos,1,(getdata(pipePos,1) + 2));

		if (getdata(pipePos,0) >= 40 && getdata(pipePos,0) < 42) {
			changevalue(pipeFlag,1, 1);
			changevalue(pipePos,1, 4);
			// generate new pipe
			genPipe(1);
		}
		if (getdata(pipePos,0) > 68) {
            // if the pipe reach start of boundary
			// if the bird pass from the gap --> increase the score
			score++;
			updateScore();
			changevalue(pipeFlag,1,0) ;

			// make the second pipe to be the first pipe after passing from a gap
			changevalue(pipePos,0, getdata(pipePos,1));
			// make the second gap to be the first gap after passing from a gap
			changevalue(gapPos,0, getdata(gapPos,1));
		}

	}

}


int main(void)
{
    insertnode(&pipePos,1,0);
    insertnode(&pipePos,2,0);
    insertnode(&gapPos,1,0);
    insertnode(&gapPos,2,0);
    insertnode(&pipeFlag,1,1);
    insertnode(&pipeFlag,2,0);

    // at first we want to set cursor of the game
	setcursor(0, 0);

	srand((unsigned)time(NULL));

	/// THE GAME
	// print name of game and options to user
	//1.start 2. game's instruction 3. quit of game
	do {
		system("cls");
		gotoxy(10, 5); cout << " -------------------------- ";
		gotoxy(10, 6); cout << " |      Flappy Bird       | ";
		gotoxy(10, 7); cout << " --------------------------\n";
		gotoxy(10,9); cout<<"1. Start Game";
		gotoxy(10,10); cout<<"2. Instructions";
		gotoxy(10,11); cout<<"3. Quit";
		gotoxy(10, 13); cout << "Select option: ";
		// variable to get the user's decision
		char op = getche();
		// start to play
		if (op == '1') play();
		// print game's instruction
		else if (op == '2') instructions();
		// quit from game
		else if (op == '3') exit(0);

	} while (1);

	return 0;
}
