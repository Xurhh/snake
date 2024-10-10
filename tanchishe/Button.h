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

    // ���ư�ť  
    void draw();

        // ���������¼�  
    bool checkClick(int mouseX, int mouseY);

        // �ͷŰ�ť������뿪��ť����  
    void release();

        // ���õ���ص�����  
    void setOnClick(std::function<void()> func);
};