#define _CRT_SECURE_NO_WARNINGS
#include <graphics.h> 
#include"Button.h"
void Button::draw() {
    
    setfillcolor(WHITE); 
    ege_fillrect(x, y, width,height);

    
    if (isPressed) {
        setfillcolor(EGERGB(128, 128, 128));
        bar(x + 1, y + 1, x + width - 1, y + height - 1);
    }
   
    setcolor(RED); 
    setbkmode(TRANSPARENT);
    setfont(20, 0, "����"); 

   
    int textWidth = textwidth(text.c_str());
    int textHeight = textheight("A"); 
    int textX = x + (width - textWidth) / 2;
    int textY = y + (height + textHeight) / 2 - textHeight; 

   

    outtextxy(textX, textY, text.c_str());
     
}

// ���������¼�  
bool Button::checkClick(int mouseX, int mouseY) {

    if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height) {

        isPressed = true;
        return true;
    }
    isPressed = false;
    return false;
}

// �ͷŰ�ť������뿪��ť����  
void Button::release() {
    if (isPressed) {
        onClick(); // ���ûص�����  
        isPressed = false;
    }
}

// ���õ���ص�����  
void Button::setOnClick(std::function<void()> func) {
    onClick = func;
}