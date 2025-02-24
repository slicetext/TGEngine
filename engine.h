#pragma once
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include "include/collision.h"
#include "include/id.h"
#include "include/math_functions.h"
#include "raylib.h"
#include "keybinds.h"
#include "include/box2d.h"
#include "include/base.h"
#include "include/types.h"
#include "include/math_functions.h"
#include <chrono>

namespace Engine{
    struct ProfileTimer{
        std::chrono::time_point<std::chrono::high_resolution_clock> start,end;
        const char* name;
        ProfileTimer(const char* name) : name(name) {
            start=std::chrono::high_resolution_clock::now();
        }
        ~ProfileTimer() {
            end=std::chrono::high_resolution_clock::now();
            auto duration=end.time_since_epoch()-start.time_since_epoch();
            auto t_duration = std::chrono::duration_cast<std::chrono::microseconds>(duration).count() / 1000.0f;
            auto s_duration = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / 1000.0f;
            std::cout<<"\033[1;106;30m"<<name<<": "<<t_duration<<"ms"<<" ("<<s_duration<<"s)\033[0m"<<std::endl;
        }
    };
    class CircularNumber{
        private:
            double range=360;
        public:
            double num=0;
            double max_value;
            double min_value;
            enum class DISTANCE{
                SHORTEST,
                LONGEST,
            };
            CircularNumber(double number,const double max_val=360, const double min_val=0) : num(number), max_value(max_val), min_value(min_val), range(max_val-min_val) {
                while(num>=max_value) num-=range;
                while(num< min_value) num+=range;
            }
            void EnsureCircular() {
                while(num>=max_value) num-=range;
                while(num< min_value) num+=range;
            }
            CircularNumber operator+(const CircularNumber &c) {
                double result=c.num+num;
                while(result>=max_value) result-=range;
                while(result< min_value) result+=range;
                return result;
            }
            CircularNumber operator-(const CircularNumber &c) {
                double result=c.num-num;
                while(result>=max_value) result-=range;
                while(result< min_value) result+=range;
                return result;
            }
            CircularNumber operator*(const CircularNumber &c) {
                double result=c.num*num;
                while(result>=max_value) result-=range;
                while(result< min_value) result+=range;
                return result;
            }
            CircularNumber operator/(const CircularNumber &c) {
                double result=c.num/num;
                while(result>=max_value) result-=range;
                while(result< min_value) result+=range;
                return result;
            }
            CircularNumber operator++() {
                return CircularNumber(num+1);
            }
            CircularNumber operator--() {
                return CircularNumber(num-1);
            }
            CircularNumber operator++(int) {
                return CircularNumber(num+1);
            }
            CircularNumber operator--(int) {
                return CircularNumber(num-1);
            }
            void operator+=(const double &d) {
                num=CircularNumber(num+d).num;
            }
            void operator-=(const double &d) {
                num=CircularNumber(num-d).num;
            }
            void operator*=(const double &d) {
                num=CircularNumber(num*d).num;
            }
            void operator/=(const double &d) {
                num=CircularNumber(num/d).num;
            }
            void operator=(const double &d) {
                num=CircularNumber(d).num;
            }
            CircularNumber operator-() {
                return CircularNumber(-num);
            }
            CircularNumber operator+() {
                return CircularNumber(+num);
            }
            operator double() {
                return num;
            }
            explicit operator double*() {
                return &num;
            }
            CircularNumber distance(CircularNumber &other, DISTANCE direction=DISTANCE::SHORTEST)const{
                CircularNumber result1=other-CircularNumber(num);
                CircularNumber result2=CircularNumber(num)-other;
                if(direction==DISTANCE::SHORTEST)
                    return CircularNumber(std::min(result1.num,result2.num));
                else if(direction==DISTANCE::LONGEST)
                    return CircularNumber(std::max(result1.num,result2.num));
                return CircularNumber(0);
            }
    };
    struct Vec2{
        double x;
        double y;
        Vec2(float x,float y) : x(x), y(y) {}
        Vec2() : x(0), y(0) {}
        Vec2 operator+(const Vec2 &v) const{
            return Vec2(x+v.x,y+v.y);
        }
        Vec2 operator-(const Vec2 &v) const{
            return Vec2(x-v.x,y-v.y);
        }
        Vec2 operator*(const Vec2 &v) const{
            return Vec2(x*v.x,y*v.y);
        }
        Vec2 operator/(const Vec2 &v) const{
            return Vec2(x/v.x,y/v.y);
        }
        Vec2 operator*(const double &d) const{
            return Vec2(x*d,y*d);
        }
        Vec2 operator/(const double &d) const{
            return Vec2(x/d,y/d);
        }
        double operator[](const int &i) const{
            switch (i) {
            case 0:
                return x;
                break;
            case 1:
                return y;
                break;
            default:
                throw std::out_of_range("Index out of Range");
                return -1;
                break;
            }
        }
        Vec2 normalized() {
            return Vec2(x/std::abs(x),y/std::abs(y));
        }
        double dot(Vec2 &d) {
            double product=0;
            product=x*d[0];
            product+=y*d[1];
            return product;
        }
        Vec2 rotated(CircularNumber degrees) {
            double rads=degrees.num * (PI / 180);
            double cs=cos(rads);
            double sn=sin(rads);
            double px = x * cs - y * sn;
            double py = x * sn + y * cs;
            return Vec2(px,py);
        }
        operator Vector2() {
            return Vector2 {float(x),float(y)};
        }
        operator b2Vec2() {
            return b2Vec2{float(x),float(y)};
        }
    };
    class Component;
    class Object {
        private:
            Vec2 position_old=Vec2(0,0);
        public:
            bool visible=true;
            Vec2 position;
            CircularNumber rotation=CircularNumber(0);
            Vec2 size=Vec2(1,1);
            Vec2 scale=Vec2(1,1);
            std::vector<Object*> children;
            std::vector<Component*> components;
            virtual void Start() {}
            virtual void Update(float DeltaTime) {}
            virtual void UpdateComponents();
            virtual void Draw() {}
            void UpdateChildren() {
                Vec2 change=position-position_old;
                for(auto i : children) {
                    i->position=i->position+change;
                    i->UpdateChildren();
                }
            }
            template<typename T> T* GetComponent()const{
                for(int i=0; i<components.size(); i++) {
                    if(dynamic_cast<T*>(components[i]) != nullptr) {
                        // static_assert(std::is_base_of<Component*, T*>::i, "Get Component failed");
                        return dynamic_cast<T*>(components[i]);
                    }
                }
                return nullptr;
            }
            template<typename T> void AddComponent() {
                components.emplace_back(new T);
            }
    };
    class Component {
        public:
            virtual void Box2dSceneInit(b2WorldId b, Object* obj) {}
            virtual void UpdateComponent(Object* obj) {}
    };
    inline void Object::UpdateComponents() {
        for(auto i : components) {
            i->UpdateComponent(this);
        }
    }
    class DynamicBody : public Component {
        public:
            b2BodyId bodyID;
            void UpdateComponent(Object* obj)override {
                b2Vec2 p = b2Body_GetWorldPoint(bodyID, {(float)-(obj->size*obj->scale).x/20,(float)-(obj->size*obj->scale).y/20});
                // b2Vec2 p = b2Body_GetWorldPoint(bodyID, {0,0});
                b2Rot rotation = b2Body_GetRotation(bodyID);
                float radians = b2Rot_GetAngle(rotation);
                obj->position=Vec2(p.x,p.y);
                obj->rotation=radians * RAD2DEG;
            }
            void Box2dSceneInit(b2WorldId id, Object* obj)override{
                b2BodyDef b=b2DefaultBodyDef();
                b.type = b2_dynamicBody;
                b.position=obj->position+(obj->scale*obj->scale)/40;
                b.rotation.s=obj->rotation.num * DEG2RAD;
                bodyID=b2CreateBody(id, &b);
                b2ShapeDef shapeDef = b2DefaultShapeDef();
                b2Polygon polygon=b2MakeBox((obj->size*obj->scale).x/4,(obj->size*obj->scale).y/4);
                b2CreatePolygonShape(bodyID, &shapeDef, &polygon);
            }
            void ApplyForce(Vec2 impulse){
                b2Body_ApplyForceToCenter(bodyID, impulse*100, true);
            }
            void ApplyRotation(float torque){
                b2Body_ApplyTorque(bodyID, torque*100, true);
            }
            void SetAngularDamping(float damping){
                b2Body_SetAngularDamping(bodyID, damping);
            }
            void SetLinearDamping(float damping){
                b2Body_SetLinearDamping(bodyID, damping);
            }
    };
    class StaticBody : public Component {
        public:
            b2BodyId bodyID;
            void Box2dSceneInit(b2WorldId id, Object* obj)override{
                b2BodyDef b=b2DefaultBodyDef();
                b.type = b2_staticBody;
                b.position=obj->position;
                b.rotation.s=obj->rotation.num * DEG2RAD;
                bodyID=b2CreateBody(id, &b);
                b2ShapeDef shapeDef = b2DefaultShapeDef();
                b2Polygon polygon=b2MakeBox((obj->size*obj->scale).x/2,(obj->size*obj->scale).y/2);
                b2CreatePolygonShape(bodyID, &shapeDef, &polygon);
            }
            void ApplyForce(Vec2 impulse){
                b2Body_ApplyForceToCenter(bodyID, impulse, true);
            }
            void ApplyRotation(float torque){
                b2Body_ApplyTorque(bodyID, torque, true);
            }
            void SetAngularDamping(float damping){
                b2Body_SetAngularDamping(bodyID, damping);
            }
            void SetLinearDamping(float damping){
                b2Body_SetLinearDamping(bodyID, damping);
            }
    };
    class ImageTexture {
        private:
            Image image=GenImageColor(40, 40, MAGENTA);
            Texture2D texture;
            bool texture_loaded=false;
        public:
            ImageTexture(const char* path) : image(LoadImage(path)), texture_loaded(false) {}
            ImageTexture() : image(GenImageColor(60, 60, MAGENTA)), texture_loaded(false) {}
            operator Texture2D() {
                return texture;
            }
            operator Image() {
                return image;
            }
            Image& GetImage() {
                return image;
            }
            Texture2D& GetTexture() {
                if(texture_loaded==false) {
                    texture=LoadTextureFromImage(image);
                    texture_loaded=true;
                }
                return texture;
            }
            void Set(Image img) {
                image=img;
                texture=LoadTextureFromImage(image);
                texture_loaded=true;
            }
            void Set(Texture2D tex) {
                texture=tex;
                image=LoadImageFromTexture(texture);
                texture_loaded=true;
            }
    };
    class TextureObject : public Object {
        public:
            ImageTexture tex;
            Color tint=WHITE;
            TextureObject() : tex(ImageTexture()) {
                size=Vec2(20,20);
            }
            virtual void Draw()override{
                Rectangle rect={float(position.x),float(position.y),float(size.x*scale.x),float(size.y*scale.y)};
                NPatchInfo n={};
                DrawTextureNPatch(tex.GetTexture(), NPatchInfo(), rect, Vector2{0,0}, rotation, tint);
            }
    };
    class Cam {
        public:
            Vec2 target;
            CircularNumber rotation;
            float zoom;
            Cam() : rotation(CircularNumber(0)), target(Vec2(0,0)), zoom(1) {}
            operator Camera2D() {
                return Camera2D{Vector2{0,0},target,float(rotation.num),zoom};
            }
    };

