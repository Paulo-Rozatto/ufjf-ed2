#include "Avaliacao.hpp"
#include <iostream>

using namespace std;

Avaliacao::Avaliacao()
{
    analise = nullptr;
}

Avaliacao::Avaliacao(fstream &bin, int indice)
{
    analise = nullptr;
    criar(bin, indice);
}

Avaliacao::~Avaliacao()
{
    if (analise != nullptr)
        delete[] analise;
}


void Avaliacao::criar(fstream &bin, int indice)
{
    //TAM é definido de acordo com o padrão do arquivo bin para poder separar cada avaliacao de forma simples
    //ele possui um vetor char de tamanho 89 para armazenar as informações do ID(padrão do arquivo csv), um inteiro para a posição;
    //3 inteiros para representar a versão da análise;
    //1 inteiro para representar a avaliação do aplicativo dada por aquele review;
    //19 char para representar a data(o formato é dd/mm/aaaa hh:minmin:segseg)
    // id (89 char) + review position (1 int) + upvote (1 int) + version (3 int) + date (19 char)
    int TAM = 89 * sizeof(char) + sizeof(int) + sizeof(int) + 3 * sizeof(int) + 19 * sizeof(char);
    int reviewPosition, size;

    indice *= TAM;

    bin.seekg(indice, bin.beg);

    bin.read(id, 89 * sizeof(char));
    bin.read(reinterpret_cast<char *>(&reviewPosition), sizeof(reviewPosition));
    bin.read(reinterpret_cast<char *>(&voto), sizeof(voto));
    bin.read(reinterpret_cast<char *>(versao), sizeof(versao));
    bin.read(data, 19 * sizeof(char));

    bin.seekg(reviewPosition, bin.beg);
    bin.read(reinterpret_cast<char *>(&size), sizeof(size));

    if (analise != nullptr)
        delete[] analise;

    analise = new char[size + 1];

    bin.read(analise, size * sizeof(char));
    //acrescenta o \0 no final de cada vetor para indicar o final.
    id[89] = data[19] = analise[size] = '\0';
}


void Avaliacao::imprimeTerminal()
{
    //Verifica se há realmente uma analise para ser impresso
    if(analise != nullptr)
    {
        cout << "ID: " << id << endl;
        cout << "Avaliacao: " << voto << endl;
        cout << "Versao:";
        for(int i = 0; i < 3; i++)
        {
            cout << versao[i] << " ";
        }
        cout << endl << "Data: " << data << endl;
        cout << "Analise: " << analise << endl;
        cout << "PS: avaliacoes com versao '0 0 0' sao avaliacoes que nao tiveram sua versao informada." << endl;
    }
    
}

void Avaliacao::imprimeArquivo(fstream &arquivo)
{
    //Verifica se há realmente uma analise para ser impresso
    if(analise != nullptr)
    {
        arquivo << "ID: " << id << endl;
        arquivo << "Avaliacao: " << voto << endl;
        arquivo << "Versao:";
        for(int i = 0; i < 3; i++)
        {
            arquivo << versao[i] << " ";
        }
        arquivo << endl << "Data: " << data << endl;
        arquivo << "Analise: " << analise << endl;
        arquivo << "PS: avaliacoes com versao '0 0 0' sao avaliacoes que nao tiveram sua versao informada." << endl;
    }
    
}