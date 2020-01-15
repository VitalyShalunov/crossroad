#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <math.h>
#include <iterator>
#include <conio.h>
#include <time.h>
#include <vld.h>
#include "crossroad optim.h"

using namespace std;

int iWidth = GetSystemMetrics(SM_CXSCREEN);  // разрешение экрана по горизонтали
int iHeight = GetSystemMetrics(SM_CYSCREEN); // разрешение экрана по вертикали
HWND hWnd;
FILE* bestr;//best record
int check = 1;//проверка, аварий или пробок
int menu = 1;//провекра, зашли ли мы в меню
int click = 0;//проверка, находимся ли мы в игре
int end_game = 1;//конец игры
int del = 0;//разрешение удалять компоненты
int best = 0;//лучшие результаты
int total = 0;
int besttime;
int instr = 0;//проерка, зашли ли мы в инструкции

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
class TrafficLight;
class TrafficLight {//светофор
public:
	RECT border;
	class Circle {//огонёк
		int radius, xPos, yPos;
		int number;//порядковый номер
		HDC hdc;
	public:
		Circle(int num);
		int color;
		void setColor(int color)
		{
			this->color = color;
		}
		void setPos(int next)
		{
			int kUmn;
			if (next) // -> зеленый
			{
				kUmn = 1;
			}
			else
			{
				kUmn = -1;
			}
			switch (number)
			{
			case 1:
				this->yPos = this->yPos - kUmn * 52;
				break;
			case 2:
				this->xPos = this->xPos + kUmn * 52;
				break;
			case 3:
				this->xPos = this->xPos - kUmn * 52;
				break;
			case 4:
				this->yPos = this->yPos + kUmn * 52;
				break;
			default:
				break;
			}
		}
		void drawCircle()
		{
			hdc = GetDC(hWnd);
			HRGN circle = CreateEllipticRgn(this->xPos - radius, this->yPos - radius, this->xPos + radius, this->yPos + radius); //создание анимации
			HBRUSH hBrush;
			switch (this->color)
			{
			case 1:
				hBrush = CreateSolidBrush(RGB(255, 0, 0)); //красный 
				break;
			case 2:
				hBrush = CreateSolidBrush(RGB(232, 228, 19)); //желтый
				break;
			case 3:
				hBrush = CreateSolidBrush(RGB(92, 196, 27)); //зеленый
				break;
			default:
				hBrush = CreateSolidBrush(RGB(255, 0, 0));
				break;
			}
			FillRgn(hdc, circle, hBrush);
			DeleteObject(hBrush);
			ReleaseDC(hWnd, hdc);
		}
		void clearCircle()
		{
			hdc = GetDC(hWnd);
			HRGN circle = CreateEllipticRgn(this->xPos - radius, this->yPos - radius, this->xPos + radius, this->yPos + radius); //создание анимации
			HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
			FillRgn(hdc, circle, hBrush);
			DeleteObject(hBrush);
			ReleaseDC(hWnd, hdc);
		}
	};
private:
	int number, color;
	Circle* circle;
	HDC hdc;
	RECT getBorder()
	{
		return this->border;
	}
public:
	TrafficLight(int num) {
		this->number = num;
		this->color = 0;
		int lenx, leny;
		switch (num)//формируем координаты светофоров
		{
		case 1:
			lenx = -200, leny = -250;
			border.left = iWidth / 2 - 37 + lenx; border.right = iWidth / 2 + lenx + 37;
			border.top = iHeight / 2 + leny - 82; border.bottom = iHeight / 2 + leny + 82;
			break;
		case 2:
			lenx = 250, leny = -200;
			border.left = iWidth / 2 - 82 + lenx; border.right = iWidth / 2 + lenx + 82;
			border.top = iHeight / 2 + leny - 37; border.bottom = iHeight / 2 + leny + 37;
			break;
		case 3:
			lenx = -250, leny = 200;
			border.left = iWidth / 2 - 82 + lenx; border.right = iWidth / 2 + lenx + 82;
			border.top = iHeight / 2 + leny - 37; border.bottom = iHeight / 2 + leny + 37;
			break;
		case 4:
			lenx = 200, leny = 250;
			border.left = iWidth / 2 - 37 + lenx; border.right = iWidth / 2 + lenx + 37;
			border.top = iHeight / 2 + leny - 82; border.bottom = iHeight / 2 + leny + 82;
			break;
		}
	}
	int getColor()
	{
		return this->circle->color;
	}
	void createCircle()
	{
		circle = new Circle(this->number);
	}
	void drawOwn()
	{
		hdc = GetDC(hWnd);
		HBRUSH hBrush = CreateSolidBrush(RGB(79, 75, 75));
		FillRect(hdc, &border, hBrush);
		DeleteObject(hBrush);

		border.left += 5; border.right -= 5;
		border.top += 5; border.bottom -= 5;
		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		FillRect(hdc, &border, hBrush);
		DeleteObject(hBrush);
		ReleaseDC(hWnd, hdc);
		circle->drawCircle();
	}
	void changeColor(int color)
	{
		if (color != this->color)
		{
			if (this->getColor() == 1)
			{
				for (int i = 2; i <= 3; i++)
				{
					circle->setColor(i);
					circle->clearCircle();
					circle->setPos(1);
					circle->drawCircle();
				}
			}
			else {
				for (int i = 2; i >= 1; i--)
				{
					circle->setColor(i);
					circle->clearCircle();
					circle->setPos(0);
					circle->drawCircle();
				}
			}
		}
	}
};

TrafficLight* light1 = new TrafficLight(1);
TrafficLight* light2 = new TrafficLight(2);
TrafficLight* light3 = new TrafficLight(3);
TrafficLight* light4 = new TrafficLight(4);



TrafficLight::Circle::Circle(int num) {
	this->radius = 23;
	this->number = num;
	this->color = 1;
	RECT r;
	switch (this->number)
	{
	case 1:
		this->xPos = (light1->border.right + light1->border.left) / 2;
		r = light1->getBorder();
		yPos = r.bottom - 7 - 23;
		break;
	case 2:
		this->yPos = (light2->border.bottom + light2->border.top) / 2;
		r = light2->getBorder();
		xPos = r.left + 7 + 23;
		break;
	case 3:
		this->yPos = (light3->border.bottom + light3->border.top) / 2;
		r = light3->getBorder();
		xPos = r.right - 7 - 23;
		break;
	case 4:
		this->xPos = (light4->border.left + light4->border.right) / 2;
		r = light4->getBorder();
		yPos = r.top + 7 + 23;
		break;
	}
}

class member_of_traffic { //учаcтники движения	
public:
	int getlength()
	{
		return this->length;
	}

	int getwidth()
	{
		return this->width;
	}

	member_of_traffic* getnext()
	{
		return this->next;
	}

	void setnext(member_of_traffic* next)
	{
		this->next = next;
	}

	int getspeed_now()
	{
		return this->speed_now;
	}

	void setspeed_now(int speed_now)
	{
		this->speed_now = speed_now;
	}

	void ride();

	int getx() //получить координату по Х
	{
		return this->x;
	}

	int gety() // получить кординату по У
	{
		return this->y;
	}

	void sety(int y)
	{
		this->y += y;
	}

	void setx(int x)
	{
		this->x += x;
	}

