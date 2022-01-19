#include "ArvoreVP.h"
#include "iostream"
#include "cstring" 

#include <time.h>
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
    raiz = exclui(raiz);    
}

NoVP * ArvoreVP::exclui(NoVP * p)
{
    if(p != sentinela){
        p->setEsq( exclui(p->getEsq() ));
        p->setDir( exclui(p->getDir() ));        
    }
    return sentinela;
}

bool ArvoreVP::busca(string chave)
{
    return buscaAux(raiz, chave); 
}

bool ArvoreVP::buscaAux(NoVP * p, string chave)
{
    numBusca++;
    if(p == sentinela)
    {
        return false;
    }
    numBusca++;
    if((p->getID().compare(chave)) == 0)
    {
        return true;
    }
    //esquerda
    numBusca++;
    if((p->getID().compare(chave)) <= -1)
    {
        return buscaAux(p->getEsq(), chave);
    }
    //direita
    numBusca++;
    if((p->getID().compare(chave)) >= +1 )
    {
        return buscaAux(p->getDir(), chave);
    }
    return false;
}

void ArvoreVP::insere(string chave, int pos)
{
    if(!busca(chave))
        raiz = insereAux(raiz, chave, pos);
}

NoVP * ArvoreVP::insereAux(NoVP * p, string chave, int pos)
{
    numInsercao++;
    if(p == sentinela){
        NoVP * noVp = new NoVP(chave, pos);
        noVp->setEsq(sentinela);
        noVp->setDir(sentinela);
        noVp->setPai(sentinela);
        return noVp;
    }
    else
    {       
        numInsercao++;
        if((p->getID().compare(chave)) <= -1)
        {
            p->setEsq(insereAux(p->getEsq(), chave, pos));
        }
        else
        {
            p->setDir(insereAux(p->getDir(), chave, pos));
        }
        p = balancearARV(p);
        return p;
    }
}

NoVP * ArvoreVP::balancearARV(NoVP * p){
    if(p->getCor() == VERMELHO)
    {
        NoVP *pai = p->getPai();
        if(pai == sentinela)
        {
            p->setCor(PRETO);
        }
        else 
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
                                rotacaoSimplesDir(p);
                                p->setCor(PRETO);
                            }
                            else
                                pai->setCor(PRETO);

                            rotacaoSimplesEsq(p);
                            avo->setCor(VERMELHO);
                        }
                    else{
                        if(pai->getDir() == p)
                        {
                            rotacaoSimplesEsq(p);
                            p->setCor(PRETO);
                        }
                        else
                            pai->setCor(PRETO);

                        rotacaoSimplesDir(p);
                        avo->setCor(VERMELHO);
                    }
                }
        }
    }
    return p;
}

void ArvoreVP::rotacaoSimplesEsq(NoVP * p)
{
    NoVP * pai = p->getPai();
    pai->setDir( p->getEsq() );
    p->setEsq(pai);
    NoVP * avo = pai->getPai();
    if(avo != sentinela)
    {
        if(pai == avo->getEsq())
            avo->setEsq(p);
        else
            avo->setDir(p);
    }
    p->setPai(avo);
    pai->setPai(p);
}


void ArvoreVP::rotacaoSimplesDir(NoVP * p)
{
    NoVP * pai = p->getPai();
    pai->setEsq( p->getDir() );
    p->setDir(pai);
    NoVP * avo = pai->getPai();
    if(avo != sentinela)
    {
        if(pai == avo->getEsq())
            avo->setEsq(p);

        else
            avo->setDir(p);
    }
    p->setPai(avo);
    pai->setPai(p);
}


