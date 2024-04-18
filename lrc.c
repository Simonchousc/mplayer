#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lrc.h"
#include "console.h"

// 歌词读取
char *read_lrc_file(char *file_name)
{
    FILE *fp = fopen(file_name, "r");
    if (NULL == fp)
    {
        perror("fopen");
        return 0;
    }
    // 测量文件长度
    fseek(fp, 0, 2);
    long file_len = ftell(fp);
    rewind(fp);
    char *text = (char *)calloc(1, file_len + 1); // 分配合适的空间
    fread(text, file_len, 1, fp);
    fclose(fp);

    return text;
}
// 歌词切割
void strtok_lrc(char *lrc, char *lrc_buf[])
{
    int i = 0;
    while ((lrc_buf[i] = strtok(lrc_buf[i], "\r\n")) && ++i)
        ;
    return;
}

char **analysis_header(char *lrc_buf[])
{
    int i = 0;
    int title_line = 0;
    // 获取题目数量
    while (lrc_buf[i] != NULL)
    {
        if (lrc_buf[i][1] >= 'a' && lrc_buf[i][1] <= 'z')
        {
            title_line++;
        }
        i++;
    }

    // 分配指针数组来存放每个标题行的指针
    char **title = (char **)calloc(title_line + 1, sizeof(char *)); // +1 for NULL
    if (NULL == title)
    {
        perror("calloc");
        return NULL;
    }

    // 遍历每一行，提取标题并存放
    for (i = 0; i < title_line; i++)
    {
        // 假设每行标题最大长度为122（留一个位置给'\0'）
        char tle[123];
        sscanf(lrc_buf[i], "%*4c %[^\n]", tle); // 使用格式字符串来避免缓冲区溢出

        // 查找并移除 ']' 字符
        char *addr = strchr(tle, ']');
        if (addr)
        {
            *addr = '\0';
        }

        // 为每个标题行分配内存，并拷贝内容
        title[i] = strdup(tle); // strdup分配内存并拷贝字符串
        if (NULL == title[i])
        {
            perror("strdup");
            // 释放之前分配的内存
            for (int j = 0; j < i; j++)
            {
                free(title[j]);
            }
            free(title);
            return NULL;
        }
    }

    // 确保数组以NULL结尾，作为字符串数组的约定
    title[title_line] = NULL;

    return title;
}

// title数组时释放内存
void free_title(char **title)
{
    if (title)
    {
        char **p = title;
        while (*p)
        {
            free(*p++);
        }
        free(title);
    }
}
// 歌词分析插入
SONG *analysis_lrc(char *lrc_buf[], SONG *head)
{
    // printf("测试:analysis_lrc\n");
    /**************************************************************/
    char *song_p = NULL;
    char *time_p = NULL;
    int i = 0;
    int title_line = 0;
    SONG tmp;
    while (lrc_buf[i] != NULL)
    {
        // printf("测试!进入循环\n");
        if (lrc_buf[i][1] >= 'a' && lrc_buf[i][1] <= 'z')
            title_line++;
        i++;
    }
    int j = title_line;
    while (lrc_buf[j] && lrc_buf[j][1] >= '0' && lrc_buf[j][1] <= '9')
    {

        song_p = lrc_buf[j];
        time_p = lrc_buf[j];
        // printf("测试whilej=%d\n", j);
        while (*song_p == '[')
        {
            song_p += 10;
        }
        while (*time_p == '[')
        {
            int min = 0, sec = 0;
            sscanf(time_p, "[%d:%d]", &min, &sec);

            tmp.time = min * 60 + sec;
            strcpy(tmp.lrc, song_p);

            head = insert_link(head, tmp);
            time_p += 10;
        }

        j++;
    }
    return head;
}

// 链表插入
SONG *insert_link(SONG *head, SONG tmp)
{
    SONG *pi = (SONG *)calloc(1, sizeof(SONG));
    if (NULL == pi)
    {
        perror("calloc");
        return head; // 如果内存分配失败，返回原链表头
    }

    *pi = tmp;       // 复制tmp到pi
    pi->next = NULL; // 新节点next初始化为NULL

    // 如果链表为空，直接返回新节点
    if (NULL == head)
    {
        return pi;
    }

    SONG *pf = head; // pf用于追踪pi应该插入位置的前一个节点
    SONG *pb = head; // pb用于遍历链表寻找插入位置

    // 寻找插入位置
    while (pb != NULL && pb->time < pi->time)
    {
        pf = pb;
        pb = pb->next;
    }

    // 插入新节点
    if (pb == head)
    {
        // 如果pi的时间比头节点还小，将pi插入到头部
        pi->next = head;
        head = pi;
    }
    else
    {
        // 否则，将pi插入到pf之后
        pf->next = pi;
        if (pb != NULL)
        {
            pi->next = pb;
        }
    }

    return head; // 返回更新后的链表头
}
// 链表遍历
void print_link(SONG *head)
{
    // 1、判断链表是否存在
    if (NULL == head)
    {
        printf("link not exist\n");
        return;
    }
    else
    {
        // 不能直接动head 所以定义一个指针变量 代替head移动
        SONG *pb = head;
        while (pb != NULL)
        {
            printf("%d %s \n", pb->time, pb->lrc);
            // pb移动到下一个节点
            pb = pb->next;
        }
    }
}
