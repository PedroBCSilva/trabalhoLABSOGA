#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
pid_t firefox = 0;
pid_t textEditor = 0;
pid_t terminal = 0;

void load_firefox(char *url)
{
    firefox = fork();
    switch (firefox)
    {
    case -1:
        printf("Problema na execucao do firefox");
        exit(-1);
        break;
    case 0:
        execlp("firefox", "firefox -new-window", url, NULL);
        exit(0);
        break;
    }
}
void load_textEditor()
{
    textEditor = fork();
    switch (textEditor)
    {
    case -1:
        printf("Problema na execucao do do gedit");
        exit(-1);
        break;
    case 0:
        execlp("gedit", "gedit", NULL);
        exit(0);
        break;
    }
}
void load_terminal()
{
    terminal = fork();
    switch (terminal)
    {
    case -1:
        printf("Problema na execucao do do terminal");
        exit(-1);
        break;
    case 0:
        execlp("gnome-terminal", "gnome-terminal", NULL);
        exit(0);
        break;
    }
}
void atualizar_Estados()
{
    //pede a atualiza texto de estado dos processos
    printf("\nAtualizar estados\n");
}
void controlador_filhos(int signum)
{
    switch (signum)
    {
    case 2: //sigint
        atualizar_Estados();
        break;
    case 17: //sigchld
        atualizar_Estados();
        break;
    case 18: //sigchld
        atualizar_Estados();
        break;
    case 20: //sigchld
        atualizar_Estados();
        break;
    }
    //printf("%d \n", signum);
}
int main()
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));

    printf("Meu pid: %d \n", getpid());
    int continua = 1;
    int opcao = 0;

    sa.sa_handler = &controlador_filhos;
    if (sigaction(SIGINT, &sa, NULL) != 0)
    { //CONTROL C
        printf("Problema com tratador SIGINT");
        exit(-1);
    }
    if (sigaction(SIGCHLD, &sa, NULL) != 0)
    { //filho morre
        printf("Problema com tratador SIGCHLD");
        exit(-1);
    }
    while (continua == 1)
    {
        printf("<<<< Applications Menu >>>\n");
        printf("\t1) Web Browser (%d)\n", firefox);
        printf("\t2) Text Editor (%d)\n", textEditor);
        printf("\t3) Terminal (%d)\n", terminal);
        printf("\t4) Finalizar processo\n");
        printf("\t5) Quit\n");
        printf("Opcao:");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
        {
            opcao = 0;
            //usado para definir o scopo do case 1
            char url[128];
            printf("Digite a url desejada\n");
            scanf("%s", url);
            //chama o navegador com a url digitada
            load_firefox(url);
        break;
        }
        case 2:
            //chama o editor de texto
            opcao = 0;
            load_textEditor();
            break;
        case 3:
            //chama o terminal
            opcao = 0;
            load_terminal();
            break;
        case 4:
        {
            //termina algum dos processo acima baseado no seu pid
            //pid_t pid;
            opcao = 0;
            int op;
            printf("Digite o pid do processo a ser terminado:");
            scanf("%d", &op);
            if (op == 1)
            {
                kill(firefox, SIGKILL);
                printf("Processo com %d foi terminado\n", firefox);
                firefox = 0;
            }
            else if (op == 2)
            {
                kill(textEditor, SIGKILL);
                printf("Processo com %d foi terminado\n", textEditor);
                textEditor = 0;
            }
            else if (op == 3)
            {
                kill(terminal, SIGKILL);
                printf("Processo com %d foi terminado\n", terminal);
                terminal = 0;
            }
        break;
        }
        case 5:
            continua = 0;
            break;
            // default:
            // printf("Opcao invalida");
            // break;
        } //fim switch
    }     //fim while
} //fim main