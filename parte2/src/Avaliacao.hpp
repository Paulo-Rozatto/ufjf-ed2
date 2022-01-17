#ifndef AVALIACAO_H
#include <fstream>

using namespace std;

class Avaliacao
{
private:
    char id[90];
    int voto;
    int versao[3];
    char data[20];
    char *analise;

public:
    //Construtor
    Avaliacao();
    //Construtor com passagem de parâmetros
    Avaliacao(fstream &file, int index);
    //Destrutor
    ~Avaliacao();

    //Função para leitura de determinada análise
    void criar(fstream &file, int index);
    //Impressão da análise no terminal
    void imprimeTerminal();
    //Impressão da análise via arquivo de texto gerado fora do código
    void imprimeArquivo(fstream &file);
};
#endif