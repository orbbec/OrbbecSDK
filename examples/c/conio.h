#include <termio.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int getch(void) {
    struct termios tm, tm_old;
    int            fd = 0, ch;

    if(tcgetattr(fd, &tm) < 0) {  //保存现在的终端设置
        return -1;
    }

    tm_old = tm;
    cfmakeraw(&tm);                        //更改终端设置为原始模式，该模式下所有的输入数据以字节为单位被处理
    if(tcsetattr(fd, TCSANOW, &tm) < 0) {  //设置上更改之后的设置
        return -1;
    }

    ch = getchar();
    if(tcsetattr(fd, TCSANOW, &tm_old) < 0) {  //更改设置为最初的样子
        return -1;
    }

    return ch;
}

int kbhit(void) {
    struct termios oldt, newt;
    int            ch;
    int            oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}