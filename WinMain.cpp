#include <Windows.h>
#include <gdiplus.h>
#include "GameLogic.h"

#pragma comment(lib, "Gdiplus.lib")

using namespace Gdiplus;

const wchar_t gClassName[] = L"SolitaireWindowClass";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

// Test
solitaire::GameLogic gLogic;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	GdiplusStartupInput gsi;
	ULONG_PTR token;
	GdiplusStartup(&token, &gsi, nullptr);


	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed to register!", L"Error", MB_OK);
		return 0;
	}

	RECT wr = { 0,0,1024,768 };
	AdjustWindowRect(&wr, WS_OVERLAPPED | WS_SYSMENU, FALSE);
	HWND hwnd = CreateWindowEx(
		0,
		gClassName,
		L"Solitaire Game",
		WS_OVERLAPPED | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == nullptr)
	{
		MessageBox(nullptr, L"Failed to create!", L"Error", MB_OK);
		return 0;

	}
	gLogic.Init(hwnd); // 시점 직접 제어 가능

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	gLogic.Release();

	GdiplusShutdown(token);
	return static_cast<int>(msg.wParam);
}

void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	Graphics graphics(hdc);

	gLogic.Draw(graphics);

	EndPaint(hwnd, &ps);
}

LRESULT WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		gLogic.OnClick(LOWORD(lparam), HIWORD(lparam));
		break;

	case WM_PAINT:
		OnPaint(hwnd);
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}

	return 0;

}
