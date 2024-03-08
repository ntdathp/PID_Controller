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
PROCESS_t tprocess;

void serial_init(void)
{
	HAL_UART_Receive_IT(&UART_COM, &urx, 1);
}
void serial_write_com(char *scmd, float dvalue)
{
	if (scmd == NULL)
	{
		// Handle the case where scmd is nullptr
		return;
	}
	char str[MAX_LEN];
	strcpy(str, scmd);

	char str_value[MAX_LEN];
	sprintf(str_value, " %.2f\r\n", dvalue);

	strcat(str, str_value);

	for (int i = 0; i < strlen(str); i++)
	{
		utx_buff[i] = (uint8_t)str[i];
	}

	 int count = 0;
	 while (str[count] != '\0' && str[count] != '\n') {
	        count++;
	    }
	HAL_UART_Transmit(&UART_COM, utx_buff, (count+1), HAL_MAX_DELAY);
}
void serial_handle(uint8_t *ubuff)
{
	if (ubuff == NULL)
	{
		// Handle the case where ubuff is nullptr
		return;
	}
	char str[MAX_LEN];
	snprintf(str, sizeof(str), "%s", ubuff);
	sscanf(str, "%s %f %f %f %f", scmd, &dkp, &dki, &dkd, &dset_point);
	HAL_UART_Transmit(&UART_COM, ubuff, urx_index, HAL_MAX_DELAY);
	urx_index = 0;
	  if (StrCompare(scmd, (uint8_t*)"SPID", 4))
	    {
	      tprocess = SPID;
	    }
	    else if (StrCompare(scmd, (uint8_t*)"VTUN", 4))
	    {
	      tprocess = VTUN;
	    }
	    else if (StrCompare(scmd, (uint8_t*)"PTUN", 4))
	    {
	      tprocess = PTUN;
	    }
	    else if (StrCompare(scmd, (uint8_t*)"STOP", 4))
	    {
	      tprocess = STOP;
	    }
	    else
	    {
	      tprocess = NONE;
	    }
}

bool StrCompare(char *pBuff, uint8_t *pSample, uint8_t nSize)
{
    for (int i = 0; i < nSize; i++)
    {
        if(pBuff[i] != pSample[i])
        {
            return false;
        }
    }
    return true;
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
