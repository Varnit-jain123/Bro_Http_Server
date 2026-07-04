#include<bro_container.h>
bool Container::contains(string keyName)
{
    auto iterator=this->dataSet.find(keyName);
    return iterator!=this->dataSet.end();
}