# Ctrl-FOC-Lite评估套件

基于[SimpleFOC](https://github.com/simplefoc/Arduino-SimpleFOCShield)的一个修改版项目，官方版本基本都是基于Arduino相关板卡开发（也有基于STM32的，但是都是使用Arduino上层库浪费了很多硬件性能，比如硬件编码器接口、DMA、CAN等），所以本仓库准备在STM32的HAL固件库中移植SimpleFOC固件，同时加入FreeRTOS的支持。

![](/4.Docs/2.Images/img1.jpg)

![](/4.Docs/2.Images/img3.jpg)

硬件设计已经完成并且打样验证过了，使用的是STM32F1系列，后续可以基于本方案轻松扩展到F4等其他系列（因为HAL库很方便移植），并且扩展到多通道（只要硬件定时器够用），扩展到大功率方案（DRV8302+大功率MOS）。本评估板中引出了所有IO资源，相关资源分配可以参考`.ioc`文件：

![](/4.Docs/2.Images/img2.jpg)

固件使用CLion开发编译，不熟悉的同学可以参考这篇文章：

[配置CLion用于STM32开发](https://www.zhihu.com/people/zhi-hui-64-54/posts)