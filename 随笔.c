1.项目经理：要求对任何部分都有了解，并且有自己擅长的一个部分。
2.蓝牙芯片出厂设置，配合IC部实现对新的蓝牙的测试。
3.蓝牙数据传输部分，stack，ble，bt。（铜霭哥）
4.SDK维护部分。（敏贤哥）
5.APP的更新部分，网络升级。
6.新应用的开发，主要研究对比别人家的芯片，在我们这边能不能用起来。


HCI：HCI层为上层级提供了一个统一访问hci控制器的接口，其接口为一系列的指令和事件
HCI共有三种链路句柄：连接句柄  逻辑链路句柄 物理链路句柄 
HCI共四种类型的数据包：command（0x01） cal（0x02） sco（0x03） event（0x04）
HCI inquiry command事件发现周围设备(文档767页)
HCI Write Local Name Command设置控制蓝牙设备器的名称
HCI Read BD_ADDR Command读取蓝牙地址：返回0x00读取成功。返回0x01-0xff读取存在问题。
HCI协议数据分析：1、Host：（在core_V5.0中的位置：Vol 2,Part E,7.3.2 Reset Command）
   00000001 00000011 00001100 00000000
   Command：00000001（0x01）
   Opcode： 00000011 00001100（ogf：000011（0x03）ocf：0000000011（0x03））根据ogf和ocf查表可知为复位指令
   说明：这里为什么采用000011而不是00000011是因为在蓝牙数据5.4.1 HCI Command Packet章节下面表示ogf为占用6bit的数据

GATT:GATT 是一个在蓝牙连接之上的发送和接收很短的数据段的通用规范
          profile可以理解为一种规范，一个标准的通信协议，它存在于从机中。蓝牙组织规定了一些标准的profile，例如 HID OVER GATT ，防丢器 ，心率计等。每个profile中会包含多个service，每个service代表从机的一种能力。
          service可以理解为一个服务，在ble从机中，通过有多个服务，例如电量信息服务、系统信息服务等，每个service中又包含多个characteristic特征值。每个具体的characteristic特征值才是ble通信的主题。比如当前的电量是80%，所以会通过电量的characteristic特征值存在从机的profile里，这样主机就可以通过这个characteristic来读取80%这个数据
          characteristic特征值，ble主从机的通信均是通过characteristic来实现，可以理解为一个标签，通过这个标签可以获取或者写入想要的内容。
          UUID，统一识别码，我们刚才提到的service和characteristic，都需要一个唯一的uuid来标识*************************（这里就和之前的修改GATT联系起来了）*****************************
GAP：控制设备**连接**和**控制广播**。决定了你的设备是否被其他设备课件，并决定你的设备怎么与配对设备进行交互。


git 下载： 要配置的很多：https://blog.csdn.net/qq_35460159/article/details/82534957  
                 ①git clone +网址
                 
git push：①去下载下来的文件夹下输入： git add .②再输入 git commit -m "frist commit"
                             [master cee3722] frist commit
                            1 file changed, 23 insertions(+)
                            create mode 100644 test/WangZhongqiang/WangZhongqiang.md  得到返回值如下：这一步的作用是检测相对于你下载下来的时候，你是改变了什么。
                            ③git pull origan master  (pull一下)  ④git push
图片浏览：sudo apt-get install fim     执行fim 图片

 
d 自动寻找补全使用过的目录，提供选择。
hash ~d sdk="你指定的目录"   需要重新加载（source ~/zshrc）
F2 打开/关闭目录树，可通过移动打开需要修改的文件。或 Ctrl+f搜索文件打开。  注：hjkl 本窗口移动，切换窗口 Ctrl+hjkl。
nyy 复制光标以下多少行的代码。
p（P）粘贴到光标之后。
ctrl+w  再输入j向下移动窗口
ctrl+k   再输入k向上移动窗口
wendows  ctrl+w试过就知道

