#include <raylib.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <mutex>

class Input {
private:
    // Static pointer to the Input instance
    static Input* instancePtr;

    // Mutex to ensure thread safety
    static std::mutex mtx;

    // Private Constructor
    Input() {}

public:
    // Deleting the copy constructor to prevent copies
    Input(const Input& obj) = delete;

    // Static method to get the Input instance
    static Input* getInstance() {
        if (instancePtr == nullptr) {
            std::lock_guard<std::mutex> lock(mtx);
            if (instancePtr == nullptr) {
                instancePtr = new Input();
            }
        }
        return instancePtr;

    }
    std::unordered_map<std::string, int> InputMap;

    bool IsPressed(std::string action_name) {
        if(IsKeyDown( InputMap[action_name] )) {
            return true;
        } else {
            return false;
        }
    }
    bool IsReleased(std::string action_name) {
        if(IsKeyUp( InputMap[action_name] )) {
            return true;
        } else {
            return false;
        }
    }
    bool IsJustPressed(std::string action_name) {
        if(IsKeyPressed( InputMap[action_name] )) {
            return true;
        } else {
            return false;
        }
    }
    bool IsJustReleased(std::string action_name) {
        if(IsKeyReleased( InputMap[action_name] )) {
            return true;
        } else {
            return false;
        }
    }
};
Input* Input::instancePtr = nullptr;
std::mutex Input::mtx;
