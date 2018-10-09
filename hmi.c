#include "hmi.h"
#include "cmd.h"

HMI_MAIN HMI_M;
int encoder_delta_car = 18550;
float power = 5;

/**************************************/
/*          HMI系统初始化函数         */
/**************************************/
//功能：初始化菜单列表与变量指向
void HMI_INIT(void)
{
    static HMI_MENU M_MAIN;                         //菜单定义
	static HMI_MENU M_MENU[3];              //主页面菜单指针定义
	static HMI_PARA para_encoder[1], para_power[1], para_control[3];//变量指针定义
	static float p = 852.21,i = 469.58,d = 655.32;

	HMI_M.PUTC = &CMD_PUTC;
	HMI_M.CLRAR = &CMD_CLEAR;

//定义变量
	//编码器查看初始化
    para_encoder[0].Name = "ENCO";							//显示的变量名
    para_encoder[0].ValType = HMI_VAL_SINT;                   //变量类型
    para_encoder[0].Val.PSI = &(encoder_delta_car);           //变量的地址
    para_encoder[0].WRS = HMI_WRS_RO;                         //是否可通过外部按键更改数值

    //电源变量
	para_power[0].Name = "POWER";
	para_power[0].ValType = HMI_VAL_FLOAT;
	para_power[0].Val.PF = &(power);
	para_power[0].WRS = HMI_WRS_RO;

    //控制变量
	para_control[0].Name = "KP";
	para_control[0].ValType = HMI_VAL_FLOAT;
	para_control[0].Val.PF = &p;
	para_control[0].WRS = HMI_WRS_RW;

	para_control[1].Name = "KI";
	para_control[1].ValType = HMI_VAL_FLOAT;
	para_control[1].Val.PF = &i;
	para_control[1].WRS = HMI_WRS_RW;

	para_control[2].Name = "KD";
	para_control[2].ValType = HMI_VAL_FLOAT;
	para_control[2].Val.PF = &d;
	para_control[2].WRS = HMI_WRS_RW;

//定义分页
	//主页面设计
	//编码器
	M_MENU[0].Name = "ENCODER";                             //子级菜单名
	M_MENU[0].ChildCount = 1;                               //内容数量
	M_MENU[0].ChildPoint = 0;                               //内容初始化指针
	M_MENU[0].Type = HMI_TYPE_NUM;                          //内容类型
	M_MENU[0].ParentMenus = &M_MAIN;                        //从属菜单
	M_MENU[0].ChildrenMenus = 0;                            //包含子菜单
	M_MENU[0].ChildPara = para_encoder;                    //包含子内容变量
	M_MENU[0].KEY_CALLBACK = &HMI_KEY_CALLBACK;             //菜单内按键回调函数

	M_MENU[1].Name = "CONTROL";
	M_MENU[1].ChildCount = 3;
	M_MENU[1].ChildPoint = 0;
	M_MENU[1].Type = HMI_TYPE_NUM;
	M_MENU[1].ParentMenus = &M_MAIN;
	M_MENU[1].ChildrenMenus = 0;
	M_MENU[1].ChildPara = &para_control;
	M_MENU[1].KEY_CALLBACK = &HMI_KEY_CALLBACK;

	M_MENU[2].Name = "POWER";
	M_MENU[2].ChildCount = 1;
	M_MENU[2].ChildPoint = 0;
	M_MENU[2].Type = HMI_TYPE_NUM;
	M_MENU[2].ParentMenus = &M_MAIN;
	M_MENU[2].ChildrenMenus = 0;
	M_MENU[2].ChildPara = &para_power;
	M_MENU[2].KEY_CALLBACK = &HMI_KEY_CALLBACK;

    //主菜单
    M_MAIN.Type = HMI_TYPE_MENU;
    M_MAIN.KEY_CALLBACK = &HMI_KEY_CALLBACK;
    M_MAIN.Name = "MENU";
    M_MAIN.ChildCount = 3;
    M_MAIN.ChildPoint = 0;
    M_MAIN.ChildrenMenus = M_MENU;

	HMI_M.Menu = &M_MAIN;
}

//HMI按键事件回调函数
void HMI_KEY_EVENT(unsigned char key_code)
{
    HMI_MENU *pm = HMI_M.Menu;
    pm->KEY_CALLBACK(key_code);
}

