#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // calling srand() so that the random angle is indeed random when calling rand()
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    radians_right = 0; radians_left = 0; direction_l = -1; direction_r = 1;

    random_radians = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    // eliminating edge cases where the duck flies vertically or horizontally
    // also, if the angle is less than 30 degrees, the duck won't come out of the grass
    if (random_radians == 0 || random_radians == glm::radians(90.0f) ||
        random_radians < glm::radians(30.0f)) {
        random_radians = glm::radians(45.0f);
    }
    
    dir_x = 1; dir_y = 1;
    time_to_fly = 800;
    escape = 0; dead = 0; speed = 170;
    duck_length = 160;
    hit_zone_x = 90; hit_zone_y = -50; hit_zone_area_r = 150;

    // initializing the translation steps
    translateX = cos(random_radians);
    translateY = sin(random_radians);

    lives = 3; bullets = 3; score = 0; score_scale_factor = 0.2;
    count_ducks = 0;

    Meshes();
}


void Tema1::Meshes() {
    glm::vec3 corner = glm::vec3(0, 0, 0);
    glm::vec3 center = glm::vec3(0, 0, 0);
    float squareSide = 100;

    Mesh* earth = object2D::CreateSquare("earth", corner, squareSide, glm::vec3(0, 0.6, 0), true);
    AddMeshToList(earth);

    Mesh* beak = object2D::CreateTriangle("beak", corner, 20, glm::vec3(1, 0.5, 0), true);
    AddMeshToList(beak);

    Mesh* body = object2D::CreateTriangle("body", corner, 50, glm::vec3(0.260, 0.079, 0.064), true);
    AddMeshToList(body);

    Mesh* wing1 = object2D::CreateTriangle("wing1", corner, 40, glm::vec3(0.260, 0.079, 0.064), true);
    AddMeshToList(wing1);

    Mesh* wing2 = object2D::CreateTriangle("wing2", corner, 40, glm::vec3(0.260, 0.079, 0.064), true);
    AddMeshToList(wing2);

    Mesh* head = object2D::CreateCircle("head", center, glm::vec3(0.043, 0.260, 0.108), true);
    AddMeshToList(head);

    Mesh* life1 = object2D::CreateCircle("life1", center, glm::vec3(1, 0, 0), true);
    AddMeshToList(life1);

    Mesh* life2 = object2D::CreateCircle("life2", center, glm::vec3(1, 0, 0), true);
    AddMeshToList(life2);

    Mesh* life3 = object2D::CreateCircle("life3", center, glm::vec3(1, 0, 0), true);
    AddMeshToList(life3);

    Mesh* bullet1 = object2D::CreateSquare("bullet1", corner, squareSide, glm::vec3(0.175, 0.700, 0.267), true);
    AddMeshToList(bullet1);

    Mesh* bullet2 = object2D::CreateSquare("bullet2", corner, squareSide, glm::vec3(0.175, 0.700, 0.267), true);
    AddMeshToList(bullet2);

    Mesh* bullet3 = object2D::CreateSquare("bullet3", corner, squareSide, glm::vec3(0.175, 0.700, 0.267), true);
    AddMeshToList(bullet3);

    Mesh* score = object2D::CreateSquare("score", corner, squareSide, glm::vec3(0, 0, 1), true);
    AddMeshToList(score);

    Mesh* score_frame = object2D::CreateSquare("score_frame", corner, squareSide, glm::vec3(0, 0, 1));
    AddMeshToList(score_frame);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.520, 1.000, 1.000, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::DrawEarth() {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Scale(13, 2);
    RenderMesh2D(meshes["earth"], shaders["VertexColor"], modelMatrix);
}


void Tema1::DrawBeak() {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Rotate(random_radians);
    modelMatrix *= transform2D::Rotate(glm::radians(-90.0f));
    modelMatrix *= transform2D::Translate(59, 168);
    modelMatrix *= transform2D::Scale(0.9, 0.9);
    modelMatrix *= transform2D::Rotate(1.5707);
    RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix);
}


void Tema1::DrawHead() {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Rotate(random_radians);
    modelMatrix *= transform2D::Rotate(glm::radians(-90.0f));
    modelMatrix *= transform2D::Translate(50, 140);
    modelMatrix *= transform2D::Scale(30, 30);
    RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);
}


void Tema1::DrawBody() {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Rotate(random_radians);
    modelMatrix *= transform2D::Rotate(glm::radians(-90.0f));
    modelMatrix *= transform2D::Translate(100, 0);
    modelMatrix *= transform2D::Scale(2, 1.4);
    modelMatrix *= transform2D::Rotate(1.5707);
    RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);
}


void Tema1::DrawRightWing() {
    radians_right += 0.01f * direction_r;
    if (radians_right > 0.2) direction_r = -1;
    else if (radians_right < -0.5) direction_r = 1;

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Rotate(random_radians);
    modelMatrix *= transform2D::Rotate(glm::radians(-90.0f));
    modelMatrix *= transform2D::Translate(51, 43);
    modelMatrix *= transform2D::Scale(0.8, 1.5);
    modelMatrix *= transform2D::Rotate(radians_right);
    RenderMesh2D(meshes["wing1"], shaders["VertexColor"], modelMatrix);
}


void Tema1::DrawLeftWing() {
    radians_left += 0.01f * direction_l;
    if (radians_left > 0.5) direction_l = -1;
    else if (radians_left < -0.2) direction_l = 1;

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translateX, translateY);
    modelMatrix *= transform2D::Rotate(random_radians);
    modelMatrix *= transform2D::Rotate(glm::radians(-90.0f));
    modelMatrix *= transform2D::Translate(42, 100);
    modelMatrix *= transform2D::Scale(0.8, 1.5);
    modelMatrix *= transform2D::Rotate(3.1415);
    modelMatrix *= transform2D::Rotate(radians_left);
    RenderMesh2D(meshes["wing2"], shaders["VertexColor"], modelMatrix);
}


