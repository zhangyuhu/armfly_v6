## v0.0.0.1-build20181023

#### RL-TCPnet的裸机工程模板的实现

*	使能了DHCP，可以自动获取IP地址。
*	创建了一个TCP Server，而且使能了局域网域名NetBIOS，用户只需在电脑端ping armfly就可以获得板子的IP地址，端口号1001。
*	可以在电脑端用网络调试软件创建TCP Client连接此服务器端。
*	按键K1按下，发送8字节的数据给TCP Client。
*	按键K2按下，发送1024字节的数据给TCP Client。
*	按键K3按下，发送5MB字节的数据给TCP Client。

####模块版本
*	ST固件库1.6.1版本
*	BSP驱动包V1.2
*	RL-TCPnet版本V4.74