/**************************************/
/*            默认按键函数            */
/**************************************/
//默认菜单与数值操作回调函数
void HMI_KEY_CALLBACK(unsigned char key_code)
{
	if(key_code == HMI_KEY_BACK){						//按下BACK键
		if(HMI_M.Menu->ParentMenus != 0){		            //返回父菜单
			HMI_M.Menu = HMI_M.Menu->ParentMenus;
		}
	}
	if(key_code == HMI_KEY_OK){					//	按下OK键
	    switch(HMI_M.Menu->Type){
        case HMI_TYPE_MENU:                                 //菜单级则进入子菜单
            if(HMI_M.Menu->ChildrenMenus+HMI_M.Menu->ChildPoint != 0){
                HMI_M.Menu = HMI_M.Menu->ChildrenMenus+HMI_M.Menu->ChildPoint;     //指向下一层菜单
            }
            break;
        case HMI_TYPE_NUM:                          //字符型变量，且属性为可读则修改变量
            if(HMI_M.Menu->ChildPara[HMI_M.Menu->ChildPoint].WRS == HMI_WRS_RW){          //变量为可更改

            }
            break;
        case HMI_TYPE_WAVE:                          //点变量
            break;
        default:
            break;
	    }
	}

	if(key_code == HMI_KEY_DOWN){			//按下DOWN键
        switch(HMI_M.Menu->Type){
        case HMI_TYPE_MENU:                                 //菜单指针向上移动
            if(HMI_M.Menu->ChildPoint < HMI_M.Menu->ChildCount-1){
                HMI_M.Menu->ChildPoint += 1;             	//指向上一列菜单
            }
            else{
                HMI_M.Menu->ChildPoint = 0;
            }
            break;
        case HMI_TYPE_NUM:                                  //指向上一变量
            if(HMI_M.Menu->ChildPoint < HMI_M.Menu->ChildCount-1){
                HMI_M.Menu->ChildPoint += 1;             	//指向上一列菜单
            }
            else{
                HMI_M.Menu->ChildPoint = 0;
            }
            break;
        case HMI_TYPE_WAVE:
            break;
        default:
            break;
	    }
	}
	if(key_code == HMI_KEY_UP){			//按下UP键
	    switch(HMI_M.Menu->Type){
        case HMI_TYPE_MENU:                                 //菜单指针向下移动
            if(HMI_M.Menu-> ChildPoint != 0){
                HMI_M.Menu-> ChildPoint -= 1;             	//指向下一列菜单
            }
            else{    //循环列表
                HMI_M.Menu -> ChildPoint = HMI_M.Menu->ChildCount - 1;
            }
            break;
        case HMI_TYPE_NUM:                                  //指向下一变量
            if(HMI_M.Menu->ChildPoint != 0){
                HMI_M.Menu->ChildPoint -= 1;             	//指向下一变量
            }
            else{                                           //循环列表
                HMI_M.Menu -> ChildPoint = HMI_M.Menu->ChildCount - 1;
            }
            break;
        case HMI_TYPE_WAVE:
            break;
        default:
            break;
	    }
	}
	else if(key_code & HMI_KEY_RIGHT){			//按下RIGHT键
        switch(HMI_M.Menu->Type){
        case HMI_TYPE_MENU:                                 //菜单级则进入子菜单
            if(HMI_M.Menu->ChildrenMenus+HMI_M.Menu->ChildPoint != 0){
                HMI_M.Menu = HMI_M.Menu->ChildrenMenus + HMI_M.Menu->ChildPoint;     //指向下一层菜单
            }
            break;
        case HMI_TYPE_NUM:                          //字符型变量，且属性为可读则修改变量
            if(HMI_M.Menu->ChildPara[HMI_M.Menu->ChildPoint].WRS == HMI_WRS_RW){          //变量为可更改

            }
            break;
        case HMI_TYPE_WAVE:                          //点变量
            break;
        default:
            break;
	    }
	}
	else if(key_code & HMI_KEY_LEFT){			//按下LEFT键
        if(HMI_M.Menu->ParentMenus != 0){		            //返回父菜单
			HMI_M.Menu = HMI_M.Menu->ParentMenus;
		}
	}
}

