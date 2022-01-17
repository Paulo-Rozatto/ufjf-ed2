#include "ArvoreVP.h"
#include "iostream"
#include "cstring" 

#include <time.h>
using namespace std;


ArvoreVP::ArvoreVP()
{
    comparacaoInsercao = 0;
    comparacaoBusca = 0;
    sentinela = new NoVP("", -1);
    sentinela->setCor(PRETO);
    raiz = sentinela;
}

ArvoreVP::~ArvoreVP()
{
    raiz = libera(raiz);    
}

NoVP * ArvoreVP::libera(NoVP * p)
{
    if(p != sentinela){
        p->setEsq( libera(p->getEsq() ));
        p->setDir( libera(p->getDir() ));        
    }
    return sentinela;
}

bool ArvoreVP::busca(string chave)
{
    return buscaAux(raiz, chave); 
}

bool ArvoreVP::buscaAux(NoVP * p, string chave)
{
    comparacaoBusca++;
    if(p == sentinela)
    {
        return false;
    }
    comparacaoBusca++;
    if((p->getID().compare(chave)) == 0)
    {
        return true;
    }
    //esq
    comparacaoBusca++;
    if((p->getID().compare(chave)) <= -1)
    {
        return buscaAux(p->getEsq(), chave);
    }
    //dir
    comparacaoBusca++;
    if((p->getID().compare(chave)) >= +1 )
    {
        return buscaAux(p->getDir(), chave);
    }
}

void ArvoreVP::insere(string chave, int pos)
{
    balanceado = false;
    raiz = insereAux(raiz, chave, pos);
}

NoVP * ArvoreVP::insereAux(NoVP * p, string chave, int pos)
{
    comparacaoInsercao++;
    if(p == sentinela){
        NoVP * noVp = new NoVP(chave, pos);
        noVp->setEsq(sentinela);
        noVp->setDir(sentinela);
        noVp->setPai(sentinela);
        return noVp;
    }
    else
    {       
        comparacaoInsercao++;
        if((p->getID().compare(chave)) <= -1)
        {
            p->setEsq(insereAux(p->getEsq(), chave, pos));
        }
        else
        {
            p->setDir(insereAux(p->getDir(), chave, pos));
        }
        p = balancear(p);
        return p;
    }
}

void ArvoreVP::imprime()
{
    imprimeAux(raiz);
    cout << endl;
}

void ArvoreVP::imprimeAux(NoVP * p)
{
    if(p != sentinela)
    {
        imprimeAux(p->getEsq());
        p->imprime();
        imprimeAux(p->getDir());
    }
}

void ArvoreVP::imprimePorNivel()
{
    imprimePorNivelAux(raiz, 0);
}

void ArvoreVP::imprimePorNivelAux(NoVP * p, int k)
{
    if(p != sentinela)
    {
        imprimePorNivelAux(p->getDir(), k+1);
        for(int i = 0; i < k; i++)
        {
            cout << "\t";
        }
        p->imprime();
        imprimePorNivelAux(p->getEsq(), k+1);
    }
}

NoVP * ArvoreVP::balancear(NoVP * p){
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


