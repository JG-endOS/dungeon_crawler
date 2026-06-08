#include <stdio.h>
#include <stdlib.h>

// COORDENADAS DO PLAYER
int playerX = 7;
int playerY = 4;

char playerLook = '^';

// VAR IMPORTANTES
int menu = 1;

int keys = 0;

int lifes = 3;

int map1_ = 1;

// PRIMERIRO MAPA
char village[10][10] = {
    "**********",
    "*        *",
    "*    #   L",
    "*    #   *",
    "*    #   *",
    "P        *",
    "*        *",
    "*   ^    *",
    "*        *",
    "**********"
};

// FUNÇÃO PARA LIMPAR TELA 
static void clear(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// LIMPAR A MEMÓRIA
static void clean_buffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// FUNÇÃO PARA LER ESCOLHAS (números inteiros)
static int read_int(int min, int max){
    int valor;
    int resultado;

    while (1)
    {
        resultado = scanf("%d", &valor);

        // USUÁRIO FECHOU A ENTRADA

        if (resultado == EOF)
        {
            printf("\nEntrada encerrada. Saindo do jogo...\n");
            exit(0);
        }

        // USUÁRIO DIGITOU ALGO QUE NÃO É NÚMERO

        if (resultado != 1)
        {
            printf("Entrada inválida! Digite um número entre %d e %d.\n", min, max);
            clean_buffer();
            continue;
        }

        // VERIFICA SE TEM LIXO DEPOIS DO NÚMERO

        int proximo = getchar();
        if (proximo == EOF)
        {
            printf("\nEntrada encerrada. Saindo do jogo...\n");
            exit(0);
        }
        if (proximo != '\n')
        {
            printf("Entrada inválida! Digite apenas o número entre %d e %d.\n", min, max);
            clean_buffer();
            continue;
        }

        // VERIFICA SE O NÚMERO ESTÁ DENTRO DO INTERVALO

        if (valor < min || valor > max)
        {
            printf("Opção fora do intervalo! Escolha entre %d e %d.\n", min, max);
            continue;
        }

        return valor;
    }
}


// FUNÇÃO PARA CAPTURAR O COMANDO DE UMA TECLA SEM PRECISAR USAR O ENTER
#ifdef _WIN32 // WINDOWS
    #include <conio.h>

    char getch() {
        return _getch();
    }
#else // LINUX/MACOS
    #include <termios.h>
    #include <unistd.h>
    
    char getch() {
        struct termios oldt, newt;
        char ch;
        
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        
        newt.c_lflag &= ~(ICANON | ECHO);
        
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        
        ch = getchar();
        
        if (ch == 27) {
            newt.c_cc[VMIN] = 0;
            newt.c_cc[VTIME] = 0;
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
            
            char lixo;
            while (read(STDIN_FILENO, &lixo, 1) > 0); 
        }
        
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif

// FUNÇÃO PARA IMPRIMIR O TÍTULO DO JOGO
static void title() {
    const char *title[] = {
"       ________                                               _________                     __                ",
"       \\______ \\  __ __  ____    ____   ____  ____   ____     \\_   ___ \\__________ __  _  _|  |   ___________ ",
"       |    |  \\|  |  \\/    \\  / ___\\_/ __ \\/  _ \\ /    \\    /    \\  \\/\\_  __ \\__  \\ \\/ \\/ /  | _/ __ \\_  __ \\ ",
"       |    `   \\  |  /   |  \\/ /_/  >  ___(  <_> )   |  \\   \\     \\____|  | \\// __ \\     /|  |_\\  ___/|  | \\/",
"      /_______  /____/|___|  /\\___  / \\___  >____/|___|  /    \\______  /|__|  (____  /\\/\\_/|____/\\___  >__|   ",
"              \\/           \\//_____/      \\/           \\/            \\/            \\/                 \\/       ",
"                                                 ===By JG and Pedro===",
"      1. Play                                           ",
"      2. Tutorial                                         ",
"      3. Exit                                             ",
    };

    for (int i = 0; i < 10; i++) {
        printf("%s\n", title[i]);
    }
}

// FUNÇÃO PARA IMPRMIR A DUNGEON 
static void dungeon() {
    const char *dungeon[] = {
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
"                |                     _| @@@@@@@@@@@@@@@@@@@@ |    )   | (       |____",
"               |       )             | @@@@@@@@@@@@@@@@@@@@@ |     )                  |    ",
"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ | @@@@@@@@@@@@@@@@@@@@@@ |^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
    };

    for (int i = 0; i < 16; i++) {
        printf("%s\n", dungeon[i]);
    }       
}  

// IMPRIME QUALQUER MAPA QUE EU DESENHAR
static void drawMap(int x, int y,char mapR[x][y]){
    clear();
    int i = 0;
    int j = 0;

    for(i = 0; i < x; i++){
        for(j = 0; j < y; j++){
            printf("%c ", mapR[i][j]);
        }
        printf("\n");
    }

}

// MOVIMENTO
static void movement(int x, int y, char mapR[x][y], char direction) {
    // Variáveis para calcular o próximo passo
    int nextX = playerX;
    int nextY = playerY;

    // Determina a nova posição 
    switch (direction) {
        case 'w': case 'W': nextX--; playerLook = '^'; break;// Cima
        case 's': case 'S': nextX++; playerLook = 'v'; break; // Baixo
        case 'a': case 'A': nextY--; playerLook = '<'; break; // Esquerda
        case 'd': case 'D': nextY++; playerLook = '>'; break; // Direita
        default: return;
    }

    // COLISÃO
    if (nextX >= 0 && nextY >= 0) {

        if (mapR[nextX][nextY] =='#' && mapR[nextX][nextY] =='X' && mapR[nextX][nextY] =='Y' && mapR[nextX][nextY] =='Z') {
            lifes--;
            if(lifes == 0){
                clear();
                printf("[GAME OVER]");
                menu = read_int(1, 1);
            }
        }

        if (mapR[nextX][nextY] != '*' && mapR[nextX][nextY] !='P' ) {
            
            mapR[playerX][playerY] = ' '; 
            
            playerX = nextX;
            playerY = nextY;
            
            mapR[playerX][playerY] = playerLook;
        }
    }
}

static void tutorial(){
    menu = 0;
    clear();

    printf("[ Story ]\n");
    printf("A retired warrior from a distant land tries to feel the glory of battle once more. \nIn this new adventure, he finds a dungeon that catches his attention.\nHis objective is to kill the final Boss.\n");
    printf("_______________________________________________________\n");
    printf("[ Symbols ]\n");
    printf("'^' ==> Player looking up\n");
    printf("'<' ==> Player looking left\n");
    printf("'>' ==> Player looking right\n");
    printf("'v' ==> Player looking down\n");
    printf("'*' ==> Wall (player can't go through)\n");
    printf("'#' ==> Spikes (drains HP when walked through)\n");
    printf("'O' ==> Button (executes an action when pressed)\n");
    printf("'D' ==> Closed door (player can't go through)\n");
    printf("'@' ==> Key (opens a closed door)\n");
    printf("'=' ==> Opened door (player can go through)\n");
    printf("'L' ==> Stairs (leads the player to the next phase)\n");
    printf("'X' ==> Monster type 1\n");
    printf("'Y' ==> Monster type 2\n");
    printf("'Z' ==> Final Boss\n");
    printf("_______________________________________________________\n");
    printf("[ Actions ]\n");
    printf("W ==> Moves up '^'\n");
    printf("A ==> Moves left '<'\n");
    printf("D ==> Moves right '>'\n");
    printf("S ==> Moves down 'v'\n");
    printf("i ==> Interaction\n");
    printf("o ==> Attack\n");
    printf("_______________________________________________________\n");
    printf("Press '1' to go back to the menu\n");
    printf("==> ");

    menu = read_int(1, 1);
}

static void map1() {
    menu = 0;
    while(map1_) {
        drawMap(10, 10, village);

        movement(10, 10, village, getch());
    }
    clean_buffer();
}

int main() {

    while(menu){
        clear();
        title();
        
        dungeon();
        
        printf("==> ");
        int op = read_int(1,3);

        if(op == 1){
            map1();
            return 0;
        } else if(op == 2) {
            tutorial();
        } else if (op ==3) {
            clear();
            printf("                                           [ THANKS FOR PLAYING ]\n"); 
            printf("                                       [ Developed by JG and Pedro ]\n"); 
            dungeon();
            return 0;
        }
    }
}
