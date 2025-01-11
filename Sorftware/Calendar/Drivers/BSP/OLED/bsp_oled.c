#include "oled.h"
// #include "bmp.h"
// #include "oledfont.h"
 #include "ds1302.h"
 #include "stdio.h"

#define SHOW_MODE 1    
#define FONT_SIZE 16

// 显示年月日的函数接口
void OLED_ShowDate(uint8_t x, uint8_t y, Time_s time)
{
    uint8_t offset = 0;
    
    // 显示年份
    OLED_ShowNum(x + offset, y, time.year + 2000, 4, FONT_SIZE, SHOW_MODE); // 假设time.year存储的是2000年之后的年份
    offset += 4 * (FONT_SIZE / 2 + 1); // 假设FONT_SIZE为16，每个数字占用16+4个像素宽度

    // 显示分隔符“年”
    OLED_ShowChinese(x + offset, y, 0, FONT_SIZE, SHOW_MODE); // 假设0对应“年”字
    offset += FONT_SIZE + 4;    // 汉字占用FONT_SIZE宽度，加上额外的间隔

    // 显示月份
    OLED_ShowNum(x + offset, y, time.month, 2, FONT_SIZE, SHOW_MODE);
    offset += 2 * (FONT_SIZE / 2 + 1);

    // 显示分隔符“月”
    OLED_ShowChinese(x + offset, y, 1, FONT_SIZE, SHOW_MODE); // 假设1对应“月”字
    offset += FONT_SIZE + 4;

    // 显示日期
    OLED_ShowNum(x + offset, y, time.date, 2, FONT_SIZE, SHOW_MODE);
    offset += 2 * (FONT_SIZE / 2 + 1);

    // 显示分隔符“日”
    OLED_ShowChinese(x + offset, y, 2, FONT_SIZE, SHOW_MODE); // 假设2对应“日”字
}

void OLED_ShowYearDate(uint8_t x, uint8_t y, Time_s time)
{
    // 计算日期字符串
    char dateStr[20];  // 假设最大长度不超过20字符
    snprintf(dateStr, sizeof(dateStr), "%04d年%02d月%02d日", (time.year + 2000), time.month, time.date);
    
    // for(int i=0;i<ARRAY_SIZE(dateStr);i++)
    // {
    //     printf("%c",dateStr[i]);
    // }
    // printf("\n");
    // 使用OLED_ShowString一次性显示日期字符串
    OLED_ShowChineseString(x, y, dateStr, FONT_SIZE, SHOW_MODE);
	
}

