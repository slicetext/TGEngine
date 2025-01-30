#include "engine.h"

class Player : public Engine::TextureObject {
}player;

int main() {
    Engine::CreateWindow("Engine",400,400);
    Engine::objectList.objects.push_back(&player);
    Engine::MainLoop();
    return 0;
}
