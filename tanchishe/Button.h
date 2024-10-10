#pragma once
#include <functional>
#include <iostream>
class Button {
public:
    int x, y; 
    int width, height; 
    bool isPressed; 
    std::function<void()> onClick; 
    std::string text = "  ";

    Button(int x, int y, int width, int height, std::function<void()> func,std::string a)
        : x(x), y(y), width(width), height(height), isPressed(false), onClick(func),text(a) {}

    // 绘制按钮  
    void draw();

        // 检测鼠标点击事件  
    bool checkClick(int mouseX, int mouseY);

        // 释放按钮（鼠标离开按钮区域）  
    void release();

        // 设置点击回调函数  
    void setOnClick(std::function<void()> func);
};