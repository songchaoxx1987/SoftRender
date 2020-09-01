#pragma once
#include"framework.h"
#include"define.h"
class Material;
class Trangle;
class RenderPipeline;

class CDevice
{
public:
    void Init(HWND hWnd, int screenW,int screenH);

    void DrawPiexl(int x, int y, UINT32 color);
    void Clear(UINT32 color);
    void ClearZBuffer();

    bool ZTest(int x, int y, float z)
    {
        return zBuffer[y][x] >= z;
    }

    void ZWrite(int x, int y, float z)
    {
        zBuffer[y][x] = z;
    }

    //void RasterizeTrangle(Trangle* pTrangle, Material* pMat, RenderPipeline* pPipeline);
    
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

