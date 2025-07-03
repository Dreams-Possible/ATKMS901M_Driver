#pragma once

//头文件
#include"sp_sys.h"
#include"te_uart_0.h"

//读取姿态角
void te_atkms901m_read(float*roll,float*pitch,float*yaw);
