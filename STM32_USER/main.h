#ifndef __MAIN_H__
#define __MAIN_H__
#include "ff.h"
#define WIND_INFO_UNVALID 0x01
#define BME_SENSOR_UNVALID 0x02
#define ADC_0_UNVALID 0x04
#define ADC_1_UNVALID 0x08
#define ADC_2_UNVALID 0x10
#define ADC_3_UNVALID 0x20

#define LOW_TEMP_LIMIT 5
#define HIGH_TEMP_LIMIT 10
#define LOW_LIGHT_LIMIT 1000

struct sys_status{
		uint32_t sys_config_flag;
		uint32_t last_record_tick;
    uint32_t last_cmd_tick;
		uint32_t last_adc;
		uint32_t last_sensor;//温湿度计
    uint32_t last_rain;
		uint8_t cmd_send_flag;
    uint8_t lora_send_flag;
		uint8_t stop_feed_flag;
		uint8_t rtc_flag;
		uint8_t heater_status;//1,加热器开启。0，加热器关闭
		uint8_t rain_status;//0-无雨，1-有雨
		uint8_t light_status;//0-光照弱,1-光照强
		uint8_t door_cur;//0-关闭，1-开135度，2-开225度
		uint8_t door_exp;//期望状态
		uint8_t door_move;//舱门运动状态，0-静止，1-运动
		uint8_t power_em27;
		uint8_t power_220v;
};

struct sys_config{
	uint32_t head;
	uint32_t baud;
	struct{
		float A;
		float B;
	}cal[4];
	uint32_t year;
	uint32_t month;
	uint32_t date;
	uint32_t hour;
	uint32_t minute;
	uint32_t second;
	uint32_t freq;
	uint32_t ad_gain;
	uint32_t rsv[16];
	uint32_t tail;
};

extern uint8_t new_file_flag;
extern struct sys_status status;
#endif
