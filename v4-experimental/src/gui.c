#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "complex.h"

#define APPTITLE "Complex/Polar Calculator"
#define IDC_MAIN_BUTTON_1 9001
#define IDC_MAIN_BUTTON_2 9002
#define IDC_INPUT_1 9003
#define IDC_DYNAMIC_OUTPUT_1 9004

HWND PushButton_Handle1, PushButton_Handle2, hEdit;
HFONT hFont = NULL;

ATOM Init_Window_Class(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);


// typedef struct{
//     char prevResult[128]
// } Buffer;
//data variables
char prevResult[128][256];
char showWork[128][256];
char showWorkBuffer[128][256];
char prevbuffer[256];
// counters
int resultCounter = 0, steps = 0, showsteps = 1, showstepsBuffer;
//booleans
int notTyping = 1;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nCmdLine, int nCmdShow) {
    
    MSG msg;

    if (!Init_Window_Class(hInstance)) {
        MessageBox(NULL, "Window Registration Failed", "ERROR", 0);
        return FALSE;
    }

    if (!InitInstance(hInstance, nCmdShow)) {
        MessageBox(NULL, "Window Creation Failed", "ERROR", 0);
        return FALSE;
    }

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

ATOM Init_Window_Class(HINSTANCE hInstance) {
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WinProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_ASTERISK);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = APPTITLE;
    wc.hIconSm = NULL;

    return RegisterClassEx(&wc);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    HWND hWnd = CreateWindow(
        APPTITLE, APPTITLE,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        600, 400,    // default size
        NULL, NULL, hInstance, NULL);

    if (!hWnd)
        return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    switch (Message) {
        case WM_CREATE: {
            // Create edit box (text input)
            hEdit = CreateWindowEx(
                0, "EDIT", "",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                0, 0, 0, 0,
                hWnd, (HMENU)IDC_INPUT_1, GetModuleHandle(NULL), NULL);

            // Create buttons
            PushButton_Handle1 = CreateWindow(
                "BUTTON", "Calculate",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                0, 0, 0, 0,
                hWnd, (HMENU)IDC_MAIN_BUTTON_1, GetModuleHandle(NULL), NULL);

            PushButton_Handle2 = CreateWindow(
                "BUTTON", "Previous",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                0, 0, 0, 0,
                hWnd, (HMENU)IDC_MAIN_BUTTON_2, GetModuleHandle(NULL), NULL);

        break;
        }

        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            int screenW = width;
            int screenH = height;
            // Control dimensions and spacing
            int buttonWidth = 120;
            int buttonHeight = 40;
            int spacing = 10;
            int editWidth = 300;
            int editHeight = 25;

            // Position edit control in center bottom
            MoveWindow(hEdit,
                (width - editWidth) / 2,
                height - editHeight - buttonHeight - spacing * 3,
                editWidth, editHeight, TRUE);

            // Bottom-right buttons stacked vertically
            //calculate button 
            MoveWindow(PushButton_Handle1,
                width - buttonWidth - spacing,
                height - buttonHeight * 2 - spacing * 2,
                buttonWidth, buttonHeight, TRUE);
            //previous button
            MoveWindow(PushButton_Handle2,
                width - buttonWidth - spacing,
                height - buttonHeight - spacing,
                buttonWidth*0.75, buttonHeight*0.5, TRUE);

        break;
        }
        
        // case WM_KEYDOWN:
        //     if(wParam == VK_RETURN){
        //         GetWindowText(hEdit, userInput, sizeof(userInput));
        //         int bP = 0;
        //         double buffMag, buffAngle;
        //         int len_temp = strlen(userInput);
        //         solveComplexEq(userInput, len_temp, &buffMag, &buffAngle, &bP, &steps, solution, showWork, &showsteps);
        //         snprintf(solution, sizeof(solution), "RESULT: Polar Form %.3f /_ %.3f    Complex Form %.3f + j%.3f", buffMag, buffAngle, buffMag*cos(buffAngle), buffMag*sin(buffAngle));
        //         InvalidateRect(hWnd, NULL, TRUE);  // Triggers WM_PAINT
        //         UpdateWindow(hWnd);
        //     }
        // break;

        case WM_COMMAND:
            int bP = 0;
            switch (LOWORD(wParam)) {
                case IDC_MAIN_BUTTON_1: {
                    GetWindowText(hEdit, userInput, sizeof(userInput));
                    showsteps = 1;
                    int len_temp = strlen(userInput);
                    double buffMag, buffAngle;
                    solveComplexEq(userInput, len_temp, &buffMag, &buffAngle, &bP, &steps, solution, showWork, &showsteps);
                    snprintf(solution, sizeof(solution), "RESULT: Polar Form: %.3f/_%.3f    Complex Form: %.3f + j%.3f", buffMag, buffAngle, buffMag*cos(buffAngle*(PI/180)), buffMag*sin(buffAngle*(PI/180)));
                    strcpy(prevResult[resultCounter++], solution);
                    for(int i = 1; i < showsteps; i++){
                                snprintf(showWorkBuffer[i], sizeof(showWorkBuffer), showWork[i]);
                            }
                            showstepsBuffer = showsteps;
                    userInput[0] = '\0';
                    InvalidateRect(hWnd, NULL, TRUE);  // Triggers WM_PAINT
                    UpdateWindow(hWnd);
                break;
                }
                case IDC_MAIN_BUTTON_2:
                    char prev[256] = "Previous ";
                    strcat(prev, prevResult[resultCounter-2]);
                    MessageBox(hWnd, prev, "Message", MB_OK);
                break;

                case IDC_INPUT_1:
                    char buffer[256];
                    if(HIWORD(wParam) == EN_UPDATE){
                        GetWindowText(hEdit, buffer, sizeof(buffer));
                        int len_buffer = strlen(buffer);
                        if(buffer[len_buffer-1] == '=' && (strcmp(buffer, prevbuffer) != 0)){
                            steps = 0, showsteps = 1;
                            strncpy(prevbuffer, buffer, sizeof(prevbuffer));
                            buffer[len_buffer-1] = '\0';
                            double buffMag, buffAngle;
                            solveComplexEq(buffer, len_buffer, &buffMag, &buffAngle, &bP, &steps, solution, showWork, &showsteps);
                            snprintf(solution, sizeof(solution), "RESULT: Polar Form: %.3f/_%.3f    Complex Form: %.3f + j%.3f", buffMag, buffAngle, buffMag*cos(buffAngle*(PI/180)), buffMag*sin(buffAngle*(PI/180)));
                            strcpy(prevResult[resultCounter++], solution);
                            for(int i = 1; i < showsteps; i++){
                                snprintf(showWorkBuffer[i], sizeof(showWorkBuffer), showWork[i]);
                            }
                            showstepsBuffer = showsteps;
                            buffer[0] = '\0';
                            InvalidateRect(hWnd, NULL, TRUE);  // Triggers WM_PAINT
                            UpdateWindow(hWnd);
                        }
                        if(len_buffer > 1){
                            notTyping = 0;
                            InvalidateRect(hWnd, NULL, TRUE);
                            UpdateWindow(hWnd);
                        } else {
                            notTyping = 1;
                            InvalidateRect(hWnd, NULL, TRUE);
                            UpdateWindow(hWnd);
                        }
                    }
                break;
            }
            break;
        break;
            
        //place directly before wm_paint
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_PAINT: {
        PAINTSTRUCT ps;
        XFORM xForm;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT clientRect;

        GetClientRect(hWnd, &clientRect); // Get client area dimensions
        int swidth = clientRect.right - clientRect.left;
        int sheight = clientRect.bottom - clientRect.top;
        //to allow for use of SetWorldTransform();
        SetGraphicsMode(hdc, GM_ADVANCED);
        
        //custom font
        LOGFONT lf = {0};
        lf.lfHeight = sheight/30+(1-swidth/1000); 
        strcpy(lf.lfFaceName, "Consolas");
        hFont = CreateFontIndirect(&lf);

        //font for text drawn
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
        //HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        SelectObject(hdc, hFont);
        //rectangle for text 
        RECT textRect = { 20, 20, swidth-50, 1000 }; // Left, Top, Right, Bottom (initial large bottom)
        
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0,0,0));

        if(notTyping){
            // scale and transform text //
            xForm.eM11 = 1.5f; // horizontal scaling
            xForm.eM22 = 1.5f; // vertical scaling
            xForm.eM12 = 0.0f; // shear 
            xForm.eM21 = 0.0f; // shear
            xForm.eDx = 0.0f;  // horizontal translation
            xForm.eDy = 0.0f;  // vertical translation
            
            SetWorldTransform(hdc, &xForm);

            char info[12][128];
            snprintf(info[0], 128, "Enter equation with the following format:");
            snprintf(info[1], 128, "Polar: p[magnitude, angle(deg)]");
            snprintf(info[2], 128, "Imag: i[real, imag]");
            snprintf(info[3], 128, "Press Calculate, or type =");
            snprintf(info[4], 128, "Example: (p[10, 45] + i[4, 60]) * i[5, 30] =");
            // text layout param //
            int xPad = 20, yPad = 20, yInc = 30;

            TextOut(hdc, xPad, yPad, info[0], strlen(info[0]));
            TextOut(hdc, xPad*2, yInc*1+yPad, info[1], strlen(info[1]));
            TextOut(hdc, xPad*2, yInc*2+yPad, info[2], strlen(info[2]));
            TextOut(hdc, xPad, yInc*3+yPad, info[3], strlen(info[3]));
            TextOut(hdc, xPad*3, yInc*4+yPad, info[4], strlen(info[4]));
        }

        // these run continously (every frame)
        // Draw the text if not empty
        int y;
        if (strlen(solution) > 1) {
            for(int i = 1; i < showstepsBuffer; i++){
                //must set rec each time before drawing text after measuring height 
                textRect.top = y;
                textRect.bottom = y + 1000;
                textRect.left = 20;
                textRect.right = swidth - 50;
                if(i % 2 != 0){
                    // Now you can draw with the exact rectangle
                    DrawText(hdc, showWorkBuffer[i], -1, &textRect, DT_WORDBREAK);
                    //TextOut(hdc, 30, 20*i, showWorkBuffer[i], strlen(showWorkBuffer[i]));
                } else {
                    textRect.left += 30;
                    DrawText(hdc, showWorkBuffer[i], -1, &textRect, DT_WORDBREAK);
                    //TextOut(hdc, 60, 20*i, showWorkBuffer[i], strlen(showWorkBuffer[i]));    
                }
                // Measure height of this line
                DrawText(hdc, showWorkBuffer[i], -1, &textRect, DT_WORDBREAK | DT_CALCRECT);
                y = textRect.bottom;
            }
            TextOut(hdc, swidth/4 - swidth/8, sheight-125, solution, strlen(solution));
            ReleaseDC(hWnd, hdc);
            
            // Sleep(60);
        }
        // debug //
        // char debug[512];
        // snprintf(debug, "DEBUG: %s", solution);
        // TextOut(hdc, 20, 40, debug, strlen(debug));

        EndPaint(hWnd, &ps);
        break;
        }

        default:
            return DefWindowProc(hWnd, Message, wParam, lParam);
    }
    return 0;
}
