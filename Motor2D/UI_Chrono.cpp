#include "UI_Chrono.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "Brofiler\Brofiler.h"

void Chrono::setStartValue(int new_start_value)
{
	start_value = new_start_value;
}

void Chrono::setAlarm(int alarm)
{
	alarms.PushBack(alarm);
}

void Chrono::restartChrono()
{
	switch (this->type)
	{
	case TIMER:
		time = start_value;
		break;
	case STOPWATCH:
		time = 0;
		break;
	}
}

void Chrono::BlitElement()
{
	BROFILER_CATEGORY("Chrono Blit", Profiler::Color::AntiqueWhite);
	time_elapsed = counter.ReadSec();

	switch (type)
	{
	case STOPWATCH:
		if (time != time_elapsed)
		{
			time = time_elapsed;
			
			if (callback != nullptr) //If has callback send event
			{
				for (int i = 0; i < alarms.Count(); i++)
				{
					if (time == (int)*alarms.At(i))
						callback->OnUIEvent(this, STOPWATCH_ALARM);
				}
			}
			
			p2SString secs("%05d", time);
			if (last_secs != secs)
				text->setText(secs);

			section = text->section;
			last_secs = ("%05d", time);
		}
		break;
	case TIMER:
		if (start_value - time_elapsed != time && time != 0)
		{
			time = start_value - time_elapsed;

			if (time == 0 && callback != nullptr) //If has callback send event
				callback->OnUIEvent(this, TIMER_ZERO);

			p2SString secs("%d", time);
			if (last_secs != secs)
			text->setText(secs);

			section = text->section;
			last_secs = ("%05d", time);
		}
		break;
	}

	text->BlitElement();
}