在app_dongle.c 中吧2.4g的模式ID给删除+dongle上电配置管理功能删除。
                           有使用到ble_enable。
                           需要配置USER_SUPPORT_PROFILE_SPP。
 	          把BT_SUPPORT_DISPLAY_BAT蓝牙电压显示删除掉。
 	          配置了TCFG_PC_ENABLE  tcfg pc使能
                           
完成vim的tutor练习。

任务：使用蓝牙连接一个设备之后，还能继续连续连接。①定位代码putchar('<')-board_ac636n_demo.c
②继续定位：power_set_callback(TCFG_LOWPOWER_LOWPOWER_SEL, sleep_enter_callback, sleep_exit_callback, board_set_soft_poweroff);设置
③针对修改代码：针对蓝牙广播使能进行测试set_adv_enable(0, 1)：在蓝牙连接成功之后加上set_adv_enable(0,1);此方案没有实现。
④换方案：发现在蓝牙connect状态set_adv_enable的返回数值为3 ，在未连接的状态下set_adv_enable的数值为0：即蓝牙工作模式影响：尝试设置为广播模式，但是未能实现 set_ble_work_state(BLE_ST_ADV, 0);
⑤问：应该设置为连接只有只广播，不连接的模式。
⑥查看蓝牙官方手册进一步了解广播（host-ll-ll-host）--2717（v5.2-vol6.partD）广播数据一般31个，不能超过，一般控制多的就存入rsp。
在ble_api.h中发现，开关广播必须先配置广播参数，考虑是广播参数没有配置好，需要先对广播做进一步的了解。暂且搁置。
⑦连接之后广播关闭！！进入别的case里面不广播》》》》ble_api.h
  普通广播参数包括：广播周期static u16 adv_interval_val;       //广播周期 (unit:0.625ms)160
                                 广播类型adv_type                                  //设置广播连接状态，广播，定向，扫描。。。。0
                                 广播通道adv_channel                            //7
⑧使能之后一直重启，查找使能问题发现按照初始化严格配置参数。所以需要查找出连接之后做了什么导致不兼容广播


7.针对广播数据提出了问题，这些数据都怎么解读：
02 01 06 03 03 12 18 03 19 C1 03       
11 09 4C 41 4F 57 41 4D 47 2D 48 49 44 28 42 4C 45 29 （LAOWAGNG-HID）
0D 09 4C 41 4F 57 41 4D 47          28 42 4C 45 29 （LAOWANG）    -----------①
11 09 41 41 4F 57 41 4D 47 2D 48 49 44 28 42 4C 45 29 （AAOWANG-HID）-----------②
定位	make_eir_packet_val--有使用到HCI_EIR_datatype_t
发现	//按(长度 + 类型 + 内容)这样的格,组合填入广播包数据static inline u8 make_eir_packet_val。发现数据是按照类型，内容，长度组成。

理解	02 01 06这一组数据包：02代表后面数据包size，01代表对应的HCI_EIR_datatype_t数据类型，比如这里01对应的就是HCI_EIR_DATATYPE_FLAGS类型。后面的06就是表示对应的数据。

	03 03 12 18这一组数据包：03代表后面数据包size，03代表对应的HCI_EIR_datatype_t数据类型，后面的12 18就是表示对应的数据：
   #define HID_UUID_16                                 0x1812（位于standard_hid.h里面，当然这里就是先发送低位再高位）
	03 19 C1 03这一组数据包：
   0x03c1（hid设备信息）：static u8 Appearance[] = {BLE_APPEARANCE_GENERIC_HID & 0xff, BLE_APPEARANCE_GENERIC_HID >> 8}; //默认为960,0x03c0。后面app_keyboard.c里面le_hogp_set_icon设置为BLE_APPEARANCE_HID_KEYBOARD==691模式即keyboard模式。所以为0x03c1.
测试后面18位	后面18位也是第一位是长度，第二位类型标志位：HCI_EIR_DATATYPE_COMPLETE_LOCAL_NAME=0x09
后面的跟的就是名字的数据+（BLE）。
发现了如果名字太长的的话会把数据放到rsp里面，如下图。

