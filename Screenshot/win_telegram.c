#include <stdio.h>
#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

#include "win_telegram.h"
#define BOT_TOKEN "NOTE: REPLACE"
#define CHAT_ID "NOTE: REPLACE"

int send_telegram(char* text)
{
	HINTERNET hSession = InternetOpenA("WinTelegram", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (!hSession) {
		return -1;
	}

	HINTERNET hConnect = InternetConnectA(hSession, "api.telegram.org", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (!hConnect) {
		InternetCloseHandle(hSession);
		return -1;
	}
	char url_path[256];
	snprintf(url_path, sizeof(url_path), "/bot%s/sendMessage", BOT_TOKEN);
	HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", url_path, NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
		
	if (!hRequest) {
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
	}

	char* header = "Content-Type: application/x-www-form-urlencoded";
	char post_data[512];

	snprintf(post_data, sizeof(post_data), "chat_id=%s&text=%s", CHAT_ID, text);
	BOOL result = HttpSendRequestA(hRequest, header, (DWORD)strlen(header), post_data, (DWORD)strlen(post_data));
	if (!result) {
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return -1;
	}
	HttpEndRequest(hRequest, NULL, 0, 0);
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hSession);


	return 0;
}