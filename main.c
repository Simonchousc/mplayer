
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lrc.h"
#include "start_mplayer.h"
#include "console.h"
#include <unistd.h>
#include "background.h"
#include <time.h>
int main(int argc, char const *argv[])
{
    SONG *head = NULL;
    char *lrc = read_lrc_file("简单爱.lrc"); // 读歌词

    char *lrc_buf[128] = {lrc};

    strtok_lrc(lrc, lrc_buf); // 逐行切割

    system("clear");           // 清屏
    print_background(100, 40); // 绘制背景框

    set_fg_color(34); // 设置前景颜色

    char **titles = analysis_header(lrc_buf); // 分析标题
    if (titles)
    {
        for (int i = 0; titles[i]; i++)
        {
            cusor_moveto(47, 10 + i);
            printf("%s", titles[i]); // 在固定位置逐行输出标题
        }
        free_title(titles); // 释放内存
    }
    head = analysis_lrc(lrc_buf, head); // 逐行分析歌词并插入链表

    mplayer_play("简单爱.mp3");

    int timestamp = 0;    // 时间戳
    int time_total = 271; // 总时间

    int visible_lines = 4; // 歌词显示行数
    SONG *current = head;  // 从头节点开始遍历链表
    SONG *temp;
    int current_line = 0; // 当前显示歌词的起始行

    while (timestamp <= time_total)
    // while (1)
    {
        cusor_moveto(43, 30);
        printf("%02d:%02d // %02d:%02d", timestamp / 60, timestamp % 60, time_total / 60, time_total % 60); // 显示时间
        cusor_moveto(49, 32);
        printf("%d%%", (int)(timestamp / (float)time_total * 100)); // 进度条

        cusor_hide(); // 隐藏光标
        fflush(stdout);
        // 歌词滚动播放
        temp = head;
        while (temp != NULL && temp->time <= timestamp)
        {
            current = temp;
            temp = temp->next;
        }

        // 计算当前显示的四行歌词
        SONG *start_line = current;
        for (int i = 0; i < current_line && start_line != NULL; i++)
        {
            start_line = start_line->next;
        }

        srand(time(NULL));

        for (int i = 0; i < visible_lines && start_line != NULL; i++)
        {
            int random_num = rand() % 6;
            cusor_moveto(40, 15 + i);
            set_fg_color(32 + random_num);
            printf("\033[K");                // 清空当前行
            printf("%s\n", start_line->lrc); // 打印歌词
            cusor_moveto(100, 15 + i);
            // set_fg_color(37);
            printf("#"); // 补上空缺
            fflush(stdout);
            start_line = start_line->next;
        }

        // 判断是否需要滚动歌词
        if (current->next != NULL && timestamp >= current->next->time)
        {
            current_line++;
            if (current_line >= visible_lines || current->next->next == NULL)
                current_line = 0;
        }

        sleep(1);
        timestamp++;
        cusor_get_pos();
        // 显示光标
    }
    cusor_set_pos();
    set_fg_color(37); // 改回前景色
    return 0;
}
