// Screenshot.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <stdio.h>
#include <windows.h>  
#include <stdlib.h>
#include "Screenshot.h"


void screenshot()
{

	int x = GetSystemMetrics(SM_XVIRTUALSCREEN);
	int y = GetSystemMetrics(SM_YVIRTUALSCREEN);
	int w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int h = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	HDC hScreenDC = GetDC(NULL);
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);	

	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, w, h);
	HGDIOBJ holdBitmap = SelectObject(hMemoryDC, hBitmap);


	BitBlt(hMemoryDC, 0, 0, w, h, hScreenDC, x, y, SRCCOPY);

	char filename[MAX_PATH];
	char* desktopPath = getenv("USERPROFILE");
	snprintf(filename, sizeof(filename), "%s\\Desktop\\Screenshots\\capture.bmp", desktopPath);

	char dirPath[MAX_PATH];
	snprintf(dirPath, sizeof(dirPath), "%s\\Desktop\\Screenshots", desktopPath);

	if (!CreateDirectoryA(dirPath, NULL)) {
	    DWORD err = GetLastError();
	    if (err != ERROR_ALREADY_EXISTS) {
	        fprintf(stderr, "CreateDirectoryA failed: %lu\n", err);
	    }
	}

   
    BITMAP bmp;
        BITMAPFILEHEADER bfh;
        BITMAPINFOHEADER bih;

        HDC hDC;
        char* lpBits;

        hDC = GetDC(NULL);
        GetObject(hBitmap, sizeof(BITMAP), &bmp);
        bih.biSize = sizeof(BITMAPINFOHEADER);
        bih.biWidth = bmp.bmWidth;
        bih.biHeight = bmp.bmHeight;
        bih.biPlanes = 1;
        bih.biBitCount = 32;
        bih.biCompression = BI_RGB;
        bih.biSizeImage = 0;
        bih.biXPelsPerMeter = 0;
        bih.biYPelsPerMeter = 0;
        bih.biClrUsed = 0;
        bih.biClrImportant = 0;

        DWORD dwBmpSize = ((bmp.bmWidth * bih.biBitCount + 31) / 32) * 4 * bmp.bmHeight;
        HANDLE hBmFile = CreateFileA(dirPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);
        lpBits = (char*)malloc(dwBmpSize);

        GetDIBits(hDC, hBitmap, 0, (UINT)bmp.bmHeight, lpBits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);
        bfh.bfType = 0x4D42;
        bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwBmpSize;
        bfh.bfReserved1 = 0;
        bfh.bfReserved2 = 0;
        bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

        DWORD dwWritten = 0;
        WriteFile(hBmFile, &bfh, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
        WriteFile(hBmFile, &bih, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
        WriteFile(hBmFile, lpBits, dwBmpSize, &dwWritten, NULL);

        CloseHandle(hBmFile);
        free(lpBits);
        ReleaseDC(NULL, hDC);


	SelectObject(hMemoryDC, holdBitmap);
	DeleteObject(hBitmap);
	DeleteDC(hMemoryDC);
	ReleaseDC(NULL, hScreenDC);

    
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
