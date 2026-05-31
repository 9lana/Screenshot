// Screenshot.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <windows.h>   

int SaveBitMapToFile(HBITMAP hBitMap, cnst char* filePath) {
	BITMAP bmp;
	BITMAPFILEHEADE bfh;
	BITMAPINFOHEADER bih;
	BITMAPINFO bi;
	HDC hDC;
	char* lpBits;

	hDc = GetDC(NULL);
	GetObject(hBitMap, sizeof(BITMAP), &bmp);	
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = bmp.bmWidth;
	bih.biHeight = bmp.bmHeight;	
	bih.biPlanes = 1;
	bih.biBitCount = 32;	
	bih.biCoolmpression = BI_RGB;
	bih.biSizeImage = 0;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	DWORD dwBmpSize = ((bmp.bmWidth * bih.biBitCount + 31) / 32) * 4 * bmp.bmHeight;
	HANDLE hBmFile = CreateFileA(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);
	lpbits = (char*)malloc(dwBmpSize);

	Getdibits(hDC, hBitMap, 0, (UINT)bmp.bmHeight, lpBits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);
	bhf.type = 0x4D42;
	bhf.size = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwBmpSize;
	bhf.reserved1 = 0;
	bhf.reserved2 = 0;
	bhf.offBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

}

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