	int getdestination() //получить направление движения
	{
		//if (this!=NULL)
		return this->destination;
	}

	int gettype()
	{
		return this->type;
	}

	int getdistance_to_next() //получить расстояние до следующего автомобиля
	{
		if (this->next != NULL)
		{
			switch (next->getdestination())
			{
			case 1:
				if (next->gety() - next->getlength() < 0)
					return 0;
				else return next->gety() - next->getlength() - this->gety();
				break;
			case 2:
				if (next->getx() + next->getlength() > iWidth)
					return 0;
				else
					return abs(next->getx() + next->getlength() - this->getx());
				break;
			case 3:
				if (next->getx() - next->getlength() < 0)
					return 0;
				return next->getx() - next->getlength() - this->getx();
				break;
			case 4:
				if (next->gety() + next->getlength() > iHeight)
					return 0;
				return abs(next->gety() + next->getlength() - this->gety());
				break;
			}
		}
		else return 40;
	}

	int getmax_speed()
	{
		return this->max_speed;
	}

	int getspeednext(member_of_traffic* next)
	{
		return next->speed_now;
	}
protected:
	int type;
	int color1, color2, color3; //увет участника движения
	int x, y; //координаты 
	int width, length; //ширина и длина
	int max_speed; //макимальная скорость
	int speed_now; //скорость в данный момент
	int destination; //направление 1..4
	//member_of_traffic*	previous;
	member_of_traffic* next;
public:
	virtual void draw_own() = 0;
	virtual	void remove_own() = 0;
};

class light_car :public member_of_traffic //легковой автомобиль
{
public:
	light_car()
	{
		this->width = 55;
		this->length = 80;
		if (rand() % 6 == 1)
			this->destination = 1;
		else
			if (rand() % 7 == 1)
				this->destination = 4;
			else
				this->destination = rand() % 2 + 2;
		this->color1 = rand() % 255;
		this->color2 = rand() % 255;
		this->color3 = rand() % 255;
		this->type = 1;
		this->speed_now = 7;
		this->max_speed = 7;
		this->next = NULL;
		switch (this->destination) //начальные кординаты в зависимости от направления движения
		{
		case 1:
			this->x = iWidth / 2 - 105 + this->width;
			this->y = 15;
			break;
		case 2:
			this->x = iWidth - 15;
			this->y = iHeight / 2 - 105;
			break;
		case 3:
			this->x = 15;
			this->y = iHeight / 2 + 105;
			break;
		case 4:
			this->x = iWidth / 2 + 105 - this->width;
			this->y = iHeight - 15;
			break;
		}
	}
protected:
	void draw_own() override {  //координаты в настоящий момент

		HBRUSH hbrush;
		hbrush = CreateSolidBrush(RGB(color1, color2, color3));
		HDC hdc = GetDC(hWnd);
		RECT r;

		switch (this->destination) //в зависимости от направления нарисовать себя
		{
		case 1:

			r.left = iWidth / 2 - 105;
			r.right = iWidth / 2 - 105 + this->width;
			r.top = this->y - this->length;
			r.bottom = this->y;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 2:

			r.left = this->x;
			r.right = this->x + this->length;
			r.top = iHeight / 2 - 105;
			r.bottom = iHeight / 2 - 105 + this->width;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 3:

			r.left = this->x - this->length;
			r.right = this->x;
			r.top = iHeight / 2 + 105 - this->width;
			r.bottom = iHeight / 2 + 105;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 4:

			r.left = iWidth / 2 + 105 - this->width;
			r.right = iWidth / 2 + 105;
			r.top = this->y;
			r.bottom = this->y + this->length;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;

		}
		ReleaseDC(hWnd, hdc);
	}
	void remove_own() override { //стереть себя
		HDC hdc = GetDC(hWnd);
		RECT r;

		HBRUSH hbrush = CreateSolidBrush(RGB(133, 137, 143));

		HRGN hRgnUpper;

		switch (this->destination) //в зависимости от направления нарисовать себя
		{
		case 1:

			r.left = iWidth / 2 - 105;
			r.right = iWidth / 2 - 105 + this->width;
			r.top = this->y - this->length;
			r.bottom = this->y;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 2:

			r.left = this->x;
			r.right = this->x + this->length;
			r.top = iHeight / 2 - 105;
			r.bottom = iHeight / 2 - 105 + this->width;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 3:

			r.left = this->x - this->length;
			r.right = this->x;
			r.top = iHeight / 2 + 105 - this->width;
			r.bottom = iHeight / 2 + 105;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 4:

			r.left = iWidth / 2 + 105 - this->width;
			r.right = iWidth / 2 + 105;
			r.top = this->y;
			r.bottom = this->y + this->length;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;

		}
		ReleaseDC(hWnd, hdc);
	}
};

class bus :public member_of_traffic
{
public:
	bus() {
		this->width = 60;
		this->length = 110;
		if (rand() % 7 == 1)
			this->destination = 1;
		else
			if (rand() % 9 == 1)
				this->destination = 4;
			else
				this->destination = rand() % 2 + 2;
		this->color1 = rand() % 255;
		this->color2 = rand() % 255;
		this->color3 = rand() % 255;
		this->type = 2;
		this->speed_now = 5;
		this->max_speed = 5;
		this->next = NULL;
		switch (this->destination) //начальные кординаты в зависимости от направления движения
		{
		case 1:
			this->x = iWidth / 2 - 105 + this->width;
			this->y = 15;
			break;
		case 2:
			this->x = iWidth - 15;
			this->y = iHeight / 2 - 105;
			break;
		case 3:
			this->x = 15;
			this->y = iHeight / 2 + 105;
			break;
		case 4:
			this->x = iWidth / 2 + 105 - this->width;
			this->y = iHeight - 15;
			break;
		}
	}
	void draw_own() override {  //координаты в настоящий момент
		HBRUSH hbrush;
		hbrush = CreateSolidBrush(RGB(color1, color2, color3));
		HDC hdc = GetDC(hWnd);
		RECT r;
		switch (this->destination) //в зависимости от направления нарисовать себя
		{
		case 1:
			r.left = iWidth / 2 - 105;
			r.right = iWidth / 2 - 105 + this->width;
			r.top = this->y - this->length;
			r.bottom = this->y;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 2:
			r.left = this->x;
			r.right = this->x + this->length;
			r.top = iHeight / 2 - 105;
			r.bottom = iHeight / 2 - 105 + this->width;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 3:
			r.left = this->x - this->length;
			r.right = this->x;
			r.top = iHeight / 2 + 105 - this->width;
			r.bottom = iHeight / 2 + 105;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 4:
			r.left = iWidth / 2 + 105 - this->width;
			r.right = iWidth / 2 + 105;
			r.top = this->y;
			r.bottom = this->y + this->length;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		}
		ReleaseDC(hWnd, hdc);
	}
	void remove_own() override { //стереть себя
		HDC hdc = GetDC(hWnd);
		RECT r;
		HBRUSH hbrush = CreateSolidBrush(RGB(133, 137, 143));
		HRGN hRgnUpper;
		switch (this->destination) //в зависимости от направления нарисовать себя
		{
		case 1:
			r.left = iWidth / 2 - 105;
			r.right = iWidth / 2 - 105 + this->width;
			r.top = this->y - this->length;
			r.bottom = this->y;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 2:
			r.left = this->x;
			r.right = this->x + this->length;
			r.top = iHeight / 2 - 105;
			r.bottom = iHeight / 2 - 105 + this->width;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 3:
			r.left = this->x - this->length;
			r.right = this->x;
			r.top = iHeight / 2 + 105 - this->width;
			r.bottom = iHeight / 2 + 105;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 4:
			r.left = iWidth / 2 + 105 - this->width;
			r.right = iWidth / 2 + 105;
			r.top = this->y;
			r.bottom = this->y + this->length;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		}
		ReleaseDC(hWnd, hdc);
	}
};

