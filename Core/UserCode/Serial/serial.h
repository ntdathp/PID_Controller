#ifndef INC_SERIAL_H_
#define INC_SERIAL_H_

#include "../UserCode/user_define.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"

typedef enum
{
  NONE = 0,
  SPID,
  VTUN,
  PTUN,
  STOP,
} PROCESS_t;



void serial_init(void);
void serial_write_com(char *ucmd, float dvalue);
void serial_handle(uint8_t *ubuff);

#endif /* INC_SERIAL_H_ */
