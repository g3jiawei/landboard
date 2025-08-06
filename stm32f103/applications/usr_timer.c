


#include "usr_timer.h"


TIM_HandleTypeDef htimer6,htimer7;  /* 定时器句柄 */

//    static uint16_t num1 = 0;
//    static uint16_t num2 = 0;
/**
 * @brief       基本定时器TIMX定时中断初始化函数
 * @note
 *              基本定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              基本定时器的时钟为APB1时钟的2倍, 而APB1为36M, 所以定时器时钟 = 72Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr: 自动重装值。
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void btim6_int_init(uint16_t arr, uint16_t psc)
{
    htimer6.Instance = BTIM6_INT;                      /* 通用定时器X */
    htimer6.Init.Prescaler = psc;                          /* 设置预分频系数 */
//    htimer6.Init.CounterMode = TIM_COUNTERMODE_UP;         /* 递增计数模式 */
    htimer6.Init.Period = arr;                             /* 自动装载值 */
    HAL_TIM_Base_Init(&htimer6);

    HAL_TIM_Base_Start_IT(&htimer6);    /* 使能定时器x及其更新中断 */
}

void btim7_int_init(uint16_t arr, uint16_t psc)
{
    htimer7.Instance = BTIM7_INT;                      /* 通用定时器X */
    htimer7.Init.Prescaler = psc;                          /* 设置预分频系数 */
//    htimer6.Init.CounterMode = TIM_COUNTERMODE_UP;         /* 递增计数模式 */
    htimer7.Init.Period = arr;                             /* 自动装载值 */
    HAL_TIM_Base_Init(&htimer7);

    HAL_TIM_Base_Start_IT(&htimer7);    /* 使能定时器x及其更新中断 */
}


/**
 * @brief       定时器底层驱动，开启时钟，设置中断优先级
                此函数会被HAL_TIM_Base_Init()函数调用
 * @param       htim:定时器句柄
 * @retval      无
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == BTIM6_INT)
    {
        BTIM6_INT_CLK_ENABLE();                     /* 使能TIM时钟 */
        HAL_NVIC_SetPriority(BTIM6_INT_IRQn, 1, 3); /* 抢占1，子优先级3，组2 */
        HAL_NVIC_EnableIRQ(BTIM6_INT_IRQn);         /* 开启ITM3中断 */
    }
     else if (htim->Instance == BTIM7_INT)
    {
        BTIM7_INT_CLK_ENABLE();                     /* 使能TIM时钟 */
        HAL_NVIC_SetPriority(BTIM7_INT_IRQn, 1, 2); /* 抢占1，子优先级3，组2 */
        HAL_NVIC_EnableIRQ(BTIM7_INT_IRQn);         /* 开启ITM3中断 */
    }
}

/**
 * @brief       定时器TIMX中断服务函数
 * @param       无
 * @retval      无
 */
void BTIM6_INT_IRQHandler(void)
{
    static uint16_t num1 = 0;
    //    HAL_TIM_IRQHandler(&htimer6); /* 定时器中断公共处理函数 */
        if (TIM_GET_ITSTATUS(&htimer6, TIM_IT_UPDATE) != RESET)
    {
      TIM_GET_CLEAR_IT(&htimer6, TIM_IT_UPDATE);      //清除中断标志位
        /***************************************************
         *   添加用户程序
        *****************************************************/
        rt_kprintf("timer 6 %d \n",num1 );          /**< 打印num1 */
        num1++;                                     /**< 没进一次中断num1加1 */
    }
}

void BTIM7_INT_IRQHandler(void)
{
    static uint16_t num2 = 0;
//    HAL_TIM_IRQHandler(&htimer7); /* 定时器中断公共处理函数 */
    if (TIM_GET_ITSTATUS(&htimer7, TIM_IT_UPDATE) != RESET)
    {
      TIM_GET_CLEAR_IT(&htimer7, TIM_IT_UPDATE);      //清除中断标志位
        /*usr code*/
        if(++num2 ==30){
        rt_kprintf("timer7 %d \n",num2);
            num2 = 0;
        /*usr code*/
        }
    }
    
}

/**
 * @brief       定时器更新中断回调函数
 * @param       htim:定时器句柄
 * @retval      无
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
    /* 运行第 10 次，停止周期定时器 */
    if (cnt++>= *((int*)parameter))
    {
        rt_timer_stop(stimer1);
        rt_kprintf("periodic timer was stopped! \n");
    }
}

