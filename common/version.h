#pragma once

#define TEXTSERVICE_NAME	L"Netsphere MiniIME"
#ifndef _DEBUG
#define TEXTSERVICE_DESC	TEXTSERVICE_NAME
#else
#define TEXTSERVICE_DESC	TEXTSERVICE_NAME L"_DEBUG"
#endif

// C:\Windows\IME\MiniIME
#define TEXTSERVICE_DIR		L"MiniIME"
#define TEXTSERVICE_VER		L"0.1.0"

//for resource
#define RC_AUTHOR			"Netsphere Laboratories"
#define RC_PRODUCT			"Netsphere MiniIME"
#define RC_VERSION			"0.1.0"
#define RC_VERSION_D		0,1,0,0
