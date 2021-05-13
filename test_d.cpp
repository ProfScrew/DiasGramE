#include <iostream>
#include "DAISGram.h"
#include "tensor.h"

int main(){

    DAISGram a;

    a.generate_random(1,1,3);

    cout << a.data;

    DAISGram b = a.warhol();

    cout << b.data;

    return 0;
}