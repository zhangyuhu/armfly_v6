/*
*********************************************************************************************************
*
*	ģ������ : ������ģ�顣
*	�ļ����� : main.c
*	��    �� : V1.0
*	˵    �� : RL-TCPnet���������ģ���ʵ��
*              ʵ��Ŀ�ģ�
*                1. ѧϰRL-TCPnet���������ģ���������
*              ʵ�����ݣ�
*                1. ǿ���Ƽ������߽ӵ�·�������߽�����������ԣ���Ϊ�Ѿ�ʹ����DHCP�������Զ���ȡIP��ַ��
*                2. ������һ��TCP Server������ʹ���˾���������NetBIOS���û�ֻ���ڵ��Զ�ping armfly
*                   �Ϳ��Ի�ð��ӵ�IP��ַ���˿ں�1001��
*                3. �û������ڵ��Զ������������������TCP Client���Ӵ˷������ˡ�
*                4. ����K1���£�����8�ֽڵ����ݸ�TCP Client��
*                5. ����K2���£�����1024�ֽڵ����ݸ�TCP Client��
*                6. ����K3���£�����5MB�ֽڵ����ݸ�TCP Client��
*              ע�����
*                1. ��ؽ��༭��������������TAB����Ϊ4���Ķ����ļ���Ҫ��������ʾ�����롣
*
*	�޸ļ�¼ :
*		�汾��   ����         ����            ˵��
*       V1.0    2017-04-17   Eric2013    1. ST�̼���1.6.1�汾
*                                        2. BSP������V1.2
*                                        3. RL-TCPnet�汾V4.74
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "bsp.h"			/* �ײ�Ӳ������ */
#include "app_tcpnet_lib.h"



/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: ��׼c������ڡ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int main (void) 
{	
	/* ��ʼ������ */
	bsp_Init();

	/* ����RL-TCPnet���Ժ��� */
	TCPnetTest();
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/