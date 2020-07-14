/**************************************************************************
   THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY OF
   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
   PARTICULAR PURPOSE.

   Copyright 2001 Microsoft Corporation.  All Rights Reserved.
**************************************************************************/

/**************************************************************************

   File:          TSFApp.cpp

   Description:   Main Windows App Implementation

**************************************************************************/

/**************************************************************************
   Include Files
**************************************************************************/

#include "TSFWnd.h"

/**************************************************************************
   Local Function Prototypes
**************************************************************************/


/**************************************************************************
   Global Variables
**************************************************************************/

ITfThreadMgr    *g_pThreadMgr = nullptr;
CTextService* g_textService = nullptr;


// @return WM_QUIT の wParam の値
int main_loop()
{
    assert( g_pThreadMgr );
  
    // TSFは、アプリケーションのメッセージポンプの前に、処理を行う
    // => ITfMessagePump を使うことで, TSFの処理をブロックできる.
    bool use_tsf_message_pump = true;
    ITfKeystrokeMgr* pKeyMgr = nullptr;
    ITfMessagePump* pMsgPump = nullptr;

    // get the keystroke manager interfce
    if (FAILED(g_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr,
               (void**)&pKeyMgr)))
        use_tsf_message_pump = false;
    else {
        if (FAILED(g_pThreadMgr->QueryInterface(IID_ITfMessagePump,
                   (void**)&pMsgPump))) {
            use_tsf_message_pump = false;
            pKeyMgr->Release();
            pKeyMgr = nullptr;
        }
    }

    MSG     msg;
    while (TRUE) {
        BOOL fResult;

        msg.message = 0;
        if (use_tsf_message_pump) {
            BOOL    fEaten;

            /*
            Get the next message in the queue. fResult receives FALSE if WM_QUIT is encountered
            */
            fResult = TRUE;
            if (FAILED(pMsgPump->GetMessage(&msg, NULL, 0, 0, &fResult)))
                fResult = -1;
            if (fResult <= 0) // When WM_QUIT, 0
                break;

            if (WM_KEYDOWN == msg.message) {
                //continue;
                // does an ime want it?
                if (pKeyMgr->TestKeyDown(msg.wParam, msg.lParam, &fEaten) == S_OK && fEaten &&
                    pKeyMgr->KeyDown(msg.wParam, msg.lParam, &fEaten) == S_OK && fEaten)
                {
                    continue;
                }
            }
            else if (WM_KEYUP == msg.message) {
                //continue;
                // does an ime want it?
                if (pKeyMgr->TestKeyUp(msg.wParam, msg.lParam, &fEaten) == S_OK && fEaten &&
                    pKeyMgr->KeyUp(msg.wParam, msg.lParam, &fEaten) == S_OK && fEaten)
                {
                    continue;
                }
            }
        }
        else {
            fResult = GetMessage(&msg, NULL, 0, 0);
            if (fResult <= 0)
                break;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (pKeyMgr)
        pKeyMgr->Release();
    if (pMsgPump)
        pMsgPump->Release();

    if (WM_QUIT == msg.message)
        return (int)msg.wParam;
    else
        return 0;
}


/**************************************************************************

   WinMain()

**************************************************************************/
int WINAPI WinMain( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int nCmdShow )
{
    CTSFMainWnd *pMainWnd;
    int         nReturn = 0;

    CoInitialize(NULL);

    pMainWnd = new CTSFMainWnd(hInstance);
    if(NULL == pMainWnd)
        return 0;
    assert( g_pThreadMgr );
    
    g_textService = new CTextService();
    g_textService->ActivateEx(g_pThreadMgr, pMainWnd->m_tfClientID, 0);
    
    if (pMainWnd->Initialize(nCmdShow))
        nReturn = main_loop(pMainWnd->m_pTSFEditWnd);

    delete g_textService;
    delete pMainWnd;
    CoUninitialize();

    return nReturn;
}

