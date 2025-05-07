

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>

using namespace std;
using namespace sf;
using namespace sfp;

const float tank_speed = 0.2;
const float shellSpeed = 0.5f;
const float enemy_speed = 0.1;
const float enemyshellSpeed = 0.1;
bool movingRight = true;

void LoadTex(Texture& tex, string filename) {
    if (!tex.loadFromFile(filename)) {
        cout << "Could not load " << filename << endl;
    }
}

void MoveEnemyTank(PhysicsSprite& enemy, int elaspedMS) {
    Vector2f newPos = enemy.getCenter();

    // side to side movement for enemy tank
    if (movingRight) {
        newPos.x += enemy_speed * elaspedMS;
        if (newPos.x >= 950) {
            movingRight = false;
        }
    }
    else {
        newPos.x -= enemy_speed * elaspedMS;
        if (newPos.x <= 50) {
            movingRight = true;
        }
    }

    enemy.setCenter(newPos);
}

//      Enemy tank firing method
void EnemyShoot(PhysicsSprite& enemy, PhysicsSprite& shell, World& world) {
    shell.getCenter(enemy.getCenter());
    shell.setVelocity(Vector2f(0, enemyshellSpeed));
    world.AddPhysicsBody(shell);
}

//    Enemy tank spawn logic
void SpawnEnemyTanks(PhysicsShapeList<PhysicsSprite>& enemys, Texture& redTex, World& world) {
    for (int i = 0; i < 6; i++) {
        PhysicsSprite& enemy = enemys.Create();
        enemy.setTexture(redTex);
        enemy.setCenter(Vector2f(100 + (150 * i), 50));
        world.AddPhysicsBody(enemy);

        enemy.onCollision = [&world, &enemy, &enemys]
        (PhysicsBodyCollisionResult result) {
            if (result.object2 == shell) {
                world.RemovePhysicsBody(enemy);
                enemys.QueueRemove(enemy);
            }
            };
    }
}