void Tema1::DrawFirstLife() {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(45, 680);
    modelMatrix *= transform2D::Scale(20, 20);
    RenderMesh2D(meshes["life1"], shaders["VertexColor"], modelMatrix);
}


void Tema1::DrawSecondLife() {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(105, 680);
    modelMatrix *= transform2D::Scale(20, 20);
    RenderMesh2D(meshes["life2"], shaders["VertexColor"], modelMatrix);
}


void Tema1::DrawThirdLife() {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(165, 680);
    modelMatrix *= transform2D::Scale(20, 20);
    RenderMesh2D(meshes["life3"], shaders["VertexColor"], modelMatrix);
}

void Tema1::DrawFirstBullet() {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(20, 620);
    modelMatrix *= transform2D::Scale(0.5, 0.25);
    RenderMesh2D(meshes["bullet1"], shaders["VertexColor"], modelMatrix);
}


void Tema1::DrawSecondBullet() {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(80, 620);
    modelMatrix *= transform2D::Scale(0.5, 0.25);
    RenderMesh2D(meshes["bullet2"], shaders["VertexColor"], modelMatrix);
}


void Tema1::DrawThirdBullet() {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(140, 620);
    modelMatrix *= transform2D::Scale(0.5, 0.25);
    RenderMesh2D(meshes["bullet3"], shaders["VertexColor"], modelMatrix);
}


void Tema1::DrawScore() {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(950, 650);
    modelMatrix *= transform2D::Scale(score * score_scale_factor, 0.5);
    RenderMesh2D(meshes["score"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(950, 650);
    modelMatrix *= transform2D::Scale(3, 0.5);
    RenderMesh2D(meshes["score_frame"], shaders["VertexColor"], modelMatrix);
}


void Tema1::DrawEverything() {
    // the beak
    DrawBeak();

    // the head
    DrawHead();

    // the body
    DrawBody();

    //right wing
    DrawRightWing();

    //left wing
    DrawLeftWing();

    // lives
    if (lives == 3) {
        DrawFirstLife();
        DrawSecondLife();
        DrawThirdLife();
    }
    else if (lives == 2) {
        DrawFirstLife();
        DrawSecondLife();
    }
    else if (lives == 1) {
        DrawFirstLife();
    }

    // bullets
    if (bullets == 3) {
        DrawFirstBullet();
        DrawSecondBullet();
        DrawThirdBullet();
    }
    else if (bullets == 2) {
        DrawFirstBullet();
        DrawSecondBullet();
    }
    else if (bullets == 1) {
        DrawFirstBullet();
    }

    // score
    DrawScore();
}


void Tema1::Update(float deltaTimeSeconds)
{
    // the earth
    DrawEarth();
    glm::ivec2 resolution = window->GetResolution();

    // increasing the speed at each 5 ducks
    if (count_ducks == 5) {
        count_ducks = 0;
        speed = speed + speed / 5;
    }

    translateX += cos(random_radians) * deltaTimeSeconds * speed;
    translateY += sin(random_radians) * deltaTimeSeconds * speed;

    // reflecting the movement of the duck
    if ((translateY >= (resolution.y - 100)) || (translateY < 0)) random_radians = -random_radians;
    if ((translateX >= (resolution.x - 100)) || (translateX < 0)) random_radians = 3.1415 - random_radians;

    // not evading, just flying around
    if (escape == 0) time_to_fly -= 50 * deltaTimeSeconds;

    // time to escape
    if (time_to_fly < 0 || bullets <= 0) {
        random_radians = glm::radians(90.0f);
        escape = 1;
    }

    // the escaping state
    if (escape == 1) {
        if (translateY > resolution.y - 101 && lives) {
            random_radians = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            // eliminating edge cases
            if (random_radians == 0 || random_radians == glm::radians(90.0f) ||
                random_radians < glm::radians(30.0f)) {
                random_radians = glm::radians(45.0f);
            }
            translateX = cos(random_radians);
            translateY = sin(random_radians);
            escape = 0;
            bullets = 3;
            time_to_fly = 800;
            lives--;
            score--;
            count_ducks++;
        }
    }

    // the dying state
    if (dead == 1) {
        random_radians = glm::radians(270.0f);
        time_to_fly = 800;
        if (translateY < 0 && lives) {
            random_radians = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            // eliminating edge cases
            if (random_radians == 0 || random_radians == glm::radians(90.0f) ||
                random_radians < glm::radians(30.0f)) {
                random_radians = glm::radians(45.0f);
            }
            translateX = cos(random_radians);
            translateY = sin(random_radians);
            dead = 0;
            time_to_fly = 800;
            bullets = 3;
            score++;
            count_ducks++;
        }
    }

    //the score
    if (score >= 15) score = 15;
    else if (score <= 0) score = 0;

    // game over and exiting the playing window
    if (lives == 0) exit(0);

    DrawEverything();
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    glm::ivec2 resolution = window->GetResolution();

    // the coordinates for the center of the duck
    float hit_zone_center_x = hit_zone_x * cos(random_radians) + translateX;
    float hit_zone_center_y = hit_zone_y * sin(random_radians) + translateY;

    if ((mouseX <= hit_zone_center_x + hit_zone_area_r && 
        mouseX >= hit_zone_center_x - hit_zone_area_r &&
        mouseY <= resolution.y - hit_zone_center_y + hit_zone_area_r && 
        mouseY >= resolution.y - hit_zone_center_y - hit_zone_area_r) &&
        bullets > 0) {
        dead = 1;
    }
    bullets--;
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
