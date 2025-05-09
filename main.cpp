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
        
        int deltaX = player_x - Dusman_X;
        int deltaY = player_y - Dusman_Y;

        float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
        if (distance > 0) {
            float speedX = (deltaX / distance) * Dusman_Speed_X ;
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
    bool ekrandisi = false;

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

void ResetGame(Player& player, vector<Dusman>& dusmanlar, vector<Bullet>& bullets, int& score, bool& gameOver,int Screen_Width, int Screen_Height) {
    player.Player_X = Screen_Width / 2;
    player.Player_Y = Screen_Height / 2;
    score = 0;
    gameOver = false;       

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
        Rectangle exitButton = { 270, 250, 260, 50 };   // Exit butonunun alanı
        
        // Start butonunu çiz
        DrawRectangleRec(startButton, BLUE);
        DrawText("Start", 280, 195, 20, WHITE);
        
        // Exit butonunu çiz
        DrawRectangleRec(exitButton, RED);
        DrawText("Exit", 280, 265, 20, WHITE);

        // Start butonuna tıklama kontrolü
        if (CheckCollisionPointRec(GetMousePosition(), startButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            gameStarted = true;
            break;  // Oyunu başlat
        }

        // Exit butonuna tıklama kontrolü
        if (CheckCollisionPointRec(GetMousePosition(), exitButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            CloseWindow(); // Oyunu kapat
            break;
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




int main() {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    int Dusman_Sayisi = 2;
    int Dusman_Speed = 2;
    float Bullet_Speed = 25.0;
    int MaxScore = 0;

    auto counter_start_time = std::chrono::high_resolution_clock::now();// Sayaç Başlangıcı
    srand(time(nullptr));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Enemy Follow Player Example");
    SetTargetFPS(60);
    Player player(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50, 5, 5);  
    vector<Dusman> dusmanlar;  
    vector<Bullet> bullets; 

    for (int i = 0; i < Dusman_Sayisi; ++i) {
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
                CloseWindow();
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
            int rnd_field = 1 + rand() % 8;
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

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            bullets.push_back(Bullet(player.Player_X, player.Player_Y - player.Player_Radius, mousePos.x, mousePos.y,Bullet_Speed, 5));
        }

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

    CloseWindow();
    return 0;
}