#pragma once
#include"framework.h"
#include"define.h"

class CDevice
{
public:
    void Init(HWND hWnd, int screenW,int screenH);

    void DrawPiexl(int x, int y, COLORREF color);
    void Clear(COLORREF color);
    //bool ZTest(int x, int y, float z, int op);
    //void ZWrite(int x, int y, float z);
    void ApplyToScreen();
protected:        
    int screenWidth;
    int screenHeight;
    float** zBuffer;
    UINT32** frameBuffer;

    HDC hMemDc = NULL;    
    HBITMAP hBMP;
    HBITMAP hOldBMP;

    
    
    HWND hMainWnd;
    

};

