/*
*********************************************************************************************************
*
*   模块名称 : TCPnet网络协议栈之TCP Client测试
*   文件名称 : app_tcpnet_lib.c
*   版    本 : V1.0
*   说    明 : 创建一个TCP Client。
*
*   修改记录 :
*       版本号   日期         作者       说明
*       V1.0    2017-5-18   Eric2013     首发
*
*   Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"


/*
*********************************************************************************************************
*                                     用于本文件的调试
*********************************************************************************************************
*/
#if 1
    #define printf_debug3 printf
#else
    #define printf_debug3(...)
#endif


/*
*********************************************************************************************************
*                                        变量
*********************************************************************************************************
*/
uint8_t socket_tcp3;


/*
*********************************************************************************************************
*   函 数 名: tcp_callback2
*   功能说明: TCP Socket的回调函数
*   形    参: soc  TCP Socket类型
*             evt  事件类型
*             ptr  事件类型是TCP_EVT_DATA，ptr指向的缓冲区记录着接收到的TCP数据，其余事件记录IP地址
*             par  事件类型是TCP_EVT_DATA，记录接收到的数据个数，其余事件记录端口号
*   返 回 值: 主要是TCP_EVT_CONREQ事件使用，返回数值1允许连接，返回数值0禁止连接。
*********************************************************************************************************
*/
static U16 tcp_callback3 (U8 soc, U8 evt, U8 *ptr, U16 par)
{
    char buf[50];
    uint16_t i;

    /* 确保是socket_tcp的回调 */
    if (soc != socket_tcp3)
    {
        return (0);
    }

    switch (evt)
    {
        /*
            远程客户端连接消息
            1、数组ptr存储远程设备的IP地址，par中存储端口号。
            2、返回数值1允许连接，返回数值0禁止连接。
        */
        case TCP_EVT_CONREQ:
            sprintf(buf, "远程客户端请求连接IP: %d.%d.%d.%d", ptr[0], ptr[1], ptr[2], ptr[3]);
            printf_debug3("IP:%s  port:%d\r\n", buf, par);
            return (1);

        /* 连接终止 */
        case TCP_EVT_ABORT:
            break;

        /* Socket远程连接已经建立 */
        case TCP_EVT_CONNECT:
            printf_debug3("Socket is connected to remote peer\r\n");
            break;

        /* 连接断开 */
        case TCP_EVT_CLOSE:
            printf_debug3("Connection has been closed\r\n");
            break;

        /* 发送的数据收到远程设备应答 */
        case TCP_EVT_ACK:
            break;

        /* 接收到TCP数据帧，ptr指向数据地址，par记录数据长度，单位字节 */
        case TCP_EVT_DATA:
            printf_debug3("TCP客户端3接收到数据长度 = %d\r\n", par);
            for(i = 0; i < par; i++)
            {
                printf_debug3("ptr[%d] = %d\r\n", i, ptr[i]);
            }
            break;
    }

    return (0);
}

/*
*********************************************************************************************************
*   函 数 名: TcpConnect_Client3
*   功能说明: 创建一个TCP Client
*   形    参: _remip     要访问的远程IP地址
*             _remport   远程端口号
*             _localport 本地端口号
*   返 回 值: 无
*********************************************************************************************************
*/
void TcpConnect_Client3(uint8_t *_remip, uint16_t _remport, uint16_t _localport)
{
    uint8_t res;

    /*
       创建TCP Socket并连接，客户端连接服务器后，10秒内无数据通信将断开连接。
       但是由于这里使能了TCP_TYPE_KEEP_ALIVE，会一直保持连接，不受10秒的时间限制。
    */
    socket_tcp3 = tcp_get_socket (TCP_TYPE_CLIENT | TCP_TYPE_KEEP_ALIVE, 0, 10, tcp_callback3);
    if(socket_tcp3 != 0)
    {
        res = tcp_connect (socket_tcp3, _remip, _remport, _localport);
        printf_debug3("TCP Socket创建成功res = %d\r\n", res);
    }
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
