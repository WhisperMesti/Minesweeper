#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <cwchar>
#include <future>

using namespace std;

int board[20][20];
string boardUI[20][20];
int currentPlace[1][2];

//n>0 - in game but not shown
//n=0 - empty and not shown
//n=-1 - bomb
//n=-2 - empty but shown
//n=-3 - in game but shown

int xsize = 10;
int ysize = 10;
int bombs = 1;
int win = -1; //-1 - nothing, 0 - lose, 1 - win

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

bool uncover(int x, int y);
void showNear(int x, int y);
void space(int n=1, bool isEndl=true) {
    if (isEndl) for(int i=0;i<n;i++) {
        cout<<endl;
    }
    else for(int i=0;i<n;i++){
        cout<<" ";
    }
}
void gotoXY(int x, int y){
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console,CursorPosition);
}
bool printMenu() {
    //CONSOLE_FONT_INFOEX cfi;cfi.cbSize = sizeof(cfi);cfi.nFont = 0;cfi.dwFontSize.X = 0;Heightcfi.FontFamily = FF_DONTCARE;cfi.FontWeight = FW_NORMAL;std::wcscpy(cfi.FaceName, L"Consolas");setCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
    int menu_item=0, run;
    int spx=20, spy=7;
    int y=spy;
	bool running = true;
	gotoXY(spx-5,spy-6);for (int i=0; i<5 ;i++) {cout<<"--------";}
    gotoXY(spx+7,spy-4); cout << "~ MINESWEEPER ~"<<endl;
	gotoXY(spx-2,spy); cout << "->";
	gotoXY(spx-5,spy+3); for (int i=0; i<5 ;i++) {cout<<"--------";}
	for (int i=0; i<8 ;i++) {gotoXY(spx-6,spy-5+i); cout<<"|";}
	for (int i=0; i<8 ;i++) {gotoXY(spx+35,spy-5+i); cout<<"|";}
	while(running){
        gotoXY(spx,spy);  cout << "START";
		gotoXY(spx,spy+1); cout << "Quit Game";
		system("pause>nul");
		if(GetAsyncKeyState(VK_DOWN) && y != spy+1) //down button pressed
			{
				gotoXY(spx-2,y); cout << "  ";
				y++;
				gotoXY(spx-2,y); cout << "->";
				menu_item++;
				continue;

			}
		if(GetAsyncKeyState(VK_UP) && y != spy) //up button pressed
			{
				gotoXY(spx-2,y); cout << "  ";
				y--;
				gotoXY(spx-2,y); cout << "->";
				menu_item--;
				continue;
			}
		if(GetAsyncKeyState(VK_RETURN)){ // Enter key pressed
			switch(menu_item){
				case 0: {
					gotoXY(0,0);
					system("CLS");
					return true;
				}
				case 1: {
					running = false;
				}
			}
            running = false;
		}
	}
	gotoXY(0,15);
	return false;
}
void creatingBoard(){
    int randx, randy;

    for (int yy=0;yy<ysize;yy++){
        for (int xx=0;xx<xsize;xx++){
            boardUI[yy][xx]="[ _ ]";
            board[yy][xx]=0;
        }
    }

    for (int i=0;i<bombs;i++) {
        bool placed=false;
        while(!placed) {
            randx=rand()%xsize;
            randy=rand()%ysize;
            if (board[randy][randx]!=-1) {
                board[randy][randx]=-1;
                for (int dy=-1;dy<=1;dy++) {
                    for(int dx=-1;dx<=1;dx++) {
                        int nx = randx + dx;
                        int ny = randy + dy;
                        if (ny >= 0 && ny < ysize && nx >= 0 && nx < xsize && !(dy == 0 && dx == 0) && board[ny][nx] != -1) {
                            board[ny][nx] += 1;
                        }
                    }
                }
                placed=true;
            }
        }
    }
}
void printBoard() {
    for (int yy=0;yy<ysize;yy++){
        space(5,false);
        for (int xx=0;xx<xsize;xx++){
            cout<<boardUI[yy][xx];
        }
        cout<<endl;
    }
}
bool isWin() {
    win=1;
    for (int yy=0;yy<ysize;yy++){
        for (int xx=0;xx<xsize;xx++){
            if(board[yy][xx]>=0) {
                win=-1;
                return false;
            }
        }
    }
    return true;
}
void navigating() {
    int flags=bombs;
    int spx=7, spy=2; //first location
    int y=spy; //first position cursor
    int x=spx;
    currentPlace[0][0]=0; //current position coordinates
    currentPlace[0][1]=0;
    char c = boardUI[0][0][2];
	bool running = true;
	gotoXY(0,0); cout<<"Flags: "<<flags; gotoXY(x,y);
    gotoXY(10+4*xsize/2,0); cout << "~ MINESWEEPER ~"<<endl; //title
	gotoXY(x,y); cout << "+"; //cursor first position
	while(running){
		system("pause>nul");
		if(GetAsyncKeyState(VK_DOWN) && y != spy+ysize-1) //down button pressed
			{
				gotoXY(x,y); cout << c;
				y++;
				currentPlace[0][1]++;
				c= boardUI[currentPlace[0][1]][currentPlace[0][0]][2];
				gotoXY(x,y); cout << "+";
				continue;

			}
		if(GetAsyncKeyState(VK_UP) && y != spy) //up button pressed
			{
				gotoXY(x,y); cout << c;
				y--;
				currentPlace[0][1]--;
				c= boardUI[currentPlace[0][1]][currentPlace[0][0]][2];
				gotoXY(x,y); cout << "+";
				continue;
			}
		if(GetAsyncKeyState(VK_RIGHT) && x < xsize*5) //up button pressed
			{
				gotoXY(x,y); cout << c;
				x=x+5;
				currentPlace[0][0]++;
				c= boardUI[currentPlace[0][1]][currentPlace[0][0]][2];
				gotoXY(x,y); cout << "+";
				continue;
			}
        if(GetAsyncKeyState(VK_LEFT) && x != spx) //up button pressed
			{
				gotoXY(x,y); cout << c;
				x=x-5;
				currentPlace[0][0]--;
				c= boardUI[currentPlace[0][1]][currentPlace[0][0]][2];
				gotoXY(x,y); cout << "+";
				continue;
			}
		if(GetAsyncKeyState(VK_SPACE)){
			gotoXY(0,2);
			running = uncover(currentPlace[0][0],currentPlace[0][1]);
			printBoard();
            c= boardUI[currentPlace[0][1]][currentPlace[0][0]][2];
			gotoXY(x,y);
            if(running) running=!isWin();
			continue;
		}
		if(GetAsyncKeyState(VK_TAB)){
            gotoXY(0,2);
            if(boardUI[currentPlace[0][1]][currentPlace[0][0]]!="[ P ]" and board[currentPlace[0][1]][currentPlace[0][0]]>-2 and flags>0) {
                boardUI[currentPlace[0][1]][currentPlace[0][0]]="[ P ]";
                flags--;
            }
            else if(boardUI[currentPlace[0][1]][currentPlace[0][0]]=="[ P ]") {
                boardUI[currentPlace[0][1]][currentPlace[0][0]]="[ _ ]";
                flags++;
            }
            printBoard();
            c= boardUI[currentPlace[0][1]][currentPlace[0][0]][2];
            gotoXY(0,0); cout<<"Flags: "<<flags<<" "; gotoXY(x,y);
            gotoXY(x,y);
            continue;
		}
		if (GetAsyncKeyState(VK_ESCAPE)) // ESC key pressed
        {
            running = false; // Exit the loop
        }
	}
	gotoXY(0,0);
}
bool uncover(int x, int y) {
    if (board[y][x]==0) {
        showNear(x,y);
        return true;
    }
    else if (board[y][x]!=-1 and board[y][x]!=-2 and board[y][x]!=-3) {
        string number = to_string(board[y][x]);
        boardUI[y][x]="[ "+ number +" ]";
        board[y][x]=-3;
        return true;
    }
    else if (board[y][x]==-1) {
        win = 0;
        return false;
    }

}
void showNear(int x, int y) {
    if (board[y][x]==0) {
        boardUI[y][x]="[   ]";
        board[y][x]=-2;
        for (int dy=-1;dy<=1;dy++) {
            for(int dx=-1;dx<=1;dx++) {
                int nx = x + dx;
                int ny = y + dy;
                if (ny >= 0 && ny < ysize && nx >= 0 && nx < xsize && !(dy == 0 && dx == 0)) {
                    showNear(nx,ny);
                }
            }
        }
    }
    else if(board[y][x]!=-2 and board[y][x]!=-3) {
        string number = to_string(board[y][x]);
        boardUI[y][x]="[ "+ number +" ]";
        board[y][x]=-3;
    }
}
void showAll() {
    for (int yy=0;yy<ysize;yy++){
            for (int xx=0;xx<xsize;xx++){
                if (board[yy][xx]==0 or board[yy][xx]==-2) {
                    boardUI[yy][xx]="[   ]";
                }
                else if (board[yy][xx]==-1) {
                    boardUI[yy][xx]="[ X ]";
                }
                else if (board[yy][xx]!=-3){
                    string number = to_string(board[yy][xx]);
                    boardUI[yy][xx]="[ "+ number +" ]";
                }
            }
        }
    gotoXY(0,2);
    printBoard();
}
bool endgame() {
    string message;
    if(win == -1) {
        showAll();
        message = "----- GAME ENDED -----";
    }
    else if(win == 0) {
        showAll();
        message = "----- GAME OVER -----";
    }
    else {
        showAll();
        message = "----- YOU WON! -----";
    }
    space(1);
    for (int i=0; i<message.length() ;i++) {cout<<"---";}
    space(1);
    space(2*xsize,false);
    cout<<message;
    space(1);
    for (int i=0; i<message.length();i++) {cout<<"---";}
    space(3);
    int menu_item=0;
    int spx=20, spy=7+ysize;
    int y=spy;
	bool running = true;
	gotoXY(spx-2,spy); cout << "->";
	while(running){
        gotoXY(spx,spy);  cout << "Continue Game";
		gotoXY(spx,spy+1); cout << "Quit Game";
		system("pause>nul");
		if(GetAsyncKeyState(VK_DOWN) && y != spy+1) //down button pressed
			{
				gotoXY(spx-2,y); cout << "  ";
				y++;
				gotoXY(spx-2,y); cout << "->";
				menu_item++;
				continue;

			}
		if(GetAsyncKeyState(VK_UP) && y != spy) //up button pressed
			{
				gotoXY(spx-2,y); cout << "  ";
				y--;
				gotoXY(spx-2,y); cout << "->";
				menu_item--;
				continue;
			}
		if(GetAsyncKeyState(VK_RETURN)){ // Enter key pressed
			switch(menu_item){
				case 0: {
					gotoXY(0,0);
					system("CLS");
					running = false;
					return true;
				}
				case 1: {
					running = false;
				}
			}
		}
	}
	return false;
}
void settings() {
    bool approved = false;
    while(!approved) {
        system("CLS");
        gotoXY(0,0); cout<<"-----------------------------";
        gotoXY(3,2); cout<<"X-size: ";
        gotoXY(3,4); cout<<"Y-size: ";
        gotoXY(3,6); cout<<"Number of bombs: ";
        gotoXY(0,8); cout<<"-----------------------------"<<endl;
        gotoXY(11,2); cin>>xsize; gotoXY(11,2); cout<<xsize;
        gotoXY(11,4); cin>>ysize; gotoXY(11,4); cout<<ysize;
        gotoXY(20,6); cin>>bombs; gotoXY(20,6); cout<<bombs;
        if (xsize>0 and xsize<=20 and ysize>0 and ysize<=20 and bombs>0 and bombs<xsize*ysize) approved = true;
        else {gotoXY(0,10);cout<<"Wrong parameters! Enter again"<<endl; Sleep(2000);}
    }
}

int main()
{
bool playAgain = printMenu();
    if(playAgain) settings();
    system("CLS");
    while(playAgain) {
        playAgain = false;
        srand(time(NULL));
        creatingBoard();
        space(2);
        printBoard();
        navigating();
        system("CLS");
        gotoXY(5*xsize/2,0); cout << "~ MINESWEEPER ~"<<endl;
        playAgain = endgame();
    }
	return 0;
}