class truck :public member_of_traffic
{
public:
	truck() {
		width = 70;
		length = 100;
		if (rand() % 7 == 1)
			this->destination = 1;
		else
			if (rand() % 6 == 1)
				this->destination = 4;
			else
				this->destination = rand() % 2 + 2;
		this->color1 = rand() % 255;
		this->color2 = rand() % 255;
		this->color3 = rand() % 255;
		this->type = 3;
		this->speed_now = 6;
		this->max_speed = 6;
		this->next = NULL;
		switch (this->destination) //начальные кординаты в зависимости от направления движения
		{
		case 1:
			this->x = iWidth / 2 - 105 + this->width;
			this->y = 15;
			break;
		case 2:
			this->x = iWidth - 15;
			this->y = iHeight / 2 - 105;
			break;
		case 3:
			this->x = 15;
			this->y = iHeight / 2 + 105;
			break;
		case 4:
			this->x = iWidth / 2 + 105 - this->width;
			this->y = iHeight - 15;
			break;
		}
	}
	void draw_own() override {  //координаты в настоящий момент
		HBRUSH hbrush;
		hbrush = CreateSolidBrush(RGB(color1, color2, color3));
		HDC hdc = GetDC(hWnd);
		RECT r;
		switch (this->destination) //в зависимости от направления нарисовать себя
		{
		case 1:
			r.left = iWidth / 2 - 105;
			r.right = iWidth / 2 - 105 + this->width;
			r.top = this->y - this->length;
			r.bottom = this->y;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 2:
			r.left = this->x;
			r.right = this->x + this->length;
			r.top = iHeight / 2 - 105;
			r.bottom = iHeight / 2 - 105 + this->width;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 3:
			r.left = this->x - this->length;
			r.right = this->x;
			r.top = iHeight / 2 + 105 - this->width;
			r.bottom = iHeight / 2 + 105;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 4:
			r.left = iWidth / 2 + 105 - this->width;
			r.right = iWidth / 2 + 105;
			r.top = this->y;
			r.bottom = this->y + this->length;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		}
		ReleaseDC(hWnd, hdc);
	}
	void remove_own() override { //стереть себя
		HDC hdc = GetDC(hWnd);
		RECT r;
		HBRUSH hbrush = CreateSolidBrush(RGB(133, 137, 143));
		switch (this->destination) //в зависимости от направления нарисовать себя
		{
		case 1:
			r.left = iWidth / 2 - 105;
			r.right = iWidth / 2 - 105 + this->width;
			r.top = this->y - this->length;
			r.bottom = this->y;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 2:
			r.left = this->x;
			r.right = this->x + this->length;
			r.top = iHeight / 2 - 105;
			r.bottom = iHeight / 2 - 105 + this->width;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 3:
			r.left = this->x - this->length;
			r.right = this->x;
			r.top = iHeight / 2 + 105 - this->width;
			r.bottom = iHeight / 2 + 105;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 4:
			r.left = iWidth / 2 + 105 - this->width;
			r.right = iWidth / 2 + 105;
			r.top = this->y;
			r.bottom = this->y + this->length;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		}
		ReleaseDC(hWnd, hdc);
	}
};

class spec_car :public member_of_traffic //спец.транспорт будет иметь особое поведение
{
public:
	spec_car()
	{
		width = 55;
		length = 80;
		if (rand() % 7 == 1)
			this->destination = 1;
		else
			if (rand() % 7 == 1)
				this->destination = 4;
			else
				this->destination = rand() % 2 + 2;
		this->type = 4;
		this->next = NULL;
		switch (rand() % 3)
		{
		case 0:
			this->color1 = 189;
			this->color2 = 21;
			this->color3 = 21;
			break;
		case 1:
			this->color1 = 247;
			this->color2 = 240;
			this->color3 = 240;
			break;
		case 2:
			this->color1 = 35;
			this->color2 = 62;
			this->color3 = 196;
			break;
		}
		switch (this->destination) //начальные кординаты в зависимости от направления движения
		{
		case 1:
			this->x = iWidth / 2 + (this->width) / 2;
			this->y = 15;
			this->speed_now = 5;
			break;
		case 2:
			this->x = iWidth - 15;
			this->y = iHeight / 2 - (this->width) / 2;
			this->speed_now = 8;
			break;
		case 3:
			this->x = 15;
			this->y = iHeight / 2 + (this->width) / 2;
			this->speed_now = 8;
			break;
		case 4:
			this->x = iWidth / 2 - (this->width) / 2;
			this->y = iHeight - 15;
			this->speed_now = 4;
			break;
		}
	}
protected:
	void draw_own() override {  //координаты в настоящий момент
		HBRUSH hbrush;
		hbrush = CreateSolidBrush(RGB(color1, color2, color3));
		HDC hdc = GetDC(hWnd);
		RECT r;
		switch (this->destination) //в зависимости от направления нарисовать себя
		{
		case 1:
			r.left = this->x - this->width;
			r.right = this->x;
			r.top = this->y - this->length;
			r.bottom = this->y;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 2:
			r.left = this->x;
			r.right = this->x + this->length;
			r.top = this->y;
			r.bottom = this->y + this->width;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 3:
			r.left = this->x - this->length;
			r.right = this->x;
			r.top = this->y - this->width;
			r.bottom = this->y;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 4:
			r.left = this->x;
			r.right = this->x + this->width;
			r.top = this->y;
			r.bottom = this->y + this->length;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		}
		ReleaseDC(hWnd, hdc);
	}

	void remove_own() override { //стереть себя
		HDC hdc = GetDC(hWnd);
		RECT r;
		HBRUSH hbrush = CreateSolidBrush(RGB(133, 137, 143));
		switch (this->destination) //в зависимости от направления нарисовать себя
		{
		case 1:
			r.left = this->x - this->width;
			r.right = this->x;
			r.top = this->y - this->length;
			r.bottom = this->y;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 2:
			r.left = this->x;
			r.right = this->x + this->length;
			r.top = this->y;
			r.bottom = this->y + this->width;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 3:
			r.left = this->x - this->length;
			r.right = this->x;
			r.top = this->y - this->width;
			r.bottom = this->y;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		case 4:
			r.left = this->x;
			r.right = this->x + this->width;
			r.top = this->y;
			r.bottom = this->y + this->length;
			FillRect(hdc, &r, hbrush);
			DeleteObject(hbrush);
			break;
		}
		ReleaseDC(hWnd, hdc);
	}
};

