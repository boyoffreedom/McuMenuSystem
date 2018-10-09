#ifndef __HMI_H
#define __HMI_H

#define HMI_VAL_UCHAR  0
#define HMI_VAL_SCHAR  1
#define HMI_VAL_UINT   2
#define HMI_VAL_SINT   3
#define HMI_VAL_FLOAT  4

#define HMI_TYPE_MENU 0
#define HMI_TYPE_NUM  1
#define HMI_TYPE_WAVE 2

#define HMI_KEY_BACK  0x01
#define HMI_KEY_OK    0x02
#define HMI_KEY_UP	  0x04
#define HMI_KEY_DOWN  0x10
#define HMI_KEY_LEFT  0x20
#define HMI_KEY_RIGHT 0x40

#define HMI_WRS_RO 0
#define HMI_WRS_RW 1


#define HMI_M hmi_1602
#define SCREEN_HEIGHT 2
#define SCREEN_WIDTH 16
//菜单显示数值
typedef struct{
	char* Name;										//the name
	unsigned char ValType;				//dtype: 0:unsigned char 1:char 2:unsigned int 3:int 4:float 5:double
	union {
		float* PF;
		int* PSI;
		unsigned int* PUI;
		char* PSC;
		unsigned char* PUC;
	}Val;
	unsigned char WRS;				//WRITE AND READ STATE: 0:READ ONLY 1:WRITE AND READ
}HMI_PARA;

//菜单
typedef struct HMI_Menu{
	short ChildCount;						//当前级菜单个数
	short ChildPoint;						//当前级菜单序号
	char* Name;
	HMI_PARA* ChildPara;   				//规范化显示的数值结构体数组
	unsigned char Type;					    //菜单类型
	void(*KEY_CALLBACK)(unsigned char);		//按键回调函数
	struct HMI_Menu *ChildrenMenus;		//子节点
	struct HMI_Menu *ParentMenus;			//父节点
}HMI_MENU;

//主屏幕参数
typedef struct HMI_Main{
	//菜单指针
	HMI_MENU* Menu;									//子菜单结构体指针

	//屏幕指针设置
	unsigned int x;
	unsigned int y;

    unsigned char FontSize;

	void (*CLRAR)();					//清除显示内容
	void (*PUTC)(unsigned int,unsigned int,char);	//打印字符函数指针:行 列 字符

}HMI_MAIN;


void HMI_INIT(void);

void HMI_KEY_CALLBACK(unsigned char key_code);

void HMI_REFRESH(void);					//刷新屏幕

void HMI_PRINT_STR(char* s);

void HMI_PRINT_NUM(int a);

void HMI_PRINT_FLOAT(float f);

void HMI_PRINT_PARA(HMI_PARA* p);

void HMI_KEY_EVENT(unsigned char key_code);

#endif



