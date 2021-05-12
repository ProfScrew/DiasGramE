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
Tensor::Tensor(): r{0}, c{0}, d{0} {
}

/*
Generalizing further, given start address (say base) of an array of size [l x b x h] dimensions, we can access the element at an arbitrary location (a, b, c) in the following way,

data = *(base + a * (b x h) + b * (h) + c); // Note that we haven’t used the higher dimension l.
*/
Tensor::Tensor(int r, int c, int d, float v){
    
    this->r=r;
    this->c=c;
    this->d=d;
    matrix = new float**[r];

    for(int i = 0; i < r; i++){
        matrix[i] = new float*[c];
        for(int j = 0; j < c; j++){
            matrix[i][j] = new float[d];
            for(int k = 0; k < d; k++ ) {
                matrix[i][j][k] = v;
            }
        }
    }  
}


Tensor::~Tensor(){

    for(int i = 0; i<c; i++){
        for(int j = 0; j<d; j++){
            delete[] matrix[i][j];
        }
        delete[] matrix[i];
    }
    

    delete[] matrix;
}

float Tensor::operator()(int i, int j, int k) const{
    
    if(i >= this->r || i < 0 || j >= this->c || j < 0 || k >= this->d || k < 0){
        throw index_out_of_bound();
    }else{
        return matrix[i][j][k];
    }  
}

float& Tensor::operator()(int i, int j, int k){
    if(i >= this->r || i < 0 || j >= this->c || j < 0 || k >= this->d || k < 0){
        throw index_out_of_bound();
    }else{
        return this->matrix[i][j][k];
    }
}


Tensor::Tensor(const Tensor& that){
    this->r = that.r;
    this->c = that.c;
    this->d = that.d;

    matrix = new float**[r];

    for(int i = 0; i < r; i++){
        matrix[i] = new float*[c];
        for(int j = 0; j < c; j++){
            matrix[i][j] = new float[d];
            for(int k = 0; k < d; k++) {
                matrix[i][j][k] = that.matrix[i][j][k];
            }
        }
    }
}

void Tensor::stampa_Tensor(){
    
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            for(int k=0;k<d;k++){
                cout<<matrix[i][j][k];
                cout << ":";
            }
            cout << "|";
        }
        cout << endl;
    }
}

Tensor Tensor::operator-(const Tensor &rhs) {
    if(r == rhs.r && c == rhs.c && d == rhs.d){
        Tensor res{r,c,d,0};
        for(int i=0;i<r;i++){
            for(int j=0;j<c;j++){
                for(int k=0;k<d;k++){
                    res.matrix[i][j][k] = this->matrix[i][j][k] - rhs.matrix[i][j][k];
                }
            }
        }
        return res;
    }else{
        throw dimension_mismatch();
    }
}