    struct Scene{
        std::vector<Object*> objects={};
        Color bgColor=WHITE;
        Cam camera=Cam();
        b2WorldId worldID;
        Scene() {
            b2WorldDef worlddef=b2DefaultWorldDef();
            worldID=b2CreateWorld(&worlddef);
        }
        void AddObject(Object* obj) {
            objects.emplace_back(obj);
            objects.back()->Start();
            for(auto i : obj->components) {
                i->Box2dSceneInit(worldID,obj);
            }
        }
        template<typename T> void AddObject() {
            T* t=new T;
            objects.emplace_back(t);
            objects.back()->Start();
            for(auto i : t->components) {
                i->Box2dSceneInit(worldID,t);
            }
        }
        enum class PROPERTY {
            GRAVITY,
        };
        template<typename T> void setProperty(PROPERTY property, T value) {
            if(property==PROPERTY::GRAVITY) {
                b2World_SetGravity(worldID, value);
            }
        }
        void Load();
    };
    struct Root{
        static Scene CurrentScene;
        // std::vector<std::string> TextureList;
        Root()=delete;
    };
    Scene Root::CurrentScene;


    inline void Scene::Load() {
        Root::CurrentScene=*this;
    }

    inline void MainLoop() {
        while(!WindowShouldClose()) {
            BeginDrawing();
            {
                ProfileTimer t("Update");
                BeginMode2D(Root::CurrentScene.camera);
                ClearBackground(Root::CurrentScene.bgColor);
                b2World_Step(Root::CurrentScene.worldID, GetFrameTime(), 4);
                for(int j=0; j<Root::CurrentScene.objects.size(); j++) {
                    auto i=Root::CurrentScene.objects[j];
                    i->UpdateComponents();
                    i->Update(GetFrameTime());
                    i->UpdateChildren();
                    if(i->visible)
                        i->Draw();
                }
            }
            EndDrawing();
        }
        for(auto i : Root::CurrentScene.objects) {
            delete i;
        }
        CloseWindow();
    }
    inline void CreateWindow(const char* name, int screen_width, int screen_height) {
        ProfileTimer t("Create Window");
        InitWindow(screen_width,screen_height,name);
        SetTargetFPS(60);
        b2SetLengthUnitsPerMeter(10);
    }
}
