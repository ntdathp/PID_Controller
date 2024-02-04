#include "serial.h"

uint8_t urx_buff[MAX_LEN];
uint8_t utx_buff[MAX_LEN];
char scmd[4];
float dkp;
float dki;
float dkd;
float dset_point;
uint8_t urx_index = 0;
uint8_t urx = 0;

void serial_init(void)
{
	HAL_UART_Receive_IT(&UART_COM, &urx, 1);
}
void serial_write_com(char *scmd, float dvalue)
{
	char str[MAX_LEN];
	strcpy(str, scmd);

	char str_value[MAX_LEN];
	sprintf(str_value, " %.2f\r\n", dvalue);

	strcat(str, str_value);

	for (int i = 0; i < strlen(str); i++)
	{
		utx_buff[i] = (uint8_t)str[i];
	}
	HAL_UART_Transmit(&UART_COM, utx_buff, sizeof(utx_buff), HAL_MAX_DELAY);
}
void serial_handle(uint8_t *ubuff)
{
	char str[MAX_LEN];
	snprintf(str, sizeof(str), "%s", ubuff);
	sscanf(str, "%s %f %f %f %f", scmd, &dkp, &dki, &dkd, &dset_point);
	HAL_UART_Transmit(&UART_COM, ubuff, urx_index, HAL_MAX_DELAY);
	urx_index = 0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == UART_COM_INSTANCE)
	{
		if (urx != '\n')
		{
			urx_buff[urx_index] = urx;
			urx_index++;
		}
		else
		{
			urx_buff[urx_index] = urx;
			urx_index++;
			serial_handle(urx_buff);
		}
	}
	HAL_UART_Receive_IT(&UART_COM, &urx, 1);
}
