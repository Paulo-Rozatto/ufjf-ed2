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
        int posReview;
        bool cor;
        NoVP *dir, *esq, *pai;

    public:
        NoVP(string ID, int posReview){
                this->ID = ID;
                this->posReview = posReview;
        };
        ~NoVP();

        int getPosicaoRegistro () {return this->posReview;};
        string getID() { return ID; };
        NoVP * getEsq() { return esq; };
        NoVP * getDir() { return dir; };
        NoVP * getPai() { return pai; };
        bool getCor() { return cor; };

        void setposReview(int posReview) { this->posReview = posReview; };
        void setID(string id) { ID = id; };
        void setEsq(NoVP * esquerdo) { esq = esquerdo; };
        void setDir(NoVP * direito) { dir = direito; };
        void setPai(NoVP * p) { pai = p; };
        void setCor(bool color) { cor = color; };
};

class ArvoreVP
{
    private:
        
        int numInsercao;
        int numBusca;
        NoVP * sentinela;
        NoVP * raiz;

    public:
        ArvoreVP();
        ~ArvoreVP();
        NoVP * exclui(NoVP * p);

        bool busca(string chave);
        bool buscaAux(NoVP * p, string chave);

        void insere(string chave, int pos);
        NoVP * insereAux(NoVP * p, string chave, int pos);

        NoVP * balancearARV(NoVP * p);

        void rotacaoSimplesEsq(NoVP * p);
        void rotacaoSimplesDir(NoVP * p);

        int getnumInsercao() {return this->numInsercao;};

        int getnumBusca() {return this->numBusca;};
        
};
#endif // ARVOREVP_H