class crossroad
{
private:
	int kolavto = 2;
	int kontspec = 1;//проверка, встретятся ли на одном пути спец.транспорты
	int kontavto = 1;
public:
	int kolstop1, kolstop2, kolstop3, kolstop4; //количество машин, остановившиеся на перекрёстке в определнном направлении
	member_of_traffic* stop1, * stop2, * stop3, * stop4;
	void delete_object();
	void perform(); //обработка авто
	void new_car();
	void check_traffic_jam(int st);
	void timer_traffic_jam(int st, member_of_traffic* last);
	int i;
	void check_crash();
	void skip_or_not(TrafficLight* light, member_of_traffic* car);
	member_of_traffic** obj;
	crossroad() {
		srand(time(NULL));
		obj = new member_of_traffic * [12];//массив автомобилей
		int sovp = 0; //если автомобили наседают друг на друга
		int i;
		for (i = 0; i < 2; i++)
		{
		suda:
			if (sovp)
			{
				delete obj[1];
				obj[1] = NULL;
				sovp = 0;
				i--;
			}
			switch (rand() % 4 + 1)
			{
			case 1:
				obj[i] = new light_car();
				break;
			case 2:
				obj[i] = new bus();
				break;
			case 3:
				obj[i] = new truck();
				break;
			case 4:
				obj[i] = new spec_car();
				if (i == 1) {
					if (obj[1]->gettype() == obj[0]->gettype()) {
						switch (obj[1]->getdestination())
						{
						case 1:
							if (obj[0]->getdestination() != 1)
								sovp = 1;
							break;
						case 2:
							if (obj[0]->getdestination() != 2)
								sovp = 1;
							break;
						case 3:
							if (obj[0]->getdestination() != 3)
								sovp = 1;
							break;
						case 4:
							if (obj[0]->getdestination() != 4)
								sovp = 1;
							break;
						}
						if (sovp == 1)
							goto suda;
					}
				}
				break;
			}
		}
		if (obj[1]->gettype() == 4 && obj[0]->gettype() == 4)
		{
			if (obj[1]->getdestination() == obj[0]->getdestination())
			{
				obj[1]->setnext(obj[0]);
				if (obj[1]->getdistance_to_next() < 30)
					sovp = 1;
			}
		}
		else
			if (obj[1]->gettype() != 4 && obj[0]->gettype() != 4)
				if (obj[1]->getdestination() == obj[0]->getdestination())
				{
					obj[1]->setnext(obj[0]);
					if (obj[1]->getdistance_to_next() < 30)
						sovp = 1;
				}
		if (sovp == 1)
			goto suda;
	}
	void Paint()
	{
		paint_crossroad(); //рисование перекрестка
	}
private:
	void paint_crossroad();
};

crossroad* cross = new crossroad();

void crossroad::paint_crossroad()
{
	HDC hdc = GetDC(hWnd);
	RECT r;
	r.left = iWidth / 2 - 125;
	r.right = iWidth / 2 - 115; //лвв
	r.top = 0;
	r.bottom = iHeight / 2 - 115;
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(hdc, &r, hBrush);

	r.left = iWidth / 2 + 115;
	r.right = iWidth / 2 + 125; //пвв
	r.top = 0;
	r.bottom = iHeight / 2 - 115;
	FillRect(hdc, &r, hBrush);


	r.left = iWidth / 2 - 125;
	r.right = iWidth / 2 - 115;
	r.top = iHeight / 2 + 115; //лвн
	r.bottom = iHeight;
	FillRect(hdc, &r, hBrush);

	r.left = 0;
	r.right = iWidth / 2 - 115;
	r.top = iHeight / 2 - 125; //лгв
	r.bottom = iHeight / 2 - 115;
	FillRect(hdc, &r, hBrush);

	r.left = iWidth / 2 + 115;
	r.right = iWidth / 2 + 125;
	r.top = iHeight / 2 + 115; //пвн
	r.bottom = iHeight;
	FillRect(hdc, &r, hBrush);

	r.left = iWidth / 2 + 115;
	r.right = iWidth;
	r.top = iHeight / 2 - 125; //пгв
	r.bottom = iHeight / 2 - 115;
	FillRect(hdc, &r, hBrush);

	r.left = 0;
	r.right = iWidth / 2 - 115;
	r.top = iHeight / 2 + 115; //лгн
	r.bottom = iHeight / 2 + 125;
	FillRect(hdc, &r, hBrush);

	r.left = iWidth / 2 + 115;
	r.right = iWidth;
	r.top = iHeight / 2 + 115; //пгн
	r.bottom = iHeight / 2 + 125;
	FillRect(hdc, &r, hBrush);
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(RGB(133, 137, 143));
	r.left = 0; r.right = iWidth;
	r.top = iHeight / 2 - 115; r.bottom = iHeight / 2 + 115;
	FillRect(hdc, &r, hBrush);

	r.left = iWidth / 2 - 115; r.right = iWidth / 2 + 115;
	r.top = 0; r.bottom = iHeight;
	FillRect(hdc, &r, hBrush);

	//газон
	hBrush = CreateSolidBrush(RGB(58, 112, 66));
	r.left = 0; r.right = iWidth / 2 - 125;
	r.top = 0; r.bottom = iHeight / 2 - 125;
	FillRect(hdc, &r, hBrush);

	r.left = iWidth / 2 + 125; r.right = iWidth;
	r.top = 0; r.bottom = iHeight / 2 - 125;
	FillRect(hdc, &r, hBrush);

	hBrush = CreateSolidBrush(RGB(58, 112, 66));
	r.left = 0; r.right = iWidth / 2 - 125;
	r.top = iHeight / 2 + 125; r.bottom = iHeight;
	FillRect(hdc, &r, hBrush);

	r.left = iWidth / 2 + 125; r.right = iWidth;
	r.top = iHeight / 2 + 125; r.bottom = iHeight;
	FillRect(hdc, &r, hBrush);
}

void crossroad::perform()
{
	new_car(); //создание нового автомобиля
	for (i = 0; i < kolavto; i++) //основной цикл движения 
	{
		if (obj[i])
		{
			obj[i]->ride();
			delete_object();
		}
	}
	check_crash(); //проверка аварий
}

void crossroad::skip_or_not(TrafficLight* light, member_of_traffic* car)
{
	if (light->getColor() == 3 || car->gettype() == 4)
	{
		if (car->getspeed_now() == 0)
		{
			car->setspeed_now(car->getmax_speed());
		}
		car->remove_own(); //стереть 
		if (car->getdistance_to_next() <= 30 && car->getspeednext(car->getnext()) <= car->getmax_speed())
			car->setspeed_now(car->getspeednext(car->getnext()));

		switch (car->getdestination())
		{
		case 1:
			car->sety(car->getspeed_now()); //изменить координату
			break;
		case 2:
			car->setx(-car->getspeed_now()); //изменить координату
			break;
		case 3:
			car->setx(car->getspeed_now()); //изменить координату
			break;
		case 4:
			car->sety(-car->getspeed_now()); //изменить координату
			break;
		}
		car->draw_own(); //нарисовать
	}
	else
	{
		int kon = 0;
		switch (car->getdestination())
		{
		case 1:
			if (car->gety() < iHeight / 2 - 115 && car->gety() > iHeight / 2 - 145)
			{
				car->setspeed_now(0);
			}
			else
				kon = 1;
			break;
		case 2:
			if (car->getx() > iWidth / 2 + 115 && car->getx() < iWidth / 2 + 145)
			{
				car->setspeed_now(0);
			}
			else
				kon = 1;
			break;
		case 3:
			if (car->getx() < iWidth / 2 - 115 && car->getx() > iWidth / 2 - 145)
			{
				car->setspeed_now(0);
			}
			else
				kon = 1;
			break;
		case 4:
			if (car->gety() > iHeight / 2 + 115 && car->gety() < iHeight / 2 + 145)
			{
				car->setspeed_now(0);
			}
			else
				kon = 1;
			break;
		}
		if (kon) {
			if (car->getnext() != NULL && car->getdistance_to_next() <= 30)
				car->setspeed_now(car->getspeednext(car->getnext()));

			car->remove_own(); //стереть 

			switch (car->getdestination())
			{
			case 1:
				car->sety(car->getspeed_now()); //изменить координату
				break;
			case 2:
				car->setx(-car->getspeed_now()); //изменить координату
				break;
			case 3:
				car->setx(car->getspeed_now()); //изменить координату
				break;
			case 4:
				car->sety(-car->getspeed_now()); //изменить координату
				break;
			}
			car->draw_own(); //нарисовать
		}
	}
}

