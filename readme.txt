CC2541工程预定义的宏
INT_HEAP_LEN=3072
HALNODEBUG
OSAL_CBTIMER_NUM_TASKS=1
HAL_AES_DMA=TRUE
HAL_DMA=TRUE
POWER_SAVING
xPLUS_BROADCASTER
HAL_LCD=FALSE
HAL_LED=TRUE
CC2540_MINIDK

HalUARTInit 没有定义，也就是没有串口的初始化

HAL 使用的是CC2540EB中的文件


HAL_BOARD_INIT： 初始化时钟 32M和32k
InitBoard ：第一次关闭所有中断 关闭LED

hal\target\xx 各个不同的hal
hal\target\xx\hal_board_cfg.h 每个板子的配置文件

HalDriverInit：
	->HalAdcInit


Hal_ProcessPoll 中只有ALLOW_SLEEP_MODE