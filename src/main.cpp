#include "inc.h"

int main() {
    Engine::GetInstance()->Init();

    while(Engine::GetInstance()->isRunning()) {
        Engine::GetInstance()->Events();
        Engine::GetInstance()->Update();
        Engine::GetInstance()->Render();
        Timer::getInstance()->Tick();
    }
    Engine::GetInstance()->Clean();
}