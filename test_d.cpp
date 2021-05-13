#include <iostream>
#include "DAISGram.h"
#include "tensor.h"

int main(){

    DAISGram a;

    a.generate_random(2,2,2);

    cout << a.data;

    DAISGram b = a.brighten(1);

    cout << b.data;
    return 0;
}