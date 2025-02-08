/**
 ******************************************************************************
 * @file         ds1302.c
 * @author       Aaron Escoboza
 * @brief        Source file for DS1302 RTC driver
 * @link         GitGub : https://github.com/aaron-ev
 ******************************************************************************
 */
#include "ds1302.h"
#include "sys.h"
#include "delay.h"
#include "string.h"
#include "error_code.h"

static Time_s g_time;      


Time_s *get_time_contx(void)
{
    return &g_time;
}

/**
 * @brief Clock cycle on CLK line
 * @param void
 * @return void
 */
static void set_clk_cycle(void)
{
    DS1302_CLK_Set();
    delay_us(1);
    DS1302_CLK_Clr();
    delay_us(1);
}

/**
 * @brief Set idle state SDA, CLK and RST low value.
 * @param void
 * @return void
 */
static void set_idle_state(void)
{
    DS1302_DAT_Clr();
    DS1302_CLK_Clr();
    DS1302_RST_Clr();
}

/**
 * @brief Initialize the DS1302 defult data
 *
 * @param void
 * @return void
 */
static void ds1302_defult_time_config(void)
{
    g_time.day = SUNDAY;                                // Day: 星期三
    g_time.sec =  0;                                    // sec: 30秒
    g_time.min = 54;                                    // min: 45分钟
    g_time.year = 25;                                   // year: 2022年（0-99）
    g_time.hour = 23;                                   // hour: 10点（24小时制）
    g_time.date = 25;                                   // date: 25号
    g_time.month = 12;                                  // month: 12月
    g_time.clockSystem = DS1302_CLK_SYSTEM_24;          // clockSystem: 24小时制
    g_time.clockPeriod = DS1302_CLK_PM_PERIOD;          // clockPeriod: 上午
    ds1302_set_time(&g_time);
}


/**
 * @brief Read mode: SDA pin is set as input
 * @param void
 * @return void
 */
