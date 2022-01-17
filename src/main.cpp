#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <time.h>
#include <fstream>
#include <string.h>

#include "FileProcessing.hpp"
#include "Avaliacao.hpp"
#include "Sort.hpp"
#include "ArvoreVP.h"

using namespace std;

void importacao(Register **registers, int n)
{
    fstream bin;
    int *indices;
    bin.open("tiktok_app_reviews.bin", ios::in | ios::binary);

    srand(time(NULL));

    cout << "Escolhendo " << n << " registros aleatoriamente..." << endl;
    int j;
    for (int i = 0; i < n; i++)
    {
        j = rand() % ROWS;
        // j = i;
        registers[i]->init(bin, j);
    }
}

Register **createArray(int n)
{
    Register **v;

    v = new Register *[n];
    for (int i = 0; i < n; i++)
        v[i] = new Register();

    return v;
}

void copyUpvotes(Register **src, Register **dst, int n)
{
    for (int i = 0; i < n; i++)
    {
        dst[i]->setUpvote(src[i]->getUpvote());
    }
}

void deleteArray(Register **v, int n)
{
    for (int i = 0; i < n; i++)
        delete v[i];

    delete[] v;
}

void ordenacao()
{
    ifstream input("input.dat");
    ofstream output("saida.txt");

    if (!input.good())
    {
        cout << "Arquivo input.dat nao encontrado." << endl;
        input.close();
        return;
    }

    if (!output.good())
    {
        cout << "Arquivo saida.txt nao pode ser criado" << endl;
        output.close();
        return;
    }

    Register **v1, **v2, **v3;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    int n, m = 3, contMoves = 0, contComps = 0;
    double t1, t2, t3;
    int cm1, cm2, cm3, cc1, cc2, cc3;

    while (input >> n)
    {
        v1 = createArray(n);
        v2 = createArray(n);
        v3 = createArray(n);

        t1 = t2 = t3 = cm1 = cm2 = cm3 = cc1 = cc2 = cc3 = 0;

        cout << "--- Ordenacao de " << n << " valores. ---" << endl;
        output << "----- " << n << " valores -----" << endl;

        for (int i = 0; i < m; i++)
        {
            cout << "M" << (i + 1) << endl;
            output << "- M" << (i + 1) << " -" << endl;

            importacao(v1, n);
            copyUpvotes(v1, v2, n);
            copyUpvotes(v1, v3, n);

            cout << endl;

            cout << "Quick sort... ";
            output << "Quick sort" << endl;
            contMoves = 0, contComps = 0;

            start = std::chrono::system_clock::now();
            quickSort(v1, 0, n - 1, &contMoves, &contComps);
            end = std::chrono::system_clock::now();

            elapsed_seconds = end - start;
            t1 += elapsed_seconds.count();
            cm1 += contMoves;
            cc1 += contComps;

            cout << "Concluindo em " << elapsed_seconds.count() << " segundos" << endl
                 << endl;

            output << "Movimentacoes: " << contMoves << endl;
            output << "Comparacoes: " << contComps << endl;
            output << "Concluindo em " << elapsed_seconds.count() << " segundos" << endl
                   << endl;
            cout << "Heap sort... ";
            output << "Heap sort" << endl;
            contMoves = 0, contComps = 0;

            start = std::chrono::system_clock::now();
            heapSort(v2, 0, n - 1, &contMoves, &contComps);
            end = std::chrono::system_clock::now();

            elapsed_seconds = end - start;
            t2 += elapsed_seconds.count();
            cm2 += contMoves;
            cc2 += contComps;

            cout << "Concluindo em " << elapsed_seconds.count() << " segundos" << endl
                 << endl;

            output << "Movimentacoes: " << contMoves << endl;
            output << "Comparacoes: " << contComps << endl;
            output << "Concluindo em " << elapsed_seconds.count() << " segundos" << endl
                   << endl;

            cout << "Intro sort... ";
            output << "Intro sort" << endl;
            contMoves = 0, contComps = 0;

            start = std::chrono::system_clock::now();
            introSort(v3, 0, n - 1, &contMoves, &contComps);
            end = std::chrono::system_clock::now();

            elapsed_seconds = end - start;
            t3 += elapsed_seconds.count();
            cm3 += contMoves;
            cc3 += contComps;

            cout << "Concluindo em " << elapsed_seconds.count() << " segundos" << endl
                 << endl;
            output << "Movimentacoes: " << contMoves << endl;
            output << "Comparacoes: " << contComps << endl;
            output << "Concluindo em " << elapsed_seconds.count() << " segundos" << endl
                   << endl;
        }

        output << "Médias" << endl;

        output << "QuickSort: " << endl
               << "Movimentacoes: " << cm1 / (float)m << endl
               << "Comparacoes: " << cc1 / (float)m << endl
               << "Tempo: " << t1 / (float)m << endl
               << endl;

        output << "HeapSort: " << endl
               << "Movimentacoes: " << cm2 / (float)m << endl
               << "Comparacoes: " << cc2 / (float)m << endl
               << "Tempo: " << t2 / (float)m << endl
               << endl;

        output << "IntroSort: " << endl
               << "Movimentacoes: " << cm3 / (float)m << endl
               << "Comparacoes: " << cc3 / (float)m << endl
               << "Tempo: " << t3 / (float)m << endl
               << endl;

        deleteArray(v1, n);
        deleteArray(v2, n);
        deleteArray(v3, n);

        cout << endl;
    }
}

