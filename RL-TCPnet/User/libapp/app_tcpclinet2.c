/*
*********************************************************************************************************
*
*   ģ������ : TCPnet����Э��ջ֮TCP Client����
*   �ļ����� : app_tcpnet_lib.c
*   ��    �� : V1.0
*   ˵    �� : ����һ��TCP Client��
*
*   �޸ļ�¼ :
*       �汾��   ����         ����       ˵��
*       V1.0    2017-5-18   Eric2013     �׷�
*
*   Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"



/*
*********************************************************************************************************
*                                     ���ڱ��ļ��ĵ���
*********************************************************************************************************
*/
#if 1
    #define printf_debug2 printf
#else
    #define printf_debug2(...)
#endif


/*
*********************************************************************************************************
*                                        ����
*********************************************************************************************************
*/
uint8_t socket_tcp2;


/*
*********************************************************************************************************
*   �� �� ��: tcp_callback2
*   ����˵��: TCP Socket�Ļص�����
*   ��    ��: soc  TCP Socket����
*             evt  �¼�����
*             ptr  �¼�������TCP_EVT_DATA��ptrָ��Ļ�������¼�Ž��յ���TCP���ݣ������¼���¼IP��ַ
*             par  �¼�������TCP_EVT_DATA����¼���յ������ݸ����������¼���¼�˿ں�
*   �� �� ֵ: ��Ҫ��TCP_EVT_CONREQ�¼�ʹ�ã�������ֵ1�������ӣ�������ֵ0��ֹ���ӡ�
*********************************************************************************************************
*/
static U16 tcp_callback2 (U8 soc, U8 evt, U8 *ptr, U16 par)
{
    char buf[50];
    uint16_t i;

    /* ȷ����socket_tcp�Ļص� */
    if (soc != socket_tcp2)
    {
        return (0);
    }

    switch (evt)
    {
        /*
            Զ�̿ͻ���������Ϣ
            1������ptr�洢Զ���豸��IP��ַ��par�д洢�˿ںš�
            2��������ֵ1�������ӣ�������ֵ0��ֹ���ӡ�
        */
        case TCP_EVT_CONREQ:
            sprintf(buf, "Զ�̿ͻ�����������IP: %d.%d.%d.%d", ptr[0], ptr[1], ptr[2], ptr[3]);
            printf_debug2("IP:%s  port:%d\r\n", buf, par);
            return (1);

        /* ������ֹ */
        case TCP_EVT_ABORT:
            break;

        /* SocketԶ�������Ѿ����� */
        case TCP_EVT_CONNECT:
            printf_debug2("Socket is connected to remote peer\r\n");
            break;

        /* ���ӶϿ� */
        case TCP_EVT_CLOSE:
            printf_debug2("Connection has been closed\r\n");
            break;

        /* ���͵������յ�Զ���豸Ӧ�� */
        case TCP_EVT_ACK:
            break;

        /* ���յ�TCP����֡��ptrָ�����ݵ�ַ��par��¼���ݳ��ȣ���λ�ֽ� */
        case TCP_EVT_DATA:
            printf_debug2("TCP�ͻ���2���յ����ݳ��� = %d\r\n", par);
            for(i = 0; i < par; i++)
            {
                printf_debug2("ptr[%d] = %d\r\n", i, ptr[i]);
            }
            break;
    }

    return (0);
}

/*
*********************************************************************************************************
*   �� �� ��: TcpConnect_Client2
*   ����˵��: ����һ��TCP Client
*   ��    ��: _remip     Ҫ���ʵ�Զ��IP��ַ
*             _remport   Զ�̶˿ں�
*             _localport ���ض˿ں�
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void TcpConnect_Client2(uint8_t *_remip, uint16_t _remport, uint16_t _localport)
{
    uint8_t res;

    /*
       ����TCP Socket�����ӣ��ͻ������ӷ�������10����������ͨ�Ž��Ͽ����ӡ�
       ������������ʹ����TCP_TYPE_KEEP_ALIVE����һֱ�������ӣ�����10���ʱ�����ơ�
    */
    socket_tcp2 = tcp_get_socket (TCP_TYPE_CLIENT | TCP_TYPE_KEEP_ALIVE, 0, 10, tcp_callback2);
    if(socket_tcp2 != 0)
    {
        res = tcp_connect (socket_tcp2, _remip, _remport, _localport);
        printf_debug2("TCP Socket�����ɹ�res = %d\r\n", res);
    }
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
