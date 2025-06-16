#if 1 /*Set it to "1" to enable content*/

#ifndef LV_CONF_H
#define LV_CONF_H

#define LV_COLOR_DEPTH 16
#define LV_COLOR_16_SWAP 0

#define LV_MEM_SIZE (48U * 1024U)          /*[bytes]*/
#define LV_MEM_ADR 0                       /*0: unused*/
#define LV_MEM_AUTO_DEFRAG 1

#define LV_INDEV_DEF_READ_PERIOD 30        /*[ms]*/
#define LV_INDEV_DEF_DRAG_LIMIT 10         /*[px]*/
#define LV_INDEV_DEF_DRAG_THROW 10         /*[%]*/
#define LV_INDEV_DEF_LONG_PRESS_TIME 400   /*[ms]*/
#define LV_INDEV_DEF_LONG_PRESS_REP_TIME 100 /*[ms]*/
#define LV_INDEV_DEF_GESTURE_LIMIT 50      /*[px]*/
#define LV_INDEV_DEF_GESTURE_MIN_VELOCITY 3

#define LV_USE_ANIMATION 1
#if LV_USE_ANIMATION
#define LV_ANIM_DEF_TIME 200 /*[ms]*/
#define LV_ANIM_DEF_DELAY 0  /*[ms]*/
#define LV_ANIM_DEF_PATH_CB lv_anim_path_linear /*Animation path for simple animations*/
#endif

#define LV_USE_ASSERT_NULL 1
#define LV_USE_ASSERT_MALLOC 1
#define LV_USE_ASSERT_STYLE 0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ 0

#define LV_USE_LOG 0
#if LV_USE_LOG
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
#define LV_LOG_PRINTF 0
#endif

#define LV_USE_GPU_STM32_DMA2D 0

#define LV_USE_GPU_ARM2D 0

#define LV_USE_GPU_SWM341_DMA2D 0

#define LV_USE_GPU_NXP_PXP 0
#define LV_USE_GPU_NXP_VG_LITE 0

#define LV_USE_GPU_SDL 0

#define LV_DISP_DEF_REFR_PERIOD 30 /*[ms]*/
#define LV_DISP_ROT_MAX_BUF (10*1024)

#define LV_USE_PERF_MONITOR 0
#define LV_USE_MEM_MONITOR 0

#define LV_USE_REFR_DEBUG 0

#define LV_SPRINTF_CUSTOM 0
#if LV_SPRINTF_CUSTOM
#define LV_SPRINTF_INCLUDE <stdio.h>
#define lv_snprintf snprintf
#define lv_vsnprintf vsnprintf
#else
#define LV_SPRINTF_USE_FLOAT 0
#endif

#define LV_USE_USER_DATA 1

#define LV_ENABLE_GC 1
#if LV_ENABLE_GC != 0
#define LV_GC_INCLUDE "gc.h"
#endif

#define LV_USE_LARGE_COORD 0

#define LV_FONT_MONTSERRAT_8 0
#define LV_FONT_MONTSERRAT_10 0
#define LV_FONT_MONTSERRAT_12 0
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 0
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 1
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_MONTSERRAT_30 0
#define LV_FONT_MONTSERRAT_32 0
#define LV_FONT_MONTSERRAT_34 0
#define LV_FONT_MONTSERRAT_36 0
#define LV_FONT_MONTSERRAT_38 0
#define LV_FONT_MONTSERRAT_40 0
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0
#define LV_FONT_MONTSERRAT_48 0

#define LV_FONT_MONTSERRAT_12_SUBPX 0
#define LV_FONT_MONTSERRAT_28_COMPRESSED 0
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0
#define LV_FONT_SIMSUN_16_CJK 0

#define LV_FONT_UNSCII_8 0
#define LV_FONT_UNSCII_16 0

#define LV_FONT_CUSTOM_DECLARE

#define LV_FONT_DEFAULT &lv_font_montserrat_14

#define LV_FONT_FMT_TXT_LARGE 0

#define LV_USE_FONT_COMPRESSED 1

#define LV_USE_FONT_SUBPX 0
#if LV_USE_FONT_SUBPX
#define LV_FONT_SUBPX_BGR 0
#endif

#define LV_TXT_ENC LV_TXT_ENC_UTF8

#define LV_TXT_BREAK_CHARS " ,.;:-_"

#define LV_TXT_LINE_BREAK_LONG_LEN 0
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN 3
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3

#define LV_TXT_COLOR_CMD "#"

#define LV_USE_BIDI 0
#if LV_USE_BIDI
#define LV_BIDI_BASE_DIR_DEF LV_BASE_DIR_AUTO
#endif

#define LV_USE_ARABIC_PERSIAN_CHARS 0

#define LV_USE_ARC 1

#define LV_USE_ANIMIMG 1

#define LV_USE_BAR 1

#define LV_USE_BTN 1

#define LV_USE_BTNMATRIX 1

#define LV_USE_CANVAS 1

#define LV_USE_CHECKBOX 1

#define LV_USE_DROPDOWN 1
#if LV_USE_DROPDOWN != 0
#define LV_DROPDOWN_DEF_ANIM_TIME 200
#endif

#define LV_USE_IMG 1

#define LV_USE_LABEL 1
#if LV_USE_LABEL
#define LV_LABEL_TEXT_SELECTION 1
#define LV_LABEL_LONG_TXT_HINT 1
#endif

