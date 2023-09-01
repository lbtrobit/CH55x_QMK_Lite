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

#define EXTRAKEY_ENABLE         //开启多媒体和系统按键

#define ENCODER_ENABLE          //开启编码器
#define NUM_ENCODERS    1       //编码器个数
#define ENCODERS_PAD_A  { 12 }  //编码器A引脚
#define ENCODERS_PAD_B  { 13 }  //编码器B引脚





