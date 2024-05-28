#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <windows.h>

#define BLACK   "\x1b[30m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1b[0m"

// ENGINE                                  //
// functions that control the entire thing //

//puts the screen cursor on top left
void rst_screen()
{
	COORD coord;
	coord.X = 0;
	coord.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//object struct
typedef struct
{
    float X, Y;
    float Width, Height;
    float vX, vY;
    int Color;

}object;

//sound struct
typedef struct 
{
    int frequency;
    int duration;
}sound;

//set display
int display[40][30]={};
int display_color = 0;

//calculate distance beetween two objects coordinates
float distance(object a, object b)
{
    float dist;
    dist= sqrt(((a.X-b.X)*(a.X-b.X)) + ((a.Y-b.Y)*(a.Y-b.Y)));
    return dist;
}

//detect if two objects colide
int colide_obj(object a, object b)
{
    if((a.X+a.Width) < b.X) {return 0;}
    if(a.X > (b.X+b.Width)) {return 0;}
    if(a.Y+a.Height < b.Y)  {return 0;}
    if(a.Y > (b.Y+b.Height)){return 0;}
    return 1;
}

//detect if a point colide with a object
int colide_point(float x, float y, object a)
{
    if(x>a.X && x<(a.X+a.Width) && y>a.Y && y<(a.Y+a.Height)){return 1;}
    return 0;
}

//simple seed based RNG functionality
float rng(float lb, float ub)
{
    float result = rand()%10000;
    result = result/10000;

    result = (result*(ub-lb)) + lb;

    return result;
}

//read real time input
char input()
{
    char in = 0;

    if(kbhit() == 1)
    {
        in = _getch();
    }

    return in;
}

//puts a object into the display array
int display_obj(object a)
{
    object pixel;
    pixel.X = 0;
    pixel.Y = 0;
    pixel.Width = 0;
    pixel.Height = 0;

    for(int rY=0 ; rY<30 ; rY++)
    {
        for(int rX=0 ; rX<40 ; rX++)
        {
            pixel.X = rX;
            pixel.Y = rY;

            if(colide_obj(pixel, a) == 1)
            {
                display[rX][rY]=a.Color;
            }
        }
    }

    return 0;
}

//apply velocity to objects
object update_obj(object a)
{
    a.X += a.vX;
    a.Y += a.vY;
    return a;
}

//show variables of especific object
void debug_obj(object a)
{
    printf("X=%.1f vX=%.1f\n", a.X, a.vX);
    printf("Y=%.1f vY=%.1f\n", a.Y, a.vY);
    printf("Width=%.1f\n", a.Width);
    printf("Height=%.1f\n", a.Height);
}

//render display array
char render()
{
    rst_screen();
    for(int rY=0 ; rY<30 ; rY++)
    {
        for(int rX=0 ; rX<40 ; rX++)
        {
            if     (display[rX][rY] == 0){printf(BLACK);}
            else if(display[rX][rY] == 1){printf(RED);}
            else if(display[rX][rY] == 2){printf(GREEN);}
            else if(display[rX][rY] == 3){printf(YELLOW);}
            else if(display[rX][rY] == 4){printf(BLUE);}
            else if(display[rX][rY] == 5){printf(MAGENTA);}
            else if(display[rX][rY] == 6){printf(CYAN);}
            else if(display[rX][rY] == 7){printf(WHITE);}

            printf("%c%c", 219, 219);
            display[rX][rY] = display_color;
        }
        printf("\n");
    }
    Sleep(1);
    return 0;
}

//play sound!
void play_sound(sound a)
{
    Beep(a.frequency, a.duration);
}


// GAME                //
// Flappy Bird Example //

int main(void)
{
    //Start System
    display_color = 0;
    system("cls");

    //Start Game Objects
    object player;
    player.X = 3;
    player.Y = 10;
    player.Width = 2;
    player.Height = 2;
    player.Color = 3;

    object uppipe;
    uppipe.X = 40;
    uppipe.Y = 0;
    uppipe.Width = 4;
    uppipe.Height = 2;
    uppipe.Color = 2;

    object downpipe;
    downpipe.X = 40;
    downpipe.Y = 25;
    downpipe.Width = 4;
    downpipe.Height = 30;
    downpipe.Color = 2;

    //create sounds
    sound flap;
    flap.frequency=200;
    flap.duration=5;

    sound scored;
    scored.frequency=2000;
    scored.duration=20;

    //Variables
    int score=0;

    //Start Menu
    printf(GREEN);
    printf("\n\nFLAPPY BIRD\n");
    printf("CGE Preview\n");
    printf(YELLOW);
    printf("\n\nType 'W' to start game:\n");
    while (input() != 'w'){}

    //Frame Update
    while(1)
    {
        if(input() == 'w')
        {
            player.vY = -4;
            play_sound(flap);
        }
        player.vY += 1;
        if(player.vY > 3){player.vY = 3;}
        player = update_obj(player);

        uppipe.X -= 4;
        downpipe.X -= 4;

        if(uppipe.X < -2)
        {
            uppipe.X = 40;
            downpipe.X = 40;

            uppipe.Height = (float)(int)rng(0, 15);
            downpipe.Y = uppipe.Height + 15;
            score++;
            play_sound(scored);
        }

        if(colide_obj(player, uppipe)){break;}
        if(colide_obj(player, downpipe)){break;}
        if(player.Y < -2){break;}
        if(player.Y > 30){break;}

        display_obj(uppipe);
        display_obj(downpipe);
        display_obj(player);
        render();
    }

    //Game End
    system("cls");
    printf(RED);
    printf("\n\nGAME OVER!\n\n");
    printf(GREEN);
    printf("Score:%i\n", score);
    printf(YELLOW);
    printf("\n\nType 'W' to close game:\n");
    while (input() != 'w'){}

    return 0;
}
