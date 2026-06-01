#include <stdio.h>
#include "Screenshot.h"
#include "win_telegram.h"

int main()
{
	printf("Starting screenshot capture...\n");
	screenshot();
	printf("Screenshot captured and saved to Desktop\\Screenshots\\capture.bmp\n");
	send_telegram("Screenshot captured and saved to Desktop\\Screenshots\\capture.bmp");

	return 0;
}