void crossroad::new_car()
{
	if (kolavto < 12)
	{
	here:
		if (kontspec == 0 || kontavto == 0) //если 2 транспорта пересеклись
		{
			delete obj[kolavto - 1];
			obj[kolavto - 1] = NULL;
			if (!kontspec) kontspec = 1;
			if (!kontavto) kontavto = 1;
			kolavto--;
		}
		if (rand() % 20 == 1)
		{
			switch (rand() % 4 + 1)
			{
			case 1:
				obj[kolavto++] = new light_car();
				break;
			case 2:
				obj[kolavto++] = new bus();
				break;
			case 3:
				obj[kolavto++] = new truck();
				break;
			case 4:
				obj[kolavto++] = new spec_car();
				for (int sp = 0; sp < kolavto - 1; sp++)//проверка 2 спец. транспорта оказалсись на одном пути
				{
					if (obj[kolavto - 1]->gettype() == obj[sp]->gettype()) {
						switch (obj[kolavto - 1]->getdestination())
						{
						case 1:
							if (obj[sp]->getdestination() != 1)
								kontspec = 0;
							break;
						case 2:
							if (obj[sp]->getdestination() != 2)
								kontspec = 0;
							break;
						case 3:
							if (obj[sp]->getdestination() != 3)
								kontspec = 0;
							break;
						case 4:
							if (obj[sp]->getdestination() != 4)
								kontspec = 0;
							break;
						}
					}
				}
				if (kontspec == 0)
					goto here;
				break;
			}
			for (int j = kolavto - 2; j >= 0; j--) //установление связей между автомобилями
			{
				int end1 = 1; //если мы установили связь со следующем автомобилем
				if (obj[kolavto - 1]->gettype() == 4 && obj[j]->gettype() == 4)
				{
					if (obj[kolavto - 1]->getdestination() == obj[j]->getdestination())
					{
						obj[kolavto - 1]->setnext(obj[j]);
						if (obj[kolavto - 1]->getdistance_to_next() < 30)
							kontspec = 0;
						end1 = 0;
					}
				}
				else
					if (obj[kolavto - 1]->gettype() != 4 && obj[j]->gettype() != 4)
						if (obj[kolavto - 1]->getdestination() == obj[j]->getdestination())
						{
							obj[kolavto - 1]->setnext(obj[j]);
							if (obj[kolavto - 1]->getdistance_to_next() < 30)
							{
								kontavto = 0;
							}
							if (kontavto)
							{
								switch (obj[kolavto - 1]->getdestination())
								{
								case 1:
									if (light1->getColor() == 1)
										check_traffic_jam(1);
									break;
								case 2:
									if (light2->getColor() == 1)
										check_traffic_jam(2);
									break;
								case 3:
									if (light3->getColor() == 1)
										check_traffic_jam(3);
									break;
								case 4:
									if (light4->getColor() == 1)
										check_traffic_jam(4);
									break;
								}
							}
							end1 = 0;
						}
				if (end1 == 0)
					break;
			}
			if (kontavto == 0 || kontspec == 0)
				goto here;
		}
	}
};
void  crossroad::check_crash()
{
	int k;
	for (k = 0; k < kolavto - 1; k++) //определяем врезаются ли автомобили
	{
		RECT OBJ1, OBJ2; //создаем структуры-прямоугольники для вычисления, пересекаются ли они (есть ли авария)
		switch (obj[k]->getdestination()) //первый автомобиль
		{
		case 1:
			OBJ1.right = obj[k]->getx();
			OBJ1.bottom = obj[k]->gety();
			OBJ1.left = obj[k]->getx() - obj[k]->getwidth();
			OBJ1.top = obj[k]->gety() - obj[k]->getlength();
			break;

		case 2:
			OBJ1.right = obj[k]->getx() + obj[k]->getlength();
			OBJ1.top = obj[k]->gety();
			OBJ1.left = obj[k]->getx();
			OBJ1.bottom = obj[k]->gety() + obj[k]->getwidth();
			break;

		case 3:
			OBJ1.right = obj[k]->getx();
			OBJ1.top = obj[k]->gety() - obj[k]->getwidth();
			OBJ1.left = obj[k]->getx() - obj[k]->getlength();
			OBJ1.bottom = obj[k]->gety();
			break;

		case 4:
			OBJ1.right = obj[k]->getx() + obj[k]->getwidth();
			OBJ1.bottom = obj[k]->gety() + obj[k]->getlength();
			OBJ1.left = obj[k]->getx();
			OBJ1.top = obj[k]->gety();
			break;
		}
		if (!(OBJ1.top > iHeight / 2 + 105 || OBJ1.bottom < iHeight / 2 - 105 || OBJ1.right < iWidth / 2 - 105 || OBJ1.left > iWidth / 2 + 105))
		{
			if (obj[k]->getx() >= iWidth / 2 - 105 && obj[k]->getx() <= iWidth / 2 + 105 && obj[k]->gety() >= iHeight / 2 - 105 && obj[k]->gety() <= iHeight / 2 + 105) //если автомобиль на перекрёстке
			{
				for (int v = k + 1; v < kolavto; v++) //ищем, с кем может пересечься
				{
					switch (obj[v]->getdestination()) //второй автомобиль
					{
					case 1:
						OBJ2.right = obj[v]->getx();
						OBJ2.bottom = obj[v]->gety();
						OBJ2.left = obj[v]->getx() - obj[v]->getwidth();
						OBJ2.top = obj[v]->gety() - obj[v]->getlength();
						break;
					case 2:
						OBJ2.right = obj[v]->getx() + obj[v]->getlength();
						OBJ2.top = obj[v]->gety();
						OBJ2.left = obj[v]->getx();
						OBJ2.bottom = obj[v]->gety() + obj[v]->getwidth();
						break;
					case 3:
						OBJ2.right = obj[v]->getx();
						OBJ2.top = obj[v]->gety() - obj[v]->getwidth();
						OBJ2.left = obj[v]->getx() - obj[v]->getlength();
						OBJ2.bottom = obj[v]->gety();
						break;
					case 4:
						OBJ2.right = obj[v]->getx() + obj[v]->getwidth();
						OBJ2.bottom = obj[v]->gety() + obj[v]->getlength();
						OBJ2.left = obj[v]->getx();
						OBJ2.top = obj[v]->gety();
						break;
					}
					if (!(OBJ2.top > iHeight / 2 + 105 || OBJ2.bottom < iHeight / 2 - 105 || OBJ2.right < iWidth / 2 - 105 || OBJ2.left > iWidth / 2 + 105)) //если второй объект тоже находится на перекрёстке
					{
						if (((obj[k]->getdestination() == 1 || obj[k]->getdestination() == 4) && (obj[v]->getdestination() == 2 || obj[v]->getdestination() == 3)) ||
							((obj[k]->getdestination() == 2 || obj[k]->getdestination() == 3) && (obj[v]->getdestination() == 1 || obj[v]->getdestination() == 4))) //если 2 объекта имеют пересекающиеся направления
						{
							if (!(OBJ1.top > OBJ2.bottom || OBJ1.bottom < OBJ2.top || OBJ1.right < OBJ2.left || OBJ1.left > OBJ2.right))
							{
								check = 0;
								click = 0;
								del = 1;
							}
						}
					}
				}
			}
		}
	}
}

