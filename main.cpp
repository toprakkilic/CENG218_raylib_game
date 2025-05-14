#include <iostream>
#include <vector>
#include <raylib.h>
#include <cmath>  
#include <chrono> 
#include <ctime>

using namespace std;

// --- Ses dosyaları için global değişkenler ---
Music bgMusic;
Sound bulletSound;
Sound menuSound;
Sound enemyHitSound;
Sound gameoverSound;


class Player {
public:
    Texture2D textureRight;
    Texture2D textureLeft;
    Texture2D textureIdleRight;
    Texture2D textureIdleLeft;

    int currentFrame;
    float timer;
    float frameSpeedIdle = 0.2f;// Idle animasyon hızı
    int frameCount;
    bool isMoving = false;
    bool isIdle;
    bool facingRight;
    int idleCount;

    int Player_X;
    int Player_Y;
    int Player_Speed_X;
    int Player_Speed_Y;
    int Player_Radius;
    bool isBoosted = true;
    float boostTimer = 5.0f;

    Player(int startX, int startY, int startSpeedX, int startSpeedY,const char* tRight,const char* tLeft
    , const char* tIdleRight, const char* tIdleLeft) 
        : Player_X(startX), Player_Y(startY), Player_Speed_X(startSpeedX), Player_Speed_Y(startSpeedY), Player_Radius(64), 
        timer(0.0f), currentFrame(0), frameCount(8), isMoving(false), facingRight(true), isIdle(true), idleCount(0),textureRight(LoadTexture(tRight)), 
        textureLeft(LoadTexture(tLeft)), textureIdleRight(LoadTexture(tIdleRight)), textureIdleLeft(LoadTexture(tIdleLeft)){}

    void Update(int SCREEN_WIDTH, int SCREEN_HEIGHT) {
        checkBoosted();
        float moveX = 0;
        float moveY = 0;

        if (IsKeyDown(KEY_W)) {
            moveY = -1;
            isMoving = true;
        }

        if (IsKeyDown(KEY_S)) {
            moveY = 1;
            isMoving = true;
        }
        
        if (IsKeyDown(KEY_D)) {
            moveX = 1;
            isMoving = true;
            SetFacingRight(true);
        }
        
        if (IsKeyDown(KEY_A)) {
            moveX = -1;
            isMoving = true;
            SetFacingRight(false);
        }


        if (moveX != 0 && moveY != 0) {
            moveX *= 0.7071f;
            moveY *= 0.7071f;
        }

        Player_X += moveX * Player_Speed_X;
        Player_Y += moveY * Player_Speed_Y;

        if (Player_X - Player_Radius < 0) Player_X = Player_Radius;
        if (Player_X + Player_Radius > SCREEN_WIDTH) Player_X = SCREEN_WIDTH - Player_Radius;
        if (Player_Y - Player_Radius < 0) Player_Y = Player_Radius;
        if (Player_Y + Player_Radius > SCREEN_HEIGHT) Player_Y = SCREEN_HEIGHT - Player_Radius;

        if (isMoving) {
            isIdle = false;
            idleCount = 0;

            frameCount = 4;
            timer += GetFrameTime();
            if (timer >= frameSpeedIdle) {
                timer = 0.0f;
                currentFrame = (currentFrame + 1) % frameCount;

            }
            isMoving = false;
        } else {
            
            isIdle = true;
            frameCount = 4;
            timer += GetFrameTime();
            if (timer >= frameSpeedIdle) {
                timer = 0.0f;
                currentFrame = (currentFrame + 1) % frameCount;
                if (currentFrame == 0) {
                    idleCount++;
                }
            }    
        }
    }

    void SetFacingRight(bool facing) {
        facingRight = facing;
    }

    void Draw() {
        //DrawCircle(Player_X, Player_Y, Player_Radius, BLUE);
        int frameToDraw = currentFrame % frameCount;

        if (isIdle) {
            DrawTexturePro(facingRight ? textureIdleRight : textureIdleLeft,
                { (float)(frameToDraw * 16), 0, 16, 16 },
                { (float)(Player_X), (float)(Player_Y), 128, 128 },
                { 0, 0 }, 0, WHITE);
        } else {
            DrawTexturePro(facingRight ? textureRight : textureLeft,
                { (float)(frameToDraw * 16), 0, 16, 16 },
                { (float)(Player_X), (float)(Player_Y), 128, 128 },
                { 0, 0 }, 0, WHITE);
        }
    }

