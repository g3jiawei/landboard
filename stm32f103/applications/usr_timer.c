


#include "usr_timer.h"


TIM_HandleTypeDef htimer6,htimer7;  /* ��ʱ����� */

//    static uint16_t num1 = 0;
//    static uint16_t num2 = 0;
/**
 * @brief       ������ʱ��TIMX��ʱ�жϳ�ʼ������
 * @note
 *              ������ʱ����ʱ������APB1,��PPRE1 �� 2��Ƶ��ʱ��
 *              ������ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ36M, ���Զ�ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void btim6_int_init(uint16_t arr, uint16_t psc)
{
    htimer6.Instance = BTIM6_INT;                      /* ͨ�ö�ʱ��X */
    htimer6.Init.Prescaler = psc;                          /* ����Ԥ��Ƶϵ�� */
//    htimer6.Init.CounterMode = TIM_COUNTERMODE_UP;         /* ��������ģʽ */
    htimer6.Init.Period = arr;                             /* �Զ�װ��ֵ */
    HAL_TIM_Base_Init(&htimer6);

    HAL_TIM_Base_Start_IT(&htimer6);    /* ʹ�ܶ�ʱ��x��������ж� */
}

void btim7_int_init(uint16_t arr, uint16_t psc)
{
    htimer7.Instance = BTIM7_INT;                      /* ͨ�ö�ʱ��X */
    htimer7.Init.Prescaler = psc;                          /* ����Ԥ��Ƶϵ�� */
//    htimer6.Init.CounterMode = TIM_COUNTERMODE_UP;         /* ��������ģʽ */
    htimer7.Init.Period = arr;                             /* �Զ�װ��ֵ */
    HAL_TIM_Base_Init(&htimer7);

    HAL_TIM_Base_Start_IT(&htimer7);    /* ʹ�ܶ�ʱ��x��������ж� */
}


/**
 * @brief       ��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
                �˺����ᱻHAL_TIM_Base_Init()��������
 * @param       htim:��ʱ�����
 * @retval      ��
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == BTIM6_INT)
    {
        BTIM6_INT_CLK_ENABLE();                     /* ʹ��TIMʱ�� */
        HAL_NVIC_SetPriority(BTIM6_INT_IRQn, 1, 3); /* ��ռ1�������ȼ�3����2 */
        HAL_NVIC_EnableIRQ(BTIM6_INT_IRQn);         /* ����ITM3�ж� */
    }
     else if (htim->Instance == BTIM7_INT)
    {
        BTIM7_INT_CLK_ENABLE();                     /* ʹ��TIMʱ�� */
        HAL_NVIC_SetPriority(BTIM7_INT_IRQn, 1, 2); /* ��ռ1�������ȼ�3����2 */
        HAL_NVIC_EnableIRQ(BTIM7_INT_IRQn);         /* ����ITM3�ж� */
    }
}

/**
 * @brief       ��ʱ��TIMX�жϷ�����
 * @param       ��
 * @retval      ��
 */
void BTIM6_INT_IRQHandler(void)
{
    static uint16_t num1 = 0;
    //    HAL_TIM_IRQHandler(&htimer6); /* ��ʱ���жϹ��������� */
        if (TIM_GET_ITSTATUS(&htimer6, TIM_IT_UPDATE) != RESET)
    {
      TIM_GET_CLEAR_IT(&htimer6, TIM_IT_UPDATE);      //����жϱ�־λ
        /***************************************************
         *   ����û�����
        *****************************************************/
        rt_kprintf("timer 6 %d \n",num1 );          /**< ��ӡnum1 */
        num1++;                                     /**< û��һ���ж�num1��1 */
    }
}

void BTIM7_INT_IRQHandler(void)
{
    static uint16_t num2 = 0;
//    HAL_TIM_IRQHandler(&htimer7); /* ��ʱ���жϹ��������� */
    if (TIM_GET_ITSTATUS(&htimer7, TIM_IT_UPDATE) != RESET)
    {
      TIM_GET_CLEAR_IT(&htimer7, TIM_IT_UPDATE);      //����жϱ�־λ
        /*usr code*/
        if(++num2 ==30){
        rt_kprintf("timer7 %d \n",num2);
            num2 = 0;
        /*usr code*/
        }
    }
    
}

/**
 * @brief       ��ʱ�������жϻص�����
 * @param       htim:��ʱ�����
 * @retval      ��
 */

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
// 

//    if (htim->Instance == TIM6)
//    {
////        rt_kprintf("timer 6 interupt num : %d \n",num);
//        num1++;
//    }
//    else if (htim->Instance == TIM7)
//    {
////        num2++;
//    }

//}
rt_timer_t stimer1,stimer2;

void stimeout1(void *parameter)
{
    static int cnt = 0;
    rt_kprintf("periodic timer is timeout %d\n", cnt);
//    rt_kprintf("periodic timer is timeout %d\n", *((int*)parameter));
    /* ���е� 10 �Σ�ֹͣ���ڶ�ʱ�� */
    if (cnt++>= *((int*)parameter))
    {
        rt_timer_stop(stimer1);
        rt_kprintf("periodic timer was stopped! \n");
    }
}

