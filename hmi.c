#include "hmi.h"
#include "cmd.h"

HMI_MAIN HMI_M;
int encoder_delta_car = 18550;
float power = 5;

/**************************************/
/*          HMIϵͳ��ʼ������         */
/**************************************/
//���ܣ���ʼ���˵��б������ָ��
void HMI_INIT(void)
{
    static HMI_MENU M_MAIN;                         //�˵�����
	static HMI_MENU M_MENU[3];              //��ҳ��˵�ָ�붨��
	static HMI_PARA para_encoder[1], para_power[1], para_control[3];//����ָ�붨��
	static float p = 852.21,i = 469.58,d = 655.32;

	HMI_M.PUTC = &CMD_PUTC;
	HMI_M.CLRAR = &CMD_CLEAR;

//�������
	//�������鿴��ʼ��
    para_encoder[0].Name = "ENCO";							//��ʾ�ı�����
    para_encoder[0].ValType = HMI_VAL_SINT;                   //��������
    para_encoder[0].Val.PSI = &(encoder_delta_car);           //�����ĵ�ַ
    para_encoder[0].WRS = HMI_WRS_RO;                         //�Ƿ��ͨ���ⲿ����������ֵ

    //��Դ����
	para_power[0].Name = "POWER";
	para_power[0].ValType = HMI_VAL_FLOAT;
	para_power[0].Val.PF = &(power);
	para_power[0].WRS = HMI_WRS_RO;

    //���Ʊ���
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

//�����ҳ
	//��ҳ�����
	//������
	M_MENU[0].Name = "ENCODER";                             //�Ӽ��˵���
	M_MENU[0].ChildCount = 1;                               //��������
	M_MENU[0].ChildPoint = 0;                               //���ݳ�ʼ��ָ��
	M_MENU[0].Type = HMI_TYPE_NUM;                          //��������
	M_MENU[0].ParentMenus = &M_MAIN;                        //�����˵�
	M_MENU[0].ChildrenMenus = 0;                            //�����Ӳ˵�
	M_MENU[0].ChildPara = para_encoder;                    //���������ݱ���
	M_MENU[0].KEY_CALLBACK = &HMI_KEY_CALLBACK;             //�˵��ڰ����ص�����

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

    //���˵�
    M_MAIN.Type = HMI_TYPE_MENU;
    M_MAIN.KEY_CALLBACK = &HMI_KEY_CALLBACK;
    M_MAIN.Name = "MENU";
    M_MAIN.ChildCount = 3;
    M_MAIN.ChildPoint = 0;
    M_MAIN.ChildrenMenus = M_MENU;

	HMI_M.Menu = &M_MAIN;
}

//HMI�����¼��ص�����
void HMI_KEY_EVENT(unsigned char key_code)
{
    HMI_MENU *pm = HMI_M.Menu;
    pm->KEY_CALLBACK(key_code);
}