void testeOrdenacao(Register **v, int n, ofstream &output)
{
    Register **v1, **v2, **v3;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    int contMoves = 0, contComps = 0;
    double t1, t2, t3;
    int cm1 = 0, cm2 = 0, cm3 = 0, cc1 = 0, cc2 = 0, cc3 = 0;

    v1 = createArray(n);
    v2 = createArray(n);
    v3 = createArray(n);
    copyUpvotes(v, v1, n);
    copyUpvotes(v, v2, n);
    copyUpvotes(v, v3, n);

    cout << "--- Teste de ordenacao de " << n << " valores. ---" << endl;
    output << "----- Teste de ordenacao de " << n << " valores -----" << endl;

    cout << "Quick sort... ";
    output << "Quick sort" << endl;
    contMoves = 0, contComps = 0;

    start = std::chrono::system_clock::now();
    quickSort(v1, 0, n - 1, &contMoves, &contComps);
    //  quickSort(v1, 0, n - 1,  &contMov, &contComp);
    end = std::chrono::system_clock::now();

    elapsed_seconds = end - start;
    t1 += elapsed_seconds.count();
    cm1 += contMoves;
    cc1 += contComps;

    cout << "Concluindo em " << elapsed_seconds.count() << " segundos" << endl
         << endl;

    output << "Movimentacoes: " << contMoves << endl;
    output << "Comparacoes: " << contComps << endl;
    output << "Concluindo em " << elapsed_seconds.count() << " segundos" << endl
           << endl;
    cout << "Heap sort... ";
    output << "Heap sort" << endl;
    contMoves = 0, contComps = 0;

    start = std::chrono::system_clock::now();
    heapSort(v2, 0, n - 1, &contMoves, &contComps);
    end = std::chrono::system_clock::now();

    elapsed_seconds = end - start;
    t2 += elapsed_seconds.count();
    cm2 += contMoves;
    cc2 += contComps;

    cout << "Concluindo em " << elapsed_seconds.count() << " segundos" << endl
         << endl;

    output << "Movimentacoes: " << contMoves << endl;
    output << "Comparacoes: " << contComps << endl;
    output << "Concluindo em " << elapsed_seconds.count() << " segundos" << endl
           << endl;

    cout << "Intro sort... ";
    output << "Intro sort" << endl;
    contMoves = 0, contComps = 0;

    start = std::chrono::system_clock::now();
    introSort(v3, 0, n - 1, &contMoves, &contComps);
    end = std::chrono::system_clock::now();

    elapsed_seconds = end - start;
    t3 += elapsed_seconds.count();
    cm3 += contMoves;
    cc3 += contComps;

    cout << "Concluindo em " << elapsed_seconds.count() << " segundos" << endl
         << endl;
    output << "Movimentacoes: " << contMoves << endl;
    output << "Comparacoes: " << contComps << endl;
    output << "Concluindo em " << elapsed_seconds.count() << " segundos" << endl
           << endl;

    output << "Valores originais: ";
    for (int i = 0; i < n; i++)
        output << v[i]->getUpvote() << " ";
    output << endl; 

    output << "Ordenado por Quick Sort: ";
    for (int i = 0; i < n; i++)
        output << v1[i]->getUpvote() << " ";
    output << endl;

    output << "Ordenado por Heap Sort:  ";
    for (int i = 0; i < n; i++)
        output << v2[i]->getUpvote() << " ";
    output << endl;

    output << "Ordenado por Intro Sort: ";
    for (int i = 0; i < n; i++)
        output << v3[i]->getUpvote() << " ";
    output << endl;

    output << "----- Fim teste de ordenacao -----" << endl;

    deleteArray(v1, n);
    deleteArray(v2, n);
    deleteArray(v3, n);
}

