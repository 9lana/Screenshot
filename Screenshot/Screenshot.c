// Screenshot.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <windows.h>    

int main()
{
    printf("Hello World\n");
	int x = GetSystemMetrics(SM_XVIRTUALSCREEN);
	int y = GetSystemMetrics(SM_YVIRTUALSCREEN);
	int w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int h = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	HDC hScreenDC = GetDC(NULL);
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, w, h);
	HGDIOBJ holdBitmap = SelectObject(hMemoryDC, hBitmap);

	printf("Capturing screen...\n");
	BitBlt(hmemoryDC, 0, 0, w, h, hScreenDC, x, y, SRCCOPY);

	char filename[MAX_PATH];
	char* desktopPath = getenv("USERPROFILE");
	snprintf(filename, sizeof(filename), "%s\\Desktop\\Screenshots\\capture.bmp");

	char fullPath[MAX_PATH];
	snprintf(fullPath, sizeof(fullPath), "mkdir \"%s\\Desktop\\Screenshots\"", desktopPath);
	system(fullPath);

	selectObject(hMemoryDC, holdBitmap);
	DeleteObject(hBitmap);
	DeleteDC(hMemoryDC);
	ReleaseDC(NULL, hScreenDC);


	printf("Taking screenshot...\n");
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
