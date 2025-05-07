// Tank_game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>

using namespace std;
using namespace sf;
using namespace sfp;

const float tank_speed = 0.4;  
float enemy_speed = 0.3;      
const float shell_speed = 0.8; 
float enemy_shell_speed = 0.5; 

// Difficulty settings
const int DIFFICULTY_INTERVAL = 500;  // Change how often difficulty increases
const float ENEMY_SPEED_INCREASE = 0.05;  // Change how much enemy speed increases
const float SHELL_SPEED_INCREASE = 0.1;   // Change how much shell speed increases
int difficulty_level = 1;  // Current difficulty level

void LoadTex(Texture& tex, string filename) {
    if (!tex.loadFromFile(filename)) {
        cout << "Could not load " << filename << endl;
    }
}

void Moveusertank(Sprite& usertank, int elapsedMS) {
    Vector2f newPos = usertank.getPosition();
    if (Keyboard::isKeyPressed(Keyboard::Right)) {
        newPos.x += tank_speed * elapsedMS;
    }
    if (Keyboard::isKeyPressed(Keyboard::Left)) {
        newPos.x -= tank_speed * elapsedMS;
    }
    usertank.setPosition(newPos);
}

// Function to update difficulty based on score
void updateDifficulty(int score) {
    int new_level = (score / DIFFICULTY_INTERVAL) + 1;
    if (new_level > difficulty_level) {
        difficulty_level = new_level;
        enemy_speed = 0.3 + (ENEMY_SPEED_INCREASE * (difficulty_level - 1));
        enemy_shell_speed = 0.5 + (SHELL_SPEED_INCREASE * (difficulty_level - 1));
        cout << "Difficulty increased to level " << difficulty_level << endl;
        cout << "Enemy speed: " << enemy_speed << ", Shell speed: " << enemy_shell_speed << endl;
    }
}

