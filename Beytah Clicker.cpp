#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <winuser.h>
#include <thread>
#include <cstdlib>

int ColorConsoleThing = 1;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
int CurrentSlot = 1;
int maxCPS = 999;
int minCPS = 10;
POINT p;

bool SpecialKeys(int S_Key) {
    switch (S_Key) {
    case 49:
        return true;
    case 50:
        return true;
    case 51:
        return true;
    case 52:
        return true;
    case 53:
        return true;
    case 54:
        return true;
    case 55:
        return true;
    case 56:
        return true;
    case 57:
        return true;
    case VK_F4:
        std::exit(1);
        return false;
    default:
        return false;
    }
}

bool clicking = false;

bool rightclicking = false;

void ColorChange()
{
    SetConsoleTextAttribute(hConsole, ColorConsoleThing);
    ColorConsoleThing += 1;
    ColorConsoleThing = (ColorConsoleThing >= 15) ? 1 : ColorConsoleThing;
}

void KeyHandler()
{
    for (int KEY = 1; KEY <= 190; KEY++)
    {
        if (GetAsyncKeyState(KEY) == -32767 && GetAsyncKeyState(SpecialKeys(KEY)))
        {
            CurrentSlot = KEY - 48;
        }
    }
}

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_MOUSEWHEEL)
        {
            MSLLHOOKSTRUCT* pMhs = (MSLLHOOKSTRUCT*)lParam;
            short zDelta = HIWORD(pMhs->mouseData);
            if (zDelta > 0)
            {
                CurrentSlot++;
            }
            else
            {
                CurrentSlot--;
            }
        }
        else
        {
            if (wParam == WM_LBUTTONDOWN)
            {
                clicking = true;
            }
            else
            {
                if (wParam == WM_LBUTTONUP)
                {
                    clicking = false;
                }
                else
                {
                    if (wParam == WM_RBUTTONDOWN) {
                        rightclicking = true;
                    }
                    else
                    {
                        if (wParam == WM_RBUTTONUP) {
                            rightclicking = false;
                        }
                    }
                }
            }
        }
    }
    return CallNextHookEx(0, nCode, wParam, lParam);
}

HHOOK LLMouseHook;

void MouseHookThread()
{
    LLMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, NULL, NULL);
    MSG msg{ 0 };
    //our application loop
    while (GetMessage(&msg, NULL, 0, 0) != 0);
    UnhookWindowsHookEx(LLMouseHook);
}

void ClickHandler()
{
    if (clicking)
    {
        GetCursorPos(&p);
        SendMessage(GetForegroundWindow(), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(p.x, p.y));
        Sleep(1000 / ((rand() % (maxCPS - minCPS) + minCPS)));
        SendMessage(GetForegroundWindow(), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(p.x, p.y));
        Sleep(1000 / ((rand() % (maxCPS - minCPS) + minCPS)));
    }
}

int main()
{
    int CPS = 15;
    int SwordSlot = 1;
    SetConsoleTitle(_T("Beytah Clicker"));
    std::thread EpicThread1(MouseHookThread);
    while (true)
    {
        std::cout << "Beytah Clicker\n";
        std::cout << "Sigma Male Mode Activated\n";
        std::cout << "F4 to Close Autoclicker\n";
        std::cout << "Slot Selected: " << CurrentSlot << "\n";
        ColorChange();
        KeyHandler();
        ClickHandler();
        if (CurrentSlot < 1) CurrentSlot = 9;
        if (CurrentSlot > 9) CurrentSlot = 1;

        system("cls");
    }
}
