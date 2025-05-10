#include <iostream>
#include <vector>
#include <raylib.h>
#include <cmath>  
#include <chrono> 

using namespace std;

class Player {
public:
    int Player_X;
    int Player_Y;
    int Player_Speed_X;
    int Player_Speed_Y;
    int Player_Radius;
    bool isBoosted = true;
    float boostTimer = 5.0f;

    Player(int startX, int startY, int startSpeedX, int startSpeedY) 
        : Player_X(startX), Player_Y(startY), Player_Speed_X(startSpeedX), Player_Speed_Y(startSpeedY), Player_Radius(15) {}

void Update(int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    checkBoosted();
    // Hareket yönleri
    float moveX = 0;
    float moveY = 0;

    if (IsKeyDown(KEY_W)) {
        moveY = -1;  // Yukarı
    }

    if (IsKeyDown(KEY_S)) {
        moveY = 1;  // Aşağı
    }

    if (IsKeyDown(KEY_D)) {
        moveX = 1;  // Sağ
    }

    if (IsKeyDown(KEY_A)) {
        moveX = -1;  // Sol
    }

    // Eğer hem X hem de Y yönü aktifse, hızları normalize et
    if (moveX != 0 && moveY != 0) {
        moveX *= 0.7071f;  // Yönü normalize etmek için çarpan (1/√2)
        moveY *= 0.7071f;  // Yönü normalize etmek için çarpan (1/√2)
    }

    // Yeni pozisyonu hesapla
    Player_X += moveX * Player_Speed_X;
    Player_Y += moveY * Player_Speed_Y;

    // Ekran sınırlarını kontrol et
    if (Player_X - Player_Radius < 0) {
        Player_X = Player_Radius;  // Sol sınır
    }
    if (Player_X + Player_Radius > SCREEN_WIDTH) {
        Player_X = SCREEN_WIDTH - Player_Radius;  // Sağ sınır
    }
    if (Player_Y - Player_Radius < 0) {
        Player_Y = Player_Radius;  // Üst sınır
    }
    if (Player_Y + Player_Radius > SCREEN_HEIGHT) {
        Player_Y = SCREEN_HEIGHT - Player_Radius;  // Alt sınır
    }
}

    
    void Draw() {
        DrawCircle(Player_X, Player_Y, Player_Radius, BLUE);
    }
    void checkBoosted() {
     if (isBoosted) {
         boostTimer -= GetFrameTime();
         if (boostTimer <= 0) {
            isBoosted = false;  // Boost bitince, boost'u kapat
            }
        }
    }
    
    
};

class Dusman {
public:
    int Dusman_X;
    int Dusman_Y;
    float Dusman_Speed_X;
    float Dusman_Speed_Y;
    int Dusman_Radius;
    bool alive = true;
    float timer = 0;
    
    Dusman(int startX, int startY, float startSpeedX, float startSpeedY, int ballRadius) 
        : Dusman_X(startX), Dusman_Y(startY), Dusman_Speed_X(startSpeedX), Dusman_Speed_Y(startSpeedY), Dusman_Radius(ballRadius) {}

void Update(int player_x, int player_y, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    int deltaX = player_x - Dusman_X;
    int deltaY = player_y - Dusman_Y;

    float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
    
    // Timer'ı artır ve hız artışını kontrol et
    timer += GetFrameTime();
    if (timer >= 0.2f) {
        Dusman_Speed_X *= 1.03f;  // Hızı %20 arttır
        Dusman_Speed_Y *= 1.03f;
        timer = 0;  // Timer'ı sıfırla
    }

    // Eğer mesafe sıfırdan büyükse, düşman hareket etmeye başlasın
    if (distance > 0) {
        float speedX = (deltaX / distance) * Dusman_Speed_X;
        float speedY = (deltaY / distance) * Dusman_Speed_Y;

        Dusman_X += speedX;
        Dusman_Y += speedY;
    }
}



    void Draw() {
        if (!alive) { return; }
        DrawCircle(Dusman_X, Dusman_Y, Dusman_Radius, RED);
    }

    bool CheckCollision(Player& player) {
        Vector2 enemyCenter = { (float)Dusman_X, (float)Dusman_Y };
        Vector2 playerCenter = { (float)player.Player_X, (float)player.Player_Y };
        return CheckCollisionCircles(playerCenter, player.Player_Radius, enemyCenter, Dusman_Radius);
    }
};

// Mermi sınıfı
class Bullet {
public:
    float x, y;
    float speedX, speedY;
    int radius;

    Bullet(float startX, float startY, float targetX, float targetY, float bulletSpeed, int bulletRadius)
        : x(startX), y(startY), radius(bulletRadius) {

        float deltaX = targetX - startX;
        float deltaY = targetY - startY;

        float length = sqrt(deltaX * deltaX + deltaY * deltaY);
        speedX = (deltaX / length) * bulletSpeed;  
        speedY = (deltaY / length) * bulletSpeed;  
    }

