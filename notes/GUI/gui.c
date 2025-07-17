#include <windows.h>
#include <stdio.h>

#define APPTITLE "Push Buttons"
#define IDC_MAIN_BUTTON_1 9001
#define IDC_MAIN_BUTTON_2 9002

char userInput[256];

// ✅ Make controls global so all message handlers can access them
HWND PushButton_Handle1, PushButton_Handle2, hEdit;

ATOM Init_Window_Class(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

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
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
                hWnd, NULL, GetModuleHandle(NULL), NULL);

            // Create buttons
            PushButton_Handle1 = CreateWindow(
                "BUTTON", "Calculate",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                0, 0, 0, 0,
                hWnd, (HMENU)IDC_MAIN_BUTTON_1, GetModuleHandle(NULL), NULL);

            PushButton_Handle2 = CreateWindow(
                "BUTTON", "Go Back",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                0, 0, 0, 0,
                hWnd, (HMENU)IDC_MAIN_BUTTON_2, GetModuleHandle(NULL), NULL);

            break;
        }

        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);

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
            MoveWindow(PushButton_Handle2,
                width - buttonWidth - spacing,
                height - buttonHeight * 2 - spacing * 2,
                buttonWidth, buttonHeight, TRUE);

            MoveWindow(PushButton_Handle1,
                width - buttonWidth - spacing,
                height - buttonHeight - spacing,
                buttonWidth, buttonHeight, TRUE);

            break;
        }

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                    case IDC_MAIN_BUTTON_1: {
                    GetWindowText(hEdit, userInput, sizeof(userInput));
                    InvalidateRect(hWnd, NULL, TRUE);  // Triggers WM_PAINT
                    printf(userInput);
                    break;
                }
                case IDC_MAIN_BUTTON_2:
                    MessageBox(hWnd, "You Clicked Button 2", "Message", MB_OK);
                    break;
            }
            break;

        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Draw the text if not empty
        if (strlen(userInput) > 0) {
            TextOut(hdc, 20, 20, userInput, strlen(userInput));
        }

        EndPaint(hWnd, &ps);
        break;
        }

        default:
            return DefWindowProc(hWnd, Message, wParam, lParam);
    }
    return 0;
}
