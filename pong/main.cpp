#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#define round(x) ((x)>=0?(float)((x)+0.5):(float)((x)-0.5))

const int MAP_SIZE = 40;
const int PLAYER_SIZE = 6;
const int SCORE_TOWIN = 5;

typedef int t_mapa[MAP_SIZE][MAP_SIZE];

enum t_modos {UP, DOWN, NONE};

struct t_player {
  int x;
  int y;
};

struct t_ball {
  int x;
  int y;
  int vx;
  int vy;
};

void win(int winner);
int mover(t_modos action, t_player player1, t_mapa mapa, t_ball ball);
void dibujar(t_mapa mapa, HANDLE hConsole, WORD saved_attributes, int contador, t_ball ball, int p1_score, int p2_score, int mode, bool menu);
bool bola(t_mapa mapa, t_ball &ball, t_player player1, t_player player2, int &contador, int &p1_score, int &p2_score);

int main(){

  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
  CONSOLE_CURSOR_INFO cinfo;
  WORD saved_attributes;

  GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
  saved_attributes = consoleInfo.wAttributes;

  _COORD coord;
  coord.X = MAP_SIZE;
  coord.Y = MAP_SIZE;

  _SMALL_RECT Rect;
  Rect.Top = 0;
  Rect.Left = 0;
  Rect.Bottom = MAP_SIZE+1;
  Rect.Right = (MAP_SIZE*2)+3;

  t_modos action;
  t_mapa mapa;
  t_player player1, player2;
  t_ball ball;

  int new_y;
  int vx, vy;
  int mode=2;
  int k = 1;
  int contador;
  int p1_score = 0;
  int p2_score = 0;
  int winner;

  bool p;
  bool menu=true;

  cinfo.dwSize = 100;
  cinfo.bVisible = false;

  SetConsoleCursorInfo(hConsole, &cinfo);

  ShowCursor(false);

  SetConsoleScreenBufferSize(hConsole, coord);
  SetConsoleWindowInfo(hConsole, TRUE, &Rect);

  punto_nuevo:

  system("cls");

  p=0;
  contador=0;

  srand(time(NULL));
  vx = rand() % 2;
  vy = rand() % 2;
  if (vx==0) {
    if (vy==0) {
      ball.vx = -1;
      ball.vy = -1;
    } else if (vy==1) {
      ball.vx = -1;
      ball.vy = 1;
    }
  } else if (vx==1){
    if (vy==0) {
      ball.vx = 1;
      ball.vy = -1;
    } else if (vy==1) {
      ball.vx = 1;
      ball.vy = 1;
    }
  }

  for (int i = 0; i < MAP_SIZE; i++) {
    for (int j = 0; j < MAP_SIZE; j++) {
      mapa[i][j] = 0;
    }
  }

  player1.x = 1;
  player2.x = MAP_SIZE-2;
  player1.y = (MAP_SIZE / 2) - (PLAYER_SIZE / 2);
  player2.y = (MAP_SIZE / 2) - (PLAYER_SIZE / 2);
  ball.x = MAP_SIZE / 2;
  ball.y = MAP_SIZE / 2;

  for (int i = 0; i < MAP_SIZE; i++) {
    for (int j = 0; j < MAP_SIZE; j++) {
      if (j==player1.x || j==player2.x) {
        if ((i >= player1.y || i >= player2.y) && (i <= player1.y+10 || i <= player2.y+10)) {
          mapa[i][j]=1;
        }
      }
      if (i==ball.x && j==ball.y) {
        mapa[i][j]=2;
      }
    }
  }

  dibujar(mapa, hConsole, saved_attributes, contador, ball, p1_score, p2_score, mode, menu);

  if (p1_score==SCORE_TOWIN) {
    winner=1;
    win(winner);
  } else if (p2_score==SCORE_TOWIN) {
    winner=2;
    win(winner);
  }

  if (mode != 2) {
    while (!GetAsyncKeyState(VK_SPACE)){
      continue;
    }
  } else {
    Sleep(2000);
  }


  while (true) {

    if (GetAsyncKeyState(VK_ESCAPE) && menu==false){
        system("pause");
    }

    dibujar(mapa, hConsole, saved_attributes, contador, ball, p1_score, p2_score, mode, menu);

    if (menu==true){
      if (GetAsyncKeyState('0')){
        mode=0;
        menu=false;
        goto punto_nuevo;
      } else if (GetAsyncKeyState('1')){
        mode=1;
        menu=false;
        goto punto_nuevo;
      } else if (GetAsyncKeyState('2')){
        mode=2;
        menu=false;
        goto punto_nuevo;
      }
    }

    p = bola(mapa, ball, player1, player2, contador, p1_score, p2_score);
    if (p==true) {goto punto_nuevo;}

    if (contador > 9) {p = bola(mapa, ball, player1, player2, contador, p1_score, p2_score);}
    if (p==true) {goto punto_nuevo;}
    if (contador > 19) {p = bola(mapa, ball, player1, player2, contador, p1_score, p2_score);}
    if (p==true) {goto punto_nuevo;}
    if (contador > 9) {k=2;}
    else if (contador > 19){k=4;}
    else {k=1;}

    if (mode==2){
      for (int i = 0; i < k; i++){
        if (mode==2){
          if (ball.x<player1.y+5){action=UP;} else if (ball.x>player1.y+5) {action=DOWN;}
        } else {
          if (GetAsyncKeyState(VK_UP)){
            action=UP;
          } else if (GetAsyncKeyState(VK_DOWN)){
            action=DOWN;
          }
        }
        new_y = mover(action, player1, mapa, ball);
        player1.y = new_y;
        action=NONE;
      }
    } else {
      if (mode==2){
        if (ball.x<player1.y+5){action=UP;} else if (ball.x>player1.y+5) {action=DOWN;}
      } else {
        if (GetAsyncKeyState(VK_UP)){
          action=UP;
        } else if (GetAsyncKeyState(VK_DOWN)){
          action=DOWN;
        }
      }
    }
    new_y = mover(action, player1, mapa, ball);
    player1.y = new_y;
    action=NONE;

    if (mode==1 || mode==2){
      for (int i = 0; i < k; i++){
        if (mode==1 || mode == 2){
          if (ball.x<player2.y+5){action=UP;} else if (ball.x>player2.y+5) {action=DOWN;}
        } else {
          if (GetAsyncKeyState('A')){
            action=UP;
          } else if (GetAsyncKeyState('Z')) {
            action=DOWN;
          } else if (GetAsyncKeyState(VK_ESCAPE)){
            system("pause");
          }
        }
        new_y = mover(action, player2, mapa, ball);
        player2.y = new_y;
        action=NONE;
      }
    } else {
      if (mode==1 || mode == 2){
        if (ball.x<player2.y+5){action=UP;} else if (ball.x>player2.y+5) {action=DOWN;}
      } else {
        if (GetAsyncKeyState('A')){
          action=UP;
        } else if (GetAsyncKeyState('Z')) {
          action=DOWN;
        } else if (GetAsyncKeyState(VK_ESCAPE)){
          system("pause");
        }
      }
      new_y = mover(action, player2, mapa, ball);
      player2.y = new_y;
      action=NONE;
    }
  }
}