/**************************************/
/*            Ĭ�ϰ�������            */
/**************************************/
//Ĭ�ϲ˵�����ֵ�����ص�����
void HMI_KEY_CALLBACK(unsigned char key_code)
{
	if(key_code == HMI_KEY_BACK){						//����BACK��
		if(HMI_M.Menu->ParentMenus != 0){		            //���ظ��˵�
			HMI_M.Menu = HMI_M.Menu->ParentMenus;
		}
	}
	if(key_code == HMI_KEY_OK){					//	����OK��
	    switch(HMI_M.Menu->Type){
        case HMI_TYPE_MENU:                                 //�˵���������Ӳ˵�
            if(HMI_M.Menu->ChildrenMenus+HMI_M.Menu->ChildPoint != 0){
                HMI_M.Menu = HMI_M.Menu->ChildrenMenus+HMI_M.Menu->ChildPoint;     //ָ����һ��˵�
            }
            break;
        case HMI_TYPE_NUM:                          //�ַ��ͱ�����������Ϊ�ɶ����޸ı���
            if(HMI_M.Menu->ChildPara[HMI_M.Menu->ChildPoint].WRS == HMI_WRS_RW){          //����Ϊ�ɸ���

            }
            break;
        case HMI_TYPE_WAVE:                          //�����
            break;
        default:
            break;
	    }
	}

	if(key_code == HMI_KEY_DOWN){			//����DOWN��
        switch(HMI_M.Menu->Type){
        case HMI_TYPE_MENU:                                 //�˵�ָ�������ƶ�
            if(HMI_M.Menu->ChildPoint < HMI_M.Menu->ChildCount-1){
                HMI_M.Menu->ChildPoint += 1;             	//ָ����һ�в˵�
            }
            else{
                HMI_M.Menu->ChildPoint = 0;
            }
            break;
        case HMI_TYPE_NUM:                                  //ָ����һ����
            if(HMI_M.Menu->ChildPoint < HMI_M.Menu->ChildCount-1){
                HMI_M.Menu->ChildPoint += 1;             	//ָ����һ�в˵�
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
	if(key_code == HMI_KEY_UP){			//����UP��
	    switch(HMI_M.Menu->Type){
        case HMI_TYPE_MENU:                                 //�˵�ָ�������ƶ�
            if(HMI_M.Menu-> ChildPoint != 0){
                HMI_M.Menu-> ChildPoint -= 1;             	//ָ����һ�в˵�
            }
            else{    //ѭ���б�
                HMI_M.Menu -> ChildPoint = HMI_M.Menu->ChildCount - 1;
            }
            break;
        case HMI_TYPE_NUM:                                  //ָ����һ����
            if(HMI_M.Menu->ChildPoint != 0){
                HMI_M.Menu->ChildPoint -= 1;             	//ָ����һ����
            }
            else{                                           //ѭ���б�
                HMI_M.Menu -> ChildPoint = HMI_M.Menu->ChildCount - 1;
            }
            break;
        case HMI_TYPE_WAVE:
            break;
        default:
            break;
	    }
	}
	else if(key_code & HMI_KEY_RIGHT){			//����RIGHT��
        switch(HMI_M.Menu->Type){
        case HMI_TYPE_MENU:                                 //�˵���������Ӳ˵�
            if(HMI_M.Menu->ChildrenMenus+HMI_M.Menu->ChildPoint != 0){
                HMI_M.Menu = HMI_M.Menu->ChildrenMenus + HMI_M.Menu->ChildPoint;     //ָ����һ��˵�
            }
            break;
        case HMI_TYPE_NUM:                          //�ַ��ͱ�����������Ϊ�ɶ����޸ı���
            if(HMI_M.Menu->ChildPara[HMI_M.Menu->ChildPoint].WRS == HMI_WRS_RW){          //����Ϊ�ɸ���

            }
            break;
        case HMI_TYPE_WAVE:                          //�����
            break;
        default:
            break;
	    }
	}
	else if(key_code & HMI_KEY_LEFT){			//����LEFT��
        if(HMI_M.Menu->ParentMenus != 0){		            //���ظ��˵�
			HMI_M.Menu = HMI_M.Menu->ParentMenus;
		}
	}
}

/**************************************/
/*            ��Ļˢ�º���            */
/**************************************/
//���ܣ�һ�����ڵ��ã�����HMI�豸����ˢ����ʾ
void HMI_REFRESH(void)
{
	unsigned char i;
	unsigned char lh;
	HMI_MENU* pm = HMI_M.Menu;

	HMI_M.CLRAR();

	HMI_M.x = 0;
	HMI_M.y = 0;
    lh = (pm->ChildCount < SCREEN_HEIGHT-1)?(pm->ChildCount):(SCREEN_HEIGHT-1);
	if(pm->ChildPoint < SCREEN_HEIGHT-1){        //ָ��ָ�����ݿ��ڽ�������ʾ
        HMI_PRINT_STR(pm->Name);                 //��ʾ�˵���
        HMI_M.y = 1;
        if(pm->ChildCount != 0){				//��ʾ���ݱ�ǩ
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
            case HMI_TYPE_NUM:                      //��ʾ���ݱ���
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
    else{                                           //ָ�볬������
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
            case HMI_TYPE_NUM:                      //��ʾ���ݱ���
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
/*            ������ʾ����            */
/**************************************/
//����p:��Ҫ��ʾ�ı�������
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
/*            �ַ���ӡ����            */
/**************************************/
//����s:��ӡ�ַ�ָ��
void HMI_PRINT_STR(char* s)
{
	while(*s != '\0'){
		HMI_M.PUTC(HMI_M.x++,HMI_M.y,*(s++));
	}
	HMI_M.PUTC(HMI_M.x++,HMI_M.y,' ');
}

/**************************************/
/*            �����ӡ����            */
/**************************************/
//num:��ӡ����������
void HMI_PRINT_NUM(int num)
{
    int i;
    char dat[6];
    if(num < 0){                                //��������ʾ
        HMI_M.PUTC(HMI_M.x++,HMI_M.y,'-');
        num = -num;
    }
    //��ʼ����ʾ�ַ�����
    for(i = 0; i < 6; i++) dat[i] = 0;
    i = 0;	//��ʼ������

    while(num / 10){								//��λ
        dat[i] = num % 10 + '0';    			//���λ
        num /= 10; i++;
    }
    dat[i] = num % 10 + '0';    		    	//���λ
    for( ; i >= 0; i--){
        HMI_M.PUTC(HMI_M.x++,HMI_M.y,dat[i]);
    }
}

/**************************************/
/*            �����ӡ����            */
/**************************************/
//f:��ӡ����������
//deci:��Ҫ��ʾ��������С����λ��
void HMI_PRINT_FLOAT(float f)
{
    unsigned char i;
    unsigned char deci = 5;             //��ʾС������λ��

    HMI_PRINT_NUM((int)f);

    if(deci > 0){
        HMI_M.PUTC(HMI_M.x++,HMI_M.y,'.');
    }
    for(i = 0; i < deci; i++){
        f *= 10;
        HMI_M.PUTC(HMI_M.x++, HMI_M.y,(unsigned int)f%10+48);
    }
}
