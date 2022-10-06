#include <stdio.h>

int main()
{
    
    int changepin(int x)
    {
        if (x == 1)
        {
        const int a = 7;
        return a;
            
        }
        if (x == 2)
        {
        const int a = 9;
        return a;
        }
    }

int b = 0;
b = changepin(1);        
printf("%x",b);        
        
}