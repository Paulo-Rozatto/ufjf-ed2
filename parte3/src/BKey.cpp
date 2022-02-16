#include "BKey.hpp"

BKey::BKey(int index, char *id)
{
    init(index, id);
}

BKey::~BKey()
{
}

void BKey::init(int index, char *id)
{
    this->index = index;
    for (int i = 0; i < 90; i++)
    {
        this->id[i] = id[i];
    }
}

bool operator==(BKey &b1, BKey &b2)
{
    return strcmp(b1.getId(), b2.getId()) == 0;
}

bool operator>(BKey &b1, BKey &b2)
{
    return strcmp(b1.getId(), b2.getId()) > 0;
}

bool operator<(BKey &b1, BKey &b2)
{
    return strcmp(b1.getId(), b2.getId()) < 0;
}

std::ostream &operator<<(std::ostream &outs, const BKey &key)
{
    outs << key.id;
    return outs;
}
