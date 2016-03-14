#ifndef __ERIC_FLASH_H
#define __ERIC_FLASH_H
#include <stdint.h>
//��¼������Ϣͷ�ṹ��
typedef struct Header
{
	uint32_t curr_date;//��ǰ����
	uint8_t up_flag;//�ϴ��ı�־��0-δ�ϴ���1-���ϴ�
	uint16_t curr_step_data;//step:��¼����ĵ�ǰ�ܲ�����
	uint8_t curr_hrs_data;//hrs�����²�������������
	uint8_t *start_add;//��ʼ��ַ
} stru_header;//32λ����

typedef struct Region
{
	stru_header * curr_header;//��ǰ��ͷ��Ϣflash��ַ
	uint16_t * step_add;//����ʱ�����ڵĲ����洢λ��
	uint16_t * hrs_add;//��ǰ�����������ݴ洢λ��
} stru_region;

//����ʱ���Ӧ�ṹ
//typedef struct date_struct
//{
//	uint32_t ����;//��ǰ����
//	uint8_t up_flag;//�ϴ��ı�־��0-δ�ϴ���1-���ϴ�
//	uint16_t curr_step_data;//step:��¼����ĵ�ǰ�ܲ�����
//	uint8_t curr_hrs_data;//hrs�����²�������������
//	uint8_t *start_add;//��ʼ��ַ
//} date_add;//32λ����

#endif