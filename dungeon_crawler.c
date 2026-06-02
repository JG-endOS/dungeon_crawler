#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static void clear(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

char getch() {
    char ch;
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); 
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); 
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
    return ch;
}

void title() {
    const char *title[] = {
"       ________                                               _________                     __                ",
"       \\______ \\  __ __  ____    ____   ____  ____   ____     \\_   ___ \\__________ __  _  _|  | _____________ ",
"       |    |  \\|  |  \\/    \\  / ___\\_/ __ \\/  _ \\ /    \\    /    \\  \\/\\_  __ \\__  \\ \\/ \\/ /  | _/ __ \\_  __ \\ ",
"       |    `   \\  |  /   |  \\/ /_/  >  ___(  <_> )   |  \\   \\     \\____|  | \\// __ \\     /|  |_\\  ___/|  | \\/",
"      /_______  /____/|___|  /\\___  / \\___  >____/|___|  /    \\______  /|__|  (____  /\\/\\_/|____/\\___  >__|   ",
"              \\/           \\//_____/      \\/           \\/            \\/            \\/                 \\/       ",
"                                                 ===By JG and Pedro==="
    };

    int linhas = sizeof(title) / sizeof(title[0]);

    for (int i = 0; i < linhas; i++) {
        printf("%s\n", title[i]);
    }
}

void castle() {
    const char *castle[] = {
"                                                   ",
"                                                   |>>>  ",
"                                                   |  ",
"                                                   |  ",
"                                     ____^___^_____|___^___^_",
"                                 ___|   )                    |____ ",
"                               _|          |  )           |       |__",
"                             _|               )       ) )            |_",
"                           _|       |          ________                ( )|_^__^",
"                        ^__|                 _| @@@@@@ |       |                |",
"                      |     ( )     | (   _| @@@@@@@@@@ |_            ( |      |",
"                   ___|                  | @@@@@@@@@@@@@@ |_     |      (       |",
"                  |          | (        | @@@@@@@@@@@@@@@@@ |_                   |",
"                |                     +| @@@@@@@@@@@@@@@@@@@@ |    )   | (       |____",
"               |       )             | @@@@@@@@@@@@@@@@@@@@@ |     )                  |    Press \"Enter\" to start",
"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ | @@@@@@@@@@@@@@@@@@@@@@ |^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
    };

    int linhas = sizeof(castle) / sizeof(castle[0]);

    for (int i = 0; i < linhas; i++) {
        printf("%s\n", castle[i]);
    }       
}

int main() {
    int menu = 1;
    char op;

    while(menu!=0){
        clear();
        title();
        castle();
        do{
            op = getch();
        } while (op != '\n');

        return 0;
    }
}
