#pragma once

struct list {
	char locale_start[1000] = " ", locale_end[1000] = " ";
	int hour_start, hour_end, min_start, min_end, free_lot;
};