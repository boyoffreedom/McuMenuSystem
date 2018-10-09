MCU MENU SYSTEM
本repo为MCU开发了一套简易的菜单系统，使用者可通过菜单控制某功能开关，查看变量数值，查看参数波形等。
外设需求：液晶显示屏（1602，12864或更高端的液晶屏均可），6个按键（分别为四个方向按键、一个OK按键和一个BACK按键，可根据需求裁剪按键个数）。
使用说明：在头文件中可设置当前显示屏的尺寸（可显示的行与列数），在HMI_INIT函数中创建菜单列表，设置清屏与打印字符函数，设置菜单与菜单间，菜单与变量间的从属关系，并在主函数中间隔一定时间调用HMI_REFRESH()与HMI_KEY_EVENT()函数即可。

