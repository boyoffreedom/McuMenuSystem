#include <stdio.h>
#include<stdlib.h>

void CMD_CLEAR(void)
{
    system("cls");
}

void CMD_PUTC(unsigned char x, unsigned char y, char c)
{
    static unsigned char x_,y_ = 0;

    if(y != y_) {printf("\n%c",c); y_ = y;}

    else printf("%c",c);
}
