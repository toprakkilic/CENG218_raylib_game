#include <iostream>
#include <raylib.h>

using namespace std;

class Ball {
public:
    int ball_x;
    int ball_y;
    int ball_speed_x;
    int ball_speed_y;
    int ball_radius;

    Ball(int startX, int startY, int startSpeedX, int startSpeedY, int ballRadius) 
        : ball_x(startX), ball_y(startY), ball_speed_x(startSpeedX), ball_speed_y(startSpeedY), ball_radius(ballRadius) {}

    void Update(int SCREEN_WIDTH, int SCREEN_HEIGHT) {
        if (IsKeyDown(KEY_W)) {
            if (ball_y - ball_radius > 0) {
                ball_y -= ball_speed_y;
            }
        }
       
        if (IsKeyDown(KEY_S)) {
            if (ball_y + ball_radius < SCREEN_HEIGHT) {
                ball_y += ball_speed_y;
            }
        }

       
        if (IsKeyDown(KEY_D)) {
            if (ball_x + ball_radius < SCREEN_WIDTH) { 
                ball_x += ball_speed_x;
            }
        }

        if (IsKeyDown(KEY_A)) {
            if (ball_x - ball_radius > 0) { 
                ball_x -= ball_speed_x;
            }
        }
    }

    void Draw() {
        DrawCircle(ball_x, ball_y, ball_radius, BLUE);
    }
};


class Dusman{


    public:
    int ball_x;
    int ball_y;
    int ball_speed_x;
    int ball_speed_y;
    int ball_radius;

    Dusman(int startX, int startY, int startSpeedX, int startSpeedY, int ballRadius) 
        : ball_x(startX), ball_y(startY), ball_speed_x(startSpeedX), ball_speed_y(startSpeedY), ball_radius(ballRadius) {}

    void Update(int SCREEN_WIDTH, int SCREEN_HEIGHT) {
        if (IsKeyDown(KEY_U)) {
            if (ball_y - ball_radius > 0) {
                ball_y -= ball_speed_y;
            }
        }
       
        if (IsKeyDown(KEY_J)) {
            if (ball_y + ball_radius < SCREEN_HEIGHT) {
                ball_y += ball_speed_y;
            }
        }

       
        if (IsKeyDown(KEY_K)) {
            if (ball_x + ball_radius < SCREEN_WIDTH) { 
                ball_x += ball_speed_x;
            }
        }

        if (IsKeyDown(KEY_H)) {
            if (ball_x - ball_radius > 0) { 
                ball_x -= ball_speed_x;
            }
        }
    }

    void Draw() {
        DrawCircle(ball_x, ball_y, ball_radius, RED);
    }



};


int main() {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    Ball ball(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 5, 5, 15);
    Dusman dusman(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 5, 5, 15);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My first RAYLIB program!");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        dusman.Update(SCREEN_WIDTH, SCREEN_HEIGHT);
        ball.Update(SCREEN_WIDTH, SCREEN_HEIGHT);

        BeginDrawing();
        ClearBackground(GRAY);
        ball.Draw();
        dusman.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
