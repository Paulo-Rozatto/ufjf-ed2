#ifndef IMPORT_HELPER
#define IMPORT_HELPER

void importacao(Register **registers, int n)
{
    fstream bin;
    vector<int> indices(n);

    bin.open("tiktok_app_reviews.bin", ios::in | ios::binary);

    srand(time(NULL));

    cout << "Escolhendo " << n << " registros aleatoriamente..." << endl;

    for (int i = 0; i < n; i++)
        indices[i] = rand() % ROWS;

    sort(indices.begin(), indices.end());

    for (int i = 0; i < n; i++)
        registers[i]->init(bin, indices[i]);

    cout << "Registros escolhidos!" << endl
         << endl;
}

Register **createArray(int n)
{
    Register **v;

    v = new Register *[n];
    for (int i = 0; i < n; i++)
        v[i] = new Register();

    return v;
}

void deleteArray(Register **v, int n)
{
    for (int i = 0; i < n; i++)
        delete v[i];

    delete[] v;
}

#endif