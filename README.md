# TCP_Server类使用说明

**src/test.cpp**是使用示例.

## 成员函数

|函数名|功能|
|-|-|
|(构造函数)|创建监听socket, 绑定端口并开始监听端口|
|(析构函数)|关闭业务socket, 关闭监听socket|
|**void recvMsg( void )**|如果未建立业务socket则在accept()处阻塞, 直到建立业务socket. 接收ROV发来的24位数据并处理第4位 (舱1是否漏水, 存至 `isOneLeak`), 第7位 (舱2是否漏水, 存至 `isTwoLeak`), 第8, 9位 (深度信息, 存至 `depth`)数据.|
|**void sendMsg( int move )**|按传递的参数`move`对应的动作 (见下表) 发送指令给ROV|

## sendMsg()中move值与动作与可用宏定义对应表

|动作|move的值|宏定义名|
|-|-|-|
|全速前进|0|SEND_FORWARD|
|全速后退|1|SEND_BACKWARD|
|全速左|2|SEND_LEFT|
|全速右|3|SEND_RIGHT|
|全速左转|4|SEND_TURN_LEFT|
|全速右转|5|SEND_TURN_RIGHT|
|全速上升|6|SEND_UP|
|全速下降|7|SEND_DOWN|
|半速前进|8|SEND_HALF_FORWARD|
|半速后退|9|SEND_HALF_BACKWARD|
|半速左|10|SEND_HALF_LEFT0|
|半速右|2|SEND_HALF_RIGHT1|
|半速左转|3|SEND_HALF_TURN_LEFT|
|半速右转|4|SEND_HALF_TURN_RIGHT|
|半速上升|5|SEND_HALF_UP|
|半速下降|6|SEND_HALF_DOWN|
|无动作|7|SEND_SLEEP|
