#ifndef ARVOREVP_H
#define ARVOREVP_H
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

const bool VERMELHO = true;
const bool PRETO = false;

class NoVP
{
    private:
        string ID;
        int posRegistro;
        bool cor;
        NoVP *dir, *esq, *pai;

    public:
        ///REVISAR
        NoVP(string ID, int posRegistro){
                this->ID = ID;
                this->posRegistro = posRegistro;

        };
        ~NoVP();

        int getPosicaoRegistro () {return this->posRegistro;};
        string getID() {return ID;};
        NoVP * getEsq() {return esq;};
        NoVP * getDir() {return dir;};
        NoVP * getPai() {return pai;};
        bool getCor() {return cor;};

        void setPosRegistro(int posRegistro) {this->posRegistro = posRegistro;};
        void setID(string id) { ID = id; };
        void setEsq(NoVP * esquerdo) { esq = esquerdo; };
        void setDir(NoVP * direito) { dir = direito; };
        void setPai(NoVP * p) { pai = p; };
        void setCor(bool color) { cor = color; };

        void imprime(){cout << endl << "Registro n : " << this->posRegistro << " : " << ID;};
};

class ArvoreVP
{
    private:
        
        int comparacaoInsercao;
        int comparacaoBusca;
        NoVP * sentinela;
        NoVP * raiz;
        bool balanceado;

    public:
        ArvoreVP();
        ~ArvoreVP();
        NoVP * libera(NoVP * p);

        bool busca(string chave);
        bool buscaAux(NoVP * p, string chave);

        void insere(string chave, int pos);
        NoVP * insereAux(NoVP * p, string chave, int pos);

        void imprime();
        void imprimeAux(NoVP * p);

        void imprimePorNivel();
        void imprimePorNivelAux(NoVP * p, int k);

        NoVP * balancear(NoVP * p);

        void rotacaoSimplesEsq(NoVP * p);
        void rotacaoSimplesDir(NoVP * p);

        int getComparacaoInsercao() {return this->comparacaoInsercao;};
        

        int getComparacaoBusca() {return this->comparacaoBusca;};
        
};
#endif // ARVOREVP_H
