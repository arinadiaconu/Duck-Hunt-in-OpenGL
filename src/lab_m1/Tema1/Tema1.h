#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void Meshes();
        void DrawEarth();
        void DrawBeak();
        void DrawHead();
        void DrawBody();
        void DrawRightWing();
        void DrawLeftWing();
        void DrawEverything();
        void DrawFirstLife();
        void DrawSecondLife();
        void DrawThirdLife();
        void DrawFirstBullet();
        void DrawSecondBullet();
        void DrawThirdBullet();
        void DrawScore();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;                                   // moving the duck
        float radians_right, radians_left, direction_r, direction_l;    // moving the wings
        float random_radians, dir_x, dir_y;                             // the flying angle
        float time_to_fly, escape, speed, dead, duck_length;
        float hit_zone_x, hit_zone_y, hit_zone_area_r;
        float lives, bullets, score, score_scale_factor;
        float count_ducks;
    };
}   // namespace m1
