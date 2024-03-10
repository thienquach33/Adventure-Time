#include <iostream>
#include "Core/Engine.cpp"
#include "Graphics/TextureManager.cpp"
#include "Characters/Sprites.cpp"
#include "Characters/Monster.cpp"
#include "Inputs/Input.cpp"
#include "Animation/Animation.cpp"
#include "Timer/timer.cpp"
#include "map/MapParser.cpp"
#include "map/TileLayer.cpp"
#include "vendor/tinystr.cpp"
#include "vendor/tinyxml.cpp"
#include "vendor/tinyxmlerror.cpp"
#include "vendor/tinyxmlparser.cpp"
#include "Camera/Camera.cpp"
#include "Collision/CollisionHandler.cpp"

int main() {
    
    Engine::GetInstance()->Init();

    while(Engine::GetInstance()->isRunning()) {
        Engine::GetInstance()->Events();
        Engine::GetInstance()->Update();
        Engine::GetInstance()->Render();
        Timer::getInstance()->Tick();
    }
    Engine::GetInstance()->Clean();

    return 0;
}