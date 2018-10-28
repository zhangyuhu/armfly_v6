#include "includes.h"

/*
*********************************************************************************************************
*                                      ���ڱ��ļ��ĵ���
*********************************************************************************************************
*/
#if 1
    #define printf_debug printf
#else
    #define printf_debug(...)
#endif

/*
*********************************************************************************************************
*                                     �ⲿ�����ͺ�������
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
*                             �궨�壬Զ�̷�������IP�Ͷ˿�
*********************************************************************************************************
*/
/* Ҫ���ʵ�Զ�̷�����IP�Ͷ˿����ã�Ҳ���ǵ��Զ˵����������õ�IP�Ͷ˿ں� */
/* ����ǵ�����Ϊ�������Ļ���ÿ����Ҫ���²�ѯ���Ե�IP�������� */
#define IP1            192
#define IP2            168
#define IP3            1
#define IP4            8
#define PORT_NUM       1001    /* TCP�����������˿ں� */

/* ����Ǳ��ض˿� */
#define LocalPort_NUM1    1024
#define LocalPort_NUM2    1025
#define LocalPort_NUM3    1026

/*
*********************************************************************************************************
*                                         ����
*********************************************************************************************************
*/
uint8_t Rem_IP[4] = {IP1,IP2,IP3,IP4};

/* TCP״̬�ķ���ֵ */
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
    "11: ���ӻ�δ����",
};

