/*
 基础 C 程序示例
 文件: main.c
 说明:
  - 支持命令行用法: `main sum 1 2 3` 会输出数字之和
  - 支持没有参数时进入交互模式
  - 演示了函数拆分、错误检查与简单注释
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void print_help(const char* prog_name)
{
    printf("用法:\n");
    printf("  %s sum <n1> <n2> ...   计算整数之和\n", prog_name);
    printf("  %s echo <text>         输出传入的文本\n", prog_name);
    printf("  %s                     进入交互模式\n", prog_name);
}

static long parse_long(const char* s, int* ok)
{
    errno = 0;
    char* end;
    long val = strtol(s, &end, 10);
    if (errno != 0 || *end != '\0') {
        *ok = 0;
        return 0;
    }
    *ok = 1;
    return val;
}

static void cmd_sum(int argc, char* argv[])
{
    if (argc <= 2) {
        printf("错误: 请至少提供一个整数进行求和。\n");
        return;
    }
    long sum = 0;
    for (int i = 2; i < argc; ++i) {
        int ok;
        long v = parse_long(argv[i], &ok);
        if (!ok) {
            printf("无法解析整数: '%s'\n", argv[i]);
            return;
        }
        sum += v;
    }
    printf("和: %ld\n", sum);
}

static void cmd_echo(int argc, char* argv[])
{
    if (argc <= 2) {
        printf("\n");
        return;
    }
    for (int i = 2; i < argc; ++i) {
        if (i > 2) putchar(' ');
        fputs(argv[i], stdout);
    }
    putchar('\n');
}

int main(int argc, char* argv[])
{
    if (argc >= 2) {
        if (strcmp(argv[1], "sum") == 0) {
            cmd_sum(argc, argv);
            return 0;
        }
        else if (strcmp(argv[1], "echo") == 0) {
            cmd_echo(argc, argv);
            return 0;
        }
        else if ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0)) {
            print_help(argv[0]);
            return 0;
        }
        else {
            printf("未知命令: %s\n\n", argv[1]);
            print_help(argv[0]);
            return 1;
        }
    }

    /* 交互模式 */
    printf("基础 C 程序示例 — 交互模式\n");
    printf("输入 'help' 查看用法, 'quit' 退出。\n");

    char buf[256];
    while (1) {
        printf("> ");
        if (!fgets(buf, sizeof(buf), stdin)) break;
        /* 去掉末尾换行 */
        buf[strcspn(buf, "\r\n")] = '\0';
        if (strcmp(buf, "quit") == 0) break;
        if (strcmp(buf, "help") == 0) {
            print_help(argv[0]);
            continue;
        }
        /* 简单解析: 支持 "sum 1 2" 和 "echo text" */
        char* tokens[32];
        int tcount = 0;
        char* context = NULL;
        char* p = strtok_s(buf, " \t", &context);
        while (p && tcount < 32) {
            tokens[tcount++] = p;
            p = strtok_s(NULL, " \t", &context);
        }
        if (tcount == 0) continue;
        if (strcmp(tokens[0], "sum") == 0) {
            /* 构造模拟 argv */
            char* fake_argv[32];
            fake_argv[0] = argv[0];
            for (int i = 0; i < tcount; ++i) fake_argv[i + 1] = tokens[i];
            cmd_sum(tcount + 1, fake_argv);
        }
        else if (strcmp(tokens[0], "echo") == 0) {
            char* fake_argv[32];
            fake_argv[0] = argv[0];
            for (int i = 0; i < tcount; ++i) fake_argv[i + 1] = tokens[i];
            cmd_echo(tcount + 1, fake_argv);
        }
        else {
            printf("未知交互命令: %s\n", tokens[0]);
        }
    }

    printf("退出。\n");
    return 0;
}