#include"te_uart_0.h"

//配置
#define TX 20
#define RX 19
#define PORT UART_NUM_1
#define RATE 115200
#define MAX_TX_LEN 1*1024
#define MAX_RX_LEN 1*1024

//保存静态数据
typedef struct uart_data_t
{
    u8 rx_data[MAX_RX_LEN];
    u8 tx_data[MAX_TX_LEN];
    QueueHandle_t uart_event;
    SemaphoreHandle_t rx_event;
    u16 rx_len;
}uart_data_t;
static uart_data_t*uart_data=NULL;

//串口接收服务
static void uart_rx_service(void*arg);
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

//串口接收服务
static void uart_rx_service(void*arg)
{
    while(1)
    {
        //阻塞直到有串口事件
        uart_event_t event={0};
        if(xQueueReceive(uart_data->uart_event,&event,portMAX_DELAY))
        {
            //如果是收到数据
            if(event.type==UART_DATA)
            {
                //重新读取
                memset(uart_data->rx_data,0,uart_data->rx_len);
                uart_data->rx_len=event.size;
                uart_read_bytes(PORT,uart_data->rx_data,event.size,portMAX_DELAY);
                //收到新串口数据
                xSemaphoreGive(uart_data->rx_event);
            }
        }
    }
}

//初始化串口
sp_t te_uart_0_init()
{
    //申请资源
    uart_data=malloc(sizeof(uart_data_t));
    if(!uart_data)
    {
        SP_LOG("mem out");
        return SP_FAIL;
    }
    memset(uart_data,0,sizeof(uart_data_t));
    uart_data->rx_event=xSemaphoreCreateBinary();
    uart_data->rx_len=0;
    esp_err_t ret=ESP_OK;
    //配置串口参数
    uart_config_t config={0};
    config.baud_rate=RATE;
    config.data_bits=UART_DATA_8_BITS;
    config.parity=UART_PARITY_DISABLE;
    config.stop_bits=UART_STOP_BITS_1;
    config.flow_ctrl=UART_HW_FLOWCTRL_DISABLE;
    config.source_clk=UART_SCLK_DEFAULT;
    //安装串口驱动程序
    ret=uart_driver_install(PORT,MAX_RX_LEN,MAX_RX_LEN,1,&uart_data->uart_event,0);
    //配置串口
    ret=uart_param_config(PORT,&config);
    //配置引脚
    ret=uart_set_pin(PORT,TX,RX,UART_PIN_NO_CHANGE,UART_PIN_NO_CHANGE);
    //串口初始化失败
    if(ret!=ESP_OK)
    {
        SP_LOG("init fail");
        return SP_FAIL;
    }
    //创建UART后台任务
    xTaskCreate(uart_rx_service,"uart_rx_service",1024*2,NULL,8,NULL);
    //串口初始化成功
    SP_LOG("init ok");
    return SP_OK;
}

//串口接收
char*te_uart_0_rx()
{
    //等待接收
    xSemaphoreTake(uart_data->rx_event,portMAX_DELAY);
    //返回读取的数据
    return (char*)uart_data->rx_data;
}

//串口接收字节
void te_uart_0_rx_bin(u8**data,u16*len)
{
    //等待接收
    xSemaphoreTake(uart_data->rx_event,portMAX_DELAY);
    //返回读取的数据
    *data=(u8*)&uart_data->rx_data;
    //返回读取的数据长度
    *len=uart_data->rx_len;
}

//串口发送
void te_uart_0_tx(char*data,...)
{
    //更新发送缓冲区
    memset(uart_data->tx_data,0,MAX_TX_LEN);
    va_list val;
    va_start(val,data);
    vsnprintf((char*)uart_data->tx_data,MAX_TX_LEN,data,val);
    va_end(val);
    //串口发送
    uart_write_bytes(PORT,(const void*)uart_data->tx_data,strlen((char*)uart_data->tx_data));
}

//串口发送字节
void te_uart_0_tx_bin(u8*data,u16 len)
{
    //串口发送
    uart_write_bytes(PORT,(const void*)data,len);
}