    void Update() {
        x += speedX;
        y += speedY;
    }

    void Draw() {
        DrawCircle(x, y, radius, RED);  
    }

    bool CheckCollisionDusman(Dusman& dusman) {
        Vector2 enemyCenter = { (float)dusman.Dusman_X, (float)dusman.Dusman_Y };
        Vector2 bulletCenter = { (float)x, (float)y };
        return CheckCollisionCircles(bulletCenter, radius, enemyCenter, dusman.Dusman_Radius);
    }
};

class Boost {
public:
    float x, y;
    int radius;
    bool active;

    Boost(float startX, float startY, int boostRadius) 
        : x(startX), y(startY), radius(boostRadius), active(true) {}

    void Draw() {
        if (active) {
            DrawCircle(x, y, radius, GREEN);  // Boost yeşil renkte olacak
        }
    }

    // Boost'un oyuncu ile çarpışıp çarpışmadığını kontrol et
    bool CheckCollision(Player& player) {
        Vector2 playerCenter = { (float)player.Player_X, (float)player.Player_Y };
        Vector2 boostCenter = { (float)x, (float)y };
        return CheckCollisionCircles(playerCenter, player.Player_Radius, boostCenter, radius);
    }
};


void ResetGame(Player& player, vector<Dusman>& dusmanlar, vector<Bullet>& bullets, int& score, bool& gameOver, int Screen_Width, int Screen_Height) {
    player.Player_X = Screen_Width / 2;
    player.Player_Y = Screen_Height / 2;
    score = 0;
    gameOver = false;       

    player.isBoosted = true;    // Boost'u tekrar aktif et
    player.boostTimer = 5.0f;   // Boost süresini 5 saniye olarak başlat

    dusmanlar.clear();
    for (int i = 0; i < 10; ++i) {
        dusmanlar.push_back(Dusman(800 + (rand() % 400), 600 + (rand() % 300), 2, 2, 15));
    }

    bullets.clear(); 
}

void ShowMainMenu(bool& gameStarted) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Ana menü başlık
        DrawText("MAIN MENU", 300, 100, 30, BLACK);
        
        // Butonlar
        Rectangle startButton = { 270, 180, 260, 50 };  // Start butonunun alanı
        
        // Start butonunu çiz
        DrawRectangleRec(startButton, BLUE);
        DrawText("Start", 280, 195, 20, WHITE);
        
        // Exit butonunu çiz
        // Start butonuna tıklama kontrolü
        if (CheckCollisionPointRec(GetMousePosition(), startButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            gameStarted = true;
            break;  // Oyunu başlat
        }

        EndDrawing();
    }
}

void ShowEndMenu(bool& status, int score, int maxScore) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Ana menü başlık
        DrawText("Game Ended", 300, 100, 30, BLACK);
        
        // Butonlar
        Rectangle restartButton = { 270, 180, 260, 50 };  // Start butonunun alanı
        Rectangle exitButton = { 270, 250, 260, 50 };   // Exit butonunun alanı
        
        DrawText(TextFormat("Max Score: %d", maxScore),280,425, 20, BLACK);
        DrawText(TextFormat(" Score: %d", score),280,335, 20, BLACK);
        // Start butonunu çiz
        DrawRectangleRec(restartButton, BLUE);
        DrawText("restart", 280, 195, 20, WHITE);
        
        // Exit butonunu çiz
        DrawRectangleRec(exitButton, RED);
        DrawText("Exit", 280, 265, 20, WHITE);

        // Start butonuna tıklama kontrolü
        if (CheckCollisionPointRec(GetMousePosition(), restartButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            status = true;
            break;  // Oyunu başlat
        }

        // Exit butonuna tıklama kontrolü
        if (CheckCollisionPointRec(GetMousePosition(), exitButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            break;
        }

        EndDrawing();
    }
}

void dusmanEkle(vector<Dusman>& dusmanlar, int dusmanSayisi, int SCREEN_WIDTH, int SCREEN_HEIGHT){
        for (int i = 0; i < dusmanSayisi; ++i) {
        int rnd_field = 1 + rand() % 8;//Ekran dışındaki rastgele bir konum için ekranın dışı 8 parçaya bölündü
        switch (rnd_field)
        {
        case 1:
            dusmanlar.push_back(Dusman(0 - rand() % 100  , 0 - rand() % 100, 3, 3, 15));
            break;
        case 2:
            dusmanlar.push_back(Dusman(15 + rand() % SCREEN_WIDTH  , 0 - rand() % 100, 3, 3, 15));
            break;
        case 3:
            dusmanlar.push_back(Dusman(0 - rand() % 100  , 15 + rand() % SCREEN_HEIGHT, 3, 3, 15));
            break;
        case 4:
            dusmanlar.push_back(Dusman(0 - rand() % 100  , SCREEN_HEIGHT + rand() % 100, 3, 3, 15));
            break;
        case 5:
            dusmanlar.push_back(Dusman(SCREEN_WIDTH + rand() % 100  , SCREEN_HEIGHT + rand() % 100, 3, 3, 15));
            break;
        case 6:
            dusmanlar.push_back(Dusman(15 + rand() % SCREEN_WIDTH  , SCREEN_HEIGHT + rand() % 100, 3, 3, 15));
            break;
        case 7:
            dusmanlar.push_back(Dusman(SCREEN_WIDTH + rand() % 100  , 15 + rand() % SCREEN_HEIGHT, 3, 3, 15));
            break;
        default:
            dusmanlar.push_back(Dusman(SCREEN_WIDTH + rand() % 100  , SCREEN_HEIGHT + rand() % 100, 3, 3, 15));
            break;
        }
    }
}