int mover(t_modos action, t_player player, t_mapa mapa, t_ball ball) {
  DWORD        mode;
  HANDLE       hstdin;

  hstdin = GetStdHandle( STD_INPUT_HANDLE );

  GetConsoleMode( hstdin, &mode );
  SetConsoleMode( hstdin, 0 );
  if (action==UP) {
    if (player.y<1) {return player.y;}
    player.y--;
    for (int i = 0; i < MAP_SIZE; i++) {
      for (int j = 0; j < MAP_SIZE; j++) {
        if (j==player.x) {
          if (i == player.y) {
              mapa[i][j]=1;
          } else if (i == player.y+10){
              mapa[i+1][j]=0;
          }
        }
      }
    }
  } else if(action==DOWN) {
    if (player.y>MAP_SIZE-12) {return player.y;}
    player.y++;
    for (int i = 0; i < MAP_SIZE; i++) {
      for (int j = 0; j < MAP_SIZE; j++) {
        if (j==player.x) {
          if (i == player.y) {
              mapa[i-1][j]=0;
          } else if (i == player.y+10){
              mapa[i][j]=1;
          }
        }
      }
    }
  }
  SetConsoleMode(hstdin, mode);
  return player.y;
}

void dibujar(t_mapa mapa, HANDLE hConsole, WORD saved_attributes, int contador, t_ball ball, int p1_score, int p2_score, int mode, bool menu){
  char LOGO[] = "SIMPLE PONG v1.0";
  char CREDITOS[] = "BY TRETORN";
  char linea1[] = "SELECCION DE MODO:      ";
  char linea2[] = "0 ---> Humano vs Humano ";
  char linea3[] = "1 ---> Humano vs CPU    ";
  char linea4[] = "2 ---> CPU vs CPU (beta)";
  COORD coord;
  coord.X = 0;
  coord.Y = 1;
  int n=1;
  int k = 0;

  SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
  printf(" contador: %d cpu: %d dificultad: ", contador, mode);

  if (contador > 19) {SetConsoleTextAttribute(hConsole, saved_attributes);}
  else if (contador > 9) {SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN | BACKGROUND_BLUE);}
  else {SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);}
  printf("  ");
  SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
  printf("                                                  \n");
  SetConsoleTextAttribute(hConsole, saved_attributes);
  for (int i = 0; i < MAP_SIZE; i++) {
    n=1;
    for (int j = 0; j < MAP_SIZE; j++) {
      if (n==1) {
        SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
        printf("  ");
      }
      switch (mapa[i][j]) {
        case 0:
          SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
          break;
        case 1:
          SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE);
          break;
        case 2:
          SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
          break;
      }
      n=0;
      if (i==10 && j>=14 && j<=21 && menu == true) {
        printf("%1c%1c", LOGO[k], LOGO[k+1]);
        k = k +2 ;
      } else if (i==11 && j >=15 && j<=19 && menu == true) {
        printf("%1c%1c", CREDITOS[k], CREDITOS[k+1]);
        k = k + 2;
      } else if (i==15 && j>=14 && j<=22 && menu == true){
        printf("%1c%1c", linea1[k], linea1[k+1]);
        k = k + 2;
      } else if (i==16 && j>=14 && j<=25 && menu == true){
        printf("%1c%1c", linea2[k], linea2[k+1]);
        k = k + 2;
      } else if (i==17 && j>=14 && j<=25 && menu == true){
        printf("%1c%1c", linea3[k], linea3[k+1]);
        k = k + 2;
      } else if (i==18 && j>=14 && j<=25 && menu == true){
        printf("%1c%1c", linea4[k], linea4[k+1]);
        k = k + 2;
      } else if (i==MAP_SIZE / 2 - 10 && j==MAP_SIZE / 2 - 5 && menu == false) {
        printf("P1");
      } else if (i==MAP_SIZE / 2 - 10 && j==MAP_SIZE / 2 - 4 && menu == false) {
        printf(" %1d", p1_score);
      } else if (i==MAP_SIZE / 2 - 10 && j==MAP_SIZE / 2 + 4 && menu == false) {
        printf("P2");
      } else if (i==MAP_SIZE / 2 - 10 && j==MAP_SIZE / 2 + 5 && menu == false) {
        printf(" %1d", p2_score);
      } else {
        printf("  ");
      }
    }
    SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
    printf("  \n");
    k = 0;
  }
  SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
  printf("                                                                                    ");
  SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ), coord);
}

