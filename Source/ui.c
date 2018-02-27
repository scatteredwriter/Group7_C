#include "ui.h"

int PageInit()
{
	page = PAGE_First;
	button = BUTTON_UNCLICKED;
	second_c = NULL_COMMAND;
	start = NULL_COMMAND;
	saved_data_show = NO;
	return 0;
}

//判断是否触摸到指定区域
int JundgeTouch(struct ts_sample *p, int x1, int x2, int y1, int y2)
{
	if ((p->x > x1 && p->x < x2) && (p->y > y1 && p->y < y2) && p->pressure > 0)
		return 1;
	else
		return 0;
}

//进入第一页
int OnFirstpage()
{
	bmp_800_480("./UI/firstpage.bmp");
	while (page == PAGE_First)
		;
}

//进入第二页
int OnSecondpage()
{
	bmp_800_480("./UI/secondpage.bmp");
	while (page == PAGE_SECOND)
	{
		switch (second_c)
		{
		case START:
			printf("START\n");
			start = START;
			second_c = NULL_COMMAND;
			break;
		case SAVE:
			printf("SAVE\n");
			SaveValueInSecondPage();
			second_c = NULL_COMMAND;
			break;
		default:
			break;
		}
		switch (start)
		{
		case START:
			bmp_800_480("./UI/secondpage.bmp");
			PrintInSecondPage();
			sleep(1);
			break;
		default:
			break;
		}
	}
	second_c = NULL_COMMAND;
	start = NULL_COMMAND;
}

//进入第三页
int OnThirdpage()
{
	bmp_800_480("./UI/thirdpage.bmp");
	printf("\n");
	while (page == PAGE_THIRD)
	{
		if (saved_data_show == NO)
		{
			PrintSavedValueInThirdPage();
			saved_data_show = OK;
		}
	}
	saved_data_show = NO;
}

//第二页中显示的传感器数值
void PrintInSecondPage()
{
	int value = (int)(0.000456515706374169 * cur_value - 4051.54529193360); //将数据换算成压强值
	if (value < 0 || value >= 10000)
		value = 0;
	printf("%d\n", value);
	if (value >= 1000)
	{
		num[0] = value / 1000 + 0x0030;
		num[1] = value / 100 % 10 + 0x0030;
		num[2] = value / 10 % 10 + 0x0030;
		num[3] = value % 10 + 0x0030;
	}
	else if (value >= 100)
	{
		num[0] = L'0';
		num[1] = value / 100 + 0x0030;
		num[2] = value / 10 % 10 + 0x0030;
		num[3] = value % 10 + 0x0030;
	}
	else if (value >= 10)
	{
		num[0] = L'0';
		num[1] = L'0';
		num[2] = value / 10 + 0x0030;
		num[3] = value % 10 + 0x0030;
	}
	else
	{
		num[0] = L'0';
		num[1] = L'0';
		num[2] = L'0';
		num[3] = value + 0x0030;
	}
	num[4] = L'P';
	num[5] = L'a';
	num[6] = L'\0';
	PrintString(num, 80, 260, 230);
}

//第二页中保存当前显示的数值
void SaveValueInSecondPage()
{
	int value_index = 1;
	if (save_data_count == SAVE_DATA_MAXCOUNT)
	{
		save_data_count = 1;
		value_index = 1;
	}
	else
		value_index = save_data_count + 1;
	int i = 0;
	for (; i < 4; i++)
	{
		save_data[4 * (value_index - 1) + i] = num[i];
	}
	save_data_count++;
}

//第三页中显示保存的数值
void PrintSavedValueInThirdPage()
{
	int i, j = 0;
	int width = 150;
	int height = 280;
	wchar_t save_num[9];
	for (i = 0; i < save_data_count; i++)
	{
		for (j = 0; j < 4; j++)
		{
			save_num[j] = save_data[4 * i + j];
			;
		}
		save_num[4] = L'P';
		save_num[5] = L'a';
		save_num[6] = L'\0';
		if (i < 5)
			PrintString(save_num, 25, 55 + (width * i), 100);
		else
			PrintString(save_num, 25, 55 + (width * (i - 5)), 100 + height);
	}
}

//监听确定键和返回键
void ButtonClick(struct ts_sample *p)
{
	//判断按下的是确定键还是返回键
	if (JundgeTouch(p, BACK_BUTTON_X, BACK_BUTTON_X + BUTTON_WIDTH, BACK_BUTTON_Y, BACK_BUTTON_Y + BUTTON_HEIGHT))
	{
		button = BUTTON_BACK;
		printf("Button_Back\n");
	}
	else if (JundgeTouch(p, ENTER_BUTTON_X, ENTER_BUTTON_X + BUTTON_WIDTH, ENTER_BUTTON_Y, ENTER_BUTTON_Y + BUTTON_HEIGHT))
	{
		button = BUTTON_ENTER;
		printf("Button_Enter\n");
	}
	else
	{
		button = BUTTON_UNCLICKED;
	}
}

void PointerListener(struct ts_sample *p)
{
	switch (page)
	{
	case PAGE_SECOND:
		if (JundgeTouch(p, SECONDPAGE_START_X, SECONDPAGE_START_X + SECONDPAGE_ICON_WIDTH, SECONDPAGE_START_Y, SECONDPAGE_START_Y + SECONDPAGE_ICON_HEIGHT))
		{
			second_c = START;
		}
		else if (JundgeTouch(p, SECONDPAGE_SAVE_X, SECONDPAGE_SAVE_X + SECONDPAGE_ICON_WIDTH, SECONDPAGE_SAVE_Y, SECONDPAGE_SAVE_Y + SECONDPAGE_ICON_HEIGHT))
		{
			second_c = SAVE;
		}
		break;
	}
}