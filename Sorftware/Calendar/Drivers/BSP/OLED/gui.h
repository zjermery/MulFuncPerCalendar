/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2025-02-08 09:49:00
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-02-08 16:35:44
 * @FilePath: \Calendar\Drivers\BSP\OLED\gui.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
		
#ifndef __GUI_H__
#define __GUI_H__

void GUI_DrawPoint(uint8_t x, uint8_t y, uint8_t color);
void GUI_Fill(uint8_t sx,uint8_t sy,uint8_t ex,uint8_t ey,uint8_t color);
void GUI_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,uint8_t color);
void GUI_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,uint8_t color);
void GUI_FillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,uint8_t color);
void GUI_DrawCircle(uint8_t xc, uint8_t yc, uint8_t color, uint8_t r);
void GUI_FillCircle(uint8_t xc, uint8_t yc, uint8_t color, uint8_t r);
void GUI_DrawTriangel(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t color);
void GUI_FillTriangel(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t color);
void GUI_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size,uint8_t mode);
void GUI_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t Size,uint8_t mode);
void GUI_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size,uint8_t mode);
void GUI_ShowFont16(uint8_t x,uint8_t y,uint8_t *s,uint8_t mode);
void GUI_ShowFont24(uint8_t x,uint8_t y,uint8_t *s,uint8_t mode);
void GUI_ShowFont32(uint8_t x,uint8_t y,uint8_t *s,uint8_t mode);
void GUI_ShowCHinese(uint8_t x,uint8_t y,uint8_t hsize,uint8_t *str,uint8_t mode);
void GUI_DrawBMP(uint8_t x,uint8_t y,uint8_t width, uint8_t height, uint8_t BMP[], uint8_t mode); 
void GUI_DrawPicture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,const unsigned char BMP[],uint8_t mode);
void GUI_ShowCHineseAndASCII(uint8_t x,uint8_t y,uint8_t *str,uint8_t mode);
#endif

