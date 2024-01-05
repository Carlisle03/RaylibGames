#include <iostream>
#include "raylib.h"

using namespace std;

// Global Variables
int player_score = 0;
int cpu_score = 0;

// The Ball thing
class Ball
{
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void draw()
    {
        DrawCircle(x, y, radius, WHITE);
    }

    void update()
    {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
        }
        if (x + radius >= GetScreenWidth()) //CPU WINS)
        {
            cpu_score++;
            ResetBall();
        }
        
        if(x - radius <= 0)
        {
            player_score++;
            ResetBall();
        }
    }  

    void ResetBall()
    {
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int speed_choices[2] = {-1,1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }  
};

class Paddle
{
protected:
void LimitMove()
{
    if (y <= 0)
    {
        y = 0;
    }
    if (y + height >= GetScreenHeight())
    {
        y = GetScreenHeight() - height;
    }
}
public:
    float x, y;
    float width, height;
    int speed;

    void draw()
    {
        DrawRectangle(x, y, width, height, WHITE);
    }
    void update(){
        if(IsKeyDown(KEY_UP))
        {
            y = y - speed;
        }
        if(IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        }
        LimitMove();
    }
};

class CpuPaddle : public Paddle
{
public:
    void update(int ball_y)
    {
        
        if (y + height < ball_y)
        {
            y = y + speed;
        }
        else if (y > ball_y)
        {
            y = y - speed;
        }

        LimitMove();
    }
};

CpuPaddle cpaddle;
Ball ball;
Paddle player;

int main()
{
    cout << "Starting raylib 5.0\n";
    const int width = 900;
    const int height = 700;
    InitWindow(width, height, "Pong");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = width / 2;
    ball.y = height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 120;
    player.x = width - player.width - 10;
    player.y = height / 2 - player.height / 2;
    player.speed = 6;

    cpaddle.width = 25;
    cpaddle.height = 120;
    cpaddle.x = 10;
    cpaddle.y = height / 2 - cpaddle.height / 2;
    cpaddle.speed = 6;

    while (!WindowShouldClose())
    {
        // Update
        ball.update();
        player.update();
        cpaddle.update(ball.y);

        // Collisions
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
        {
            ball.speed_x *= -1;
        }
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpaddle.x, cpaddle.y, cpaddle.width, cpaddle.height}))
        {
            ball.speed_x *= -1;
        }
        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        ball.draw();
        cpaddle.draw();
        player.draw();
        DrawLine(width / 2, 0, width / 2, height, WHITE);
        DrawText(TextFormat("%i", cpu_score), width/4 -20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3*width/4 -20, 20, 80, WHITE);
        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
