#include "iocc2530.h"
#include "oled.h"
#include "bmp.h"
#include "stdlib.h"

unsigned char abscissa = 0; //�Զ��˵ĺ�����
unsigned char ordinate = 0; //�Զ��˵�������
unsigned char direction = 3;  //�Զ��˵ķ���

unsigned char food_abscissa = 0;  //ʳ��ĺ�����
unsigned char food_ordinate = 0;  //ʳ���������
unsigned char food = 4;  //  

//�������Ļ�ϲ���һ��ʳ��
void produce_food(void)
{
   if(abscissa==food_abscissa && ordinate==food_ordinate)
   {
      OLED_Clear();       //OLEDˢ��
      OLED_ShowCHinese(abscissa , ordinate , direction);  //��ʾ�Զ���
      food_abscissa = rand()%8 * 16;
      food_ordinate = rand()%4 * 2;
      OLED_ShowCHinese(food_abscissa , food_ordinate , food);  //��ʾʳ��   
   }
    if(abscissa==food_abscissa && ordinate==food_ordinate)
    {
        produce_food();
    }
}

//�ж��Ƿ񵽴��˱߽磬�������ִ�в���
int borderJudgment(int dir)
{
    if(dir == 1) //��
    {
        if (ordinate == 0) return 0;
    } else if (dir ==2) //��
    {
        if(ordinate == 3*2) return 0;
    } else if (dir ==3) //��
    {
        if(abscissa == 0) return 0;
    } else if (dir ==4) //��
    {
        if(abscissa == 7*16) return 0;
    }
    return 1;
}

void main(void)
{   
    P1SEL &= ~0xf4; //P1_2Ϊ��ͨI/O��
    P1DIR &= ~0xf4; //P1_2Ϊ�����
    
    /*************�����ⲿ�жϳ�ʼ������****************/
    IEN2 |= 0x10;           //ʹ��P1���ж�
    P1IEN |= 0xf4;          //ʹ��P1_2���ж�
    PICTL |= 0x06;          //P1_3��P1_0���½��ش����ж�
    EA = 1;                 //ʹ�����ж�
    P1IFG&=~0xf4;           //��ʼ���жϱ�־λ
    /***************************************************/
    
    P0DIR|=0x03;        //��ʼ�� IIC��SCL��ʱ���ߣ�SDA �������ߣ�
    OLED_Clear();       //OLEDˢ��
    OLED_Init();	 //��ʼ��OLED  
    produce_food();
}

/*P1���ⲿ�жϷ�����*/
#pragma  vector = P1INT_VECTOR
__interrupt void P1_INT(void)
{
    if(P1IFG & 0x04)       //���P1_2���жϱ�־λ��λ
    {
      abscissa = 0; //�Զ��˵ĺ�����
      ordinate = 0; //�Զ��˵�������
      direction = 3;  //�Զ��˵ķ���

      food_abscissa = 0;  //ʳ��ĺ�����
      food_ordinate = 0;  //ʳ���������
      produce_food();
      
      P1IFG &= ~0x04;        //���P1_2���жϱ�־λ 
    }
    
    if(P1IFG & 0x10)       //P1_4   ��    V1
    {
      OLED_Clear();       //OLEDˢ��
      direction=3;
      
      if(borderJudgment(4)) abscissa+=16;   
      
      OLED_ShowCHinese(abscissa,ordinate,direction);
      OLED_ShowCHinese(food_abscissa , food_ordinate , food);
      
      if(abscissa==food_abscissa && ordinate==food_ordinate) produce_food();
      
      P1IFG &= ~0x10;        //���P1_2���жϱ�־λ 
    }    
    
    
    if(P1IFG & 0x20)       //P1_5   ��    V2
    {
      OLED_Clear();       //OLEDˢ��
      direction=1;
      
      if(borderJudgment(2)) ordinate+=2;        

      OLED_ShowCHinese(abscissa,ordinate,direction);
      OLED_ShowCHinese(food_abscissa , food_ordinate , food);
      
      if(ordinate==food_ordinate) produce_food();
      
      P1IFG &= ~0x20;        //���P1_2���жϱ�־λ 
    }
    
    if(P1IFG & 0x80)       //P1_7   ��     V3
    {
      OLED_Clear();       //OLEDˢ��
      direction=2;
      
      if(borderJudgment(3)) abscissa-=16;

      OLED_ShowCHinese(abscissa,ordinate,direction);
      OLED_ShowCHinese(food_abscissa , food_ordinate , food);
      
      if(abscissa==food_abscissa && ordinate==food_ordinate) produce_food();

      P1IFG &= ~0x80;        //���P1_2���жϱ�־λ 
    }

    if(P1IFG & 0x40)       //P1_8   ��     V4
    {
      OLED_Clear();       //OLEDˢ��
      direction=0;
      
      if(borderJudgment(1)) ordinate-=2;  
      
      OLED_ShowCHinese(abscissa,ordinate,direction);
      OLED_ShowCHinese(food_abscissa , food_ordinate , food);
      
      if(abscissa==food_abscissa && ordinate==food_ordinate) produce_food();
      
      P1IFG &= ~0x40;        //���P1_2���жϱ�־λ 
    }

   P1IF = 0;              //���P1���жϱ�־λ         
}


