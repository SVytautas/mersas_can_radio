#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "main.h"


enum Source_audio {
		CD4052_SOURCE_RADIO,
		CD4052_SOURCE_BT,
		CD4052_SOURCE_AUX,
};


void CD4052_init();
void CD4052_tick_1khz();


void CD4052_SET_BT_OUT();
void CD4052_SET_AUX_OUT();
void CD4052_SET_RADIO();

void CD4052_SET_TOGGLE_SOURCE();
void CD4052_REFRESH_SOURCE();

bool CD4052_source_now_not_BT();

void CD4052_temporary_switch_to_BT_source();
void CD4052_if_BT_off_turnON_delay20s();