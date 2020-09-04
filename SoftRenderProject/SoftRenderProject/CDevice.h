#pragma once
#include"framework.h"
#include"define.h"

class CDevice
{
public:
    void Init(HWND hWnd, int screenW,int screenH);

    void DrawPiexl(int x, int y, UINT32 color);
    void Clear(UINT32 color);    
    void ApplyToScreen();

    int screenWidth;
    int screenHeight;
protected:                
    UINT32** frameBuffer;
    HDC hMemDc = NULL;    
    HBITMAP hBMP;
    HBITMAP hOldBMP;
    HWND hMainWnd;
};