    void checkBoosted() {
        if (isBoosted) {
            boostTimer -= GetFrameTime();
            if (boostTimer <= 0) {
                isBoosted = false;
            }
        }
    }

    void Unload() {
        UnloadTexture(textureRight);
        UnloadTexture(textureLeft);
        UnloadTexture(textureIdleRight);
        UnloadTexture(textureIdleLeft);
    }

};

class Dusman {
public:
    Texture2D textureRight;
    Texture2D textureLeft;

    
    int currentFrame;
    int Dusman_X;
    int Dusman_Y;
    float Dusman_Speed_X;
    float Dusman_Speed_Y;
    int Dusman_Radius;
    bool alive = true;
    float timer;
    float timer2;
    int frameCount;
    bool facingRight;


    Dusman(int startX, int startY, float startSpeedX, float startSpeedY, int ballRadius, const char* tRight, const char* tLeft) 
        : Dusman_X(startX), Dusman_Y(startY), Dusman_Speed_X(startSpeedX),facingRight(true), Dusman_Speed_Y(startSpeedY), Dusman_Radius(ballRadius),
        timer2(0.0f), currentFrame(0), frameCount(8), textureRight(LoadTexture(tRight)), textureLeft(LoadTexture(tLeft)){}

    void Update(int player_x, int player_y, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
        int deltaX = player_x - Dusman_X;
        int deltaY = player_y - Dusman_Y;

        float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
        timer += GetFrameTime();

        if (timer >= 0.2f) {
            Dusman_Speed_X *= 1.03f;
            Dusman_Speed_Y *= 1.03f;
            timer = 0;
        }

        if (distance > 0) {
            float speedX = (deltaX / distance) * Dusman_Speed_X;
            float speedY = (deltaY / distance) * Dusman_Speed_Y;
            Dusman_X += speedX;
            Dusman_Y += speedY;
        }

        frameCount = 4;
        timer2 += GetFrameTime();
        if (timer2 >= 0.2f)
        {
            timer2 = 0.0f;
            currentFrame = (currentFrame + 1) % frameCount;
        }
    }

    void Draw() {
        if (!alive) return;
        int frameToDraw = currentFrame % frameCount;
        if (Dusman_Speed_X > 0 )
        {           
            DrawTexturePro(textureRight ,
            { (float)(frameToDraw * 16), 0, 16, 16 },
            { (float)(Dusman_X), (float)(Dusman_Y), 128, 128 },
            { 0, 0 }, 0, WHITE);
        }else{
            DrawTexturePro(textureLeft ,
            { (float)(frameToDraw * 16), 0, 16, 16 },
            { (float)(Dusman_X), (float)(Dusman_Y), 128, 128 },
            { 0, 0 }, 0, WHITE);
        }
        
    }

    bool CheckCollision(Player& player) {
        Vector2 enemyCenter = { (float)Dusman_X, (float)Dusman_Y };
        Vector2 playerCenter = { (float)player.Player_X, (float)player.Player_Y };
        return CheckCollisionCircles(playerCenter, player.Player_Radius, enemyCenter, Dusman_Radius);
    }

    void Unload() {
        UnloadTexture(textureRight);
        UnloadTexture(textureLeft);
    }
};

class Bullet {
public:
    float x, y;
    float speedX, speedY;
    int radius;
    Texture2D texture;

    Bullet(float startX, float startY, float targetX, float targetY, float bulletSpeed, int bulletRadius,Texture2D textureImg)
        : x(startX), y(startY), radius(bulletRadius),texture(textureImg) {
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
        DrawTexture(texture, x, y, WHITE);
    }

    bool CheckCollisionDusman(Dusman& dusman) {
        Vector2 enemyCenter = { (float)dusman.Dusman_X, (float)dusman.Dusman_Y };
        Vector2 bulletCenter = { (float)x, (float)y };
        bool isColliding = CheckCollisionCircles(bulletCenter, radius, enemyCenter, dusman.Dusman_Radius);
        
        if (isColliding) {
            PlaySound(enemyHitSound);  // Düşman vurulduğunda ses çalacak
        }
        
        return isColliding;
    }
};

