#include"atkms901m_driver.h"


//串口接收
uint8_t uart_rx_bin(uint8_t**data,uint16_t*len);
//定位信息
static uint8_t*find_pos(uint8_t*data,uint8_t data_len,uint8_t*tg,uint8_t tg_len);
//读取姿态角
uint8_t atkms901m_read(float*roll,float*pitch,float*yaw);


//串口接收
uint8_t uart_rx_bin(uint8_t**data,uint16_t*len)
{
    te_uart_0_rx_bin(data,len);
    return 0;
}

//定位信息
static uint8_t*find_pos(uint8_t*data,uint8_t data_len,uint8_t*tg,uint8_t tg_len)
{
    for(uint8_t f=0;f<=data_len-tg_len;++f)
    {
        if(!memcmp(data+f,tg,tg_len))
        {
            return data+f;
        }
    }
    return NULL;
}

//读取姿态角
uint8_t atkms901m_read(float*roll,float*pitch,float*yaw)
{
    uint8_t*data=NULL;
    uint16_t len=0;
    //等待接收
    if(uart_rx_bin(&data,&len))
    {
        return 1;
    }
    //目标头
    uint8_t tg[]={0x55,0x55,0x01,0x06};
    //定位信息
    uint8_t*pos=find_pos(data,len,tg,4);
    if(pos)
    {
        //定位数据
        uint8_t*roll_pos=pos+4;
        uint8_t*pitch_pos=roll_pos+2;
        uint8_t*yaw_pos=roll_pos+4;
        //计算
        *roll=(float)((int16_t)((roll_pos[1]<<8)|roll_pos[0]))/32768*180;
        *pitch=(float)((int16_t)((pitch_pos[1]<<8)|pitch_pos[0]))/32768*180;
        *yaw=(float)((int16_t)((yaw_pos[1]<<8)|yaw_pos[0]))/32768*180;
        return 0;
    }
    return 1;
}
