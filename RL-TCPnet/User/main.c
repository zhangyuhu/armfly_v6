/*
*********************************************************************************************************
*
*    ģ������ : ������ģ�顣
*    �ļ����� : main.c
*    ��    �� : V1.0
*    ˵    �� : FreeRTOS + RL-TCPnet�Ĺ���ģ���ʵ�֡�
*              ʵ��Ŀ�ģ�
*                1. ѧϰFreeRTOS + RL-TCPnet�Ĺ���ģ���������
*              ʵ�����ݣ�
*                1. ����ҡ�˵�OK������ͨ�����ڴ�ӡ����ִ�������������115200������λ8����żУ��λ�ޣ�ֹͣλ1��
*                   =================================================
*                   ������      ����״̬ ���ȼ�   ʣ��ջ �������
*                   vTaskUserIF     R       1       181     1
*                   IDLE            R       0       117     6
*                   vTaskMsgPro     B       3       489     3
*                   vTaskStart      B       5       434     5
*                   vTaskLED        B       2       483     2
*                   vTaskTCPnet     B       4       448     4
*
*                   ������       ���м���         ʹ����
*                   vTaskUserIF     226             <1%
*                   IDLE            1335930         97%
*                   vTaskStart      36419           2%
*                   vTaskLED        0               <1%
*                   vTaskTCPnet     5               <1%
*                   vTaskMsgPro     1               <1%
*                  �����������ʹ��SecureCRT��V6���������д�������鿴��ӡ��Ϣ��
*                  ��������ʵ�ֵĹ������£�
*                   vTaskTaskUserIF ����: ������Ϣ����
*                   vTaskLED        ����: LED��˸��
*                   vTaskMsgPro     ����: ��Ϣ������������������⡣
*                   vTaskTCPnet     ����: RL-TCPnet��������
*                   vTaskStart      ����: ��������ʵ��RL-TCPnet��ʱ���׼���¡�
*                2. ��������״̬�Ķ������£������洮�ڴ�ӡ��ĸB, R, D, S��Ӧ��
*                    #define tskBLOCKED_CHAR        ( 'B' )  ����
*                    #define tskREADY_CHAR            ( 'R' )  ����
*                    #define tskDELETED_CHAR        ( 'D' )  ɾ��
*                    #define tskSUSPENDED_CHAR        ( 'S' )  ����
*                3. ǿ���Ƽ������߽ӵ�·�������߽�����������ԣ���Ϊ�Ѿ�ʹ����DHCP�������Զ���ȡIP��ַ��
*                4. ������һ��TCP Server������ʹ���˾���������NetBIOS���û�ֻ���ڵ��Զ�ping armfly
*                   �Ϳ��Ի�ð��ӵ�IP��ַ���˿ں�1001��
*                5. �û������ڵ��Զ�����������������TCP Client���Ӵ˷������ˡ�
*                6. ����K1���£�����8�ֽڵ����ݸ�TCP Client��
*                7. ����K2���£�����1024�ֽڵ����ݸ�TCP Client��
*                8. ����K3���£�����5MB�ֽڵ����ݸ�TCP Client��
*              ע�����
*                1. ��ʵ���Ƽ�ʹ�ô������SecureCRT��Ҫ�����ڴ�ӡЧ�������롣�������
*                   V6��������������С�
*                2. ��ؽ��༭��������������TAB����Ϊ4���Ķ����ļ���Ҫ��������ʾ�����롣
*
*    �޸ļ�¼ :
*        �汾��    ����         ����            ˵��
*       V1.0    2017-04-28   Eric2013    1. ST�̼��⵽V1.6.1�汾
*                                        2. BSP������V1.2
*                                        3. FreeRTOS�汾V8.2.3
*                                        4. RL-TCPnet�汾V4.74
*
*    Copyright (C), 2016-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"



/*
**********************************************************************************************************
                                            ��������
**********************************************************************************************************
*/
static void vTaskTaskUserIF(void *pvParameters);
static void vTaskLED(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskTCPnet(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);
static void AppObjCreate (void);
static void App_Printf(char *format, ...);


/*
**********************************************************************************************************
                                            ��������
**********************************************************************************************************
*/
static TaskHandle_t xHandleTaskUserIF = NULL;
static TaskHandle_t xHandleTaskLED = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskTCPnet = NULL;
static TaskHandle_t xHandleTaskStart = NULL;
static SemaphoreHandle_t  xMutex = NULL;
EventGroupHandle_t xCreatedEventGroup = NULL;


/*
*********************************************************************************************************
*    �� �� ��: main
*    ����˵��: ��׼c������ڡ�
*    ��    �Σ���
*    �� �� ֵ: ��
*********************************************************************************************************
*/
int main(void)
{
    /*
      ����������ǰ��Ϊ�˷�ֹ��ʼ��STM32����ʱ���жϷ������ִ�У������ֹȫ���ж�(����NMI��HardFault)��
      �������ĺô��ǣ�
      1. ��ִֹ�е��жϷ����������FreeRTOS��API������
      2. ��֤ϵͳ�������������ܱ���ж�Ӱ�졣
      3. �����Ƿ�ر�ȫ���жϣ���Ҹ����Լ���ʵ��������ü��ɡ�
      ����ֲ�ļ�port.c�еĺ���prvStartFirstTask�л����¿���ȫ���жϡ�ͨ��ָ��cpsie i������__set_PRIMASK(1)
      ��cpsie i�ǵ�Ч�ġ�
     */
    __set_PRIMASK(1);

    /* Ӳ����ʼ�� */
    bsp_Init();

    /* 1. ��ʼ��һ����ʱ���жϣ����ȸ��ڵδ�ʱ���жϣ������ſ��Ի��׼ȷ��ϵͳ��Ϣ ��������Ŀ�ģ�ʵ����
          Ŀ�в�Ҫʹ�ã���Ϊ������ܱȽ�Ӱ��ϵͳʵʱ�ԡ�
       2. Ϊ����ȷ��ȡFreeRTOS�ĵ�����Ϣ�����Կ��ǽ�����Ĺر��ж�ָ��__set_PRIMASK(1); ע�͵���
    */
    vSetupSysInfoTest();

    /* �������� */
    AppTaskCreate();

    /* ��������ͨ�Ż��� */
    AppObjCreate();

    /* �������ȣ���ʼִ������ */
    vTaskStartScheduler();

    /*
      ���ϵͳ���������ǲ������е�����ģ����е����Ｋ�п��������ڶ�ʱ��������߿��������
      heap�ռ䲻����ɴ���ʧ�ܣ���Ҫ�Ӵ�FreeRTOSConfig.h�ļ��ж����heap��С��
      #define configTOTAL_HEAP_SIZE          ( ( size_t ) ( 17 * 1024 ) )
    */
    while(1);
}

/*
*********************************************************************************************************
*    �� �� ��: vTaskTaskUserIF
*    ����˵��: �ӿ���Ϣ����
*    ��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*    �� �� ֵ: ��
*   �� �� ��: 1  (��ֵԽС���ȼ�Խ�ͣ������uCOS�෴)
*********************************************************************************************************
*/
static void vTaskTaskUserIF(void *pvParameters)
{
    uint8_t ucKeyCode;
    uint8_t pcWriteBuffer[500];


    while(1)
    {
        ucKeyCode = bsp_GetKey();

        if (ucKeyCode != KEY_NONE)
        {
            switch (ucKeyCode)
            {
                /* K1�����£�ֱ�ӷ����¼���־������vTaskTCPnet������bit0 */
                case KEY_DOWN_K1:
                    App_Printf("K1�����£�ֱ�ӷ����¼���־������AppTaskTCPMain��bit0������\r\n");
                    xEventGroupSetBits(xCreatedEventGroup, KEY1_BIT0);
                    break;

                /* K2�����£�ֱ�ӷ����¼���־������vTaskTCPnet������bit1 */
                case KEY_DOWN_K2:
                    App_Printf("K2�����£�ֱ�ӷ����¼���־������AppTaskTCPMain��bit1������\r\n");
                    xEventGroupSetBits(xCreatedEventGroup, KEY2_BIT1);
                    break;

                /* K3�����£�ֱ�ӷ����¼���־������vTaskTCPnet������bit2 */
                case KEY_DOWN_K3:
                    App_Printf("K3�����£�ֱ�ӷ����¼���־������AppTaskTCPMain��bit2������\r\n");
                    xEventGroupSetBits(xCreatedEventGroup, KEY3_BIT2);
                    break;

                /* ҡ�˵�OK�����£���ӡ����ִ����� */
                case JOY_DOWN_OK:
                    App_Printf("=================================================\r\n");
                    App_Printf("������      ����״̬ ���ȼ�   ʣ��ջ �������\r\n");
                    vTaskList((char *)&pcWriteBuffer);
                    App_Printf("%s\r\n", pcWriteBuffer);

                    App_Printf("\r\n������       ���м���         ʹ����\r\n");
                    vTaskGetRunTimeStats((char *)&pcWriteBuffer);
                    App_Printf("%s\r\n", pcWriteBuffer);
                    App_Printf("��ǰ��̬�ڴ�ʣ���С = %d�ֽ�\r\n", xPortGetFreeHeapSize());
                    break;

                /* �����ļ�ֵ������ */
                default:
                    break;
            }
        }

        vTaskDelay(20);
    }
}

/*
*********************************************************************************************************
*    �� �� ��: vTaskLED
*    ����˵��: LED��˸
*    ��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*    �� �� ֵ: ��
*   �� �� ��: 2
*********************************************************************************************************
*/
static void vTaskLED(void *pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 500;

    /* ��ȡ��ǰ��ϵͳʱ�� */
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        bsp_LedToggle(2);

        /* vTaskDelayUntil�Ǿ����ӳ٣�vTaskDelay������ӳ١�*/
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

/*
*********************************************************************************************************
*    �� �� ��: vTaskMsgPro
*    ����˵��: ��Ϣ�������������������
*    ��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*    �� �� ֵ: ��
*   �� �� ��: 3
*********************************************************************************************************
*/
static void vTaskMsgPro(void *pvParameters)
{
    while(1)
    {
        /* ����ɨ�� */
        bsp_KeyScan();
        vTaskDelay(10);
    }
}

/*
*********************************************************************************************************
*    �� �� ��: vTaskTCPnet
*    ����˵��: RL-TCPnet��������
*    ��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*    �� �� ֵ: ��
*   �� �� ��: 4
*********************************************************************************************************
*/
static void vTaskTCPnet(void *pvParameters)
{
    while(1)
    {
        TCPnetTest();
    }
}

/*
*********************************************************************************************************
*    �� �� ��: vTaskStart
*    ����˵��: ��������Ҳ��������ȼ���������ʵ��RL-TCPnet��ʱ���׼����
*    ��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*    �� �� ֵ: ��
*   �� �� ��: 5
*********************************************************************************************************
*/
static void vTaskStart(void *pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 100;

    /* ��ʼ��RL-TCPnet */
    init_TcpNet ();

    /* ��ȡ��ǰ��ϵͳʱ�� */
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        /* RL-TCPnetʱ���׼���º��� */
        timer_tick ();

        /* vTaskDelayUntil�Ǿ����ӳ٣�vTaskDelay������ӳ١�*/
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

/*
*********************************************************************************************************
*    �� �� ��: AppTaskCreate
*    ����˵��: ����Ӧ������
*    ��    �Σ���
*    �� �� ֵ: ��
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
    xTaskCreate( vTaskTaskUserIF,       /* ������  */
                 "vTaskUserIF",         /* ������    */
                 512,                   /* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,                  /* �������  */
                 1,                     /* �������ȼ�*/
                 &xHandleTaskUserIF );  /* ������  */


    xTaskCreate( vTaskLED,            /* ������  */
                 "vTaskLED",          /* ������    */
                 512,                 /* stack��С����λword��Ҳ����4�ֽ� */
                 NULL,                /* �������  */
                 2,                   /* �������ȼ�*/
                 &xHandleTaskLED ); /* ������  */

    xTaskCreate( vTaskMsgPro,             /* ������  */
                 "vTaskMsgPro",           /* ������    */
                 512,                     /* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,                   /* �������  */
                 3,                       /* �������ȼ�*/
                 &xHandleTaskMsgPro );  /* ������  */

    xTaskCreate( vTaskTCPnet,             /* ������  */
                 "vTaskTCPnet",           /* ������    */
                 512,                    /* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,                   /* �������  */
                 4,                      /* �������ȼ�*/
                 &xHandleTaskTCPnet );  /* ������  */


    xTaskCreate( vTaskStart,             /* ������  */
                 "vTaskStart",           /* ������    */
                 512,                    /* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,                   /* �������  */
                 5,                      /* �������ȼ�*/
                 &xHandleTaskStart );   /* ������  */
}

/*
*********************************************************************************************************
*    �� �� ��: AppObjCreate
*    ����˵��: ��������ͨ�Ż���
*    ��    ��: ��
*    �� �� ֵ: ��
*********************************************************************************************************
*/
static void AppObjCreate (void)
{
    /* ���������ź��� */
    xMutex = xSemaphoreCreateMutex();

    if(xMutex == NULL)
    {
        /* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
    }

    /* �����¼���־�� */
    xCreatedEventGroup = xEventGroupCreate();

    if(xCreatedEventGroup == NULL)
    {
        /* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
    }
}

/*
*********************************************************************************************************
*    �� �� ��: App_Printf
*    ����˵��: �̰߳�ȫ��printf��ʽ
*    ��    ��: ͬprintf�Ĳ�����
*             ��C�У����޷��г����ݺ���������ʵ�ε����ͺ���Ŀʱ,������ʡ�Ժ�ָ��������
*    �� �� ֵ: ��
*********************************************************************************************************
*/
static void  App_Printf(char *format, ...)
{
    char  buf_str[512 + 1];
    va_list   v_args;


    va_start(v_args, format);
   (void)vsnprintf((char       *)&buf_str[0],
                   (size_t      ) sizeof(buf_str),
                   (char const *) format,
                                  v_args);
    va_end(v_args);

    /* �����ź��� */
    xSemaphoreTake(xMutex, portMAX_DELAY);

    printf("%s", buf_str);

       xSemaphoreGive(xMutex);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
