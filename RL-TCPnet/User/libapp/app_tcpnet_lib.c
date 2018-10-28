#include "includes.h"

/*
*********************************************************************************************************
*                                      用于本文件的调试
*********************************************************************************************************
*/
#if 1
    #define printf_debug printf
#else
    #define printf_debug(...)
#endif

/*
*********************************************************************************************************
*                                     外部变量和函数调用
*********************************************************************************************************
*/
extern uint8_t socket_tcp1;
extern void TcpConnect_Client1(uint8_t *_remip, uint16_t _remport, uint16_t _localport);

extern uint8_t socket_tcp2;
extern void TcpConnect_Client2(uint8_t *_remip, uint16_t _remport, uint16_t _localport);

extern uint8_t socket_tcp3;
extern void TcpConnect_Client3(uint8_t *_remip, uint16_t _remport, uint16_t _localport);

/*
*********************************************************************************************************
*                             宏定义，远程服务器的IP和端口
*********************************************************************************************************
*/
/* 要访问的远程服务器IP和端口配置，也就是电脑端调试助手设置的IP和端口号 */
/* 如果是电脑作为服务器的话，每次需要重新查询电脑的IP进行配置 */
#define IP1            192
#define IP2            168
#define IP3            1
#define IP4            8
#define PORT_NUM       1001    /* TCP服务器监听端口号 */

/* 这个是本地端口 */
#define LocalPort_NUM1    1024
#define LocalPort_NUM2    1025
#define LocalPort_NUM3    1026

/*
*********************************************************************************************************
*                                         变量
*********************************************************************************************************
*/
uint8_t Rem_IP[4] = {IP1,IP2,IP3,IP4};

/* TCP状态的返回值 */
const char * ReVal_Table[]=
{
    "0: TCP_STATE_FREE",
    "1: TCP_STATE_CLOSED",
    "2: TCP_STATE_LISTEN",
    "3: TCP_STATE_SYN_REC",
    "4: TCP_STATE_SYN_SENT",
    "5: TCP_STATE_FINW1",
    "6: TCP_STATE_FINW2",
    "7: TCP_STATE_CLOSING",
    "8: TCP_STATE_LAST_ACK",
    "9: TCP_STATE_TWAIT",
    "10: TCP_STATE_CONNECT",
    "11: 连接还未创建",
};

