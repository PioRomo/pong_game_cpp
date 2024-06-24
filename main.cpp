#include <iostream>
#include <raylib.h> 

using namespace std; 
const int screenWidth = 1280; 
const int screenHeight = 800; 
const int targetFPS = 60; 
int playerScore; 
int cpuScore; 

class Ball{

    public: 
        int speedX, speedY; 
        float x, y; 
        int radius; 

        void Draw(){
            DrawCircle(x, y, radius, WHITE); 
        }

        void Update(){
            x += speedX; 
            y += speedY; 

            if(y + radius >= screenHeight || y-radius <= 0){
                speedY *= -1; 
            }
            if(x + radius >= screenWidth){//player scores
                playerScore++; 
                ResetBall(); 
            }

            if(x-radius <= 0){//cpu scores
                cpuScore++; 
                ResetBall(); 
            }
        }

        void ResetBall(){
            x = GetScreenWidth() /2; 
            y = GetScreenHeight() / 2; 

            int speedChoices[2] = {-1, 1}; 
            speedX *= speedChoices[GetRandomValue(0,1)];
            speedY *= speedChoices[GetRandomValue(0,1)];

        }

};

class Paddle{
    protected: 
        void LimitMovement(){
            if(y <= 0){
                y = 0; 
            }

            if(y + padHeight >= screenHeight){
                y = screenHeight - padHeight; 
            }
        } 

    public: 
        float x,y; 
        float padWidth, padHeight; 
        int speed; 

        void Draw(){
            DrawRectangle(x, y, padWidth, padHeight, WHITE); 
        }

        void Update(){
            if(IsKeyDown(KEY_UP)){
                y = y - speed; 
            }
            if(IsKeyDown(KEY_DOWN)){
                y = y + speed; 
            }

            LimitMovement(); 
        }

};

class CPUPaddle: public Paddle{

    public: 
        void Update(int ballY){
            if(y + padHeight/2 > ballY){
                y = y - speed; 
            }
            if(y + padHeight/2 <= ballY){
                y = y + speed; 
            }
            LimitMovement(); 
        }
};


Ball ball; 
Paddle player; 
CPUPaddle cpu; 

int main() {
    cout << "Starting the game" << endl;

    //Open the window for the game
    InitWindow(screenWidth, screenHeight, "My Pong Game" ); 
    SetTargetFPS(targetFPS); 

    //Ball vars
    ball.speedX = 7; 
    ball.speedY = 7; 
    ball.x = screenWidth / 2; 
    ball.y = screenHeight / 2; 
    ball.radius = 20; 

    //Player Paddle vars
    player.padWidth = 25; 
    player.padHeight = 120; 
    player.x = 10; 
    player.y = screenHeight/2 - 60; 
    player.speed = 6; 

    //CPU Paddle vars
    cpu.padHeight = 120; 
    cpu.padWidth = 25; 
    cpu.x = screenWidth - cpu.padWidth - 10; 
    cpu.y = screenHeight/2 - 60; 
    cpu.speed = 6; 


    while(!WindowShouldClose()){
        BeginDrawing(); 

        //Updating
        ball.Update(); 
        player.Update(); 
        cpu.Update(ball.y);

        //Checking for collisions
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.padWidth, player.padHeight})){
            ball.speedX *= -1; 
        }
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.padWidth, cpu.padHeight})){
            ball.speedX *= -1; 
        }


        //Drawing
        ClearBackground(BLACK); 
        ball.Draw(); 
        player.Draw(); 
        cpu.Draw(); 
        DrawLine(screenWidth/2, 0, screenWidth/2, screenHeight ,WHITE); 
        DrawText(TextFormat("%i", playerScore), screenWidth/4 -20, 20, 80, WHITE);
        DrawText(TextFormat("%i", cpuScore), 3 *screenWidth/4 -20, 20, 80, WHITE);

        EndDrawing(); 
    }

    //Close game window
    CloseWindow(); 
    return 0; 
}
