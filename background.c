#include <stdio.h>

void print_background(int w, int h)
{
    int width = w;  // 方形的宽度
    int height = h; // 方形的高度

    // 绘制方形的顶部
    for (int i = 0; i < width; i++)
    {
        printf("#");
    }
    printf("\n");

    // 绘制方形的中间部分（如果有的话）
    for (int i = 0; i < height - 2; i++)
    {
        printf("#");
        for (int j = 1; j < width - 1; j++)
        {
            printf(" "); // 使用空格填充方形内部
        }
        printf("#\n");
    }

    // 绘制方形的底部
    for (int i = 0; i < width; i++)
    {
        
        printf("#");
    }
    printf("\n");
}