void crossroad::timer_traffic_jam(int st, member_of_traffic* last) { //если по завершению таймера есть пробки
	kolstop1 = 0, kolstop2 = 0, kolstop3 = 0, kolstop4 = 0;
	//для определения последнего элемента в данном направлении
	int check_last = 0;
	member_of_traffic* last_now=NULL;
	for (int t = kolavto - 1; t >= 0; t--)
	{
		if (obj[t]->getdestination() == st)
		{
			switch (st)
			{
			case 1:
				if (obj[t]->gety() < iHeight / 2 - 115)
					kolstop1++;
				if (check_last == 0)
				{
					last_now = obj[t];
					check_last = 1;
				}
				break;
			case 2:
				if (obj[t]->getx() > iWidth / 2 + 115)
					kolstop2++;
				if (check_last == 0)
				{
					last_now = obj[t];
					check_last = 1;
				}
				break;
			case 3:
				if (obj[t]->getx() < iWidth / 2 - 115)
					kolstop3++;
				if (check_last == 0)
				{
					last_now = obj[t];
					check_last = 1;
				}
				break;
			case 4:
				if (obj[t]->gety() > iHeight / 2 + 115)
					kolstop4++;
				if (check_last == 0)
				{
					last_now = obj[t];
					check_last = 1;
				}
				break;
			}
		}
	}
	switch (st)
	{
	case 1:
		if ((kolstop1 == 2 || kolstop1 == 3) && (last_now == last))
		{
			check = 0;
			click = 0;
			del = 1;
		}
		break;
	case 2:
		if ((kolstop2 == 4 || kolstop2 == 5 || kolstop2 == 6) && (last_now == last))
		{
			check = 0;
			click = 0;
			del = 1;
		}
		break;
	case 3:
		if ((kolstop3 == 4 || kolstop3 == 5 || kolstop3 == 6) && (last_now == last))
		{
			check = 0;
			click = 0;
			del = 1;
		}
		break;
	case 4:
		if ((kolstop4 == 2 || kolstop4 == 3) && (last_now == last))
		{
			check = 0;
			click = 0;
			del = 1;
		}
		break;
	}
}

void crossroad::check_traffic_jam(int st)
{
	int st1;
	kolstop1 = 0, kolstop2 = 0, kolstop3 = 0, kolstop4 = 0; //количество машин, остановившиеся на перекрёстке в определнном направлении
	if (st == 1)st1 = 4;
	if (st == 4)st1 = 1;
	if (st == 2)st1 = 3;
	if (st == 3)st1 = 2;
	int last_obj = 0;
	for (int t = kolavto - 1; t >= 0; t--)
	{
		if (obj[t]->gettype() != 4) {
			if (obj[t]->getdestination() == st)
			{
				switch (st)
				{
				case 1:
					if (obj[t]->gety() < iHeight / 2 - 115)
						kolstop1++;

					if (last_obj == 0)
					{
						stop1 = obj[t];
						last_obj = 1;
					}
					break;
				case 2:
					if (obj[t]->getx() > iWidth / 2 + 115)
						kolstop2++;
					if (last_obj == 0)
					{
						stop2 = obj[t];
						last_obj = 1;
					}
					break;
				case 3:
					if (obj[t]->getx() < iWidth / 2 - 115)
						kolstop3++;

					if (last_obj == 0)
					{
						stop3 = obj[t];
						last_obj = 1;
					}
					break;
				case 4:
					if (obj[t]->gety() > iHeight / 2 + 115)
						kolstop4++;

					if (last_obj == 0)
					{
						stop4 = obj[t];
						last_obj = 1;
					}
					break;

				}
			}
			if (obj[t]->getdestination() == st1)
			{
				switch (st1)
				{
				case 1:
					kolstop1++;
					break;
				case 2:
					kolstop2++;
					break;
				case 3:
					kolstop3++;
					break;
				case 4:
					kolstop4++;
					break;
				}
			}
		}
	}
	if (kolstop1 == 2 || kolstop1 == 3)
		WndProc(hWnd, WM_CREATE, 1, 0);
	if (kolstop2 == 4 || kolstop2 == 5)
		WndProc(hWnd, WM_CREATE, 2, 0);
	if (kolstop3 == 4 || kolstop3 == 5)
		WndProc(hWnd, WM_CREATE, 3, 0);
	if (kolstop4 == 2 || kolstop4 == 3)
		WndProc(hWnd, WM_CREATE, 4, 0);
}

void crossroad::delete_object()
{

	if (obj[i]->gety() - obj[i]->getlength() >= iHeight || obj[i]->getx() + obj[i]->getlength() <= 0 || obj[i]->getx() - obj[i]->getlength() >= iWidth || obj[i]->gety() + obj[i]->getlength() <= 0)
	{

		int napr = obj[i]->getdestination(); //направление удаляемого
		int typedel = obj[i]->gettype();//тип удаляемого, важен спец.транспорт
		int napr1 = -1; //направление следующего
		delete obj[i];
		obj[i] = NULL;
		for (int j = i; j < kolavto - 1; j++)
		{
			obj[j] = obj[j + 1];
			if (typedel != 4) //для спец.транспорта отдельные условия
			{
				if (obj[j]->getdestination() == napr && napr1 == -1 && obj[j]->gettype() != 4)
					napr1 = j;
			}
			else
			{
				if (obj[j]->getdestination() == napr && napr1 == -1 && obj[j]->gettype() == 4)
					napr1 = j;
			}

		}
		if (napr1 != -1)
			obj[napr1]->setnext(NULL);
		obj[kolavto - 1] = NULL;
		kolavto--;
		total++;
	}
}

void member_of_traffic::ride()
{
	switch (this->destination) { //в зависимости от направления
	case 1:
		cross->skip_or_not(light1, this);
		break;
	case 2:
		cross->skip_or_not(light2, this);
		break;
	case 3:
		cross->skip_or_not(light3, this);
		break;
	case 4:
		cross->skip_or_not(light4, this);
		break;
	}
}

int sig = 0;

