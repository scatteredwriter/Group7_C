#ifndef _UI_H_
#define _UI_H_

#define BUTTON_HEIGHT 50
#define BUTTON_WIDTH 400
#define SECONDPAGE_ICON_HEIGHT 60
#define SECONDPAGE_ICON_WIDTH 60

#define BACK_BUTTON_X 0
#define BACK_BUTTON_Y 430
#define ENTER_BUTTON_X 400
#define ENTER_BUTTON_Y 430
#define SECONDPAGE_START_X 300
#define SECONDPAGE_START_Y 280
#define SECONDPAGE_SAVE_X 410
#define SECONDPAGE_SAVE_Y 280

#define SAVE_DATA_MAXCOUNT 10

#include <stdio.h>
#include <wchar.h>
#include "lcd.h"
#include "ts.h"
#include "freetype.h"
#include "serial.h"

//程序显示页面定义
enum _page
{
	PAGE_First,
	PAGE_SECOND,
	PAGE_THIRD
};

//程序下方的返回键和确定键定义
enum _button
{
	BUTTON_ENTER,
	BUTTON_BACK,
	BUTTON_UNCLICKED
};

enum _second_command
{
	START,
	SAVE,
	NULL_COMMAND
};

enum _show
{
	NO,
	OK
};

enum _page page;
enum _button button;
enum _second_command second_c;
enum _second_command start;
enum _show saved_data_show;
wchar_t num[7];
wchar_t save_data[4 * 10];
int save_data_count;

int JundgeTouch(struct ts_sample *p, int x1, int x2, int y1, int y2);
int OnFirstpage();
int OnSecondpage();
int OnThirdpage();
int PageInit();
void PrintInSecondPage();
void SaveValueInSecondPage();
void PrintSavedValueInThirdPage();
void ButtonClick(struct ts_sample *p);
void PointerListener(struct ts_sample *p);
#endif