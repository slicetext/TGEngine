#pragma once
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include "raylib.h"
#include "keybinds.h"
namespace Engine{
    class Error : public std::exception{
        private:
            const std::string msg;
        public:
            Error(std::string message) : msg(message) {}
            const char* what() {
                return msg.c_str();
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
            CircularNumber distance(CircularNumber &other, DISTANCE direction=DISTANCE::SHORTEST) {
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
        double x=0;
        double y=0;
        Vec2(float xpos,float ypos){
            x=xpos;
            y=ypos;
        }
        Vec2() {}
        Vec2 operator+(const Vec2 &v) {
            return Vec2(x+v.x,y+v.y);
        }
        Vec2 operator-(const Vec2 &v) {
            return Vec2(x-v.x,y-v.y);
        }
        Vec2 operator*(const Vec2 &v) {
            return Vec2(x*v.x,y*v.y);
        }
        Vec2 operator/(const Vec2 &v) {
            return Vec2(x/v.x,y/v.y);
        }
        Vec2 operator*(const double &d) {
            return Vec2(x*d,y*d);
        }
        Vec2 operator/(const double &d) {
            return Vec2(x/d,y/d);
        }
        double operator[](const int &i) {
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
        Vector2 toRaylibVector2() {
            return Vector2 {float(x),float(y)};
        }
    };
    class Object {
        public:
            Vec2 position;
            CircularNumber rotation=CircularNumber(0);
            Vec2 size=Vec2(1,1);
            Vec2 scale=Vec2(1,1);
            virtual void Start() {}
            virtual void Update(float DeltaTime) {}
            virtual void Draw() {}
    };
    class ImageTexture {
        private:
            Image image=GenImageColor(40, 40, MAGENTA);
            Texture2D texture;
            bool texture_loaded=false;
        public:
            ImageTexture(std::string path) {
                image=LoadImage(path.c_str());
                texture_loaded=false;
            }
            ImageTexture() {
                image=GenImageColor(60, 60, MAGENTA);
                texture_loaded=false;
            }
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
            TextureObject() {
                tex=ImageTexture();
                size=Vec2(20,20);
            }
            virtual void Draw()override{
                Rectangle rect={float(position.x),float(position.y),float(size.x*scale.x),float(size.y*scale.y)};
                NPatchInfo n={};
                DrawTextureNPatch(tex.GetTexture(), NPatchInfo(), rect, Vector2{0,0}, rotation, tint);
            }
    };
    const std::string RESET_COLOR    = "\033[0m";
    const std::string ERROR_COLOR    = "\033[1;31m";

    struct ObjectList{
        std::vector<Object*> objects;
    } objectList;

    void MainLoop() {
        try {
            for(auto& i: objectList.objects) {
                i->Start();
            }
            while(!WindowShouldClose()) {
                BeginDrawing();
                ClearBackground(RAYWHITE);
                for(auto& i: objectList.objects) {
                    i->Update(GetFrameTime());
                    i->Draw();
                }
                EndDrawing();
            }
            CloseWindow();
        }catch(Error e) {
            std::cout<<ERROR_COLOR<<e.what()<<RESET_COLOR<<std::endl;
        }
    }
    void CreateWindow(std::string name, int screen_width, int screen_height) {
        InitWindow(screen_width,screen_height,"Engine");
        SetTargetFPS(60);
    }
}
