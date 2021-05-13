
#include <iostream>


#include "tensor.h"
#include "dais_exc.h"
int main() {
    try{
    Tensor t(3,3,3,7);

    
    t(1,1,0)=5;
    t(1,1,1)=10;
    
    std::cout << t;

    t.read_file("tensors/t_5_4_2_random.txt");
    t.write_file("test.txt");
    std::cout << t;
    /*Tensor a = t.padding(1,1);
    t.stampa_Tensor();
    std::cout << std::endl;
    a.stampa_Tensor();
    t = a.subset(1,3,1,3,0,5);
    std::cout << std::endl;
    t.stampa_Tensor();*/
    }catch(index_out_of_bound err){
        std::cout<<err.msg;
    }catch( dimension_mismatch err){
        std::cout<<err.msg;
    }catch( concat_wrong_dimension err){
        std::cout<<err.msg;
    }  
}
