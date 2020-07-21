// -*- coding:utf-8-with-signature -*-
// skkserv_test.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>

#include <string>
#include "common.h"

using namespace std;

void _DisconnectDic(HANDLE hPipe)
{
    if (hPipe != INVALID_HANDLE_VALUE) {
        CloseHandle(hPipe);
        // hPipe = INVALID_HANDLE_VALUE;
    }
}


// @return If failed, INVALID_HANDLE_VALUE.
HANDLE _ConnectDic(LPCWSTR mgrpipename)
{
    DWORD dwMode;

    if ( !WaitNamedPipe(mgrpipename, NMPWAIT_USE_DEFAULT_WAIT) )
        return INVALID_HANDLE_VALUE;

    HANDLE hPipe = CreateFile(mgrpipename,
            GENERIC_WRITE | GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            nullptr,
            OPEN_EXISTING,
            SECURITY_SQOS_PRESENT | SECURITY_EFFECTIVE_ONLY | SECURITY_IDENTIFICATION,
            nullptr);
    if (hPipe == INVALID_HANDLE_VALUE)
        return INVALID_HANDLE_VALUE;

    dwMode = PIPE_READMODE_MESSAGE | PIPE_WAIT;
    if (SetNamedPipeHandleState(hPipe, &dwMode, nullptr, nullptr) == FALSE) {
        _DisconnectDic(hPipe);
        return INVALID_HANDLE_VALUE;
    }

    return hPipe;
}


wstring _CreateIpcName()
{
    wstring r;
    LPWSTR pszUserUUID = nullptr;

    if (GetUserUUID(&pszUserUUID)) {
        r = wstring(IMCRVMGRPIPE) + pszUserUUID;
        LocalFree(pszUserUUID);
    }

    return r;
}


int main()
{
    std::cout << "Hello World!\n";
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント:
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
