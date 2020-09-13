#include "iocc2530.h"
#include "oled.h"
#include "bmp.h"
#include "stdlib.h"

unsigned char abscissa = 0; //吃豆人的横坐标
unsigned char ordinate = 0; //吃豆人的纵坐标
unsigned char direction = 3;  //吃豆人的方向

unsigned char food_abscissa = 0;  //食物的横坐标
unsigned char food_ordinate = 0;  //食物的纵坐标
unsigned char food = 4;  //  

//随机在屏幕上产生一个食物
void produce_food(void)
{
   if(abscissa==food_abscissa && ordinate==food_ordinate)
   {
      OLED_Clear();       //OLED刷屏
      OLED_ShowCHinese(abscissa , ordinate , direction);  //显示吃豆人
      food_abscissa = rand()%8 * 16;
      food_ordinate = rand()%4 * 2;
      OLED_ShowCHinese(food_abscissa , food_ordinate , food);  //显示食物   
   }
    if(abscissa==food_abscissa && ordinate==food_ordinate)
    {
        produce_food();
    }
}

//判断是否到达了边界，如果是则不执行操作
int borderJudgment(int dir)
{
    if(dir == 1) //下
    {
        if (ordinate == 0) return 0;
    } else if (dir ==2) //上
    {
        if(ordinate == 3*2) return 0;
    } else if (dir ==3) //右
    {
        if(abscissa == 0) return 0;
    } else if (dir ==4) //左
    {
        if(abscissa == 7*16) return 0;
    }
    return 1;
}

void main(void)
{   
    P1SEL &= ~0xf4; //P1_2为普通I/O口
    P1DIR &= ~0xf4; //P1_2为输入口
    
    /*************新增外部中断初始化部分****************/
    IEN2 |= 0x10;           //使能P1口中断
    P1IEN |= 0xf4;          //使能P1_2口中断
    PICTL |= 0x06;          //P1_3到P1_0口下降沿触发中断
    EA = 1;                 //使能总中断
    P1IFG&=~0xf4;           //初始化中断标志位
    /***************************************************/
    
    P0DIR|=0x03;        //初始化 IIC的SCL（时钟线）SDA （数据线）
    OLED_Clear();       //OLED刷屏
    OLED_Init();	 //初始化OLED  
    produce_food();
}

/*P1口外部中断服务函数*/
#pragma  vector = P1INT_VECTOR
__interrupt void P1_INT(void)
{
    if(P1IFG & 0x04)       //如果P1_2口中断标志位置位
    {
      abscissa = 0; //吃豆人的横坐标
      ordinate = 0; //吃豆人的纵坐标
      direction = 3;  //吃豆人的方向

      food_abscissa = 0;  //食物的横坐标
      food_ordinate = 0;  //食物的纵坐标
      produce_food();
      
      P1IFG &= ~0x04;        //清除P1_2口中断标志位 
    }
    
    if(P1IFG & 0x10)       //P1_4   左    V1
    {
      OLED_Clear();       //OLED刷屏
      direction=3;
      
      if(borderJudgment(4)) abscissa+=16;   
      
      OLED_ShowCHinese(abscissa,ordinate,direction);
      OLED_ShowCHinese(food_abscissa , food_ordinate , food);
      
      if(abscissa==food_abscissa && ordinate==food_ordinate) produce_food();
      
      P1IFG &= ~0x10;        //清除P1_2口中断标志位 
    }    
    
    
    if(P1IFG & 0x20)       //P1_5   上    V2
    {
      OLED_Clear();       //OLED刷屏
      direction=1;
      
      if(borderJudgment(2)) ordinate+=2;        

      OLED_ShowCHinese(abscissa,ordinate,direction);
      OLED_ShowCHinese(food_abscissa , food_ordinate , food);
      
      if(ordinate==food_ordinate) produce_food();
      
      P1IFG &= ~0x20;        //清除P1_2口中断标志位 
    }
    
    if(P1IFG & 0x80)       //P1_7   右     V3
    {
      OLED_Clear();       //OLED刷屏
      direction=2;
      
      if(borderJudgment(3)) abscissa-=16;

      OLED_ShowCHinese(abscissa,ordinate,direction);
      OLED_ShowCHinese(food_abscissa , food_ordinate , food);
      
      if(abscissa==food_abscissa && ordinate==food_ordinate) produce_food();

      P1IFG &= ~0x80;        //清除P1_2口中断标志位 
    }

    if(P1IFG & 0x40)       //P1_8   下     V4
    {
      OLED_Clear();       //OLED刷屏
      direction=0;
      
      if(borderJudgment(1)) ordinate-=2;  
      
      OLED_ShowCHinese(abscissa,ordinate,direction);
      OLED_ShowCHinese(food_abscissa , food_ordinate , food);
      
      if(abscissa==food_abscissa && ordinate==food_ordinate) produce_food();
      
      P1IFG &= ~0x40;        //清除P1_2口中断标志位 
    }

   P1IF = 0;              //清除P1口中断标志位         
}