/*
*********************************************************************************************************
*    �� �� ��: TCPnetTest
*    ����˵��: TCPnetӦ��
*    ��    ��: ��
*    �� �� ֵ: ��
*********************************************************************************************************
*/
void TCPnetTest(void)
{
    int32_t iCount;
    uint8_t *sendbuf;
    uint16_t maxlen;
    uint8_t TcpState;
    const TickType_t xTicksToWait = 2; /* �ӳ�2ms */
    EventBits_t uxBits;


    while (1)
    {
        /* RL-TCPnet������ */
        main_TcpNet();

        /* �ȴ������������¼���־ */
        uxBits = xEventGroupWaitBits(xCreatedEventGroup, /* �¼���־���� */
                                     0xFFFF,             /* �ȴ�0xFFFFĳһλ������ */
                                     pdTRUE,             /* �˳�ǰ0xFFFFλ�����������������0xFFFFλ�����þ͡��˳���*/
                                     pdFALSE,            /* ����ΪpdTRUE��ʾ�ȴ�0xFFFF����λ������*/
                                     xTicksToWait);      /* �ȴ��ӳ�ʱ�� */

        if(uxBits != 0)
        {
            switch (uxBits)
            {
                /* ���յ�K1�����£�TCP�ͻ���1�������� */
                case KEY1_BIT0:
                    TcpState = tcp_get_state(socket_tcp1);

                    /*
                       TCP_STATE_CONNECT�����һ��״̬��δ�������ӵĻ����᷵����ֵ255���������ר�Ŵ���
                       ����״̬������������һ��TCP_STATE_CONNECT+1�������������Զ�����һ��δ�������ӵ�
                       ״̬��
                    */
                    if(TcpState > TCP_STATE_CONNECT)
                    {
                        TcpState = TCP_STATE_CONNECT + 1;
                    }
                    printf_debug("TCP�ͻ���1״̬ = %s\r\n", ReVal_Table[TcpState]);

                    if(TcpState != TCP_STATE_CONNECT)
                    {
                        TcpConnect_Client1(Rem_IP, PORT_NUM, LocalPort_NUM1);
                    }
                    break;

                /* ���յ�K2�����£�TCP�ͻ���2�������� */
                case KEY2_BIT1:
                    TcpState = tcp_get_state(socket_tcp2);

                    /*
                       TCP_STATE_CONNECT�����һ��״̬��δ�������ӵĻ����᷵����ֵ255���������ר�Ŵ���
                       ����״̬������������һ��TCP_STATE_CONNECT+1�������������Զ�����һ��δ�������ӵ�
                       ״̬��
                    */
                    if(TcpState > TCP_STATE_CONNECT)
                    {
                        TcpState = TCP_STATE_CONNECT + 1;
                    }
                    printf_debug("TCP�ͻ���2״̬ = %s\r\n", ReVal_Table[TcpState]);

                    if(TcpState != TCP_STATE_CONNECT)
                    {
                        TcpConnect_Client2(Rem_IP, PORT_NUM, LocalPort_NUM2);
                    }
                    break;

                /* ���յ�K3�����£�TCP�ͻ���3�������� */
                case KEY3_BIT2:
                    TcpState = tcp_get_state(socket_tcp3);

                    /*
                       TCP_STATE_CONNECT�����һ��״̬��δ�������ӵĻ����᷵����ֵ255���������ר�Ŵ���
                       ����״̬������������һ��TCP_STATE_CONNECT+1�������������Զ�����һ��δ�������ӵ�
                       ״̬��
                    */
                    if(TcpState > TCP_STATE_CONNECT)
                    {
                        TcpState = TCP_STATE_CONNECT + 1;
                    }

                    printf_debug("TCP�ͻ���3״̬ = %s\r\n", ReVal_Table[TcpState]);

                    if(TcpState != TCP_STATE_CONNECT)
                    {
                        TcpConnect_Client3(Rem_IP, PORT_NUM, LocalPort_NUM3);
                    }
                    break;


                /* ���յ�ҡ���ϼ����£�TCP�ͻ���1��TCP����������4096�ֽ����� */
                case JOY_U_BIT3:
                    TcpState = tcp_get_state(socket_tcp1);
                    printf_debug("TCP�ͻ���1״̬ = %s\r\n", ReVal_Table[TcpState]);
                    iCount = 4096;
                    /* ��֤�Ѿ����������˲ſ��Է������� */
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
                                    /* ��ô����û����� */
                                    maxlen = iCount + maxlen;
                                }

                                /* �������ʼ����ÿ�����������ݰ���ǰ8���ֽ� */
                                sendbuf = tcp_get_buf(maxlen);
                                sendbuf[0] = 'a';
                                sendbuf[1] = 'b';
                                sendbuf[2] = 'c';
                                sendbuf[3] = 'd';
                                sendbuf[4] = 'e';
                                sendbuf[5] = 'f';
                                sendbuf[6] = 'g';
                                sendbuf[7] = 'h';

                                /* ���Է���ֻ��ʹ�û�ȡ���ڴ� */
                                tcp_send (socket_tcp1, sendbuf, maxlen);
                            }

                        }while(iCount > 0);
                    }
                    break;

                /* ���յ�ҡ��������£�TCP�ͻ���2��TCP����������4096�ֽ����� */
                case JOY_L_BIT4:
                    TcpState = tcp_get_state(socket_tcp2);
                    printf_debug("TCP�ͻ���2״̬ = %s\r\n", ReVal_Table[TcpState]);
                    iCount = 4096;
                    /* ��֤�Ѿ����������˲ſ��Է������� */
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
                                    /* ��ô����û����� */
                                    maxlen = iCount + maxlen;
                                }

                                /* �������ʼ����ÿ�����������ݰ���ǰ8���ֽ� */
                                sendbuf = tcp_get_buf(maxlen);
                                sendbuf[0] = 'a';
                                sendbuf[1] = 'b';
                                sendbuf[2] = 'c';
                                sendbuf[3] = 'd';
                                sendbuf[4] = 'e';
                                sendbuf[5] = 'f';
                                sendbuf[6] = 'g';
                                sendbuf[7] = 'h';

                                /* ���Է���ֻ��ʹ�û�ȡ���ڴ� */
                                tcp_send (socket_tcp2, sendbuf, maxlen);
                            }

                        }while(iCount > 0);
                    }
                    break;

                /* ���յ�ҡ���Ҽ����£�TCP�ͻ���3��TCP����������4096�ֽ����� */
                case JOY_R_BIT5:
                    TcpState = tcp_get_state(socket_tcp3);
                    printf_debug("TCP�ͻ���3״̬ = %s\r\n", ReVal_Table[TcpState]);
                    iCount = 4096;
                    /* ��֤�Ѿ����������˲ſ��Է������� */
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
                                    /* ��ô����û����� */
                                    maxlen = iCount + maxlen;
                                }

                                /* �������ʼ����ÿ�����������ݰ���ǰ8���ֽ� */
                                sendbuf = tcp_get_buf(maxlen);
                                sendbuf[0] = 'a';
                                sendbuf[1] = 'b';
                                sendbuf[2] = 'c';
                                sendbuf[3] = 'd';
                                sendbuf[4] = 'e';
                                sendbuf[5] = 'f';
                                sendbuf[6] = 'g';
                                sendbuf[7] = 'h';

                                /* ���Է���ֻ��ʹ�û�ȡ���ڴ� */
                                tcp_send (socket_tcp3, sendbuf, maxlen);
                            }

                        }while(iCount > 0);
                    }
                    break;

                /* �����ļ�ֵ������ */
                default:
                    break;
            }
        }
    }
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
