#include "raylib.h"

int main()
{
    int canvas_height{380
    };
    int canvas_width{512};

    const int width = 50;
    const int lenght = 80;
    int posy = canvas_height -lenght;

    int velocity = 0;

    InitWindow(canvas_width,canvas_height,"hyperDash");
    SetTargetFPS(60);

    

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
      posy += velocity;
        if(IsKeyPressed(KEY_SPACE))
        {
            velocity += -10 ;
        }
      

        DrawRectangle(canvas_width/2,posy,width,lenght,BLUE);

        EndDrawing();
    }
    CloseWindow();
}
