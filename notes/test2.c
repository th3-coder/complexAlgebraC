#include <windows.h>
#include<stdio.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    HBITMAP hBmp = (HBITMAP)LoadImage(NULL, "C:\\Users\\haydo\\Desktop\\Home_Server\\scripts\\ECE35\\complexAlgebra\\notes\\binaryReflection.bmp",
                                     IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!hBmp) {
        DWORD err = GetLastError();
        char buf[256];
        sprintf(buf, "Failed to load bitmap. Error code: %lu", err);
        MessageBox(NULL, buf, "Error", MB_ICONERROR);
        return 1;
    }
    MessageBox(NULL, "Bitmap loaded successfully!", "Success", MB_OK);
    DeleteObject(hBmp);
    return 0;
}