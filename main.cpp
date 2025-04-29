#include <iostream>
#include <vector>
#include <raylib.h>
#include <cmath>  // Mesafe hesaplama için


using namespace std;

class Player {
public:
    int Player_X;
    int Player_Y;
    int Player_Speed_X;
    int Player_Speed_Y;
    int Player_Radius;

    Player(int startX, int startY, int startSpeedX, int startSpeedY) 
        : Player_X(startX), Player_Y(startY), Player_Speed_X(startSpeedX), Player_Speed_Y(startSpeedY), Player_Radius(15) {}

    void Update(int SCREEN_WIDTH, int SCREEN_HEIGHT) {
        if (IsKeyDown(KEY_W)) {
            if (Player_Y - Player_Radius > 0) {
                Player_Y -= Player_Speed_Y;
            }
        }
       
        if (IsKeyDown(KEY_S)) {
            if (Player_Y + Player_Radius < SCREEN_HEIGHT) {
                Player_Y += Player_Speed_Y;
            }
        }

        if (IsKeyDown(KEY_D)) {
            if (Player_X + Player_Radius < SCREEN_WIDTH) { 
                Player_X += Player_Speed_X;
            }
        }

        if (IsKeyDown(KEY_A)) {
            if (Player_X - Player_Radius > 0) { 
                Player_X -= Player_Speed_X;
            }
        }
    }

    void Draw() {
        DrawCircle(Player_X, Player_Y, Player_Radius, BLUE);
    }
};

class Dusman {
public:
    int Dusman_X;
    int Dusman_Y;
    int Dusman_Speed_X;
    int Dusman_Speed_Y;
    int Dusman_Radius;
    bool alive = true;

    Dusman(int startX, int startY, int startSpeedX, int startSpeedY, int ballRadius) 
        : Dusman_X(startX), Dusman_Y(startY), Dusman_Speed_X(startSpeedX), Dusman_Speed_Y(startSpeedY), Dusman_Radius(ballRadius) {}

    void Update(int player_x, int player_y, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
        if (!alive)
        {
            return;
        }
        
        // Düşman, oyuncuyu kovalayacak
        int deltaX = player_x - Dusman_X;  // Yatay mesafe farkı
        int deltaY = player_y - Dusman_Y;  // Dikey mesafe farkı

        // Yönü normalize et (yani hız vektörünü oluştur)
        float distance = sqrt(deltaX * deltaX + deltaY * deltaY);  // İki nokta arasındaki mesafe
        if (distance > 0) {  // Mesafe sıfır değilse, hareket et
            float speedX = (deltaX / distance) * Dusman_Speed_X ;  // X yönünde hız
            float speedY = (deltaY / distance) * Dusman_Speed_Y;  // Y yönünde hız

            // Düşmanı bu hızlarla hareket ettir
            Dusman_X += speedX;
            Dusman_Y += speedY;
        }
    }

    void Draw() {
        if (!alive)
        {
            return;
        }
        
        DrawCircle(Dusman_X, Dusman_Y, Dusman_Radius, RED);  // Düşmanı kırmızı çizer
    }

    bool CheckCollision(Player& player) {
        // Raylib'in CheckCollisionCircles fonksiyonunu kullanarak çarpışmayı kontrol et
        Vector2 enemyCenter = { (float)Dusman_X, (float)Dusman_Y };
        Vector2 playerCenter = { (float)player.Player_X, (float)player.Player_Y };
        return CheckCollisionCircles(playerCenter, player.Player_Radius, enemyCenter, Dusman_Radius);  // Çarpışma kontrolü
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
        Vector2 enemyCenter = { (float)dusman.Dusman_X, (float)dusman.Dusman_Y };
        Vector2 bulletCenter = { (float)x, (float)y };
        return CheckCollisionCircles(bulletCenter,radius, enemyCenter, dusman.Dusman_Radius);  // Çarpışma kontrolü
    }
};

int main() {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    int Dusman_Sayisi = 10;
    int Dusman_Speed = 2;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Enemy Follow Player Example");
    SetTargetFPS(60);

    Player player(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50, 5, 5);  // Oyuncu
    vector<Dusman> dusmanlar;  //Dusmanlar
    vector<Bullet> bullets; // Mermiler

    for (int i = 0; i < Dusman_Sayisi; ++i) {
        dusmanlar.push_back(Dusman(SCREEN_WIDTH + (rand() % SCREEN_WIDTH / 2) , SCREEN_HEIGHT + (rand() % SCREEN_HEIGHT / 2), Dusman_Speed, Dusman_Speed, 15));
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
            continue; 
        }

        player.Update(SCREEN_WIDTH, SCREEN_HEIGHT);

        
        for (auto& dusman : dusmanlar) {
            dusman.Update(player.Player_X, player.Player_Y, SCREEN_WIDTH, SCREEN_HEIGHT);
        }

        if (dusmanlar.size() < Dusman_Sayisi)
        {
            dusmanlar.push_back(Dusman(SCREEN_WIDTH + (rand() % SCREEN_WIDTH / 2) , SCREEN_HEIGHT + (rand() % SCREEN_HEIGHT / 2), Dusman_Speed, Dusman_Speed, 15));
        }
        

        // Mermi ateşleme (Mouse sol tuşu ile)
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Mouse tuşuna basıldığında, topun biraz üstünde bir mermi ateşle
            bullets.push_back(Bullet(player.Player_X, player.Player_Y - player.Player_Radius, mousePos.x, mousePos.y, 25.0f, 5));
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
            if (it->CheckCollision(player)) {
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

        player.Draw();  // Oyuncu topunu çiz
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
