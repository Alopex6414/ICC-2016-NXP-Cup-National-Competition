/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：delay.c
 * 描述         ：延时函数定义
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

#include  "delay.h"


/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：delay
*  功能说明：延时函数（不准确）
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-1    已测试
*  备    注：
*************************************************************************/
void delay(void)
{
    delayms(500);
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：delayms
*  功能说明：延时函数（不准确），内核频率为100M时较为准确
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-1    已测试
*  备    注：time_delay_ms(10); //已测试准确10ms
*************************************************************************/
void delayms(uint32  ms)
{
    uint32  i, j;
    for(i = 0; i < ms; i++)
    {
        for(j = bus_clk_khz; j > 0; j--)
        {
            asm("nop");
        }
    }
}
