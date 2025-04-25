#include <iostream>
#include <raylib.h>
#include <cmath>  // Mesafe hesaplama için

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

class Dusman {
public:
    int ball_x;
    int ball_y;
    int ball_speed_x;
    int ball_speed_y;
    int ball_radius;

    Dusman(int startX, int startY, int startSpeedX, int startSpeedY, int ballRadius) 
        : ball_x(startX), ball_y(startY), ball_speed_x(startSpeedX), ball_speed_y(startSpeedY), ball_radius(ballRadius) {}

    void Update(int player_x, int player_y, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
        // Düşman, oyuncuyu kovalayacak
        int deltaX = player_x - ball_x;  // Yatay mesafe farkı
        int deltaY = player_y - ball_y;  // Dikey mesafe farkı

        // Yönü normalize et (yani hız vektörünü oluştur)
        float distance = sqrt(deltaX * deltaX + deltaY * deltaY);  // İki nokta arasındaki mesafe
        if (distance > 0) {  // Mesafe sıfır değilse, hareket et
            float speedX = (deltaX / distance) * ball_speed_x;  // X yönünde hız
            float speedY = (deltaY / distance) * ball_speed_y;  // Y yönünde hız

            // Düşmanı bu hızlarla hareket ettir
            ball_x += speedX;
            ball_y += speedY;
        }
    }

    void Draw() {
        DrawCircle(ball_x, ball_y, ball_radius, RED);  // Düşmanı kırmızı çizer
    }

    bool CheckCollision(Ball& player) {
        // Düşman ve oyuncu arasındaki mesafeyi hesapla
        float distance = sqrt(pow(ball_x - player.ball_x, 2) + pow(ball_y - player.ball_y, 2));

        // Çarpışma olup olmadığını kontrol et
        if (distance < (ball_radius + player.ball_radius)) {
            return true;  // Çarpışma varsa true döndür
        }
        return false;  // Çarpışma yoksa false döndür
    }
};

int main() {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Enemy Follow Player Example");
    SetTargetFPS(60);

    Ball ball(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50, 5, 5, 15);  // Oyuncu topu
    Dusman dusman(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 3, 3, 15);  // Düşman topu

    bool gameOver = false;

    while (!WindowShouldClose()) {
        if (gameOver) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 30, RED);
            EndDrawing();
            continue;  // Eğer oyun bitmişse, döngüye devam etme
        }

        // Oyuncu hareketi
        ball.Update(SCREEN_WIDTH, SCREEN_HEIGHT);

        // Düşman hareketi (oyuncuya doğru)
        dusman.Update(ball.ball_x, ball.ball_y, SCREEN_WIDTH, SCREEN_HEIGHT);

        // Çarpışma kontrolü (eğer mermi topa çarptıysa, "Game Over" yazdır)
        if (dusman.CheckCollision(ball)) {
            gameOver = true;  // Çarpışma olduğunda oyun biter
        }

        // Ekranı çizme işlemi
        BeginDrawing();
        ClearBackground(RAYWHITE);

        ball.Draw();  // Oyuncu topunu çiz
        dusman.Draw();  // Düşmanı çiz

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