#define LV_USE_LINE 1

#define LV_USE_ROLLER 1
#if LV_USE_ROLLER
#define LV_ROLLER_INF_PAGES 7
#endif

#define LV_USE_SLIDER 1

#define LV_USE_SWITCH 1

#define LV_USE_TEXTAREA 1
#if LV_USE_TEXTAREA != 0
#define LV_TEXTAREA_DEF_CURSOR_BLINK_TIME 400
#define LV_TEXTAREA_DEF_PWD_SHOW_TIME 1500
#endif

#define LV_USE_TABLE 1

#define LV_USE_CALENDAR 1
#if LV_USE_CALENDAR
#define LV_CALENDAR_WEEK_STARTS_MONDAY 0
#if LV_CALENDAR_WEEK_STARTS_MONDAY
#define LV_CALENDAR_DEFAULT_DAY_NAMES {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"}
#else
#define LV_CALENDAR_DEFAULT_DAY_NAMES {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"}
#endif
#define LV_CALENDAR_DEFAULT_MONTH_NAMES {"January", "February", "March",  "April", "May",  "June", "July", "August", "September", "October", "November", "December"}
#define LV_USE_CALENDAR_HEADER_ARROW 1
#define LV_USE_CALENDAR_HEADER_DROPDOWN 1
#endif

#define LV_USE_CHART 1

#define LV_USE_COLORWHEEL 1

#define LV_USE_IMGBTN 1

#define LV_USE_KEYBOARD 1

#define LV_USE_LED 1

#define LV_USE_LIST 1

#define LV_USE_MENU 1

#define LV_USE_METER 1

#define LV_USE_MSGBOX 1

#define LV_USE_SPINBOX 1
#define LV_USE_SPINNER 1

#define LV_USE_TABVIEW 1

#define LV_USE_TILEVIEW 1

#define LV_USE_WIN 1

#define LV_USE_SPAN 1
#if LV_USE_SPAN
#define LV_SPAN_SNIPPET_STACK_SIZE 64
#endif

#define LV_USE_THEME_DEFAULT 1
#if LV_USE_THEME_DEFAULT
#define LV_THEME_DEFAULT_DARK 0
#define LV_THEME_DEFAULT_GROW 1
#define LV_THEME_DEFAULT_TRANSITION_TIME 80
#endif

#define LV_USE_THEME_BASIC 1

#define LV_USE_FLEX 1
#define LV_USE_GRID 1

#define LV_USE_FS_STDIO 0
#if LV_USE_FS_STDIO
#define LV_FS_STDIO_LETTER '\0'
#define LV_FS_STDIO_PATH ""
#define LV_FS_STDIO_CACHE_SIZE 0
#endif

#define LV_USE_FS_POSIX 0
#if LV_USE_FS_POSIX
#define LV_FS_POSIX_LETTER '\0'
#define LV_FS_POSIX_PATH ""
#define LV_FS_POSIX_CACHE_SIZE 0
#endif

#define LV_USE_FS_WIN32 0
#if LV_USE_FS_WIN32
#define LV_FS_WIN32_LETTER '\0'
#define LV_FS_WIN32_PATH ""
#define LV_FS_WIN32_CACHE_SIZE 0
#endif

#define LV_USE_FS_FATFS 0
#if LV_USE_FS_FATFS
#define LV_FS_FATFS_LETTER '\0'
#define LV_FS_FATFS_CACHE_SIZE 0
#endif

#define LV_USE_PNG 0
#define LV_USE_BMP 0
#define LV_USE_SJPG 0
#define LV_USE_GIF 0
#define LV_USE_QRCODE 0
#define LV_USE_FREETYPE 0
#if LV_USE_FREETYPE
#define LV_FREETYPE_CACHE_SIZE (16 * 1024)
#if LV_FREETYPE_CACHE_SIZE >= 0
#define LV_FREETYPE_SBIT_CACHE 0
#define LV_FREETYPE_CACHE_FT_FACES 0
#define LV_FREETYPE_CACHE_FT_SIZES 0
#endif
#endif

#define LV_USE_RLOTTIE 0

#define LV_USE_FFMPEG 0
#if LV_USE_FFMPEG
#define LV_FFMPEG_AV_DUMP_FORMAT 0
#endif

#define LV_USE_SNAPSHOT 0

#define LV_USE_MONKEY 0

#define LV_USE_GRIDNAV 0

#define LV_USE_FRAGMENT 0

#define LV_USE_IMGFONT 0

#define LV_USE_MSG 0

#define LV_USE_IME_PINYIN 0
#if LV_USE_IME_PINYIN
#define LV_IME_PINYIN_USE_K9_MODE 1
#if LV_IME_PINYIN_USE_K9_MODE == 0
#define LV_IME_PINYIN_CAND_TEXT_NUM 6
#endif

#define LV_IME_PINYIN_USE_DEFAULT_DICT 1
#if LV_IME_PINYIN_USE_DEFAULT_DICT
#define LV_IME_PINYIN_DICT_FILE "lv_ime_pinyin_dict.h"
#endif
#endif

#define LV_USE_FILE_EXPLORER 0
#if LV_USE_FILE_EXPLORER
#define LV_FILE_EXPLORER_PATH_MAX_LEN (128)
#define LV_FILE_EXPLORER_QUICK_ACCESS 1
#endif

#endif /*LV_CONF_H*/

#endif /*End of "Content enable"*/