int main() {
    RenderWindow window(VideoMode(1000, 1000), "Tank Game");
    World world(Vector2f(0, 0));
    int score(0);
    int userlives(3);

    // Load textures
    Texture userTankTex, enemyTankTex, shellTex, enemyShellTex;
    LoadTex(userTankTex, "images/user_tank.png");
    LoadTex(enemyTankTex, "images/enemytank.png");
    LoadTex(shellTex, "images/tank_shell.png");
    LoadTex(enemyShellTex, "images/enemytank_shell.png");

    // Create user tank
    Sprite usertank;
    usertank.setTexture(userTankTex);
    usertank.setPosition(Vector2f(500, 800));

    // Create shell
    Sprite shell;
    shell.setTexture(shellTex);
    bool shellActive = false;

    // Create enemy tanks and their shells
    vector<Sprite> enemies;
    vector<Sprite> enemyShells;
    vector<float> enemyShootTimers;
    
    for (int i = 0; i < 5; i++) {
        Sprite enemy;
        enemy.setTexture(enemyTankTex);
        enemy.setPosition(Vector2f(50 + (800 / 5) * i, 50));
        enemies.push_back(enemy);
        enemyShootTimers.push_back(0);
    }

    // Text setup
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cout << "Couldn't load font" << endl;
        exit(1);
    }

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setPosition(10, 10);

    Text livesText;
    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setPosition(10, 40);

    Text difficultyText;
    difficultyText.setFont(font);
    difficultyText.setCharacterSize(24);
    difficultyText.setPosition(10, 70);

    Clock clock;
    while (userlives > 0) {
        Time deltaTime = clock.restart();
        float deltaMS = deltaTime.asMilliseconds();

        // Update difficulty based on score
        updateDifficulty(score);

        // Move user tank
        Moveusertank(usertank, deltaMS);

        // Move enemy tanks
        bool shouldReverse = false;
        for (auto& enemy : enemies) {
            Vector2f pos = enemy.getPosition();
            // Check if any enemy hits the boundary
            if (pos.x <= 50 || pos.x >= 950) {
                shouldReverse = true;
                break;
            }
        }

        // If any enemy hit the boundary, reverse direction for all enemies
        if (shouldReverse) {
            enemy_speed = -enemy_speed;
            // Move all enemies slightly away from the boundary
            for (auto& enemy : enemies) {
                Vector2f pos = enemy.getPosition();
                if (pos.x <= 50) {
                    enemy.setPosition(Vector2f(51, pos.y));
                } else if (pos.x >= 950) {
                    enemy.setPosition(Vector2f(949, pos.y));
                }
            }
        }

        // Move all enemies
        for (auto& enemy : enemies) {
            enemy.move(enemy_speed * deltaMS, 0);
        }

        // Handle enemy shooting
        for (size_t i = 0; i < enemies.size(); i++) {
            enemyShootTimers[i] += deltaMS;
            if (enemyShootTimers[i] > 2000) {
                Sprite enemyShell;
                enemyShell.setTexture(enemyShellTex);
                enemyShell.setPosition(enemies[i].getPosition() + 
                    Vector2f(enemies[i].getGlobalBounds().width / 2 - enemyShell.getGlobalBounds().width / 2, 
                            enemies[i].getGlobalBounds().height));
                enemyShells.push_back(enemyShell);
                enemyShootTimers[i] = 0;
            }
        }

        // Move enemy shells
        for (auto it = enemyShells.begin(); it != enemyShells.end();) {
            it->move(0, enemy_shell_speed * deltaMS);
            if (it->getPosition().y > 1000) {
                it = enemyShells.erase(it);
            } else {
                ++it;
            }
        }

        // Handle shell shooting
        if (Keyboard::isKeyPressed(Keyboard::Space) && !shellActive) {
            shellActive = true;
            shell.setPosition(usertank.getPosition() + 
                Vector2f(usertank.getGlobalBounds().width / 2 - shell.getGlobalBounds().width / 2, -20));
        }

        // Move shell
        if (shellActive) {
            shell.move(0, -shell_speed * deltaMS);
            if (shell.getPosition().y < 0) {
                shellActive = false;
            }
        }

        // Check collisions with enemy tanks
        if (shellActive) {
            for (auto it = enemies.begin(); it != enemies.end();) {
                if (shell.getGlobalBounds().intersects(it->getGlobalBounds())) {
                    it = enemies.erase(it);
                    shellActive = false;
                    score += 100;
                } else {
                    ++it;
                }
            }
        }

        if (enemies.empty()) {
            difficulty_level++;
            enemy_speed += ENEMY_SPEED_INCREASE;
            enemy_shell_speed += SHELL_SPEED_INCREASE;

            cout << "Difficulty increased to level " << difficulty_level << endl;
            cout << "New enemy speed: " << enemy_speed << ", Shell speed: " << enemy_shell_speed << endl;

            // Respawn logic
            int numEnemies = 5 + difficulty_level;
            float spacing = 800.0f / (numEnemies - 1);  // Calculate spacing between enemies
            
            for (int i = 0; i < numEnemies; i++) {
                Sprite enemy;
                enemy.setTexture(enemyTankTex);
                enemy.setPosition(Vector2f(50 + (spacing * i), 50));
                enemies.push_back(enemy);
                enemyShootTimers.push_back(0);
            }
        }

        // Check collisions with enemy shells
        for (auto it = enemyShells.begin(); it != enemyShells.end();) {
            if (it->getGlobalBounds().intersects(usertank.getGlobalBounds())) {
                it = enemyShells.erase(it);
                userlives--;
            } else {
                ++it;
            }
        }

        // Update text
        scoreText.setString("Score: " + to_string(score));
        livesText.setString("Lives: " + to_string(userlives));
        difficultyText.setString("Level: " + to_string(difficulty_level));

        // Check user lives
        if (userlives <= 0) {
            break;
        }

        // Draw everything
        window.clear();
        window.draw(usertank);
        if (shellActive) {
            window.draw(shell);
        }
        for (auto& enemy : enemies) {
            window.draw(enemy);
        }
        for (auto& enemyShell : enemyShells) {
            window.draw(enemyShell);
        }
        window.draw(scoreText);
        window.draw(livesText);
        window.draw(difficultyText);
        window.display();
        
    }


    // Game over screen
    window.clear();
    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(50);
    gameOverText.setPosition(400, 400);
    window.draw(gameOverText);
    window.display();

    // Pause on the Game Over screen
    sf::sleep(sf::seconds(3));

    return 0;
}