void dusmanEkle(vector<Dusman>& dusmanlar, int dusmanSayisi, int SCREEN_WIDTH, int SCREEN_HEIGHT, const char* tRight, const char* tLeft){
    for (int i = 0; i < dusmanSayisi; ++i) {
        int rnd_field = 1 + rand() % 8;
        switch (rnd_field) {
            case 1:
                dusmanlar.push_back(Dusman(-rand() % 100, -rand() % 100, 3, 3, 15, tRight, tLeft));
                break;
            case 2:
                dusmanlar.push_back(Dusman(rand() % SCREEN_WIDTH, -rand() % 100, 3, 3, 15, tRight, tLeft));
                break;
            case 3:
                dusmanlar.push_back(Dusman(-rand() % 100, rand() % SCREEN_HEIGHT, 3, 3, 15, tRight, tLeft));
                break;
            case 4:
                dusmanlar.push_back(Dusman(-rand() % 100, SCREEN_HEIGHT + rand() % 100, 3, 3, 15, tRight, tLeft));
                break;
            case 5:
                dusmanlar.push_back(Dusman(SCREEN_WIDTH + rand() % 100, SCREEN_HEIGHT + rand() % 100, 3, 3, 15, tRight, tLeft));
                break;
            case 6:
                dusmanlar.push_back(Dusman(rand() % SCREEN_WIDTH, SCREEN_HEIGHT + rand() % 100, 3, 3, 15, tRight, tLeft));
                break;
            case 7:
                dusmanlar.push_back(Dusman(SCREEN_WIDTH + rand() % 100, rand() % SCREEN_HEIGHT, 3, 3, 15, tRight, tLeft));
                break;
            default:
                dusmanlar.push_back(Dusman(SCREEN_WIDTH + rand() % 100, SCREEN_HEIGHT + rand() % 100, 3, 3, 15, tRight, tLeft));
                break;
        }
    }
}

void ShowMainMenu(bool& gameStarted) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("MAIN MENU", 300, 100, 30, BLACK);
        Rectangle startButton = { 270, 180, 260, 50 };
        DrawRectangleRec(startButton, BLUE);
        DrawText("Start", 280, 195, 20, WHITE);

        if (CheckCollisionPointRec(GetMousePosition(), startButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            PlaySound(menuSound);
            gameStarted = true;
            break;
        }

        EndDrawing();
    }
}

void ShowEndMenu(bool& status, int score, int maxScore) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Game Ended", 300, 100, 30, BLACK);
        Rectangle restartButton = { 270, 180, 260, 50 };
        Rectangle exitButton = { 270, 250, 260, 50 };
        DrawText(TextFormat("Max Score: %d", maxScore), 280, 425, 20, BLACK);
        DrawText(TextFormat(" Score: %d", score), 280, 335, 20, BLACK);
        DrawRectangleRec(restartButton, BLUE);
        DrawText("restart", 280, 195, 20, WHITE);
        DrawRectangleRec(exitButton, RED);
        DrawText("Exit", 280, 265, 20, WHITE);

        if (CheckCollisionPointRec(GetMousePosition(), restartButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            PlaySound(menuSound);
            status = true;
            break;
        }

        if (CheckCollisionPointRec(GetMousePosition(), exitButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            PlaySound(menuSound);
            break;
        }

        EndDrawing();
    }
}

void ResetGame(Player& player, vector<Dusman>& dusmanlar, vector<Bullet>& bullets, int& score, bool& gameOver, int Screen_Width, int Screen_Height) {
    player.Player_X = Screen_Width / 2;
    player.Player_Y = Screen_Height / 2;
    score = 0;
    gameOver = false;
    player.isBoosted = true;
    player.boostTimer = 5.0f;

    dusmanlar.clear();

    bullets.clear(); 
}

