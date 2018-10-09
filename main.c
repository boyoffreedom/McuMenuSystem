#include <stdio.h>
#include <stdlib.h>
#include<windows.h>
#include "hmi.h"

int main()
{
    char key = 0;
    char sys_run = 1;
    HMI_INIT();
    printf("\n");
    while(sys_run)
    {
        HMI_KEY_EVENT(key);
        HMI_REFRESH();
        printf("\nkey_pressed: %d\n",key);
        key = 0;
        while(key == 0 && sys_run)
        {
            key = getchar();
            switch(key)
            {
                case 'w': key = HMI_KEY_UP;break;
                case 's': key = HMI_KEY_DOWN;break;
                case 'a': key = HMI_KEY_LEFT; break;
                case 'd': key = HMI_KEY_RIGHT; break;
                case 'o': key = HMI_KEY_OK;   break;
                case 'b': key = HMI_KEY_BACK; break;
                case 'q': sys_run = 0;break;
                default:  key = 0;break;
            }
        }
    }
    return 0;
}
