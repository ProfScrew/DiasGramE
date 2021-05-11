
#include <iostream>


#include "tensor.h"
#include "dais_exc.h"
int main() {
    try{
    Tensor t(3,3,3,2.0);
    //std::cout<<t(1,1,1);
    //std:: cout<<endl;
    Tensor c(t);
    t(1,1,1)=1;
    Tensor res=c-t;
    t.stampa_Tensor();
    std::cout<<endl;
    std::cout<<endl;
    c.stampa_Tensor();
    std::cout<<endl;
    std::cout<<endl;
    res.stampa_Tensor();
    }catch( index_out_of_bound err){
        std::cout<<err.msg;
    }
    }