bool bola(t_mapa mapa, t_ball &ball, t_player player1, t_player player2, int &contador, int &p1_score, int &p2_score) {
  if (ball.y==0) {
    p2_score++;
    return 1;
  } else if (ball.y==MAP_SIZE-1) {
    p1_score++;
    return 1;
  }
  if (ball.x<1 || ball.x > MAP_SIZE-2){
    ball.vx = ball.vx * -1;
  }
  if (ball.y==2 || ball.y==MAP_SIZE-3){
    contador++;
    if ((ball.x>=player1.y && ball.x<=player1.y+3 && ball.y==2) || (ball.x>=player2.y && ball.x<=player2.y+3 && ball.y==MAP_SIZE-3)) {
      printf("\a");
      ball.vy = ball.vy * -1;
      ball.vx = ball.vx * -1;
    }
    if ((ball.x>player1.y+3 && ball.x<=player1.y+8 && ball.y==2) || (ball.x>player2.y+3 && ball.x<=player2.y+8 && ball.y==MAP_SIZE-3)) {
      printf("\a");
      ball.vy = ball.vy * -1;
    }
    if ((ball.x>player1.y+8 && ball.x<=player1.y+11 && ball.y==2) || (ball.x>player2.y+8 && ball.x<=player2.y+11 && ball.y==MAP_SIZE-3)) {
      printf("\a");
      ball.vy = ball.vy * -1;
      ball.vx = ball.vx * -1;
    }
  }
  mapa[ball.x][ball.y]=0;
  ball.x = ball.x+ball.vx;
  ball.y = ball.y+ball.vy;
  mapa[ball.x][ball.y]=2;
  return 0;
}

void win(int winner){
  char opt;
  COORD coord;
  coord.X = 37;
  coord.Y = 20;
  SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ), coord);
  SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
  printf("J%d WINS!!!\n", winner);
  coord.Y++;
  coord.X=coord.X-3;
  SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ), coord);
  fflush(stdin);
  printf("New match? (S/N) ");
  opt = getchar();
  switch (toupper(opt)) {
    case 'S':
      main();
      break;
    case 'N':
      exit(0);
      break;
    default:
      win(winner);
  }
}