/**************************************/
/*            屏幕刷新函数            */
/**************************************/
//功能：一定周期调用，根据HMI设备参数刷新显示
void HMI_REFRESH(void)
{
	unsigned char i;
	unsigned char lh;
	HMI_MENU* pm = HMI_M.Menu;

	HMI_M.CLRAR();

	HMI_M.x = 0;
	HMI_M.y = 0;
    lh = (pm->ChildCount < SCREEN_HEIGHT-1)?(pm->ChildCount):(SCREEN_HEIGHT-1);
	if(pm->ChildPoint < SCREEN_HEIGHT-1){        //指针指向内容可在界面内显示
        HMI_PRINT_STR(pm->Name);                 //显示菜单名
        HMI_M.y = 1;
        if(pm->ChildCount != 0){				//显示内容标签
            switch(pm->Type){
            case HMI_TYPE_MENU:
                for(i = 0; i < lh; i++){
                    HMI_M.x = 0;
                    HMI_PRINT_STR((pm->ChildrenMenus+i)->Name);
                    if(i == pm->ChildPoint){
                        HMI_M.x = SCREEN_WIDTH-2;
                        HMI_PRINT_STR("<=");
                    }
                    HMI_M.y += 1;
                }
                break;
            case HMI_TYPE_NUM:                      //显示内容变量
                for(i = 0; i < lh; i++){
                    HMI_M.x = 0;
                    HMI_PRINT_PARA(pm->ChildPara+i);
                    if(i == pm->ChildPoint){
                        HMI_M.x = SCREEN_WIDTH-2;
                        HMI_PRINT_STR("<=");
                    }
                    HMI_M.y += 1;
                }
                break;
            case HMI_TYPE_WAVE:
                break;
            }
        }
    }
    else{                                           //指针超出行数
        if(pm->ChildCount != 0){
            switch(pm->Type){
            case HMI_TYPE_MENU:
                for(i = pm->ChildPoint - SCREEN_HEIGHT+1; i <= pm->ChildPoint; i++){
                    HMI_M.x = 0;
                    HMI_PRINT_STR((pm->ChildrenMenus+i)->Name);
                    if(i == pm->ChildPoint){
                        HMI_M.x = SCREEN_WIDTH-2;
                        HMI_PRINT_STR("<=");
                    }
                    HMI_M.y += 1;
                }
                break;
            case HMI_TYPE_NUM:                      //显示内容变量
                for(i = pm->ChildPoint-SCREEN_HEIGHT+1; i <= pm->ChildPoint; i++){
                    HMI_M.x = 0;
                    HMI_PRINT_PARA(pm->ChildPara+i);
                    if(i == pm->ChildPoint){
                        HMI_M.x = SCREEN_WIDTH-2;
                        HMI_PRINT_STR("<=");
                    }
                    HMI_M.y += 1;
                }
                break;
            case HMI_TYPE_WAVE:
                break;
            }
        }
    }
}

/**************************************/
/*            变量显示函数            */
/**************************************/
//输入p:需要显示的变量输入
void HMI_PRINT_PARA(HMI_PARA* p)
{
	HMI_PRINT_STR(p->Name);
	switch(p->ValType)
	{
		case HMI_VAL_SINT:
			HMI_PRINT_NUM(*(p->Val.PSI));
			break;
		case HMI_VAL_UINT:
			HMI_PRINT_NUM(*(p->Val.PUI));
			break;
		case HMI_VAL_FLOAT:
			HMI_PRINT_FLOAT(*(p->Val.PF));
			break;
		case HMI_VAL_SCHAR:
			HMI_PRINT_NUM(*(p->Val.PSC));
			break;
		case HMI_VAL_UCHAR:
			HMI_PRINT_NUM(*(p->Val.PUC));
			break;
	}
}

/**************************************/
/*            字符打印函数            */
/**************************************/
//输入s:打印字符指针
void HMI_PRINT_STR(char* s)
{
	while(*s != '\0'){
		HMI_M.PUTC(HMI_M.x++,HMI_M.y,*(s++));
	}
	HMI_M.PUTC(HMI_M.x++,HMI_M.y,' ');
}

/**************************************/
/*            定点打印函数            */
/**************************************/
//num:打印定点数输入
void HMI_PRINT_NUM(int num)
{
    int i;
    char dat[6];
    if(num < 0){                                //正负号显示
        HMI_M.PUTC(HMI_M.x++,HMI_M.y,'-');
        num = -num;
    }
    //初始化显示字符数组
    for(i = 0; i < 6; i++) dat[i] = 0;
    i = 0;	//初始化数据

    while(num / 10){								//拆位
        dat[i] = num % 10 + '0';    			//最低位
        num /= 10; i++;
    }
    dat[i] = num % 10 + '0';    		    	//最低位
    for( ; i >= 0; i--){
        HMI_M.PUTC(HMI_M.x++,HMI_M.y,dat[i]);
    }
}

/**************************************/
/*            浮点打印函数            */
/**************************************/
//f:打印浮点数输入
//deci:需要显示浮点数的小数点位数
void HMI_PRINT_FLOAT(float f)
{
    unsigned char i;
    unsigned char deci = 5;             //显示小数点后的位置

    HMI_PRINT_NUM((int)f);

    if(deci > 0){
        HMI_M.PUTC(HMI_M.x++,HMI_M.y,'.');
    }
    for(i = 0; i < deci; i++){
        f *= 10;
        HMI_M.PUTC(HMI_M.x++, HMI_M.y,(unsigned int)f%10+48);
    }
}
