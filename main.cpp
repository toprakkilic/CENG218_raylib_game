#include <iostream>
#include <vector>
#include <raylib.h>
#include <cmath>  
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

int main() {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    int Dusman_Sayisi = 10;
    int Dusman_Speed = 2;
    float Bullet_Speed = 25.0;
    int MaxScore = 0;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Enemy Follow Player Example");
    SetTargetFPS(60);

    Player player(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50, 5, 5);  
    vector<Dusman> dusmanlar;  
    vector<Bullet> bullets; 

    for (int i = 0; i < Dusman_Sayisi; ++i) {
        dusmanlar.push_back(Dusman(SCREEN_WIDTH + (rand() % SCREEN_WIDTH / 2), SCREEN_HEIGHT + (rand() % SCREEN_HEIGHT / 2), Dusman_Speed, Dusman_Speed, 15));
    }

    bool gameOver = false;
    int score = 0;  
    float timer = 0.0f;

    while (!WindowShouldClose()) {
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
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText(TextFormat("Max Score: %d", MaxScore), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 200, 20, BLACK);
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 30, RED);
            DrawText(TextFormat("Final Score: %d", score), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 40, 20, BLACK);
            DrawText("Press R to Replay", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 80, 20, BLACK);
            EndDrawing();

            if (IsKeyPressed(KEY_R)) {
                ResetGame(player, dusmanlar, bullets, score, gameOver, SCREEN_WIDTH , SCREEN_HEIGHT);  // Oyun sıfırlanır
            }

            continue; 
        }

        player.Update(SCREEN_WIDTH, SCREEN_HEIGHT);

        for (auto& dusman : dusmanlar) {
            dusman.Update(player.Player_X, player.Player_Y, SCREEN_WIDTH, SCREEN_HEIGHT);
        }

        if (dusmanlar.size() < Dusman_Sayisi) {
            dusmanlar.push_back(Dusman(SCREEN_WIDTH + (rand() % SCREEN_WIDTH / 2), SCREEN_HEIGHT + (rand() % SCREEN_HEIGHT / 2), Dusman_Speed, Dusman_Speed, 15));
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