void paint_car() //рисование машины в меню
{
	HBRUSH hBrush;
	HDC hdc;
	hdc = GetDC(hWnd);
	hBrush = CreateSolidBrush(RGB(0, 0, 0));
	RECT r;
	r.left = 500;
	r.right = 930;
	r.top = 100;
	r.bottom = 400;
	FillRect(hdc, &r, hBrush);
	HPEN Pen;
	Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255)); //Используется для установки цвета, стиля, ширины и режима пера
	SelectObject(hdc, Pen);
	SetBkColor(hdc, RGB(255, 255, 255));
	MoveToEx(hdc, 650, 130, NULL); //Определяет текущую позицию пера
	LineTo(hdc, 850, 130); //Рисует прямую до заданной точки
	Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	SelectObject(hdc, Pen);
	MoveToEx(hdc, 650, 130, NULL);
	LineTo(hdc, 600, 200);
	MoveToEx(hdc, 850, 130, NULL);
	LineTo(hdc, 900, 200);
	LineTo(hdc, 600, 200);
	MoveToEx(hdc, 850, 130, NULL);
	LineTo(hdc, 900, 97);
	hBrush = CreateSolidBrush(RGB(105, 9, 9));
	SelectObject(hdc, hBrush);
	RoundRect(hdc, 517, 195, 910, 300, 70, 30);
	hBrush = CreateSolidBrush(RGB(30, 30, 30));
	SelectObject(hdc, hBrush);
	Ellipse(hdc, 560, 250, 650, 340);
	Ellipse(hdc, 790, 250, 880, 340);
	hBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, hBrush);
	Ellipse(hdc, 580, 270, 630, 320);
	Ellipse(hdc, 810, 270, 860, 320);
	MoveToEx(hdc, 740, 130, NULL);
	LineTo(hdc, 740, 300);
	MoveToEx(hdc, 510, 190, NULL);
	if (sig == 0)
	{
		hBrush = CreateSolidBrush(RGB(255, 165, 0));
		sig = 1;
	}
	else
	{
		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		sig = 0;
	}
	SelectObject(hdc, hBrush);
	RoundRect(hdc, 525, 210, 545, 235, 10, 10);
	RoundRect(hdc, 885, 205, 905, 245, 10, 10);
	hBrush = CreateSolidBrush(RGB(20, 20, 20));
	SelectObject(hdc, hBrush);
	RoundRect(hdc, 700, 205, 730, 215, 5, 5);
	RoundRect(hdc, 750, 205, 780, 215, 5, 5);
	DeleteObject(Pen);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hdc);
}

void instruction()
{
	HBRUSH hBrush;
	HDC hdc;
	hdc = GetDC(hWnd);
	hBrush = CreateSolidBrush(RGB(0, 0, 0));
	RECT r;
	r.left = 0;
	r.right = iWidth;
	r.top = 0;
	r.bottom = iHeight;
	FillRect(hdc, &r, hBrush);

	TextOut(hdc, 600, 100, L"Инструкция дли игры \"Перекрёсток\" ", 35);
	TextOut(hdc, 300, 250, L"Итак, Ваша задача в данной игре управлять траффиком с помощью светофоров так, чтоб не создавались пробки и не было аварий!", 121);
	TextOut(hdc, 440, 300, L"Для управлением светофорами Вам понадобится либо мышка, либо клавиатура", 72);
	TextOut(hdc, 430, 350, L"Светофоры будут переключаться попарно: 2 вертикальных или 2 горизонтальных", 76);
	TextOut(hdc, 479, 400, L"С помощью кликов на сфетофоры, они будут переключать свой свет", 63);
	TextOut(hdc, 505, 450, L"С помощью стрелок Вы так же можете управлять светофорами:", 57);
	TextOut(hdc, 425, 500, L"левая стрелка - включить горизонтальные светофоры, правая стрелка - выключить их", 81);
	TextOut(hdc, 433, 550, L"левая стрелка - включить вертикальные светофоры, правая стрелка - выключить их", 79);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hdc);
}

void paint_menu() {

	HBRUSH hBrush;
	HDC hdc;
	hdc = GetDC(hWnd);
	hBrush = CreateSolidBrush(RGB(0, 0, 0));
	RECT r;
	r.left = 0;
	r.right = iWidth;
	r.top = 0;
	r.bottom = iHeight;
	FillRect(hdc, &r, hBrush);
	hBrush = CreateSolidBrush(RGB(30, 7, 61));
	SelectObject(hdc, hBrush);
	RoundRect(hdc, 640, 505, 830, 555, 5, 5);
	RoundRect(hdc, 640, 570, 830, 620, 5, 5);
	RoundRect(hdc, 640, 635, 830, 685, 5, 5);
	LOGFONT font;
	font.lfHeight = 20;// Устанавливает высоту шрифта или символа
	font.lfWidth = 0;// Устанавливает среднюю ширину символов в шрифте
	font.lfEscapement = 0;// Устанавливает угол, между вектором наклона и осью X устройства
	font.lfOrientation = 0;// Устанавливает угол, между основной линией каждого символа и осью X устройства
	font.lfWeight = 100;// Устанавливает толщину шрифта в диапазоне от 0 до 1000
	font.lfItalic = 0;// Устанавливает курсивный шрифт
	font.lfUnderline = 0;// Устанавливает подчеркнутый шрифт
	font.lfStrikeOut = 0;// Устанавливает зачеркнутый шрифт
	font.lfCharSet = RUSSIAN_CHARSET;// Устанавливает набор символов
	font.lfOutPrecision = 0;// Устанавливает точность вывода
	font.lfClipPrecision = 0;// Устанавливает точность отсечения
	font.lfQuality = 0;// Устанавливает качество вывода
	font.lfPitchAndFamily = 0;// Устанавливает ширину символов и семейство шрифта
	//strcpy(font.lfFaceName, "VERDANA");//  устанавливает название шрифта
	HFONT hfont;
	hfont = ::CreateFontIndirect(&font);
	SelectObject(hdc, hfont);
	TextOut(hdc, 690, 520, L"Начать игру", 11);
	TextOut(hdc, 710, 585, L"Выйти", 6);
	TextOut(hdc, 693, 650, L"Инструкция", 10);
	WCHAR  tex[28];
	wsprintf(tex, TEXT("BEST ALL time : %d"), besttime);
	int kolsim;
	if (besttime < 10)
		kolsim = 18;
	else
		if (besttime < 100)
			kolsim = 19;
		else kolsim = 20;
	TextOut(hdc, 1000, 200, tex, kolsim);
	wsprintf(tex, TEXT("BEST in this session : %d"), best);
	//int kolsim;
	if (best < 10)
		kolsim = 24;
	else
		if (best < 100)
			kolsim = 25;
		else kolsim = 26;
	TextOut(hdc, 1000, 250, tex, kolsim);
	wsprintf(tex, TEXT("Total : %d"), total);
	//int kolsim;
	if (total < 10)
		kolsim = 9;
	else
		if (total < 100)
			kolsim = 10;
		else kolsim = 11;
	TextOut(hdc, 1000, 300, tex, kolsim);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hdc);
}