int proximoPrimo(int n)
{
	int divisores = 0;
	int primo = 0;

	for (primo = n; primo <= n*2; primo++)
	{
		divisores = 0;

		for (int i = 2; i <= sqrt(primo); i++)
		{
			if (primo % i == 0)
				divisores++;
		}
		if (divisores == 0)
		{
			break;
		}
	}
	return primo;
}

int fHash(int version[3], int n)
{
    int num = version[0] * 100 + version[1] * 10 + version[2] * 1;
    return num % n;
}

int fHash2(int version[3], int n)
{
    int num = version[0] * 100 + version[1] * 10 + version[2] * 1;
    return 7 - (num % 7);
}

void deleteTabela(versao **tabela, int n)
{
    for (int i = 0; i < n; i++)
        if(tabela[i] != nullptr)
            delete tabela[i];

    delete[] tabela;
}

bool verificaVersao(versao **tabela, int indice, int vet[3])
{
    if(tabela[indice]->version[0] == vet[0]
        && tabela[indice]->version[1] == vet[1]
        && tabela[indice]->version[2] == vet[2])
        {
            return true;
        }
        
    else
        return false;
}

void proximoVazio(versao **tabela,int ver[3], int tam, int verifica[1000000], int posI)
{
    int i = posI;
    while(verifica[i] == 1)
    {  
        if(i >= tam - 1)
        {
            i = 0;
        }
        else
        {
            i++;
        }
        if(i == posI)
        {
            cout << "Tabela cheia" << endl;
            break;
        }

    }
    tabela[i] = new versao;
    tabela[i]->version[0] = ver[0];
    tabela[i]->version[1] = ver[1];
    tabela[i]->version[2] = ver[2];
    tabela[i]->quant ++;
    verifica[i] = 1;
}
//função para colocar as versoes que foram adicionadas antes das posições vazias
versao** removeNull(versao **tabela, int n)
{
    int aux = 0;

         for(int i = 0; i < n; i++)
        {
            aux = 0;
            if(tabela[i] == nullptr)
            {
                for(int j = i; j < n; j++)
                {
                    if(tabela[j] != nullptr)
                    {
                        tabela[i] = tabela[j];
                        tabela[j] = nullptr;
                        aux = 1;
                    }
                }
                //caso não elemento tenha sido movido, termina a execução
                if(aux == 0)
                {
                    i = n;
                }
            }
        }
    return tabela;
   
}

