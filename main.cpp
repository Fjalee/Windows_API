#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>

#define PARAM_MENU_EXIT 1
#define CLICK_PAD_CLICKED 2
#define TEST 3000

#define RIBON_HEIGHT 30

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

HMENU hMenu;
HWND hScore;
HWND hSpeed;

int playerScore = 0;
int playerSpeed = 0;
DWORD startTime;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR args,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    wincl.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("SpeedyFingers"), /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           1000,                 /* The programs width */
           700,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

void testFilePrint(std::string s)
{
    std::ofstream MyFile("test.txt");
    MyFile << s;
    MyFile.close();
}

/////////////////////////////////////////////////////////////////////
//////////////////////////GameRibbon/////////////////////////////////
/////////////////////////////////////////////////////////////////////
HWND AppendRibbonStatsElement(HWND hRibon, std::string text, int number, int x, int lenght)
{
    std::string elString = text;
    elString += std::to_string(number);

    std::wstring elWString = std::wstring(elString.begin(), elString.end());
    HWND h = CreateWindowW(L"static", elWString.c_str(), WS_VISIBLE | WS_CHILD, x, 0, lenght, RIBON_HEIGHT, hRibon, NULL, NULL, NULL);
    return h;
}

void SetRibbonStatsElementText(HWND handler, std::string text, int number)
{
    std::string elString = text;
    elString += std::to_string(number);

    std::wstring elWString = std::wstring(elString.begin(), elString.end());
    SetWindowTextW(handler, elWString.c_str());
}

std::string DWORDToString(DWORD value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string doubleToString(double value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

double GetPassedTimeInSeconds()
{
    DWORD currentTime = GetTickCount();
    DWORD passedSecondsDWORD = (currentTime - startTime)/1000;
    return (double)passedSecondsDWORD;
}

double GetClicksPerMinute()
{
    double minutesPassed = GetPassedTimeInSeconds() / 60;
    double result = playerScore / minutesPassed;
    return result;
}

void AppendGameStatusRibbon(HWND hParentWnd){
    int currX = 0;
    int length = 0;
    HWND hRibon = CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD, 0, 0, 1000, RIBON_HEIGHT, hParentWnd, NULL, NULL, NULL);

    length = 200;
    hScore = AppendRibbonStatsElement(hRibon, "Score: ", 0, currX, length);

    currX += (length+(100));
    hSpeed = AppendRibbonStatsElement(hRibon, "Speed: ", 0, currX, length);
}

void ReloadGameRibbon()
{
    SetRibbonStatsElementText(hScore, "Score: ", playerScore);
    SetRibbonStatsElementText(hSpeed, "Speed: ", (int)GetClicksPerMinute());
}
/////////////////////////////////////////////////////////////////////
//////////////////////////GameRibbon/////////////////////////////////
/////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////
///////////////////////////GameMap///////////////////////////////////
/////////////////////////////////////////////////////////////////////
void AddGameMap(HWND hParentWnd){
    AppendGameStatusRibbon(hParentWnd);
    CreateWindowW(L"button", L"Test button", WS_VISIBLE | WS_CHILD, 500, 500, 100, 50, hParentWnd, (HMENU)CLICK_PAD_CLICKED, NULL, NULL);
}
/////////////////////////////////////////////////////////////////////
///////////////////////////GameMap///////////////////////////////////
/////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////
///////////////////////////MENU//////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void AppendGameNewSubMenu(HMENU hParentMenu)
{
    HMENU hGameNewMenu = CreateMenu();
    AppendMenu(hParentMenu, MF_POPUP, (UINT_PTR)hGameNewMenu, "New");

    AppendMenu(hGameNewMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hGameNewMenu, MF_STRING, NULL, "Custom");
}

void AppendGameSubMenu(HMENU hParentMenu)
{
    HMENU hGameMenu = CreateMenu();
    AppendMenu(hParentMenu, MF_POPUP, (UINT_PTR)hGameMenu, "Game");

    AppendGameNewSubMenu(hGameMenu);
    AppendMenu(hGameMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hGameMenu, MF_STRING, NULL, "SaveSettings");
}

void AddMenus(HWND hWnd)
{
    hMenu = CreateMenu();

    AppendGameSubMenu(hMenu);

    AppendMenu(hMenu, MF_STRING, PARAM_MENU_EXIT, "Exit");

    SetMenu(hWnd, hMenu);
}
/////////////////////////////////////////////////////////////////////
///////////////////////////MENU//////////////////////////////////////
/////////////////////////////////////////////////////////////////////





void HandleWmCommand(WPARAM wParam){
    switch(wParam)
    {
        case PARAM_MENU_EXIT:
            PostQuitMessage (0);
            break;
        case CLICK_PAD_CLICKED:
            playerScore += 1;
            ReloadGameRibbon();
            break;
        case TEST:
            break;
    }
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_COMMAND:
            HandleWmCommand(wParam);
            break;
        case WM_CREATE:
            startTime = GetTickCount();
            AddMenus(hwnd);
            AddGameMap(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
