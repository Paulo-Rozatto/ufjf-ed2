#include "ArvoreVP.h"
#include "iostream"
using namespace std;


ArvoreVP::ArvoreVP()
{
    numInsercao = 0;
    numBusca = 0;
    sentinela = new NoVP("", -1);
    sentinela->setCor(PRETO);
    raiz = sentinela;
}

ArvoreVP::~ArvoreVP()
{
    raiz = remove(raiz);    
}

NoVP * ArvoreVP::remove(NoVP * p)
{
    if(p != sentinela){
        p->setEsq( remove(p->getEsq() ));
        p->setDir( remove(p->getDir() ));        
    }
    return sentinela;
}

bool ArvoreVP::busca(string reviewID)
{
    return buscaAux(raiz, reviewID); 
}

bool ArvoreVP::buscaAux(NoVP * p, string reviewID)
{
    numBusca++;
    string aux = p->getID();
    int op = aux.compare(reviewID);
    if(p == sentinela)
    {
        return false;
    }
    numBusca++;
    if(op == 0)
    {
        return true;
    }
    //insere esquerda
    numBusca++;
    if(op < 0)
    {
        return buscaAux(p->getEsq(), reviewID);
    }
    //insere direita
    numBusca++;
    if(op > 0)
    {
        return buscaAux(p->getDir(), reviewID);
    }
    return false;
}

void ArvoreVP::insere(string reviewID, int pos)
{
    if(!busca(reviewID))
        raiz = insereAux(raiz, reviewID, pos);
}

NoVP * ArvoreVP::insereAux(NoVP * p, string reviewID, int pos)
{
    numInsercao++;
    if(p == sentinela){
        NoVP * noVp = new NoVP(reviewID, pos);
        noVp->setEsq(sentinela);
        noVp->setDir(sentinela);
        noVp->setPai(sentinela);
        return noVp;
    }
    else
    {       
        numInsercao++;
        if((p->getID().compare(reviewID)) <= -1)
        {
            p->setEsq(insereAux(p->getEsq(), reviewID, pos));
        }
        else
        {
            p->setDir(insereAux(p->getDir(), reviewID, pos));
        }
        p = balancearARV(p);
        return p;
    }
}

NoVP * ArvoreVP::balancearARV(NoVP * p) {
    if(p->getCor() == VERMELHO)
    {
        NoVP *pai = p->getPai();
        if(pai == sentinela)
        {
            p->setCor(PRETO);
        }
        else
        { 
            if(pai->getCor() == VERMELHO)
            {
                NoVP * avo = pai->getPai();
                if(avo == sentinela)
                {
                    pai->setCor(PRETO);
                }
                else
                {
                    NoVP * tio = (avo->getDir() == pai) ? avo->getEsq() : avo->getDir();
                    if(tio->getCor() == VERMELHO)
                    {
                        pai->setCor(PRETO);
                        tio->setCor(PRETO);
                        avo->setCor(VERMELHO);
                    }

                    else 
                        if(avo->getDir() == pai)
                        {
                            if(pai->getEsq() == p)
                            {
                                rotacionaDireita(p);
                                p->setCor(PRETO);
                            }
                            else
                                pai->setCor(PRETO);

                            rotacionaEsquerda(p);
                            avo->setCor(VERMELHO);
                        }
                    else{
                        if(pai->getDir() == p)
                        {
                            rotacionaEsquerda(p);
                            p->setCor(PRETO);
                        }
                        else
                            pai->setCor(PRETO);

                        rotacionaDireita(p);
                        avo->setCor(VERMELHO);
                    }
                }
            }
        }
    }
    return p;
}

void ArvoreVP::rotacionaEsquerda(NoVP * p)
{
    NoVP * pai = p->getPai();
    pai->setDir( p->getEsq() );
    p->setEsq(pai);
    NoVP * avo = pai->getPai();
    if(avo != sentinela)
    {
        if(pai == avo->getEsq())
        {
            avo->setEsq(p);
        }
        else
        {
            avo->setDir(p);
        }
    }
    p->setPai(avo);
    pai->setPai(p);
}


void ArvoreVP::rotacionaDireita(NoVP * p)
{
    NoVP * pai = p->getPai();
    pai->setEsq( p->getDir() );
    p->setDir(pai);
    NoVP * avo = pai->getPai();
    if(avo != sentinela)
    {
        if(pai == avo->getEsq())
        {
            avo->setEsq(p);
        }
        else
        {
            avo->setDir(p);
        }
    }
    p->setPai(avo);
    pai->setPai(p);
}


