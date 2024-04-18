#ifndef _LRC_H_
#define _LRC_H_
typedef struct song
{
    int time;
    char lrc[128];

    struct song *next;

} SONG;
extern char *read_lrc_file(char *file_name);        // 读取歌词
extern void strtok_lrc(char *lrc, char *lrc_buf[]); // 歌词行分割

extern char **analysis_header(char *lrc_buf[]); // 标题分析

extern void free_title(char **title);                   // 释放标题
extern SONG *analysis_lrc(char *lrc_buf[], SONG *head); // 歌词分析插入

extern SONG *insert_link(SONG *head, SONG tmp); // 有序插入链表

extern void print_link(SONG *head); // 遍历链表

#endif