8.抓取广播包：
encrypted:no---是否加密
direction--------传输方向
CRC即循环冗余校验码（Cyclic Redundancy Check）：是数据通信领域中最常用的一种查错校验码，其特征是信息字段和校验字段的长度可以任意选定
在K位信息码后再拼接R位的校验码，整个编码长度为N位，因此，这种编码也叫（N，K）码。对于一个给定的（N，K）码，可以证明存在一个最高次幂为N-K=R的多项式G(x)。根据G(x)可以生成K位信息的校验码，而G(x)叫做这个CRC码的生成多项式。 校验码的具体生成过程为：假设发送信息用信息多项式C(X)表示，将C(x)左移R位，则可表示成C(x)*x的R次方，这样C(x)的右边就会空出R位，这就是校验码的位置。通过C(x)*X的R次方除以生成多项式G(x)得到的余数就是校验码
RSST（dBm）无线传输功率
通道，delta time等

广播在接入时有两种：广播接入：固定为0x8e89bed6  数据接入：随机值，两个设备间使用。
广播报头决定是广播接入还是数据接入：->BLE教程 pdf
BLE报文：nordic ble sniffer meta
BLE Link layer（链路层）PDU：报文报头：4bit报文类型+2bit保留位+1bit发送地址+1bit接收地址类型
         公共地址是由制造商从ieee申请独一无二的，OUI
UUID:一般来说，uuid是由128bit构成，但是，实在太长了，所以规定了uuid基数，为00000000-0000 1000-8000-008059b34fb0
         如果发送16位uuid为2a01的话：0000（2a01）-0000 1000-8000-008059b34fb0  （就是2a01加上基数）
         低功耗蓝牙使用的那部分 UUID 被分为下列几组：  
               0x1800 ~ 0x26FF：用作服务类通用唯一识别码。  
               0x2700 ~ 0x27FF：用于标识计量单位。  
               0x2800 ~ 0x28FF：用于区分属性类型。 
               0x2900 ~ 0x29FF：用作特性描述。  
               0x2A00 ~ 0x7FFF：用于区分特性类型。
/*广播包格式为：其实这不是因为乱序，是因为请求包（蓝牙发的），回应包（请求的那个设备发给蓝牙）这两个包是随机发的，就像你的手机搜索到蓝牙的话其实是蓝牙这边先只发蓝牙名字和一些地址给手机，
让你的手机好在搜索列表里面显示他的样式以及名字。所以本质的话，一般来说还是只有定向广播。*/
int 对广播已经建立了模型;

