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
//�˵���ʾ��ֵ
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

//�˵�
typedef struct HMI_Menu{
	short ChildCount;						//��ǰ���˵�����
	short ChildPoint;						//��ǰ���˵����
	char* Name;
	HMI_PARA* ChildPara;   				//�淶����ʾ����ֵ�ṹ������
	unsigned char Type;					    //�˵�����
	void(*KEY_CALLBACK)(unsigned char);		//�����ص�����
	struct HMI_Menu *ChildrenMenus;		//�ӽڵ�
	struct HMI_Menu *ParentMenus;			//���ڵ�
}HMI_MENU;

//����Ļ����
typedef struct HMI_Main{
	//�˵�ָ��
	HMI_MENU* Menu;									//�Ӳ˵��ṹ��ָ��

	//��Ļָ������
	unsigned int x;
	unsigned int y;

    unsigned char FontSize;

	void (*CLRAR)();					//�����ʾ����
	void (*PUTC)(unsigned int,unsigned int,char);	//��ӡ�ַ�����ָ��:�� �� �ַ�

}HMI_MAIN;


void HMI_INIT(void);

void HMI_KEY_CALLBACK(unsigned char key_code);

void HMI_REFRESH(void);					//ˢ����Ļ

void HMI_PRINT_STR(char* s);

void HMI_PRINT_NUM(int a);

void HMI_PRINT_FLOAT(float f);

void HMI_PRINT_PARA(HMI_PARA* p);

void HMI_KEY_EVENT(unsigned char key_code);

#endif



