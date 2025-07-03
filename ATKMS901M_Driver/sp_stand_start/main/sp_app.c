#include"sp_app.h"
#include"te_uart_0.h"
#include"te_atkms901m.h"
#include"atkms901m_driver.h"

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

    //串口0初始化
    while(te_uart_0_init()!=SP_OK)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    while(1)
    {
        float roll=0;
        float pitch=0;
        float yaw=0;
        if(!atkms901m_read(&roll,&pitch,&yaw))
        {
            //输出
            printf("data:%.2f,%.2f,%.2f\n",roll,pitch,yaw);
        }
    }
}
