
#include "CS1237.h"
#include "hal_drivers.h"
#include "ioCC2541.h"

//CLK p1.1 pull up
//DIO p1.2 pull down 

static  void CS1237_WriteReg(uint8 adreg);
void Delay(int n)
{
  while(n--);
}
void io_dir_in(void)
{
   P0DIR &= ~(1<<5);  //P1_1����Ϊ���� DIO
}
void io_dir_out(void)
{
   P0DIR |= (1<<5);  //P1_1����Ϊ��� DIO
}
void clk_high(void)
{
  P0 |= (1<<6);
}
void clk_low(void)
{
  P0 &= ~(1<<6);
}
void data_high(void)
{
  P0 |= (1<<5);
}
void data_low(void)
{
  P0 &= ~(1<<5);
}
/******************************************************************************
  * @brief  .
  * @param  None
  * @retval 
******************************************************************************/
void CS1237_Init(void)
{
  //GPIO_Init(CS1237_DIO_GPIO_PORT, CS1237_DIO_GPIO_PINS, GPIO_MODE_IN_PU_NO_IT); //pull up dio
  //GPIO_Init(CS1237_CLK_GPIO_PORT, CS1237_CLK_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST); //pull down  clk
#if 0
  P1SEL &= ~0x01; //��ͨIO��
  P1DIR |= 0x01;  //P1_1����Ϊ���� DIO
  P1INP &=~0x01;	//������
  
  P1 |= 0x1;
  P1 &= ~0x1;
  P1 |= 0x1;
  
  P1SEL &= ~0x02; //��ͨIO��
  P1DIR |= 0x02;  //P1_0����Ϊ��� LED
  P1INP &=~0x02;	//������

  P1SEL &= ~0x04; //��ͨIO��
  P1DIR &= ~0x04;  //P1_2����Ϊ���� DIO
  P1INP &=~0x04;	//������
#else
  P1SEL &= ~0x01; //��ͨIO��
  P1DIR |= 0x01;  //P1_0����Ϊ���
  P1INP &=~0x01;	//������
  
  P1 |= 0x1;
  P1 &= ~0x1;
  P1 |= 0x1;
 
  
  P0SEL &= ~(1<<6); //��ͨIO��
  P0DIR |= (1<<6);  //P0_6����Ϊ��� CLK
  P0INP &=~(1<<6);	//������

  P0SEL &= ~(1<<5); //��ͨIO��
  P0DIR &= ~(1<<5);  //P0_5����Ϊ���� DIO
  P0INP &=~(1<<5);	//������

#endif
  Delay(1000);
  CS1237_WriteReg(0x1C);        //40Hz,128PGA
}
uint8 read_io(void)
{
  return (P0&(1<<5))?1:0;
}
/******************************************************************************
  * @brief  .
  * @param  None
  * @retval 
******************************************************************************/
void CS1237_Clock(void)
{
    clk_high();
    Delay(6);
    clk_low();
    Delay(6);
}
/******************************************************************************
  * @brief  .
  * @param  None
  * @retval 
******************************************************************************/
int8 CS1237_ReadAD(int32* ad)
{
  uint8 i;
  int32 addat = 0;
  
  if(read_io())
    return 0;
  // while(IO_Data_AD);
  for(i = 0; i < 24; i++) //���� 24 �� CLK����������
  {
    addat <<= 1;
    CS1237_Clock();
    if(read_io())
      addat ++;
  }
  CS1237_Clock();
  CS1237_Clock();
  CS1237_Clock(); //CLK27������ DRDY
  addat <<= 8;
  addat >>= 14;
  *ad = addat;
  return 1;
}
/******************************************************************************
  * @brief  .
  * @param  None
  * @retval 
******************************************************************************/
uint8 CS1237_ReadReg(void)
{
  uint8 i,command = 0x56;
  
  for(i = 0; i < 27; i++)//clk1-clk27
    CS1237_Clock();
  io_dir_out();//�� MCU_Data �ܽ���Ϊ���
  CS1237_Clock();//clk28
  CS1237_Clock();//clk29
  
  for(i = 0; i < 7; i ++) //clk30-clk36(���Ͷ�����),7bits ����
  {
    if(command & 0x40) //MSB -> LSB
      data_high();
    else
      data_low();
    command <<= 1;
    CS1237_Clock();
  }
  io_dir_in();//�� MCU_Data �ܽ���Ϊ����
  CS1237_Clock();//clk37
  command = 0;
  for(i = 0; i < 8; i++) //clk38-clk45(��ȡ�Ĵ���),8bits ����
  {
    command <<= 1;
    CS1237_Clock();
    if(read_io())
      command++;
  }
  CS1237_Clock();//clk46
  return command;
}
/******************************************************************************
  * @brief  .
  * @param  None
  * @retval 
******************************************************************************/
void CS1237_WriteReg(uint8 adreg)
{
  uint8 i,command = 0x65;
  
  for(i = 0; i < 27; i++)//clk1-clk27
    CS1237_Clock();
  io_dir_out();//�� MCU_Data �ܽ���Ϊ���
  CS1237_Clock();//clk28
  CS1237_Clock();//clk29
  
  for(i = 0; i < 7; i ++) //clk30-clk36(���Ͷ�����),7bits ����
  {
    if(command & 0x40) //MSB -> LSB
      data_high();
    else
      data_low();
    command <<= 1;
    CS1237_Clock();
  }
  CS1237_Clock();//clk37
  
  for(i = 0; i < 8; i++) //clk38-clk45(д��Ĵ���),8bits ����
  {
    if(adreg & 0x80) //MSB -> LSB
      data_high();
    else
      data_low();
    adreg <<= 1;
    CS1237_Clock();
  }
  io_dir_in();//�� MCU_Data �ܽ���Ϊ����
  CS1237_Clock();//clk46
}














