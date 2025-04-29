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

class Dusman {
public:
    int ball_x;
    int ball_y;
    int ball_speed_x;
    int ball_speed_y;
    int ball_radius;
    bool alive = true;

    Dusman(int startX, int startY, int startSpeedX, int startSpeedY, int ballRadius) 
        : ball_x(startX), ball_y(startY), ball_speed_x(startSpeedX), ball_speed_y(startSpeedY), ball_radius(ballRadius) {}

    void Update(int player_x, int player_y, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
        if (!alive)
        {
            return;
        }
        
        // Düşman, oyuncuyu kovalayacak
        int deltaX = player_x - ball_x;  // Yatay mesafe farkı
        int deltaY = player_y - ball_y;  // Dikey mesafe farkı

        // Yönü normalize et (yani hız vektörünü oluştur)
        float distance = sqrt(deltaX * deltaX + deltaY * deltaY);  // İki nokta arasındaki mesafe
        if (distance > 0) {  // Mesafe sıfır değilse, hareket et
            float speedX = (deltaX / distance) * ball_speed_x ;  // X yönünde hız
            float speedY = (deltaY / distance) * ball_speed_y;  // Y yönünde hız

            // Düşmanı bu hızlarla hareket ettir
            ball_x += speedX;
            ball_y += speedY;
        }
    }

    void Draw() {
        if (!alive)
        {
            return;
        }
        
        DrawCircle(ball_x, ball_y, ball_radius, RED);  // Düşmanı kırmızı çizer
    }

    bool CheckCollision(Ball& player) {
        // Raylib'in CheckCollisionCircles fonksiyonunu kullanarak çarpışmayı kontrol et
        Vector2 enemyCenter = { (float)ball_x, (float)ball_y };
        Vector2 playerCenter = { (float)player.ball_x, (float)player.ball_y };
        return CheckCollisionCircles(playerCenter, player.ball_radius, enemyCenter, ball_radius);  // Çarpışma kontrolü
    }

};

// Mermi sınıfı
class Bullet {
public:
    float x, y;
    float speedX, speedY;
    int radius;
    bool ekrandisi = false;

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
        if ((0 < (int)x) && ((int)x < GetScreenWidth()) && (0 < (int)y) && ((int)y < GetScreenHeight())) {
            DrawCircle(x, y, radius, RED);  // Mermiyi kırmızı çizer
        } else {
            ekrandisi = true;  // Ekranın dışında kalırsa, mermiyi çizme
        }
    }

    bool CheckCollisionDusman(Dusman& dusman) {
        // Raylib'in CheckCollisionCircles fonksiyonunu kullanarak çarpışmayı kontrol et
        Vector2 enemyCenter = { (float)dusman.ball_x, (float)dusman.ball_y };
        Vector2 bulletCenter = { (float)x, (float)y };
        return CheckCollisionCircles(bulletCenter,radius, enemyCenter, dusman.ball_radius);  // Çarpışma kontrolü
    }
};

int main() {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Enemy Follow Player Example");
    SetTargetFPS(60);

    Ball ball(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50, 5, 5, 15);  // Oyuncu topu
    vector<Dusman> dusmanlar;  // Düşmanlar vektörü
    vector<Bullet> bullets; // Mermiler

    // Düşmanları vektöre ekle
    for (int i = 0; i < 15; ++i) {
        dusmanlar.push_back(Dusman(rand() % SCREEN_WIDTH * 2 , rand() % SCREEN_HEIGHT * 2, 3, 3, 15));
    }

    bool gameOver = false;
    int score = 0;  // Skor başlangıçta sıfır
    float timer = 0.0f;  // Zamanlayıcı (her saniyede skor artacak)

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition(); // Mouse pozisyonu

        // Zamanı güncelle (her frame'de)
        if (!gameOver) {
            timer += GetFrameTime();  // Zamanlayıcıyı arttır

            // Her saniyede bir skoru artır
            if (timer >= 1.0f) {
                score++;  // Skoru bir artır
                timer = 0.0f;  // Zamanı sıfırla
            }
        }

        if (gameOver) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 30, RED);
            DrawText(TextFormat("Final Score: %d", score), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 40, 20, BLACK);
            EndDrawing();
            continue;  // Eğer oyun bitmişse, döngüye devam etme
        }

        // Oyuncu hareketi
        ball.Update(SCREEN_WIDTH, SCREEN_HEIGHT);

        // Düşman hareketi (oyuncuya doğru)
        for (auto& dusman : dusmanlar) {
            dusman.Update(ball.ball_x, ball.ball_y, SCREEN_WIDTH, SCREEN_HEIGHT);
        }

        if (dusmanlar.size() < 15)
        {
            dusmanlar.push_back(Dusman(rand() % SCREEN_WIDTH * 2 , rand() % SCREEN_HEIGHT * 2, 3, 3, 15));
        }
        

        // Mermi ateşleme (Mouse sol tuşu ile)
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Mouse tuşuna basıldığında, topun biraz üstünde bir mermi ateşle
            bullets.push_back(Bullet(ball.ball_x, ball.ball_y - ball.ball_radius, mousePos.x, mousePos.y, 25.0f, 5));
        }

        // Mermileri güncelle
        for (auto& bullet : bullets) {
            bullet.Update();
        }

        // Çarpışma kontrolü ve düşmanları vektörden silme
        for (auto it = bullets.begin(); it != bullets.end(); ) {
            bool hit = false;
            for (auto& dusman : dusmanlar) {
                if (it->CheckCollisionDusman(dusman)) {
                    dusman.alive = false;  // Düşmanı öldür
                    score += 10;
                    hit = true;
                    break;  // Çarpışma bulduktan sonra diğer düşmanlarla kontrol etmeye gerek yok
                }
            }
            if (hit) {
                it = bullets.erase(it);  // Çarpışan mermiyi vektörden sil
            } else {
                ++it;  // Mermi ekran içinde ve çarpışma yoksa, bir sonraki mermiye geç
            }
            
        }

        // Düşmanları vektörden silme (alive == false)
        for (auto it = dusmanlar.begin(); it != dusmanlar.end(); ) {
            if (it->CheckCollision(ball)) {
                gameOver = true;  // Çarpışma olduğunda oyun biter
            }
            if (!it->alive) {
                it = dusmanlar.erase(it);  // Düşmanı vektörden sil
            } else {
                ++it;  // Bir sonraki düşmana geç
            }
        }

        // Ekranı çizme işlemi
        BeginDrawing();
        ClearBackground(RAYWHITE);

        ball.Draw();  // Oyuncu topunu çiz
        for (auto& dusman : dusmanlar) {
            dusman.Draw();  // Düşmanları çiz
        }

        // Eğer oyun bitmemişse skoru göster
        if (!gameOver) {
            DrawText(TextFormat("Score: %d", score), 10, 10, 20, BLACK);
        }

        // Ateşlenen mermileri çiz
        for (auto& bullet : bullets) {
            bullet.Draw();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
