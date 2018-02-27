#include "main.h"

void ButtonListener(struct ts_sample *p)
{
	ButtonClick(p);
	switch (button)
	{
	case BUTTON_ENTER:
		switch (page)
		{
		case PAGE_First:
			page = PAGE_SECOND;
			break;
		case PAGE_SECOND:
			page = PAGE_THIRD;
			break;
		default:
			break;
		}
		break;
	case BUTTON_BACK:
		switch (page)
		{
		case PAGE_SECOND:
			page = PAGE_First;
			break;
		case PAGE_THIRD:
			page = PAGE_SECOND;
			break;
		}
		break;
	default:
		break;
	}
}

void *PointerRoutine(void *arg)
{
	struct ts_sample p;
	while (1)
	{
		read_ts(&p);
		ButtonListener(&p);
		PointerListener(&p);
	}
	return NULL;
}

void *SerialRoutine(void *arg)
{
	while (1)
	{
		if (start == START)
			GetValue();
	}
	return NULL;
}

int main(int argc, char const *argv[])
{
	PageInit();
	pthread_t pointer_thread;
	pthread_create(&pointer_thread, NULL, PointerRoutine, NULL);
	pthread_t serial_thread;
	pthread_create(&serial_thread, NULL, SerialRoutine, NULL);
	while (1)
	{
		switch (page)
		{
		case PAGE_First:
			OnFirstpage();
			break;
		case PAGE_SECOND:
			OnSecondpage();
			break;
		case PAGE_THIRD:
			OnThirdpage();
			break;
		}
	}
	return 0;
}
