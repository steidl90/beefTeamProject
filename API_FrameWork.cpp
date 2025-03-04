﻿// API_FrameWork.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "API_FrameWork.h"
#include"mainGame.h"
HINSTANCE m_hInstance;
HWND m_hWnd;
POINT m_ptMouse = { 0,0 };
int zDelta = 0;

//메인게임
mainGame mg;



LPTSTR m_lpszClass = TEXT("히어로시즈");
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void setWindowSize(int x, int y, int width, int height);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam, int nCmdShow)
{
	m_hInstance = hInstance;

	WNDCLASS wndClass;


	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);


	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = m_lpszClass;
	wndClass.lpszMenuName = NULL;

	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndClass);


	m_hWnd = CreateWindow(
		WINNAME,
		WINNAME,
		WS_OVERLAPPEDWINDOW,
		WINSTARTX,
		WINSTARTY,
		WINSIZEX,
		WINSIZEY,
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);



	setWindowSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);
	ShowWindow(m_hWnd, nCmdShow);
	//메인게임 초기화 실패하면 종료
	if (FAILED(mg.init()))
	{
		return 0;
	}




	MSG message;




	//게임용 루프
	while (true)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{

			if (message.message == WM_QUIT)break;
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			TIME->update(60.0f);
			mg.update();
			mg.render();
		}
	}

	//MSG : 운영체제에서 발생하는 메세지 정보를 저장하기 위한 구조체
	//TranslateMessage : 키보드 입력 메세지 처리 담당
	//DispatchMessage : 윈ㄷ도우 프로시져에 전달된 메세지를 실제 윈도우에 전달
	//getMessage : 대기상태
	/*while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}*/


	//메시지 루프를 다돌면 해제하자.
	mg.release();

	return message.wParam;

}

//윈도우 프로시져 : 메세지를 운영체제에 전달한다. 강제로 운영체제가 호출
//hWnd : 윈도우가 발생한 메세지인지 구분
//imessage : 메세지 구분번호
//wParam : 마우스 버튼의 상태, 키보드 조합키의 상태를 전달한다.
//lParam :  마우스 클릭 좌표를 전달


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return mg.MainProc(hWnd, iMessage, wParam, lParam);
}

//위도우 크기조정 
//클라이언트 영역 사이즈 잡자
void setWindowSize(int x, int y, int width, int height)
{
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = width;
	rc.bottom = height;

	//위 RECT정보로 윈도우 사이즈 셋팅
	SetWindowPos(m_hWnd, NULL, x, y, (rc.right - rc.left), (rc.bottom - rc.top), SWP_NOZORDER | SWP_NOMOVE);
}