void testeHash(Register **r, int n, ofstream &output)
{

/*
    for (int i = 0; i < n; i++)
    {
        cout << r[i]->getVersion()[0] << " ";
        cout << r[i]->getVersion()[1] << " ";
        cout << r[i]->getVersion()[2] << " " << endl;
    }
*/

    int op, imprime = 0;
    cout << "Informe quantas versões você deseja visualizar." << endl;
    do{
        cin >> op;
        if(op < 0)
        {
            cout << "A quantidade de versões precisa ser maior ou igual a 0." << endl;
        }
    } while (op < 0);

    cout << "Gostaria de visualizar os elementos da tabela Hash? Se sim, digite 1" << endl;
    cin >> imprime;
    
    int tam = proximoPrimo(n);
            
    versao **tabela = new versao *[tam];
   
    int h = 0, verifica[tam];

    for(int i = 0; i < tam; i++)
    {
        verifica[i] = 0;
        tabela[i] = nullptr;
    }

    int aux = 1, j = 0, count = 0;
    for(int i = 0; i < n; i++)
    {
                //aux = 1;
        h = fHash(r[i]->getVersion(),tam);
        do
        {

            if(verifica[h] != 1)
            {
                tabela[h] = new versao;
                tabela[h]->version[0] = r[i]->getVersion()[0];
                tabela[h]->version[1] = r[i]->getVersion()[1];
                tabela[h]->version[2] = r[i]->getVersion()[2];
                tabela[h]->quant ++;
                aux = 0;
                verifica[h] = 1;
                j = 0;
                if(imprime == 1)
                    cout << tabela[h]->version[0] << "." << tabela[h]->version[1] << "." << tabela[h]->version[2] << endl;
            }
                    
                    
            else
            {

                if(verificaVersao(tabela, h, r[i]->getVersion()))
                {
                    tabela[h]->quant++;               
                        aux = 0;
                    }
                        
                    else
                    {        
                        j++;
                        h = (fHash(r[i]->getVersion(),tam) + j*fHash2(r[i]->getVersion(),tam));
                        int verification = (fHash(r[i]->getVersion(),tam) + fHash2(r[i]->getVersion(),tam));
                        if (verification == h)
                        {
                            count ++;
                            if(count > 2)
                            {
                                        //cout << "ERRO, indice repetindo varias vezes" << endl;
                                proximoVazio(tabela, r[i]->getVersion(), tam,verifica, h);
                                aux = 0;
                            }
                        }   
                    }
            }
                
        } while( aux != 0);
    }
    tabela = removeNull(tabela, tam);
    int vet[3], quantidade, k = 0;
    for(int i = 0; i < op; i++)
        {
            k = i;
                /*while(tabela[k]->quant < 1)
                {
                    k++;
                    cout << k << " ";
                }*/

            if(tabela[k] != nullptr)
            {
                quantidade = tabela[k]->quant;
                for(int j = 0; j < 3; j++)
                {
                    vet[j] = tabela[k]->version[j];
                    cout << vet[j] << " ";
                }
                output << "Versão " << vet[0] << "." << vet[1] << "." << vet[2] << " " << quantidade << " vezes" << endl;
            }
        }
    deleteArray(r, n);
    deleteTabela(tabela, n);
}

void tabelaHash()
{
    ifstream input("input.dat");
    ofstream output("saida_hash.txt");

    if(!input.good())
    {
        cout << "Arquivo input.dat nao encontrado." << endl;
        input.close();
        return;
    }

    if(!output.good())
    {
        cout << "Arquivo saida_hash.txt nao pode ser criado" << endl;
        output.close();
        return;
    }

    int n, op;
    cout << "Informe quantas versões você deseja visualizar." << endl;
    do{
        cin >> op;
        if(op < 0)
        {
            cout << "A quantidade de versões precisa ser maior ou igual a 0." << endl;
        }
    } while (op < 0);

    while(input >> n)
    {
        if(n < 1000000)
        {
            cout << "--- Teste para " << n << " valores. ---" << endl;
            output << "----- " << n << " valores -----" << endl;
            int tam = proximoPrimo(n);

            Register **r = createArray(n);
            importacao(r, n);

            versao **tabela = new versao *[tam];

            int h = 0, verifica[tam];

            for(int i = 0; i < tam; i++)
            {
                verifica[i] = 0;
                tabela[i] = nullptr;
            }

            int aux = 1, j = 0, count = 0;
            for(int i = 0; i < n; i++)
            {
                //aux = 1;
                h = fHash(r[i]->getVersion(),tam);
                do
                {

                    if(verifica[h] != 1)
                    {
                        tabela[h] = new versao;
                        tabela[h]->version[0] = r[i]->getVersion()[0];
                        tabela[h]->version[1] = r[i]->getVersion()[1];
                        tabela[h]->version[2] = r[i]->getVersion()[2];
                        tabela[h]->quant ++;
                        aux = 0;
                        verifica[h] = 1;
                        j = 0;
                    }
                    
                    
                    else
                    {

                        if(verificaVersao(tabela, h, r[i]->getVersion()))
                        {
                            tabela[h]->quant++;
                            
                            aux = 0;
                        }
                        
                        else
                        {
                            
                                j++;
                                h = (fHash(r[i]->getVersion(),tam) + j*fHash2(r[i]->getVersion(),tam));
                                int verification = (fHash(r[i]->getVersion(),tam) + fHash2(r[i]->getVersion(),tam));
                                if (verification == h)
                                {
                                    count ++;
                                    if(count > 2)
                                    {
                                        //cout << "ERRO, indice repetindo varias vezes" << endl;
                                        proximoVazio(tabela, r[i]->getVersion(), tam,verifica, h);
                                        aux = 0;
                                    }
                                }
                            

                        }
                    }
                
                } while( aux != 0);
            }
            tabela = removeNull(tabela, tam);
            int vet[3], quantidade, k = 0;
            for(int i = 0; i < op; i++)
            {
                k = i;
                /*while(tabela[k]->quant < 1)
                {
                    k++;
                    cout << k << " ";
                }*/

                if(tabela[k] != nullptr)
                {
                    quantidade = tabela[k]->quant;
                    for(int j = 0; j < 3; j++)
                    {
                        vet[j] = tabela[k]->version[j];
                        cout << vet[j] << " ";
                    }
                    output << "Versão " << vet[0] << "." << vet[1] << "." << vet[2] << " " << quantidade << " vezes" << endl;
                }
            }
            deleteArray(r, n);
            deleteTabela(tabela, n);

        }
        
    }
}

