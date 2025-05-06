// Tank_game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>

using namespace std;
using namespace sf;
using namespace sfp;

void LoadTex(Texture& tex, string filename) {
    if (!tex.loadFromFile(filename)) {
        cout << "Could not load " << filename << endl;
    }
}

int main()
{
    RenderWindow window(VideoMode(1000, 1000), "Tank Assualt");
    Texture worldTex;
    LoadTex(worldTex, "images/tank_mat.png");
    Sprite worldSprite;
    worldSprite.setTexture(worldTex);
    World world(Vector2f(0, 0));
    int score(0);
    PhysicsSprite& usertank = *new PhysicsSprite();
    Texture ftankTex;
    LoadTex(ftankTex, "images/user_tank.png");
    usertank.setTexture(ftankTex);
    Vector2f sz = usertank.getSize();
    usertank.setCenter(Vector2f(500, 500));

    PhysicsSprite shell;
    Texture shellTex;
    LoadTex(shellTex, "images/tank_shell.png");
    shell.setTexture(shellTex);
    bool drawingshell(false);

    PhysicsRectangle top;
    top.setSize(Vector2f(1000, 10));
    top.setCenter(Vector2f(500, 5));
    top.setStatic(true);
    world.AddPhysicsBody(top);

    PhysicsRectangle bottom;
    bottom.setSize(Vector2f(5, 1000));
    bottom.setCenter(Vector2f(5, 500));
    top.setStatic(true);
    world.AddPhysicsBody(bottom);

    PhysicsRectangle left;
    left.setSize(Vector2f(10, 1000));
    left.setCenter(Vector2f(5, 500));
    left.setStatic(true);
    world.AddPhysicsBody(left);

    PhysicsRectangle right;
    right.setSize(Vector2f(10, 1000));
    right.setCenter(Vector2f(1000, 500));
    right.setStatic(true);
    world.AddPhysicsBody(right);



}
