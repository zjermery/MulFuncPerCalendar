#ifndef LIB_LANGUAGE_H
#define LIB_LANGUAGE_H

typedef enum
{
    SYSTEM_LANGUAGE_CHINESE = 0,
    SYSTEM_LANGUAGE_ENGLISH,

    SYSTEM_LANGUAGE_ALL,
} SystemLanguage_e;


typedef enum
{
    TEXT_MENU = 0,              // 菜单
    TEXT_MAIN_MENU,             // 主菜�?
    TEXT_SELECT_OPTION,         // 选择操作
    TEXT_ENTER,                 // 进入
    TEXT_EXIT,                  // 退�?
    TEXT_RETURN,                // 返回
    TEXT_MUSIC,                 // 音乐
    TEXT_VIDEO,                 // 视�??
    TEXT_CAMERA,                // 摄像�?
    TEXT_SETTING,               // 设置
    TEXT_SHORTCUT,              // �?�?
    TEXT_MORE,                  // 更�??
    TEXT_LANGUAGE,              // �?言
    TEXT_NEXT,                  // 下一�?
    TEXT_PREVIOUS,              // 上一�?
    TEXT_FUNCTION_TEST,         // 功能测试
    TEXT_TEST,                  // 测试
    TEXT_BLUETOOTH,             // 蓝牙
    TEXT_BATTERY,               // 电池
    TEXT_STORE,                 // 储存
    TEXT_UPGRADE,               // 升级
    TEXT_ABOUT,                 // 关于
    TEXT_PHOTO,                 // 拍照
    TEXT_PHOTOGRAPHY,           // 摄影
    TEXT_LOADING,               // 加载�?
    TEXT_STOPPING_PLAYBACK,     // 正在停�?�播�?
    TEXT_EXIT_MAIN_MENU,        // 退出主菜单
    TEXT_RETURN_MAIN_MENU,      // 返回主菜�?
    TEXT_MORE_SETTING,          // 更�?��?�置
    TEXT_MORE_PARAM,            // 更�?�参�?
    TEXT_ENTER_MAIN_MENU,       // 进入主菜�?
    TEXT_DEV_NAME,              // 设�?�名�?
    TEXT_AUTO_SLEEP_FUN,        // �?动休�?
    TEXT_AUTO_SLEEP_TIME,       // �?动休眠时�?
    TEXT_AUTO_POWER_OFF_FUN,    // �?动关�?
    TEXT_AUTO_POWER_OFF_TIME,   // �?动关机时�?
    TEXT_BOOL_OPEN,             // 开
    TEXT_BOOL_CLOSE,            // �?
    TEXT_BEING_PLAYING,         // 正在�?�?
    TEXT_PLAY_STOP,             // �?放暂�?

    TEXT_ALL,
} TextId_e;

void set_language(SystemLanguage_e lang);
const char *get_text(TextId_e id);
const char *get_text_by_language(SystemLanguage_e lang, TextId_e id);

#endif


