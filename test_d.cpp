#include <iostream>
#include "DAISGram.h"
#include "tensor.h"

int main(){

    DAISGram a;

    a.generate_random(2,2,3);

    cout << a.data;

    DAISGram b = a.grayscale();

    cout << b.data;

    return 0;
}