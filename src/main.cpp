﻿#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"


int hide_run_cmd(char* cmdline);
bool open_kcp_server(void);
bool open_start_app(void);
bool close_kcp_server(void);
bool open_wireguard_udp2raw(void);

void App_Initdialog(HWND & hwnd);

HBITMAP g_hBitmap_DONATE; // 打赏图片的句柄
HICON   g_hIcon;    // 对话框图标句柄

HINSTANCE hInst;


BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch (uMsg) {

    case WM_INITDIALOG: {
            App_Initdialog(hwndDlg); // 设置标题栏图标,// 设置图片

        }
        break;

    case WM_CLOSE: {
            EndDialog(hwndDlg, 0);
        }
        return TRUE;

    case WM_COMMAND: {
            switch (LOWORD(wParam)) {

            case OPEN_START_APP: {
                    open_start_app();
                    EndDialog(hwndDlg, 0);
                }
                break;

            case OPEN_KCP: {
                    open_kcp_server();
                    EndDialog(hwndDlg, 0);
                }
                break;

            case OPEN_UDP2RAW: {
                    open_wireguard_udp2raw();
                    EndDialog(hwndDlg, 0);
                }
                break;

            case CLOSE_KCP: {
                    close_kcp_server();
                //  EndDialog(hwndDlg, 0);
                }
                break;

                return TRUE;
            }
        }
        return TRUE;
    }
    return FALSE;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst = hInstance;
//   InitCommonControls();

    // 从资源中加载BMP文件和图标，这些工作也可以在WM_INITDIALOG消息中进行
    g_hBitmap_DONATE = ::LoadBitmap(hInst, (LPCTSTR)IDB_BITMAP1);
    g_hIcon = ::LoadIcon(hInst, (LPCTSTR)IDI_ICON1);

    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}


bool open_start_app(void)
{
    char cmdline[] = "cmd /c START_APP.cmd";
    hide_run_cmd(cmdline);

    return true;
}



bool open_kcp_server(void)
{
    char cmdline[] = "cmd /c Windows_KCP.cmd";
    hide_run_cmd(cmdline);

    return true;
}

bool open_wireguard_udp2raw(void)
{
    char cmdline[] = "cmd /c Windows_UDP2RAW.cmd";
    hide_run_cmd(cmdline);

    return true;
}


bool close_kcp_server(void)
{
    char cmdline[] = "taskkill.exe /im kcp-client.exe  /f /im udp2raw.exe";
    hide_run_cmd(cmdline);

    return true;
}


// 后台执行命令行函数
int hide_run_cmd(char* cmdline)
{
    SetConsoleTitle(cmdline);
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    // 后台隐藏
    si.dwFlags   =   STARTF_USESHOWWINDOW;
    si.wShowWindow   =   SW_HIDE;

    ZeroMemory(&pi, sizeof(pi));
    // Start the child process.
    CreateProcess(NULL, TEXT(cmdline), NULL, NULL, FALSE, 0,
                  NULL, NULL, &si, &pi);
    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);
    // Get the return value of the child process
    DWORD ret;
    GetExitCodeProcess(pi.hProcess, &ret);
//    if (!ret) {
//        // .....
//    }
    // Close process and thread handles.
//    CloseHandle(pi.hProcess);
//    CloseHandle(pi.hThread);
    return ret;
}


void App_Initdialog(HWND & hwnd)
{

    // 大图标：按下alt+tab键切换窗口时对应的图标
    // 小图标：就是窗口左上角对应的那个图标
    ::SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)g_hIcon);
    ::SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)g_hIcon);

    // 设置图片
    ::SendDlgItemMessage(hwnd, DONATE_PIC, STM_SETIMAGE, IMAGE_BITMAP, (long)g_hBitmap_DONATE);




}
