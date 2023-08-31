## 适配轻量qmk功能到ch552, 基于ch55xduino

### 功能特性

* 单层键，键值包括普通键，多媒体键，系统键
* via改键
* via可录制宏，发送字符串
* 适配编码器

### 使用方法

1. 克隆本仓库
2. 参考RP17工程修改默认矩阵和键值：`keymap.h`、`keymap.c`，修改功能配置：`config.h`，修改`RP17_via.json`文件来适配via
3. 用Arduino IDE打开修改后的`***.ino`文件
4. 安装[CH55xduino](https://github.com/DeqingSun/ch55xduino)
5. 在`USB Settings` 中选择 `USER CODE w/ 266B ram`
6. 编译上传，需要安装驱动，安装方式参考[CH55xduino](https://github.com/DeqingSun/ch55xduino)，中文说明[CH55xduino](https://github.com/lbtrobit/ch55xduino)

