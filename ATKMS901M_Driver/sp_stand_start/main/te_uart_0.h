#pragma once

//头文件
#include"sp_sys.h"
#include<stdio.h>
#include<string.h>
#include"driver/uart.h"

//初始化串口
sp_t te_uart_0_init();
//串口接收
char*te_uart_0_rx();
//串口接收字节
void te_uart_0_rx_bin(u8**data,u16*len);
//串口发送
void te_uart_0_tx(char*data,...);
//串口发送字节
void te_uart_0_tx_bin(u8*data,u16 len);
