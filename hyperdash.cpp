#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    // update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        // update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{
    // array with window dimensions
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    // initialize the window
    InitWindow(windowDimensions[0], windowDimensions[1], "hyperDash!");

    // acceleration due to gravity (pixels/s)/s
    const int gravity{1'000};

    // nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfNebulae{3};
    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 0.0;

        nebulae[i].pos.x =  windowDimensions[0] + i * 300;
    }

    float finishLine{ nebulae[sizeOfNebulae - 1].pos.x };

    // nebula X velocity (pixels/second)
    int nebVel{-200};

    // xanro variables
    Texture2D xanro = LoadTexture("textures/scarfy.png");
    AnimData xanroData;
    xanroData.rec.width = xanro.width/6;
    xanroData.rec.height = xanro.height;
    xanroData.rec.x = 0;
    xanroData.rec.y = 0;
    xanroData.pos.x = windowDimensions[0]/2 - xanroData.rec.width/2;
    xanroData.pos.y = windowDimensions[1] - xanroData.rec.height;
    xanroData.frame = 0;
    xanroData.updateTime = 1.0/12.0;
    xanroData.runningTime = 0.0;

    // is the rectanlge in the air?
    bool isInAir{};
    // jump velocity (pixels/second)
    const int jumpVel{-600};

    int velocity{0};

    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};

    bool collision{};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // delta time (time since last frame)
        const float dT{GetFrameTime()};

        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // Scroll background
        bgX -= 20 * dT;
        if (bgX <= -background.width*2)
        {
            bgX = 0.0;
        }
        // Scroll the midground
        mgX -= 40 * dT;
        if (mgX <= -midground.width*2)
        {
            mgX = 0.0;
        }
        // Scroll the foreground
        fgX -= 80 * dT;
        if (fgX <= -foreground.width*2)
        {
            fgX = 0.0;
        }

        // draw the background
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        Vector2 bg2Pos{bgX + background.width*2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        // draw the midground
        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        Vector2 mg2Pos{mgX + midground.width*2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

        // draw the foreground
        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        Vector2 fg2Pos{fgX + foreground.width*2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

        // perform ground check
        if (isOnGround(xanroData, windowDimensions[1]))
        {
            // rectangle is on the ground
            velocity = 0;
            isInAir = false;
        }
        else
        {
            // rectangle is in the air
            velocity += gravity * dT;
            isInAir = true;
        }

        // jump check
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }


        for (int i = 0; i < sizeOfNebulae; i++)
        {
            // update the position of each nebula
            nebulae[i].pos.x += nebVel * dT;
        }

        // update finishLine
        finishLine += nebVel * dT;

        // upxanro position
        xanroData.pos.y += velocity * dT;

        // update xanro's animation frame
        if (!isInAir)
        {
            xanroData = updateAnimData(xanroData, dT, 5);
        }

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }

        for (AnimData nebula : nebulae)
        {
            float pad{50};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad
            };
            Rectangle xanroRec{
                xanroData.pos.x,
                xanroData.pos.y,
                xanroData.rec.width,
                xanroData.rec.height
            };
            if (CheckCollisionRecs(nebRec, xanroRec))
            {
                collision = true;
            }
        }

        if (collision)
        {
            // lose the game
            DrawText("Game Over!", windowDimensions[0]/4, windowDimensions[1]/2, 40, RED);
        }
        else if (xanroData.pos.x >= finishLine)
        {
            // win the game
            DrawText("You Win!", windowDimensions[0]/4, windowDimensions[1]/2, 40, RED);
        }
        else
        {
            for (int i = 0; i < sizeOfNebulae; i++)
            {
                // draw nebula
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }

            // draw xanro
            DrawTextureRec(xanro, xanroData.rec, xanroData.pos, WHITE);
        }

        // stop drawing
        EndDrawing();
    }
    UnloadTexture(xanro);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}