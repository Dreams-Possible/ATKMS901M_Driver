#include"sp_app.h"
#include"te_uart_0.h"
#include"te_atkms901m.h"

//用户应用
void sp_app();

//用户应用
void sp_app()
{
    //串口初始化
    while(sp_uart_init()!=SP_OK)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    // while(1)
    // {
    //     // //从串口输入
    //     // char*data=sp_uart_rx();
    //     // printf("data=%s\n",data);

    //     //从串口输入
    //     u8*data=NULL;
    //     u16 len=0;
    //     sp_uart_rx_bin(&data,&len);
    //     sp_uart_tx_bin(data,len);
    // }

    //串口0初始化
    while(te_uart_0_init()!=SP_OK)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // //串口0接收测试
    // while(1)
    // {
    //     u8*data=NULL;
    //     u8 data0[4]={0};
    //     u16 len=0;
    //     te_uart_0_rx_bin(&data,&len);
    //     sp_uart_tx_bin(data0,4);
    //     sp_uart_tx_bin(data,len);
    // }

    while(1)
    {
        float roll=0;
        float pitch=0;
        float yaw=0;
        te_atkms901m_read(&roll,&pitch,&yaw);
        //输出
        // printf("Roll: %.2f, Pitch: %.2f, Yaw: %.2f\n", roll, pitch, yaw);
        printf("data:%.2f,%.2f,%.2f\n",roll,pitch,yaw);
    }

}
