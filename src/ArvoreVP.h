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
