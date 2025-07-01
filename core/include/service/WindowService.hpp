//
// Created by arunc on 01/07/2025.
//

#ifndef WINDOWSERVICE_HPP
#define WINDOWSERVICE_HPP
#include <mutex>

class WindowService {
private:
    static WindowService* instance;
    static std::mutex mtx;

    int width = 1200;
    int height = 600;
    float deltaTime = 0.0f;

    WindowService() {}

public:


    WindowService(const WindowService&) = delete;
    WindowService& operator=(const WindowService&) = delete;

    static WindowService* getInstance() {
        std::lock_guard<std::mutex> lock(mtx);
        if (instance == nullptr) {
            instance = new WindowService();
        }
        return instance;
    }

    // Getters și setters
    int getWidth() const {
        return width;
    }

    void setWidth(int w) {
        width = w;
    }

    int getHeight() const {
        return height;
    }

    void setHeight(int h) {
        height = h;
    }

    void setDeltaTime(float dt) {
        deltaTime = dt;
    }

    float getDeltaTime() {
        return deltaTime;
    }
};


#endif //WINDOWSERVICE_HPP
