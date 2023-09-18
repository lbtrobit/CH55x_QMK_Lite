#pragma once

#define VENDOR_ID       0x5262    
#define PRODUCT_ID      0x5217                  //VENDOR_ID和PRODUCT_ID组成键盘的唯一编号，和json文件设成一样，VIA设置软件靠这个编号识别
#define MANUFACTURER    'R','o','b','i','t'     //键盘厂商
#define PRODUCT         'R','P','1','7'         //键盘名称

#define MATRIX_ROWS     5       //键盘矩阵行数
#define MATRIX_COLS     4       //键盘矩阵列数

#define MATRIX_ROW_PINS { 32, 14, 34, 33, 11 }  //键盘矩阵行引脚
#define MATRIX_COL_PINS { 30, 31, 17, 16 }      //键盘矩阵列引脚

#define DIODE_DIRECTION ROW2COL /* COL2ROW or ROW2COL - 矩阵二极管方向，行到列或列到行 */

// #define DIRECT_PINS { { 32, 14, 34, 33 }, { 30, 31, 17, 16 } }   //按键为引脚直连到地，从左到右为为行道列定义引脚

/* 总可用rom:14336bytes，以上基础功能rom:9011 bytes，剩余rom:5325 bytes，以下功能根据需求和rom开启*/

#define TWO_LAYER_ENABLE        //开启双层按键切换，多于25键时要关掉，dataflash不够，rom: 222 bytes

#define TAP_DANCE_ENABLE        //开启复用键功能，一个按键可设置单击、双击、长按三种功能，rom: 716 bytes
#define TAP_DANCE_COUNT  2      //复用键个数，最多5个，每个占用dataflash 6 bytes
#define TAPPING_TERM     200    //复用键判断时间，单位ms

#define EXTRAKEY_ENABLE         //开启可设置为多媒体和系统按键，rom: 619 bytes

#define MOUSE_ENABLE            //开启可设置为鼠标键，rom:886 bytes
#define FCODE_TO_MOUSE_ENABLE   //开启发宏时F13-F23键转鼠标按键，补充VIA不能设置发鼠标按键宏，能拓展更多应用。键值对应关系如表：FCodeToMousemap

// #define DIAL_ENABLE             //开启surface dial按键功能，rom:404 bytes

#define RGB_MATRIX_ENABLE       //开启RGB灯效，开启后默认开启via设置固定RGB和signalrgb两种模式，rom:1158 bytes
#define WS2812_DI_PIN        15 //RGB灯数据引脚
#define RGB_MATRIX_LED_COUNT 17 //RGB灯数量
#define RGB_EFFECTS_PLUS        //开启6种色彩循环本地灯效，rom: 1206 bytes
#define RGB_KEY_CONTROL         //开启按键控制6种色彩循环灯效，rom: 404 bytes

// #define ENCODER_ENABLE          //开启编码器，rom:1316 bytes
#define NUM_ENCODERS    1       //编码器个数
#define ENCODERS_PAD_A  { 12 }  //编码器A引脚
#define ENCODERS_PAD_B  { 13 }  //编码器B引脚






