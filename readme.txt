CC2541����Ԥ����ĺ�
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

HalUARTInit û�ж��壬Ҳ����û�д��ڵĳ�ʼ��

HAL ʹ�õ���CC2540EB�е��ļ�


HAL_BOARD_INIT�� ��ʼ��ʱ�� 32M��32k
InitBoard ����һ�ιر������ж� �ر�LED

hal\target\xx ������ͬ��hal
hal\target\xx\hal_board_cfg.h ÿ�����ӵ������ļ�

HalDriverInit��
	->HalAdcInit


Hal_ProcessPoll ��ֻ��ALLOW_SLEEP_MODE