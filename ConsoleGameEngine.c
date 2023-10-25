#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

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

//object struct
typedef struct
{
    float X, Y; 
    float Width, Height;
    float vX, vY;
    int Color;

}object;

//set display
int display[40][30]={};

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

//simple seed based RNG functionality
float rng_seed=0.45673;
float rng(float lb, float ub)
{
    float result;
    result = rng_seed * 3.73 * (1-rng_seed);
    rng_seed = result;

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

//render display array
char render()
{
    system("cls");
    for(int rY=0 ; rY<30 ; rY++)
    {
        printf("\n");
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
            display[rX][rY] = 0;
        }
    }
    return 0;
}

// GAME                //
// Flappy Bird Example //

int main(void)
{

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

    //Frame Update
    while(1)
    {
        if(input() == 'w'){player.vY = -4;}
        player.vY += 1;
        if(player.vY > 3){player.vY = 3;}
        player = update_obj(player);

        uppipe.X -= 2;
        downpipe.X -= 2;

        if(uppipe.X < -2)
        {
            uppipe.X = 40;
            downpipe.X = 40;

            uppipe.Height = (float)(int)rng(0, 15);
            downpipe.Y = uppipe.Height + 15;
        }

        if(colide_obj(player, uppipe)){return 0;}
        if(colide_obj(player, downpipe)){return 0;}
        if(player.Y < -2){return 0;}
        if(player.Y > 30){return 0;}

        display_obj(player);
        display_obj(uppipe);
        display_obj(downpipe);
        render();
    }
    return 0;
}