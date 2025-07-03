#include"te_atkms901m.h"

//定位信息
static u8*find_pos(u8*data,u8 data_len,u8*tg,u8 tg_len);
//读取姿态角
void te_atkms901m_read(float*roll,float*pitch,float*yaw);

//定位信息
static u8*find_pos(u8*data,u8 data_len,u8*tg,u8 tg_len)
{
    for(u8 f=0;f<=data_len-tg_len;++f)
    {
        if(!memcmp(data+f,tg,tg_len))
        {
            return data+f;
        }
    }
    return NULL;
}

//读取姿态角
void te_atkms901m_read(float*roll,float*pitch,float*yaw)
{
    u8*data=NULL;
    u16 len=0;
    //等待接收
    te_uart_0_rx_bin(&data,&len);
    //目标头
    u8 tg[]={0x55,0x55,0x01,0x06};
    //定位信息
    u8*pos=find_pos(data,len,tg,4);
    if(pos)
    {
        //定位数据
        u8*roll_pos=pos+4;
        u8*pitch_pos=roll_pos+2;
        u8*yaw_pos=roll_pos+4;
        //计算
        *roll=(float)((i16)((roll_pos[1]<<8)|roll_pos[0]))/32768*180;
        *pitch=(float)((i16)((pitch_pos[1]<<8)|pitch_pos[0]))/32768*180;
        *yaw=(float)((i16)((yaw_pos[1]<<8)|yaw_pos[0]))/32768*180;
    }
}
