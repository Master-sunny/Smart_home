# Smart_home
多传感器采集数据并显示
使用STM32作为中控制芯片，使用ADC采样烟雾传感器的的电压值，并根据传感器自身的公式计算出当前烟雾的浓度，使用DHT11传感器读取当前房间里面的温度和湿度
使用OLED显示屏显示当前的的数值，使用LED灯和有源蜂鸣器作为警报装置，当温度或者烟雾浓度达到阈值即可触动警报，以避免房间夜间起火来不及反应。

使用的工具时Keil5,烧录工具是ST_Link，具体对应的管脚对应可根据不同模块的的头文件，由于没有使用特殊的计时器，所以兼容F10X全系列。