static void set_read_mode(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = DS1302_DAT_GPIO_PIN;
    GPIO_InitStructure.Pull = GPIO_PULLDOWN;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Mode =  GPIO_MODE_INPUT;
    HAL_GPIO_Init(DS1302_DAT_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief Write mode: SDA pin is set as ouput
 * @param void
 * @return void
 */
static void set_write_mode(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = DS1302_DAT_GPIO_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(DS1302_DAT_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief write a command/address on SDA line
 *
 * @param bit you being written
 * @return void
 */
static void write_cmd(uint8_t cmd)
{
    uint8_t i;
    /* RST high to start communication and write cmd/address */
    // set_rst();
	DS1302_RST_Set();
    for (i = 0; i < DS1302_CMD_SIZE; i++)
    {
        (cmd & 1) ? DS1302_DAT_Set() : DS1302_DAT_Clr();
        set_clk_cycle();
        cmd >>= 1;
    }
}

/**
 * @brief write a byte following serial communication specified in the spec
 *
 * @param addr where you want write to
 * @param data that will write to address
 * @return void
 */
static void write_data(uint8_t addr, uint8_t data)
{
    uint8_t i;

    write_cmd(addr);
    /* Write data bit by bit */
    for (i = 0; i < DS1302_DATA_SIZE; i++)
    {
        (data & 1) ? DS1302_DAT_Set() : DS1302_DAT_Clr();
        set_clk_cycle();
        data >>= 1;
    }
    set_idle_state();
}

static GPIO_PinState read_data_pin(void)
{
    return HAL_GPIO_ReadPin(DS1302_DAT_GPIO_PORT, DS1302_DAT_GPIO_PIN);
}

/**
 * @brief read a byte following serial communication specified in the spec
 *
 * @param addr address you want to read from
 * @return byte read
 */
static uint8_t read_data(uint8_t addr)
{
    uint8_t i;
    uint8_t data = 0;

    /* Make sure LSB bit is high for reading an address */
    addr |= 0x1;
    write_cmd(addr);

    set_read_mode();
    for (i = 0; i < DS1302_DATA_SIZE; i++)
    {
        /* Ones come from the MSB to LSB
         * by shifting data to the right
        */
        if (read_data_pin() == GPIO_PIN_SET)
        {
            data |= 0x80;
        }
        set_clk_cycle();
        if (i != (DS1302_DATA_SIZE - 1))
        {
            data >>= 1;
        }
    }
    set_write_mode();

    set_idle_state();

    return data;
}

/**
 * @brief Get time from calendar registers
 *
 * @param time pointer to time structure
 * @return void
 */
Time_s *ds1302_get_time(void)
{
    uint8_t tmpMaskClockSystem = 0;

    tmpMaskClockSystem = ((g_time.clockSystem == DS1302_CLK_SYSTEM_24) ?MASK_HOURS_24: MASK_HOURS_12);
    g_time.min = BCD_TO_DEC(read_data(DS1302_REG_MIN));
    g_time.day = (Day)BCD_TO_DEC(read_data(DS1302_REG_DAY));
    g_time.year = BCD_TO_DEC(read_data(DS1302_REG_YEAR));
    g_time.date = BCD_TO_DEC(read_data(DS1302_REG_DATE));
    g_time.month = BCD_TO_DEC(read_data(DS1302_REG_MONTH));
    g_time.sec = BCD_TO_DEC((read_data(DS1302_REG_SEC) & MASK_SECONDS));
    g_time.hour = BCD_TO_DEC((read_data(DS1302_REG_HOUR) & tmpMaskClockSystem));
    
    return &g_time;
}

/**
 * @brief set time into calendar registers
 *        
 * @param time pointer to time structure
 * @return void
 */
error_code_t ds1302_set_time(Time_s *time)
{    
    uint8_t tmpMaskClockSystem = 0;

    /* When 12 clock system is set, bit 7 should be high 
     * according to the spec, low for 24 clock system.
    */
    if (time->clockSystem != DS1302_CLK_SYSTEM_24)
    {
        /* Set 12 hour clock system */
        tmpMaskClockSystem |=  MASK_CLOCK_SYSTEM;
        if (time->clockPeriod == DS1302_CLK_PM_PERIOD)
        {
            /* Set PM clock period */
            tmpMaskClockSystem |= MASK_CLOCK_PERIOD;
        }
    }
    /* Enable write by driving protected bit to 0*/
    write_data(DS1302_REG_CONTROL, 0);

    /* Write time data to RTC calendar registers in BCD format */
    write_data(DS1302_RAM_ADDR_START,FLAG_VAL);//写入已经设置时间标记
    write_data(DS1302_REG_MIN, DEC_TO_BCD(time->min));
    write_data(DS1302_REG_SEC, DEC_TO_BCD(time->sec));
    write_data(DS1302_REG_DAY, DEC_TO_BCD(time->day));
    write_data(DS1302_REG_YEAR, DEC_TO_BCD(time->year));
    write_data(DS1302_REG_DATE, DEC_TO_BCD(time->date));
    write_data(DS1302_REG_MONTH, DEC_TO_BCD(time->month));
    write_data(DS1302_REG_HOUR, DEC_TO_BCD(time->hour) | tmpMaskClockSystem);

    /* Disable write by driving protected bit to 1 */
    write_data(DS1302_REG_CONTROL, 0x80);
    return ERROR_CODE_SUCCESS;
}


/**
 * @brief Write to RAM, valid addresses 0 - 30
 * @param addr address you want to write to
 * @param data data to write into the address
 * @return void
 */
void ds1302_write_ram(const uint8_t addr, uint8_t data)
{
    if (addr > (DS1302_RAM_SIZE - 1))
    { 
        return;
    }
    /* Enable write by driving protected bit to 0*/
    write_data(DS1302_REG_CONTROL, 0);
    /* Write addresses for RAM are multiple of 2 */
    write_data(DS1302_RAM_ADDR_START + (2 * addr), data);
    /* Disable write by driving protected bit to 1 */
    write_data(DS1302_REG_CONTROL, 0x80);
}

/**
 * @brief Read from RAM, valid addresses 0 - 30
 * @param addr address you want to read from
 * @return data read
 */
uint8_t ds1302_read_ram(const uint8_t addr)
{
    if (addr > (DS1302_RAM_SIZE - 1))
    {
        return 0;
    }
    return read_data(DS1302_RAM_ADDR_START + (2 * addr));
}

/**
 * @brief Clear the entire RAM addresses (0 - 30)
 * @param void
 * @return void
 */
void ds1302_clear_ram(void)
{
    int i;
    for (i = 0; i < DS1302_RAM_SIZE; i++)
    {
        ds1302_write_ram(i, 0);
    }
}

/**
 * @brief get clock system 12 or 24 format
 * @param void
 * @return ClockSystem 
 */
ClockSystem ds1302_get_clock_system(void)
{
    return (read_data(DS1302_REG_HOUR) & MASK_CLOCK_SYSTEM) ?
            DS1302_CLK_SYSTEM_12 : DS1302_CLK_SYSTEM_24;
}

/**
 * @brief get clock period AM or PM
 * @param void
 * @return ClockPeriod 
 */
ClockPeriod ds1302_get_clock_period(void)
{
    return (read_data(DS1302_REG_HOUR) & MASK_CLOCK_PERIOD) ?
           DS1302_CLK_PM_PERIOD : DS1302_CLK_AM_PERIOD;
}

/**
 * @brief read in burst mode
 *
 * @param addr address for ram or calendar
 * @param len  number of bytes to read
 * @param buff buffer where data will be stored
 * @return void
 */
static void read_burst(uint8_t addr, uint8_t len, uint8_t *buff)
{
    uint8_t i, j;

    /* Make sure LSB bit is high for reading an address */
    addr |= 0x1;
    write_cmd(addr);

    set_read_mode();
    for (i = 0; i < len; i++)
    {
        buff[i] = 0;
        for (j = 0; j < DS1302_DATA_SIZE; j++)
        {
            /* Ones come from the MSB to LSB
            * by shifting data to the right
            */
            if (read_data_pin() == GPIO_PIN_SET)
            {
                buff[i] |= 0x80;
            }
            set_clk_cycle();
            if (j != (DS1302_DATA_SIZE - 1))
            {
                buff[i] >>= 1;
            }
        }
    }
    set_write_mode();
    set_idle_state();
}

/**
 * @brief write in burst mode
 *
 * @param addr address for ram or calendar
 * @param len number of addresses to write to
 * @param buff buffer that will write to RTC
 * @return void
 */
static void write_burst(uint8_t addr, uint8_t len, uint8_t *buff)
{
    uint8_t i, j;

    write_cmd(addr);
    /* Write data bit by bit */
    for (i = 0; i < len; i++)
    {
        for (j = 0; j < DS1302_DATA_SIZE; j++)
        {
            (buff[i] & 1) ? DS1302_DAT_Set() : DS1302_DAT_Clr();
            set_clk_cycle();
            buff[i] >>= 1;
        }
    }
    set_idle_state();
}

/**
 * @brief Write to RAM in burst mode
 * @param len Number of addresses to write to 
 * @param buff Buffer that will write to RTC
 * @return void
 */
void ds1302_write_ram_burst(uint8_t len, uint8_t *buff)
{
    if (len > DS1302_RAM_SIZE)
    { 
        return;
    }
    /* Enable write by driving protected bit to 0*/
    write_data(DS1302_REG_CONTROL, 0);
    /* Write addresses for RAM are multiple of 2 */
    write_burst(D1302_RAM_BURST_MODE, len, buff);
    /* Disable write by driving protected bit to 1 */
    write_data(DS1302_REG_CONTROL, 0x80);
}

/**
 * @brief Read RAM in burst mode
 * @param len Number of addresses to read from RTC
 * @param buff Buffer where data will be stored
 */
void ds1302_read_ram_burst(uint8_t len, uint8_t *buff)
{
    if (len > DS1302_RAM_SIZE)
    {
        return;
    }
    memset(buff, 0, 1);
    read_burst(D1302_RAM_BURST_MODE, len, buff);
}


/**
 * @brief Initialize the DS1302 device
 *
 * @param void
 * @return void
 */
void ds1302_init(void)
{
    uint16_t temp_reg;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Initialize  SCLK, SDA and RST as output, pull push and speed high */
    GPIO_InitStructure.Pin = DS1302_CLK_GPIO_PIN | DS1302_DAT_GPIO_PIN | DS1302_RST_GPIO_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Mode =  GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(DS1302_GPIO_PORT, &GPIO_InitStructure);

    set_idle_state();

    temp_reg = read_data(DS1302_RAM_ADDR_START);  //先读TIME[7]地址的值

    // printf("temp_reg= 0X%x\r\n",temp_reg);
    if(temp_reg == 0xFF)       //检验该地址的值是否被更改
    {        
        ;      //如果已经被更该就不用初始化了，说明曾经初始化了
    }
    else   //如果没有更该就初始化
    {
            write_data(DS1302_REG_CONTROL, 0);                //禁止写保护，就是关闭写保护功能
            ds1302_defult_time_config();
            write_data(DS1302_REG_CONTROL, 0x80);                //打开写保护功能
    }
}



