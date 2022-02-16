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
        NoVP * remove(NoVP * p);

        bool busca(string reviewID);
        bool buscaAux(NoVP * p, string reviewID);

        void insere(string reviewID, int pos);
        NoVP * insereAux(NoVP * p, string reviewID, int pos);

        NoVP * balancearARV(NoVP * p);

        void rotacionaEsquerda(NoVP * p);
        void rotacionaDireita(NoVP * p);

        int getnumInsercao() {return this->numInsercao;};
        int getnumBusca() {return this->numBusca;};
        
};
#endif
