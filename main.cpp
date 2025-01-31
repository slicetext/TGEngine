#include "engine.h"
#include <memory>

class Player : public Engine::TextureObject {
    public:
        void Start()override{
            scale=Engine::Vec2(20,20);
        }
        void Update(float DeltaTime)override{
            position.x+=1;
            position.y+=3;
            scale=scale*1.01;
        }
}player;

int main() {
    Engine::CreateWindow("Engine",400,400);
    Engine::objectList.objects.push_back(&player);
    Engine::MainLoop();
    return 0;
}
