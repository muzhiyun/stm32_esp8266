西安石油大学大创项目--无人机管道自动巡检
==

基于telesky STM32F103C8T6网络版系统板开发 板载esp8266接口+NRF24L01接口   
基于stm32_esp8266_ov2640  

****

###### v0.1
使用USART2接口  
上电时stm32通过串口初始化esp8266，自动连接TCP server并进入透传模式  
使用UART2Test()函数返回接受到的命令  
完成日期：2018-11-24 02:27  

###### v0.1.1
修复stm32串口乱码  
增加GPIO操作，初始化完成后将PA1口置高电平点亮LED指示灯  
对8266回传的数据做判断 相应做出操作  
完成日期：2018-11-24 15:12  