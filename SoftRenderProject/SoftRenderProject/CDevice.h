#pragma once
#include"framework.h"
#include"define.h"
class Material;
class Trangle;
class CDevice
{
public:
    void Init(HWND hWnd, int screenW,int screenH);

    void DrawPiexl(int x, int y, UINT32 color);
    void Clear(UINT32 color);
    void ClearZBuffer();

    void RasterizeTrangle(Trangle* pTrangle, Material* pMat);
    //bool ZTest(int x, int y, float z, int op);
    //void ZWrite(int x, int y, float z);
    void ApplyToScreen();

    int screenWidth;
    int screenHeight;
protected:            
    float** zBuffer;
    UINT32** frameBuffer;

    HDC hMemDc = NULL;    
    HBITMAP hBMP;
    HBITMAP hOldBMP;

    
    
    HWND hMainWnd;
    

};