#define MAX_LOADSTRING 100
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
												// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	// TODO: разместите код здесь.
	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CROSSROADOPTIM, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	srand(time(NULL));
	light1->createCircle();
	light2->createCircle();
	light3->createCircle();
	light4->createCircle();
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CROSSROADOPTIM));
	MSG msg;
	bestr = fopen("best_record.txt", "r");
	if (bestr) {
		fscanf(bestr, "%d", &besttime);
		fclose(bestr);
	}
	fflush(bestr);
	paint_menu();
	paint_car();
	while (GetMessage(&msg, nullptr, 0, 0) && end_game == 1) {
		if (check == 0 && del == 1)
		{
			menu = 1;
			delete cross;
			delete light1;
			delete light2;
			delete light3;
			delete light4;

			cross = new crossroad();
			light1 = new TrafficLight(1);
			light2 = new TrafficLight(2);
			light3 = new TrafficLight(3);
			light4 = new TrafficLight(4);
			light1->createCircle();
			light2->createCircle();
			light3->createCircle();
			light4->createCircle();
			del = 0;
			if (total > best)
			{
				best = total;
				if (besttime < best)
				{
					bestr = fopen("best_record.txt", "w");
					besttime = best;
					fprintf(bestr, "%d", besttime);
					fclose(bestr);
				}
			}
			KillTimer(hWnd, 0);
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 4);
			paint_menu();
			paint_car();
			click = 0;
			WndProc(hWnd, WM_CREATE, 5, 0);
		}
		if (instr)
		{
			instruction();
		}
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
	}
	return (int)msg.wParam;
}

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wc; // создаём экземпляр, для обращения к членам класса WNDCLASSEX
	wc.cbSize = sizeof(WNDCLASSEX); // размер структуры (в байтах)
	wc.style = CS_HREDRAW | CS_VREDRAW; // стиль класса окошка
	wc.lpfnWndProc = WndProc; // указатель на пользовательскую функцию
	wc.lpszMenuName = NULL; // указатель на имя меню (у нас его нет)
	wc.lpszClassName = szWindowClass; // указатель на имя класса
	wc.cbWndExtra = NULL; // число освобождаемых байтов в конце структуры
	wc.cbClsExtra = NULL; // число освобождаемых байтов при создании экземпляра приложения
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // декриптор пиктограммы
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); // дескриптор маленькой пиктограммы (в трэе)
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // дескриптор курсора
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);// дескриптор кисти для закраски фона окна
	wc.hInstance = hInst; // указатель на строку, содержащую имя меню, применяемого для класса

	return RegisterClassEx(&wc);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	hWnd = CreateWindow(
		szWindowClass, // имя класса
		L"Crossroad", // имя окошка (то что сверху)
		WS_OVERLAPPEDWINDOW, // режимы отображения окошка
		CW_USEDEFAULT, // позиция окошка по оси х
		0, // позиция окошка по оси у (раз дефолт в х, то писать не нужно)
		CW_USEDEFAULT, // ширина окошка
		0, // высота окошка (раз дефолт в ширине, то писать не нужно)
		NULL, // дескриптор родительского окна
		NULL, // дескриптор меню
		hInstance, // дескриптор экземпляра приложения
		NULL); // ничего не передаём из WndProc
	if (!hWnd) {
		// в случае некорректного создания окошка (неверные параметры и тп):
		MessageBox(NULL, L"Не получилось создать окно!", L"Ошибка", MB_OK);
		return FALSE;;
	}
	ShowWindow(hWnd, SW_SHOWMAXIMIZED);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
//
//


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);

		if (click == 0)
		{
			if (xPos >= 640 && xPos <= 830 && yPos >= 505 && yPos <= 555)
			{
				click = 1;
				cross->Paint(); //отрисовка
				light1->changeColor(0);
				light2->changeColor(0);
				light3->changeColor(0);
				light4->changeColor(0);
				light1->drawOwn();
				light2->drawOwn();
				light3->drawOwn();
				light4->drawOwn();
				menu = 0;
				check = 1;
				total = 0;
				KillTimer(hWnd, 5);
				WndProc(hWnd, WM_CREATE, 0, 0);
			}

			if (xPos >= 640 && xPos <= 830 && yPos >= 635 && yPos <= 685)
			{
				instruction();
				instr = 1;
				menu = 0;
				KillTimer(hWnd, 5);
			}

			if (xPos >= 640 && xPos <= 830 && yPos >= 570 && yPos <= 620)
			{
				end_game = 0;
			}

		}
	}

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_CREATE:
	{
		if (click == 1 && check == 1)
			SetTimer(hWnd, 0, 5, 0);
		if (menu)
		{
			SetTimer(hWnd, 5, 1000, 0);//таймер для меню
		}
		if (click == 1) {
			if ((cross->kolstop1 == 2 || cross->kolstop1 == 3) && check == 1)
				SetTimer(hWnd, 1, 4000, 0);
			if ((cross->kolstop2 == 4 || cross->kolstop2 == 5 || cross->kolstop2 == 6) && check == 1)
				SetTimer(hWnd, 2, 4000, 0);
			if ((cross->kolstop3 == 4 || cross->kolstop3 == 5 || cross->kolstop3 == 6) && check == 1)
				SetTimer(hWnd, 3, 4000, 0);
			if ((cross->kolstop4 == 2 || cross->kolstop4 == 3) && check == 1)
				SetTimer(hWnd, 4, 4000, 0);
			return(0);
		}
	}break;

	case WM_TIMER:
	{
		switch (wParam)
		{
		case 0:
			if (click == 1 && check == 1)
				cross->perform();
			break;
		case 1:
			cross->timer_traffic_jam(1, cross->stop1);
			break;
		case 2:
			cross->timer_traffic_jam(2, cross->stop2);
			break;
		case 3:
			cross->timer_traffic_jam(3, cross->stop3);
			break;
		case 4:
			cross->timer_traffic_jam(4, cross->stop4);
			break;
		case 5:
			if (menu)
				paint_car();
			break;
		}
		return (0);
	} break;

	case WM_KEYDOWN:
	{
		if (click == 1) {
			switch (wParam)
			{
			case 37:
				light2->changeColor(3);
				light3->changeColor(3);
				break;
			case 38:
				light1->changeColor(3);
				light4->changeColor(3);
				break;
			case 39:
				light2->changeColor(1);
				light3->changeColor(1);
				cross->check_traffic_jam(2);
				cross->check_traffic_jam(3);
				break;
			case 40:
				light1->changeColor(1);
				light4->changeColor(1);
				cross->check_traffic_jam(1);
				cross->check_traffic_jam(4);
				break;
			case 27:
				check = 0;
				del = 1;
				break;
			}
			break;
		}
		else
			if (instr == 0) {
				switch (wParam)
				{
				case 13:
					click = 1;
					cross->Paint(); //отрисовка
					light1->changeColor(0);
					light2->changeColor(0);
					light3->changeColor(0);
					light4->changeColor(0);
					light1->drawOwn();
					light2->drawOwn();
					light3->drawOwn();
					light4->drawOwn();
					menu = 0;
					check = 1;
					total = 0;
					KillTimer(hWnd, 5);
					WndProc(hWnd, WM_CREATE, 0, 0);
					break;
				case 27:
					WndProc(hWnd, WM_DESTROY, 0, 0);
					break;
				default:
					check = 0;
					del = 1;
					break;
				}
			}
		if (instr)
		{
			switch (wParam)
			{
			default:
				paint_menu();
				paint_car();
				instr = 0;
				menu = 1;
				SetTimer(hWnd, 5, 1000, 0);//таймер для меню
				break;
			}
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{

	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}