// Global değişkenler (shootTimer ve shootCooldown)
float shootTimer = 0.0f;  // Zamanlayıcıyı başlatıyoruz
float shootCooldown = 0.1f;  // Her mermi atışında bekleme süresi (0.1 saniye)

int main() {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    int Dusman_Sayisi = 2;
    float Bullet_Speed = 25.0;
    int MaxScore = 0;
    InitAudioDevice();
    

    auto counter_start_time = std::chrono::high_resolution_clock::now();// Sayaç Başlangıcı
    srand(time(nullptr));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Enemy Follow Player Example");
    SetTargetFPS(60);
    Player player(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50, 5, 5);  
    vector<Dusman> dusmanlar;  
    vector<Bullet> bullets; 



    bool gameStarted = false;
    ShowMainMenu(gameStarted);

    bool gameOver = false;
    int score = 0;  
    float timer = 0.0f;

    while (!WindowShouldClose() && gameStarted) {
        Vector2 mousePos = GetMousePosition();  

        if (!gameOver) {
            timer += GetFrameTime();
            if (timer >= 1.0f) {
                score++;  
                timer = 0.0f;  
            }
        }

        if (gameOver) {
            if (score > MaxScore)
            {
                MaxScore = score;
            }
            bool status = 0;

            ShowEndMenu(status, score, MaxScore);

            if (status)
            {
                ResetGame(player, dusmanlar, bullets, score, gameOver, SCREEN_WIDTH , SCREEN_HEIGHT);  // Oyun sıfırlanır
            }else{
                goto label;
            }
        }

        player.Update(SCREEN_WIDTH, SCREEN_HEIGHT);

        for (auto& dusman : dusmanlar) {
            dusman.Update(player.Player_X, player.Player_Y, SCREEN_WIDTH, SCREEN_HEIGHT);
        }

        auto counter_time = std::chrono::high_resolution_clock::now();
        chrono::duration<double> duration_sec = counter_time-counter_start_time;
        int enemy_count =(duration_sec.count()/15)*log(duration_sec.count()/15) ;//t*log(t) tane düşman 
        
        if (dusmanlar.size() < Dusman_Sayisi || dusmanlar.size() < enemy_count) {
            dusmanEkle(dusmanlar, Dusman_Sayisi, SCREEN_WIDTH, SCREEN_HEIGHT);
        }

        // Mermiyi atma kısmı
        if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || player.isBoosted) && shootTimer >= shootCooldown) {
            shootTimer = 0.0f;  // Zamanlayıcıyı sıfırlıyoruz
            bullets.push_back(Bullet(player.Player_X, player.Player_Y - player.Player_Radius, mousePos.x, mousePos.y, Bullet_Speed, 5));
        }

        // Timer'ı güncelle
        shootTimer += GetFrameTime();

        for (auto& bullet : bullets) {
            bullet.Update();
        }

        //mermileri olusturup ekranin disindayken veya düsmana carpinca silen kod
        for (auto it = bullets.begin(); it != bullets.end(); ) {
            bool hit = false;
            for (auto& dusman : dusmanlar) {
                if (it->CheckCollisionDusman(dusman)) {
                    dusman.alive = false;
                    score += 10;
                    hit = true;
                    break;  
                }
            }
            if (hit) {
                it = bullets.erase(it);  
            }
            else if ((0 > (int)it->x) || ((int)it->x > GetScreenWidth()) || (0 > (int)it->y) ||  ((int)it->y > GetScreenHeight())) {
                it = bullets.erase(it);  
            }
             else {
                ++it;  
            }
        }


        //dusmanlari olusturan kod
        for (auto it = dusmanlar.begin(); it != dusmanlar.end(); ) {
            if (it->CheckCollision(player)) {
                gameOver = true;  
            }
            if (!it->alive) {
                it = dusmanlar.erase(it);  
            } else {
                ++it;  
            }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

        player.Draw();  
        for (auto& dusman : dusmanlar) {
            dusman.Draw();  
        }

        if (!gameOver) {
            DrawText(TextFormat("Score: %d", score), 10, 10, 20, BLACK);
        }

        for (auto& bullet : bullets) {
            bullet.Draw();
        }

        EndDrawing();
    }
    label:
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