/*
*********************************************************************************************************
*    函 数 名: TCPnetTest
*    功能说明: TCPnet应用
*    形    参: 无
*    返 回 值: 无
*********************************************************************************************************
*/
void TCPnetTest(void)
{
    int32_t iCount;
    uint8_t *sendbuf;
    uint16_t maxlen;
    uint8_t TcpState;
    const TickType_t xTicksToWait = 2; /* 延迟2ms */
    EventBits_t uxBits;


    while (1)
    {
        /* RL-TCPnet处理函数 */
        main_TcpNet();

        /* 等待所有任务发来事件标志 */
        uxBits = xEventGroupWaitBits(xCreatedEventGroup, /* 事件标志组句柄 */
                                     0xFFFF,             /* 等待0xFFFF某一位被设置 */
                                     pdTRUE,             /* 退出前0xFFFF位被清除，这里是任意0xFFFF位被设置就“退出”*/
                                     pdFALSE,            /* 设置为pdTRUE表示等待0xFFFF任意位被设置*/
                                     xTicksToWait);      /* 等待延迟时间 */

        if(uxBits != 0)
        {
            switch (uxBits)
            {
                /* 接收到K1键按下，TCP客户端1建立连接 */
                case KEY1_BIT0:
                    TcpState = tcp_get_state(socket_tcp1);

                    /*
                       TCP_STATE_CONNECT是最后一个状态，未创建连接的话，会返回数值255，这里就是专门处理
                       这种状态，将其设置下一个TCP_STATE_CONNECT+1，即我们这里自定义了一个未创建连接的
                       状态。
                    */
                    if(TcpState > TCP_STATE_CONNECT)
                    {
                        TcpState = TCP_STATE_CONNECT + 1;
                    }
                    printf_debug("TCP客户端1状态 = %s\r\n", ReVal_Table[TcpState]);

                    if(TcpState != TCP_STATE_CONNECT)
                    {
                        TcpConnect_Client1(Rem_IP, PORT_NUM, LocalPort_NUM1);
                    }
                    break;

                /* 接收到K2键按下，TCP客户端2建立连接 */
                case KEY2_BIT1:
                    TcpState = tcp_get_state(socket_tcp2);

                    /*
                       TCP_STATE_CONNECT是最后一个状态，未创建连接的话，会返回数值255，这里就是专门处理
                       这种状态，将其设置下一个TCP_STATE_CONNECT+1，即我们这里自定义了一个未创建连接的
                       状态。
                    */
                    if(TcpState > TCP_STATE_CONNECT)
                    {
                        TcpState = TCP_STATE_CONNECT + 1;
                    }
                    printf_debug("TCP客户端2状态 = %s\r\n", ReVal_Table[TcpState]);

                    if(TcpState != TCP_STATE_CONNECT)
                    {
                        TcpConnect_Client2(Rem_IP, PORT_NUM, LocalPort_NUM2);
                    }
                    break;

                /* 接收到K3键按下，TCP客户端3建立连接 */
                case KEY3_BIT2:
                    TcpState = tcp_get_state(socket_tcp3);

                    /*
                       TCP_STATE_CONNECT是最后一个状态，未创建连接的话，会返回数值255，这里就是专门处理
                       这种状态，将其设置下一个TCP_STATE_CONNECT+1，即我们这里自定义了一个未创建连接的
                       状态。
                    */
                    if(TcpState > TCP_STATE_CONNECT)
                    {
                        TcpState = TCP_STATE_CONNECT + 1;
                    }

                    printf_debug("TCP客户端3状态 = %s\r\n", ReVal_Table[TcpState]);

                    if(TcpState != TCP_STATE_CONNECT)
                    {
                        TcpConnect_Client3(Rem_IP, PORT_NUM, LocalPort_NUM3);
                    }
                    break;


                /* 接收到摇杆上键按下，TCP客户端1向TCP服务器发送4096字节数据 */
                case JOY_U_BIT3:
                    TcpState = tcp_get_state(socket_tcp1);
                    printf_debug("TCP客户端1状态 = %s\r\n", ReVal_Table[TcpState]);
                    iCount = 4096;
                    /* 保证已经建立连接了才可以发送数据 */
                    if(TcpState == TCP_STATE_CONNECT)
                    {
                        do
                        {
                            main_TcpNet();
                            if (tcp_check_send (socket_tcp1) == __TRUE)
                            {
                                maxlen = tcp_max_dsize (socket_tcp1);
                                iCount -= maxlen;

                                if(iCount < 0)
                                {
                                    /* 这么计算没问题的 */
                                    maxlen = iCount + maxlen;
                                }

                                /* 这里仅初始化了每次所发送数据包的前8个字节 */
                                sendbuf = tcp_get_buf(maxlen);
                                sendbuf[0] = 'a';
                                sendbuf[1] = 'b';
                                sendbuf[2] = 'c';
                                sendbuf[3] = 'd';
                                sendbuf[4] = 'e';
                                sendbuf[5] = 'f';
                                sendbuf[6] = 'g';
                                sendbuf[7] = 'h';

                                /* 测试发现只能使用获取的内存 */
                                tcp_send (socket_tcp1, sendbuf, maxlen);
                            }

                        }while(iCount > 0);
                    }
                    break;

                /* 接收到摇杆左键按下，TCP客户端2向TCP服务器发送4096字节数据 */
                case JOY_L_BIT4:
                    TcpState = tcp_get_state(socket_tcp2);
                    printf_debug("TCP客户端2状态 = %s\r\n", ReVal_Table[TcpState]);
                    iCount = 4096;
                    /* 保证已经建立连接了才可以发送数据 */
                    if(TcpState == TCP_STATE_CONNECT)
                    {
                        do
                        {
                            main_TcpNet();
                            if (tcp_check_send (socket_tcp2) == __TRUE)
                            {
                                maxlen = tcp_max_dsize (socket_tcp2);
                                iCount -= maxlen;

                                if(iCount < 0)
                                {
                                    /* 这么计算没问题的 */
                                    maxlen = iCount + maxlen;
                                }

                                /* 这里仅初始化了每次所发送数据包的前8个字节 */
                                sendbuf = tcp_get_buf(maxlen);
                                sendbuf[0] = 'a';
                                sendbuf[1] = 'b';
                                sendbuf[2] = 'c';
                                sendbuf[3] = 'd';
                                sendbuf[4] = 'e';
                                sendbuf[5] = 'f';
                                sendbuf[6] = 'g';
                                sendbuf[7] = 'h';

                                /* 测试发现只能使用获取的内存 */
                                tcp_send (socket_tcp2, sendbuf, maxlen);
                            }

                        }while(iCount > 0);
                    }
                    break;

                /* 接收到摇杆右键按下，TCP客户端3向TCP服务器发送4096字节数据 */
                case JOY_R_BIT5:
                    TcpState = tcp_get_state(socket_tcp3);
                    printf_debug("TCP客户端3状态 = %s\r\n", ReVal_Table[TcpState]);
                    iCount = 4096;
                    /* 保证已经建立连接了才可以发送数据 */
                    if(TcpState == TCP_STATE_CONNECT)
                    {
                        do
                        {
                            main_TcpNet();
                            if (tcp_check_send (socket_tcp3) == __TRUE)
                            {
                                maxlen = tcp_max_dsize (socket_tcp3);
                                iCount -= maxlen;

                                if(iCount < 0)
                                {
                                    /* 这么计算没问题的 */
                                    maxlen = iCount + maxlen;
                                }

                                /* 这里仅初始化了每次所发送数据包的前8个字节 */
                                sendbuf = tcp_get_buf(maxlen);
                                sendbuf[0] = 'a';
                                sendbuf[1] = 'b';
                                sendbuf[2] = 'c';
                                sendbuf[3] = 'd';
                                sendbuf[4] = 'e';
                                sendbuf[5] = 'f';
                                sendbuf[6] = 'g';
                                sendbuf[7] = 'h';

                                /* 测试发现只能使用获取的内存 */
                                tcp_send (socket_tcp3, sendbuf, maxlen);
                            }

                        }while(iCount > 0);
                    }
                    break;

                /* 其他的键值不处理 */
                default:
                    break;
            }
        }
    }
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
