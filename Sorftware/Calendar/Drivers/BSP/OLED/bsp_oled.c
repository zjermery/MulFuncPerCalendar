#include "oled.h"
// #include "bmp.h"
// #include "oledfont.h"
 #include "ds1302.h"

// 显示年月日的函数接口
void OLED_ShowDate(uint8_t x, uint8_t y, Time_s time)
{
    uint8_t offset = 0;
    uint8_t size1 = 16;
    uint8_t mode = 1; // 1表示白底黑字，0表示黑底白字
    // 显示年份
    OLED_ShowNum(x + offset, y, time.year + 2000, 4, size1, mode); // 假设time.year存储的是2000年之后的年份
    offset += 4 * (size1 / 2 + 2); // 假设size1为16，每个数字占用16+4个像素宽度

    // 显示分隔符“年”
    OLED_ShowChinese(x + offset, y, 0, size1, mode); // 假设0对应“年”字
    offset += size1 + 8;    // 汉字占用size1宽度，加上额外的间隔

    // 显示月份
    OLED_ShowNum(x + offset, y, time.month, 2, size1, mode);
    offset += 2 * (size1 / 2 + 2);

    // 显示分隔符“月”
    OLED_ShowChinese(x + offset, y, 1, size1, mode); // 假设1对应“月”字
    offset += size1 + 8;

    // 显示日期
    OLED_ShowNum(x + offset, y, time.date, 2, size1, mode);
    offset += 2 * (size1 / 2 + 2);

    // 显示分隔符“日”
    OLED_ShowChinese(x + offset, y, 2, size1, mode); // 假设2对应“日”字
}