自己配制广播：
  先设置工作模式：set_ble_work_state(next_state, 0);（(next_state=BLE_ST_ADV或者BLE_ST_IDLE）
  初始化： advertisements_setup_init1();{
                 uint8_t adv_type = ADV_NONCONN_IND;    //设置为只广播，不连接
                 uint8_t adv_channel = ADV_CHANNEL_ALL;
    	int   ret = 0;

    	ret |= make_set_adv_data();                          //自己配置广播数据
    	ret |= make_set_rsp_data();

   	if (ret) {
      	puts("adv_setup_init fail !!!!!!\n");
     	ASSERT(0);
  	 }
  	ble_op_set_adv_param(160,adv_type, 7);     //设置广播参数}
   ble_op使能：ble_op_adv_enable(1);
8.cbk协议栈事件处理里面用到了小段数据读取：con_handle = little_endian_read_16(packet, 4);  
    1）Little-endian：将低序字节存储在起始地址（低位编址）
    2）Big-endian：将高序字节存储在起始地址（高位编址）
    在sdk中使用app_htonl来返回一个网络字节顺序的值，来源：music_info.cur_time = app_htonl(music_player_get_dec_cur_time());---智能音箱

9.为什么hid要对低功耗要求较高：像这类蓝牙hid设备，一般都是使用的是纽扣电池或者干电池，所以各大厂商的卖点就成为了，谁家的蓝牙能够用的更久，所以，这就是我们追求低功耗的原因了。
   在代码中，我们在本来ble的模式下使能关闭外设，以及软关机来实现进一步的低功耗。
  
10.蓝牙鼠标结构：

11.初识键盘standard_keyboard：
   ①  __LINE__, 行号
     __FUNCTION__ 函数
   ②edr蓝牙使能：
   void edr_set_enable(u8 en)
	{
    	printf("edr enable :%d\n", en);
   	 if (en) {
    	   user_hid_enable(1);                 //键盘使能
    	   btctrler_task_init_bredr();         //edr蓝牙任务初始化
      	   bt_wait_phone_connect_control(1);   //设置设备为可发现可连接的接口
      	  sys_auto_sniff_controle(1, NULL);   //⑤中七小点
    	}
	}
    ③hid_report_map通用桌面控制-----转到第12小点：
    ④发现无论是烧录br23（sdk说明文档里面使用br23）还有br25都不会定向广播：
        ？：是因为此键盘代码需要按下定义的按键之后才会广播。
        Way1.修改键盘对应的矩阵按键为板子上面的固定按键。
        Way2.直接把按键启动关闭。
        对Way1进行测试：
            1.√发现在矩阵按键matrix_keyboard.c中矩阵按键初始化中追加了一个定时器。
            2.        JL_PORTA->S |= porta_value;                //         设置输出方向                     |=输入设置 &=~输出设置。 整个A全部设置输入：JL_PORTA->DIR |=-1。全部输出=0。
                      JL_PORTA->DIE |= porta_value;                        //        设置数字还是模拟                   数字        模拟                    数字                          模拟
       	             JL_PORTA->PU &= ~porta_value;                        //         （只存在输入模式）设置上拉         开上拉      关上拉                  开上拉                        关上拉
      	             JL_PORTA->PD &= ~porta_value;                        //         （只存在输入模式）设置下拉         开下拉      关下拉                  开下拉                        关下拉
      	             JL_PORTA->IN&BIT(1)|=0;                              //            读取引脚电平                  |=0读取到高 ==0读取到低电平（JL_PORTA->IN&BIT(1)|=0  JL_PORTA->IN&BIT(1)==0）  
                      JL_PORTA->OUT|=BIT(1);                       //                   输出引脚电平           |=BIT(1)输出到高 &=~BIT(6)输出到低（ JL_PORTB->OUT&=~BIT(6); ）                  
                      JL_PORTA->HD |=BIT(1);  //                      只存在输出模式(电流)是否打开强驱           |=BIT(1)打开强驱 &=~BIT(6)关闭强驱（ JL_PORTB->HD&=~BIT(6); ）         
                      JL_PORTA->HDO|=BIT(1);//                 只存在输出模式          内阻设置                 |=BIT(1)去掉内阻 &=~BIT(6)打开内阻（JL_PORTB->HDO&=~BIT(6);） 
          3.通过对矩阵按键的理解，对应到初始化为row 和col ，尝试接线测试矩阵按键，发现触发了蓝牙广播并连接。
          4.在矩阵键盘结构中：event_handler(传入app和事件)->materix_key_map_deal()->special_key_deal()->usr_key_deal(读取按键的数值);
   ⑤在app_main（）函数中初始化结构
          1.设置时钟，随即获取时钟参数
          2.设置蓝牙pll参数
          3.设置蓝牙后缀名、设置蓝牙名称、增加后缀
          4.初始化蓝牙事件bt_handle_register
          5.edr中断注册
          6.蓝牙协议栈初始化
          7.系统自动sniff 使能，按键消息使能。
            ☆sniff作用：
              ☆△定义：在蓝牙传输之间的一种模式：（SNIFF reqquest（sniff 请求））。就是主从机作了约定：每5ms发送数据包这样的一个请求，并且这个时间内从机的监听次数会减少，主机也在
              指定的slot（位置）传输给从机。总而言之：Sniff Mode下Slave只在固定间隔中的某个Master 传输Slot监听{个人感觉这个对低功耗也有很多帮助}、在Bluetooth协议中，Slave不能主动发
              送数据给Master除非Master向它发送Poll，双方约定Sniff Time后，Master 每5ms发送一个Poll。则Slave就可以发送数据给主机了。这就做到了手柄每5ms发送一个数据包给主机
          8.设置无操作定时软关机时间


12.理解报告描述符中的那些数字字符所表达的含义（进一步理解）：
   ①针对于sel： Array（数组中的每个值，数据域(Report Count )中的每个值）可以在Usage Minimum（数值范围）和Usage Maximum之间取值，不是一一对应的关系
   Usage Minimum和Usage Maximum对于DF，则表示数据域(Report Count )中的每个个数与Usage Minimum和Usage Maximum中定义的用法一一对应，即一个Report Count对应一个用途(Usage).
   ②Main Item共有5个：Input、Output、Feature、Collection和End Collection，其中Input、Output和Feature会产生实际的数据域并附带说明操作特性。
      Global Item：可对其后的所有主项目产生的数据域作用，直到出现一个新的Global Item为止。
      Local Item：只对其后的一个主项目产生的数据域有效。
   ③report ID ：若要将同一个报告分为若干个部分，就可以采用report ID（不能设置report id为1）
   ④Lagical min和Lagical max就是指的是传输数据的最大最小值（数据范围：定义何种范围多少bit）
   ⑤report size和report count一般配套使用。：
          Report Size 用来设定主项目(Input，Output，Feature)的报告字段大小。
          √Report Count 用来设定主项目之报告字段的数目，其等于操作的数目---------------貌似可以表示多少个状态
          大概可以理解为size*count）举例说明：鼠标的三个按键，每个按键占用一个一位的字段，则Report Size (1), Report Count (3)﹔那么这个报告长度为三个位，可以同时呈现出三个按键的状态
   ⑥collection（app）就是和collection end配对使用的。两个必须同时存在。
   √⑦：发现若有逻辑min max的话基本就可以确定确定好report size的数值。比如逻辑min为0 max为255 则report size为 8bit
   ⑧问题：在每一组数据中：有哪些是继承的？？？？？
    1.在代码app_standard_keyboard.c中：最开始有定义了logical min为0 logical max为1 。本段输入结束后。一直继承这两个数值直到到了普通键盘定义的时候才取消。
    因为普通键盘的取值需要0到101以上的取值。这就是继承
    2.又比如在键盘特殊输入完成以后，填充了一个数据域，因为本身没有定义logical 和usage 所以就会完全继承上一部分的
    ⑨鼠标分辨率：计算公式 r = ((lM-lm)/(PM-Pm))X10i*Unit（这里的i是10的次方）
       （lm = Logical Minimum，lM = Logical Maximum， pm = Physical Minimum，pM = Physical Maximum，i = Unit Exponent）
    ⑩使用范围（usage max）和规定范围（logical max）之间的关系：如果使用范围远远大于规定范围？？
       Lagical min和Lagical max数据范围：定义何种范围多少bit   ||   Usage Minimum（数值范围）
13.利用小米手机的自动抓包工具，进行手机直接连接蓝牙进行抓包：拨号输入*#*#5959#*#*开启，随后再关闭。

14.对蓝牙包进行分析：
 ①广播包：
  ❥使用ellisys软件。
  ❥把之前学习广播包的知识起来
    经过文件对比再次学习。
    △对边发现：主要是前面的前导+地址+报头+数据+校验的格式，以及对广播包+请求包+回应包。
    在hid-keyboard中做了详细分析。

15.要一个抓好的包来玩一下。
  ①先把蓝牙整体结构进行回顾：
     
16.ibeacon：配备有低功耗蓝牙（BLE）通信功能的设备使用BLE技术向周围发送自己特有的ID，接收到该ID的应用软件会根据该ID采取一些行动。


√追加定时器和执行中断。
sys_timer_add(0, led_timer_handle, led_timer_ms);
√追加adc以及读取。
√设置io口方向：参考matrix_keyboard.c中设置io口方向。
√在安卓端修改gatt之后记得删除gatt cache，不然需要在app上更新服务