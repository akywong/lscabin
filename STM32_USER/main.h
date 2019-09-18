#ifndef __MAIN_H__
#define __MAIN_H__

//�������ޣ����ڸ��¶ȣ���������
#define LOW_TEMP_LIMIT 5
//�������ޣ����ڸ��¶ȣ�ֹͣ����
#define HIGH_TEMP_LIMIT 10
//���������ֵ
#define LOW_LIGHT_LIMIT 100
//����ʱ���趨,���ſ���135��
#define MORNING_START_HOUR  3
#define MORNING_START_MINUTE 30
#define MORNING_START_SECOND 0
#define MORNING_START (3600*MORNING_START_HOUR+60*MORNING_START_MINUTE+MORNING_START_SECOND)
//����ʱ���趨,���ſ���215��
#define AFTERNOON_START_HOUR  12
#define AFTERNOON_START_MINUTE 30
#define AFTERNOON_START_SECOND 0
#define AFTERNOON_START (3600*AFTERNOON_START_HOUR+60*AFTERNOON_START_MINUTE+AFTERNOON_START_SECOND)
//���Źر�
#define NIGHT_START_HOUR  17
#define NIGHT_START_MINUTE 30
#define NIGHT_START_SECOND 0
#define NIGHT_START (3600*NIGHT_START_HOUR+60*NIGHT_START_MINUTE+NIGHT_START_SECOND)

struct sys_status{
		uint32_t sys_config_flag;
		uint32_t last_record_tick;
    uint32_t last_cmd_tick;
		uint32_t last_adc;
		uint32_t last_sensor;//��ʪ�ȼ�
    uint32_t last_rain;
		uint32_t morning_start;
		uint32_t afternoon_start;
		uint32_t night_start;
		uint8_t cmd_send_flag;
    uint8_t lora_send_flag;
		uint8_t stop_feed_flag;
		uint8_t rtc_flag;
		uint8_t heater_status;//1,������������0���������ر�
		uint8_t rain_status;//0-���꣬1-����
		uint8_t light_status;//0-������,1-����ǿ
		uint8_t door_cur;//0-�رգ�1-��135�ȣ�2-��225��
		uint8_t door_exp;//����״̬
		uint8_t door_move;//�����˶�״̬��0-��ֹ��1-�˶�
		uint8_t power_em27;
		uint8_t power_220v;
};
struct time_set{
	uint32_t hour;
	uint32_t minute;
	uint32_t second;
};
struct sys_config{
	uint32_t head;
	uint32_t year;
	uint32_t month;
	uint32_t date;
	uint32_t hour;
	uint32_t minute;
	uint32_t second;
	struct time_set morning;
	struct time_set afternoon;
	struct time_set night;
	uint32_t tail;
};

extern struct sys_status status;
#endif
