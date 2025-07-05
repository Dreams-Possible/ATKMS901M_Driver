#pragma once

//头文件
#include<string.h>
#include<stdint.h>
// #include"uart_driver.h"
#include"te_uart_0.h"

//读取姿态角
uint8_t atkms901m_read(float*roll,float*pitch,float*yaw);