void arvoreVP()
{
    ifstream input("input.dat");
    ofstream output("saida_hash.txt");

    if(!input.good())
    {
        cout << "Arquivo input.dat nao encontrado." << endl;
        input.close();
        return;
    }

    if(!output.good())
    {
        cout << "Arquivo saida_hash.txt nao pode ser criado" << endl;
        output.close();
        return;
    }
    int n = 1000000;
    Register **r = createArray(n);
    importacao(r, n);
    
    ArvoreVP arv;
    for(int i = 0; i < n; i++)
    {
        string idReview = r[i]->getID();
        arv.insere(idReview, i);
    }
    cout << arv.getComparacaoInsercao() << " comparacaoInsercao " << endl;
    //arv.imprimePorNivel();
}

void teste()
{
    int n = 100;
    Register **r = createArray(n);
    ofstream output("teste.txt");
    ofstream outputHash("teste_Hash.txt");

    importacao(r, n);

    testeOrdenacao(r, n, output);
    testeHash(r, n, outputHash);

    deleteArray(r, n);
}

int main(int argc, char const *argv[])
{
    char option;

    ifstream bin("tiktok_app_reviews.bin");
    if (!bin.good())
    {
        cout << "Arquivo binario nao encontrado." << endl;

        if (argc < 2)
        {
            cout << "Localizacao de tiktok_app_reviews.csv nao informado." << endl;
            return 1;
        }
        cout << "Tentando abrir " << argv[1] << endl;

        fstream csv;
        csv.open(argv[1], ios::in);

        if (csv.is_open())
        {
            std::chrono::time_point<std::chrono::system_clock> start, end;

            cout << "Processando " << argv[1] << " para tiktok_app_reviews.bin..." << endl;
            start = std::chrono::system_clock::now();
            csvToBin(csv);
            end = std::chrono::system_clock::now();

            std::chrono::duration<double> elapsed_seconds = end - start;
            cout << "Arquivo processado em " << elapsed_seconds.count() << "s" << endl;
        }
        else
        {
            cout << "Nao foi possivel abrir " << argv[1] << endl;
            return 1;
        }
    }

    do
    {
        cout << "Menu: " << endl
             << "1 - Ordenacao: " << endl
             << "2 - Hash" << endl
             << "3 - Modulo de teste:" << endl
             << "0 - Sair" << endl
             << "Digite a opcao: ";

        cin >> option;

        switch (option)
        {
        case '0':
            break;
        case '1':
            arvoreVP();
            break;
        case '2':
            tabelaHash();
            break;
        case '3':
            teste();
            break;
        default:
            cout << "Opcao invalida" << endl;
            break;
        }

    } while (option != '0');

    return 0;
}