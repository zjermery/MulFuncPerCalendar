#include "lib_language.h"

static SystemLanguage_e sg_eSystemLanguage = SYSTEM_LANGUAGE_CHINESE;

const char *(sg_kSystemLanguage[TEXT_ALL])[SYSTEM_LANGUAGE_ALL] =
{
    [TEXT_MENU] = {"�˵�", "menu"},
    [TEXT_MAIN_MENU] = {"���˵�", "main menu"},
    [TEXT_SELECT_OPTION] = {"ѡ�����", "select option"},
    [TEXT_ENTER] = {"����", "enter"},
    [TEXT_EXIT] = {"�˳�", "exit"},
    [TEXT_RETURN] = {"����", "return"},
    [TEXT_MUSIC] = {"����", "music"},
    [TEXT_VIDEO] = {"��Ƶ", "video"},
    [TEXT_CAMERA] = {"�����", "camera"},
    [TEXT_SETTING] = {"����", "setting"},
    [TEXT_SHORTCUT] = {"���", "shortcut"},
    [TEXT_MORE] = {"����", "more"},
    [TEXT_LANGUAGE] = {"����", "language"},
    [TEXT_NEXT] = {"��һ��", "next"},
    [TEXT_PREVIOUS] = {"��һ��", "previous"},
    [TEXT_FUNCTION_TEST] = {"���ܲ���", "functional testing"},
    [TEXT_TEST] = {"����", "test"},
    [TEXT_BLUETOOTH] = {"����", "bluetooth"},
    [TEXT_BATTERY] = {"���", "battery"},
    [TEXT_STORE] = {"����", "store"},
    [TEXT_UPGRADE] = {"����", "upgrade"},
    [TEXT_ABOUT] = {"����", "about"},
    [TEXT_PHOTO] = {"����", "Photo"},
    [TEXT_PHOTOGRAPHY] = {"��Ӱ", "photography"},
    [TEXT_LOADING] = {"������", "loading"},
    [TEXT_STOPPING_PLAYBACK] = {"����ֹͣ����", "stopping playback"},
    [TEXT_EXIT_MAIN_MENU] = {"�˳����˵�", "exit main menu"},
    [TEXT_RETURN_MAIN_MENU] = {"�������˵�", "return main menu"},
    [TEXT_MORE_SETTING] = {"��������", "more setting"},
    [TEXT_MORE_PARAM] = {"�������", "more param"},
    [TEXT_ENTER_MAIN_MENU] = {"�������˵�", "Enter Main Eenu"},
    [TEXT_DEV_NAME] = {"�豸����", "device name"},
    [TEXT_AUTO_SLEEP_FUN] = {"����", "Sleep"},
    [TEXT_AUTO_SLEEP_TIME] = {"����ʱ��", "SleepTime"},
    [TEXT_AUTO_POWER_OFF_FUN] = {"�ػ�", "PowerOff"},
    [TEXT_AUTO_POWER_OFF_TIME] = {"�ػ�ʱ��", "OffTime"},
    [TEXT_BOOL_OPEN] = {"��", "ON"},
    [TEXT_BOOL_CLOSE] = {"��", "OFF"},
    [TEXT_BEING_PLAYING] = {"���ڲ���", " playing "},
    [TEXT_PLAY_STOP] = {"������ͣ", "play stop"},
};

void set_language(SystemLanguage_e lang)
{
    if (lang < SYSTEM_LANGUAGE_ALL)
    {
        sg_eSystemLanguage = lang;
    }
}


const char *get_text(TextId_e id)
{
    static const char *pszNullString = "N/A";

    if (id < TEXT_ALL)
    {
        return sg_kSystemLanguage[id][sg_eSystemLanguage];
    }

    return pszNullString; // δ�ҵ���Ӧ���ı�
}

const char *get_text_by_language(SystemLanguage_e lang, TextId_e id)
{
    static const char *pszNullString = "N/A";

    if (id < TEXT_ALL)
    {
        return sg_kSystemLanguage[id][lang];
    }

    return pszNullString; // δ�ҵ���Ӧ���ı�
}



