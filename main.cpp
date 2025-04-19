#include <iostream>
#include <vector>
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

// Mermi sınıfı
class Bullet {
public:
    float x, y;
    float speedX, speedY;
    int radius;

    // Parametreli yapıcı
    Bullet(float startX, float startY, float targetX, float targetY, float bulletSpeed, int bulletRadius)
        : x(startX), y(startY), radius(bulletRadius) {

        // Mermiyi mouse'un olduğu yere doğru yönlendir
        float deltaX = targetX - startX;
        float deltaY = targetY - startY;

        // Yönü normalize et
        float length = sqrt(deltaX * deltaX + deltaY * deltaY);
        speedX = (deltaX / length) * bulletSpeed;  // Hız vektörünü normalize et
        speedY = (deltaY / length) * bulletSpeed;  // Hız vektörünü normalize et
    }

    void Update() {
        // Mermi hedefe doğru hareket eder
        x += speedX;
        y += speedY;
    }

    void Draw() {
        DrawCircle(x, y, radius, RED);  // Mermiyi kırmızı çizer
    }
};

int main() {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bullet System Example");
    SetTargetFPS(60);

    Ball ball(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50, 5, 5, 15);  // Oyuncu topu
    vector<Bullet> bullets;  // Dinamik dizi ile mermiler burada saklanacak

    while (!WindawShouldClose()) {
        Vector2 mousePos = GetMousePosition();  // Mouse pozisyonunu al

        // Topu güncelle
        ball.Update(SCREEN_WIDTH, SCREEN_HEIGHT);

        // Mermi ateşleme (Mouse sol tuşu ile)
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Mouse tuşuna basıldığında, topun biraz üstünde bir mermi ateşle
            bullets.push_back(Bullet(ball.ball_x, ball.ball_y - ball.ball_radius, mousePos.x, mousePos.y, 25.0f, 55));
        }

        // Mermileri güncelle
        for (auto& bullet : bullets) {
            bullet.Update();
        }

        // Ekranı çizme işlemi
        BeginDrawing();
        ClearBackground(RAYWHITE);

        ball.Draw();  // Topu çiz

        // Ateşlenen mermileri çiz
        for (auto& bullet : bullets) {
            bullet.Draw();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
