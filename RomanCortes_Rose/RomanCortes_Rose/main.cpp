////////////////////////////////////////////////////////////////////
// 程序名称：情人节的玫瑰
// 原 作 者：西班牙程序员 Roman Cortes
// 原 程 序：http://js1k.com/2012-love/demo/1022 (javascript 版本)
// 编译环境：VC6.0 / VC2010，EasyX_20210730
// 移植作者：krissi <zh@easyx.cn>
// 最后修改：2012-02-13
// 编译环境：VC2013
// 二次移植作者：xy
// 最后修改：2022-11-21
// 注：程序中的很多精简变量名都沿用原 javascript 中的变量名。
// 原作者 Roman Cortes 对这段程序的讲解贴在了自己的博客：http://www.romancortes.com/blog/1k-rose/ ，想要学习的同学可以点过去看看。
//
// 推荐两个网址
// https://easyx.cn/
// https://codebus.cn/zhaoh/rose-for-valentine-s-day

#include <Windows.h>
#include <math.h>

// 定义结构体
struct DOT
{
	double x;
	double y;
	double z;
	double r;	// 红色
	double g;	// 绿色
				// b(蓝色) 通过 r 计算
};

// 定义全局变量
int	rosesize = 500;
int	h = -250;

short* zBuffer;
UINT idtimer_;

// 计算点
bool calc(double a, double b, double c, DOT *d)
{
	double j, n, o, w, z;

	if (c > 60)				// 花柄
	{
		d->x = sin(a * 7) * (13 + 5 / (0.2 + pow(b * 4, 4))) - sin(b) * 50;
		d->y = b * rosesize + 50;
		d->z = 625 + cos(a * 7) * (13 + 5 / (0.2 + pow(b * 4, 4))) + b * 400;
		d->r = a * 1 - b / 2;
		d->g = a;
		return true;
	}

	double A = a * 2 - 1;
	double B = b * 2 - 1;
	if (A * A + B * B < 1)
	{
		if (c > 37)			// 叶
		{
			j = (int(c) & 1);
			n = j ? 6 : 4;
			o = 0.5 / (a + 0.01) + cos(b * 125) * 3 - a * 300;
			w = b * h;

			d->x = o * cos(n) + w * sin(n) + j * 610 - 390;
			d->y = o * sin(n) - w * cos(n) + 550 - j * 350;
			d->z = 1180 + cos(B + A) * 99 - j * 300;
			d->r = 0.4 - a * 0.1 + pow(1 - B * B, -h * 6) * 0.15 - a * b * 0.4 + cos(a + b) / 5 + pow(cos((o * (a + 1) + (B > 0 ? w : -w)) / 25), 30) * 0.1 * (1 - B * B);
			d->g = o / 1000 + 0.7 - o * w * 0.000003;
			return true;
		}
		if (c > 32)			// 花萼
		{
			c = c * 1.16 - 0.15;
			o = a * 45 - 20;
			w = b * b * h;
			z = o * sin(c) + w * cos(c) + 620;

			d->x = o * cos(c) - w * sin(c);
			d->y = 28 + cos(B * 0.5) * 99 - b * b * b * 60 - z / 2 - h;
			d->z = z;
			d->r = (b * b * 0.3 + pow((1 - (A * A)), 7) * 0.15 + 0.3) * b;
			d->g = b * 0.7;
			return true;
		}

		// 花
		o = A * (2 - b) * (80 - c * 2);
		w = 99 - cos(A) * 120 - cos(b) * (-h - c * 4.9) + cos(pow(1 - b, 7)) * 50 + c * 2;
		z = o * sin(c) + w * cos(c) + 700;

		d->x = o * cos(c) - w * sin(c);
		d->y = B * 99 - cos(pow(b, 7)) * 50 - c / 3 - z / 1.35 + 450;
		d->z = z;
		d->r = (1 - b / 1.2) * 0.9 + a * 0.1;
		d->g = pow((1 - b), 20) / 4 + 0.05;
		return true;
	}

	return false;
}

VOID WindowOnCreate(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	// 初始化 z-buffer
	zBuffer = new short[rosesize * rosesize];
}
VOID WindowOnDestroy(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	if (idtimer_)
	{
		KillTimer(hwnd, idtimer_);
		idtimer_ = 0;
	}

	// 退出
	delete[]zBuffer;
}
VOID WindowOnTimer(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	HDC hdc;
	DOT		dot;
	int		x, y, z, zBufferIndex;

	hdc = GetDC(hwnd);

	for (int i = 0; i < 10000; i++)
	{
		if (calc(double(rand()) / RAND_MAX, double(rand()) / RAND_MAX, rand() % 46 / 0.74, &dot))
		{
			z = int(dot.z + 0.5);
			x = int(dot.x * rosesize / z - h + 0.5);
			y = int(dot.y * rosesize / z - h + 0.5);
			if (y >= rosesize) continue;

			zBufferIndex = y * rosesize + x;

			if (!zBuffer[zBufferIndex] || zBuffer[zBufferIndex] > z)
			{
				zBuffer[zBufferIndex] = z;

				// 画点
				int r = ~int((dot.r * h));				if (r < 0) r = 0;	if (r > 255) r = 255;
				int g = ~int((dot.g * h));				if (g < 0) g = 0;	if (g > 255) g = 255;
				int b = ~int((dot.r * dot.r * -80));	if (b < 0) b = 0;	if (b > 255) b = 255;

				SetPixel(hdc, x + 50, y - 20, RGB(r, g, b));
			}
		}
	}

	ReleaseDC(hwnd, hdc);
}
VOID WindowOnLButtonDown(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	srand(GetTickCount());

	memset(zBuffer, 0, sizeof(short) * rosesize * rosesize);

	if (idtimer_)
	{
		KillTimer(hwnd, idtimer_);
	}
	idtimer_ = SetTimer(hwnd, 1, 10, NULL);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	LRESULT result = NULL;

	switch (message)
	{
	case WM_CREATE:
		WindowOnCreate(hwnd, wparam, lparam);
		break;
	case WM_LBUTTONDOWN:
		WindowOnLButtonDown(hwnd, wparam, lparam);
		break;
	case WM_DESTROY:
		WindowOnDestroy(hwnd, wparam, lparam);
		PostQuitMessage(0);
		break;
	case WM_TIMER:
		WindowOnTimer(hwnd, wparam, lparam);
		break;
	default:
		break;
	}

	if (result == NULL)
	{
		result = DefWindowProc(hwnd, message, wparam, lparam);
	}

	return(result);
}
int APIENTRY WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASSEX winclass;
	LONG width, height;

	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS | CS_OWNDC;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = L"RomanCortesRoseClass";

	RegisterClassEx(&winclass);

	width = 800;
	height = 510;
	hwnd = CreateWindowEx(NULL,
		winclass.lpszClassName,
		L"Roman Cortes",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		(GetSystemMetrics(SM_CXSCREEN) - width) >> 1, (GetSystemMetrics(SM_CYSCREEN) - height) >> 1,
		width, height,
		NULL,
		NULL,
		hinstance,
		NULL);

	if (hwnd != NULL)
	{
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	UnregisterClass(winclass.lpszClassName, hinstance);

	return msg.wParam;
}
//---------------------------------------------------------------------------