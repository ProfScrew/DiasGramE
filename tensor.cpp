#include <iostream>
#include <string>
#include <random>
#include <math.h>
#include <fstream>

#include "dais_exc.h"
#include "tensor.h"

#define PI 3.141592654
#define FLT_MAX 3.402823466e+38F /* max value */
#define FLT_MIN 1.175494351e-38F /* min positive value */

using namespace std;


/**
 * Random Initialization
 * 
 * Perform a random initialization of the tensor
 * 
 * @param mean The mean
 * @param std  Standard deviation
 */
/*void Tensor::init_random(float mean, float std){
    if(data){

        std::default_random_engine generator;
        std::normal_distribution<float> distribution(mean,std);

        for(int i=0;i<r;i++){
            for(int j=0;j<c;j++){
                for(int k=0;k<d;k++){
                    this->operator()(i,j,k)= distribution(generator);
                }
            }
        }    

    }else{
        throw(tensor_not_initialized());
    }
}*/
Tensor::Tensor(){
    r = c = d = 0;
}

/*
Generalizing further, given start address (say base) of an array of size [l x b x h] dimensions, we can access the element at an arbitrary location (a, b, c) in the following way,

data = *(base + a * (b x h) + b * (h) + c); // Note that we haven’t used the higher dimension l.
*/
Tensor::Tensor(int r, int c, int d, float v){
    /*
    data = new float[r*c*d];
    this->r=r;
    




    for(int i; i<(r*c*d); i++){
        data[i] = v;
        std::cout << data[i];
    }
    */
    this->r = r;
    this->c=c;
    this->d=d;
    
    int i, j;
    matrix = new float**[r];

    for(i = 0; i<c; i++){
        matrix[i] = new float*[c];
        for(j = 0; j<d; j++){
            matrix[i][j] = new float[d];
        }
    }

    /*

    float** matrix = new float*[r];

        for (int i = 0; i < c; i++)
 
            matrix[i] = new float[c];
        
        

        for (int i = 1; i < c; i++)  //Questo ciclo serve per mettere a posto i puntatori
            matrix[i] = matrix[0] + i*c;

    */  
}


Tensor::~Tensor(){
    int i, j;

    for(i = 0; i<c; i++){
        for(j = 0; j<d; j++){
            delete[] matrix[i][j];
        }
        delete[] matrix[i];
    }
    

    delete[] matrix;
}
