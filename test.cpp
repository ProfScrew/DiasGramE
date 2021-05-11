#include <iostream>


#include "tensor.h"
#include "dais_exc.h"
int main() {
    try{
    Tensor t(2,2,2,2.0);
    //std::cout<<t(1,1,1);
    }catch( index_out_of_bound err){
        std::cout<<err.msg;
    }
    }