int main() {
    const int SCREEN_WIDTH = 1920;
    const int SCREEN_HEIGHT = 1080;
    int Dusman_Sayisi = 2;
    float Bullet_Speed = 25.0f;
    int MaxScore = 0;
    float shootTimer = 0.0f;
    float shootCooldown = 0.1f;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bullet Hell Game");
    InitAudioDevice();
    ToggleFullscreen();

    const char* rightFile = "assets/chicken_run_right-Sheet.png";
    const char* leftFile = "assets/chicken_run_left-Sheet.png";
    const char* idleRightFile = "assets/chicken_idle_right-Sheet.png";
    const char* idleLeftFile = "assets/chicken_idle_left-Sheet.png";
    Texture2D eggTexture = LoadTexture("assets/egg.png");

    bgMusic = LoadMusicStream("resources/backgroundMusic.ogg");
    bulletSound = LoadSound("resources/bulletFireSound.mp3");
    enemyHitSound = LoadSound("resources/enemyHitSound.mp3");
    gameoverSound = LoadSound("resources/gameoverSound.mp3");
    menuSound = LoadSound("resources/menuSound.mp3");



    PlayMusicStream(bgMusic);
    SetMusicVolume(bgMusic, 0.4f);
    SetSoundVolume(bulletSound, 0.8f);
    SetSoundVolume(menuSound, 0.6f);

    SetTargetFPS(60);
    srand(time(nullptr));
    Player player(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 5, 5, rightFile, leftFile, idleRightFile, idleLeftFile);
    vector<Dusman> dusmanlar;
    vector<Bullet> bullets;

    bool gameStarted = false;
    ShowMainMenu(gameStarted);

    bool gameOver = false;
    int score = 0;
    float timer = 0.0f;
    auto counter_start_time = chrono::high_resolution_clock::now();

    while (!WindowShouldClose() && gameStarted) {
        UpdateMusicStream(bgMusic);
        Vector2 mousePos = GetMousePosition();

        if (!gameOver) {
            timer += GetFrameTime();
            if (timer >= 1.0f) {
                score++;
                timer = 0.0f;
            }
        }

        if (gameOver) {
            if (score > MaxScore) MaxScore = score;
            bool status = false;
            ShowEndMenu(status, score, MaxScore);
            if (status) {
                ResetGame(player, dusmanlar, bullets, score, gameOver, SCREEN_WIDTH, SCREEN_HEIGHT);
            } else {
                break;
            }
        }

        player.Update(SCREEN_WIDTH, SCREEN_HEIGHT);
        for (auto& d : dusmanlar) d.Update(player.Player_X, player.Player_Y, SCREEN_WIDTH, SCREEN_HEIGHT);

        auto counter_time = chrono::high_resolution_clock::now();
        chrono::duration<double> duration_sec = counter_time - counter_start_time;
        int enemy_count = (duration_sec.count() / 15) * log(duration_sec.count() / 15);
        if (dusmanlar.size() < Dusman_Sayisi || dusmanlar.size() < enemy_count) {
            dusmanEkle(dusmanlar, Dusman_Sayisi, SCREEN_WIDTH, SCREEN_HEIGHT, rightFile, leftFile);
        }

        if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || player.isBoosted) && shootTimer >= shootCooldown) {
            shootTimer = 0.0f;
            bullets.push_back(Bullet(player.Player_X, player.Player_Y, mousePos.x, mousePos.y, Bullet_Speed, 32, eggTexture));
            PlaySound(bulletSound);
        }
        shootTimer += GetFrameTime();

        for (auto& b : bullets) b.Update();

        for (auto it = bullets.begin(); it != bullets.end(); ) {
            bool hit = false;
            for (auto& d : dusmanlar) {
                if (it->CheckCollisionDusman(d)) {
                    d.alive = false;
                    d.Unload();
                    score += 10;
                    hit = true;
                    break;
                }
            }
            if (hit || it->x < 0 || it->x > SCREEN_WIDTH || it->y < 0 || it->y > SCREEN_HEIGHT) {
                it = bullets.erase(it);
            } else {
                ++it;
            }
        }

        for (auto it = dusmanlar.begin(); it != dusmanlar.end(); ) {
            if (it->CheckCollision(player)) {
                PlaySound(gameoverSound); // Oyuncu öldüğünde sesi çal
                gameOver = true;
            }

            if (!it->alive) {
                it = dusmanlar.erase(it);
                it->Unload();
            } else {
                ++it;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        player.Draw();
        for (auto& d : dusmanlar) d.Draw();
        for (auto& b : bullets) b.Draw();
        if (!gameOver) DrawText(TextFormat("Score: %d", score), 10, 10, 20, BLACK);
        EndDrawing();
    }

    player.Unload();
    for (auto it = dusmanlar.begin(); it != dusmanlar.end(); ) {
        it->Unload();
    }
    UnloadTexture(eggTexture);
    UnloadMusicStream(bgMusic);
    UnloadSound(bulletSound);
    UnloadSound(menuSound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
