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
void Tensor::init_random(float mean, float std)
{
    if (matrix)
    {

        std::default_random_engine generator;
        std::normal_distribution<float> distribution(mean, std);

        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                for (int k = 0; k < d; k++)
                {
                    this->operator()(i, j, k) = distribution(generator);
                }
            }
        }
    }
    else
    {
        throw(tensor_not_initialized());
    }
}

Tensor::Tensor() : r{0}, c{0}, d{0}
{
    matrix = nullptr;
}

/*
Generalizing further, given start address (say base) of an array of size [l x b x h] dimensions, we can access the element at an arbitrary location (a, b, c) in the following way,

data = *(base + a * (b x h) + b * (h) + c); // Note that we haven’t used the higher dimension l.
*/
Tensor::Tensor(int r, int c, int d, float v)
{

    this->r = r;
    this->c = c;
    this->d = d;
    /*matrix = new float **[r];

    for (int i = 0; i < r; i++)
    {
        matrix[i] = new float *[c];
        for (int j = 0; j < c; j++)
        {
            matrix[i][j] = new float[d];
            for (int k = 0; k < d; k++)
            {
                matrix[i][j][k] = v;
            }
        }
    }
    */
    matrix = get_matrix(r, c, d, v);
}

Tensor::~Tensor()
{
    for (int i = 0; i < r; i++)
    {

        for (int j = 0; j < c; j++)
        {
            delete[] matrix[i][j];
        }
        delete[] matrix[i];
    }
    delete[] matrix;
}

float Tensor::operator()(int i, int j, int k) const
{

    if (i >= this->r || i < 0 || j >= this->c || j < 0 || k >= this->d || k < 0)
    {
        throw index_out_of_bound();
    }
    else
    {
        return matrix[i][j][k];
    }
}

float &Tensor::operator()(int i, int j, int k)
{
    if (i >= this->r || i < 0 || j >= this->c || j < 0 || k >= this->d || k < 0)
    {
        throw index_out_of_bound();
    }
    else
    {
        return this->matrix[i][j][k];
    }
}

Tensor::Tensor(const Tensor &that)
{
    this->r = that.r;
    this->c = that.c;
    this->d = that.d;

    matrix = new float **[r];

    for (int i = 0; i < r; i++)
    {
        matrix[i] = new float *[c];
        for (int j = 0; j < c; j++)
        {
            matrix[i][j] = new float[d];
            for (int k = 0; k < d; k++)
            {
                matrix[i][j][k] = that.matrix[i][j][k];
            }
        }
    }
}

Tensor Tensor::operator-(const Tensor &rhs)
{
    if (r == rhs.r && c == rhs.c && d == rhs.d)
    {
        Tensor res{r, c, d, 0};
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                for (int k = 0; k < d; k++)
                {
                    res.matrix[i][j][k] = this->matrix[i][j][k] - rhs.matrix[i][j][k];
                }
            }
        }
        return res;
    }
    else
    {
        throw dimension_mismatch();
    }
}

Tensor Tensor::operator+(const Tensor &rhs)
{
    if (r == rhs.r && c == rhs.c && d == rhs.d)
    {
        Tensor res{r, c, d, 0};
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                for (int k = 0; k < d; k++)
                {
                    res.matrix[i][j][k] = this->matrix[i][j][k] + rhs.matrix[i][j][k];
                }
            }
        }
        return res;
    }
    else
    {
        throw dimension_mismatch();
    }
}

Tensor Tensor::operator*(const Tensor &rhs)
{
    if (r == rhs.r && c == rhs.c && d == rhs.d)
    {
        Tensor res{r, c, d, 0};
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                for (int k = 0; k < d; k++)
                {
                    res.matrix[i][j][k] = this->matrix[i][j][k] * rhs.matrix[i][j][k];
                }
            }
        }
        return res;
    }
    else
    {
        throw dimension_mismatch();
    }
}

Tensor Tensor::operator/(const Tensor &rhs)
{
    if (r == rhs.r && c == rhs.c && d == rhs.d)
    {
        Tensor res{r, c, d, 0};
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                for (int k = 0; k < d; k++)
                {
                    res.matrix[i][j][k] = this->matrix[i][j][k] / rhs.matrix[i][j][k];
                }
            }
        }
        return res;
    }
    else
    {
        throw dimension_mismatch();
    }
}

Tensor Tensor::operator-(const float &rhs)
{
    Tensor res{r, c, d, 0};
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            for (int k = 0; k < d; k++)
            {
                res.matrix[i][j][k] = this->matrix[i][j][k] - rhs;
            }
        }
    }
    return res;
}

Tensor Tensor::operator+(const float &rhs)
{
    Tensor res{r, c, d, 0};
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            for (int k = 0; k < d; k++)
            {
                res.matrix[i][j][k] = this->matrix[i][j][k] + rhs;
            }
        }
    }
    return res;
}

Tensor Tensor::operator*(const float &rhs)
{
    Tensor res{r, c, d, 0};
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            for (int k = 0; k < d; k++)
            {
                res.matrix[i][j][k] = this->matrix[i][j][k] * rhs;
            }
        }
    }
    return res;
}

Tensor Tensor::operator/(const float &rhs)
{
    Tensor res{r, c, d, 0};
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            for (int k = 0; k < d; k++)
            {
                res.matrix[i][j][k] = this->matrix[i][j][k] / rhs;
            }
        }
    }
    return res;
}

Tensor &Tensor::operator=(const Tensor &other)
{
    if (this == &other)
        return *this;
    /* 1. Distruggere la vecchia metrice in this
       2. Ricrearla con i valori che ci sono in other
    */

    this->~Tensor();

    this->r = other.r;
    this->c = other.c;
    this->d = other.d;

    matrix = new float **[r];

    for (int i = 0; i < r; i++)
    {
        matrix[i] = new float *[c];
        for (int j = 0; j < c; j++)
        {
            matrix[i][j] = new float[d];
            for (int k = 0; k < d; k++)
            {
                matrix[i][j][k] = other.matrix[i][j][k];
            }
        }
    }
    return *this;
}

void Tensor::init(int r, int c, int d, float v)
{
    if (matrix)
    {
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++)
                for (int k = 0; k < d; k++)
                    matrix[i][j][k] = v;
    }
    else
        throw(tensor_not_initialized());
}

void Tensor::clamp(float low, float high)
{
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            for (int k = 0; k < d; k++)
            {
                if (matrix[i][j][k] < low)
                    matrix[i][j][k] = low;
                if (matrix[i][j][k] > high)
                    matrix[i][j][k] = high;
            }
}

void Tensor::rescale(float new_max)
{
    /*  newvalue(i,j,k) = ((data(i,j,k)-min(k))/(max(k)-min(k)))*new_max
     *
     * where max(k) and min(k) are the maximum and minimum value in the k-th channel.
     *
     * new_max is the new value for the maximum.

     1. Scorrere e trovare min e max
     2. Definire un nuovo valore di k
     3. Applicare la formula
    */

    float min, max;

    for (int k = 0; k < d; k++)
    {

        max = getMax(k);
        min = getMin(k);

        for (int j = 0; j < c; j++)
        {
            for (int i = 0; i < r; i++)
            {
                if (min != max)
                    matrix[i][j][k] = ((matrix[i][j][k] - min) / (max - min)) * new_max;
                else
                    matrix[i][j][k] = new_max;
            }
        }
    }
}

Tensor Tensor::padding(int pad_h, int pad_w)
{
    /*Creare nuova matrice che sostituirà quella vecchia
      Si copiano i valori della vecchia metrice in quella nuova
      Si elimina la vecchia matrice
    */
    Tensor temp{r + (2 * pad_h), c + (2 * pad_w), d, 0};

    for (int k = 0; k < d; k++)
    {
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                temp.matrix[i + pad_h][j + pad_w][k] = matrix[i][j][k];
            }
        }
    }

    return temp;
}

Tensor Tensor::subset(unsigned int row_start, unsigned int row_end, unsigned int col_start, unsigned int col_end, unsigned int depth_start, unsigned int depth_end)
{
    if (row_start >= (unsigned int)r || row_end >= (unsigned int)r || col_start >= (unsigned int)c || col_end >= (unsigned int)c || depth_start >= (unsigned int)d || depth_end >= (unsigned int)d)
        throw dimension_mismatch();
    Tensor temp{(int)(row_end - row_start + 1), (int)(col_end - col_start + 1), (int)(depth_end - depth_start + 1), 0};

    /* Si procede ora con la copia */

    for (unsigned int k = 0; k < (depth_end - depth_start + 1); k++)
    { //k deve scorrere per gli elementi che sono uguali alla distanza tra depth_end - depth_start
        for (unsigned int i = 0; i < (row_end - row_start + 1); i++)
        {
            for (unsigned int j = 0; j < (col_end - col_start + 1); j++)
            {
                //l'indice parte dalla posizione iniziale + il valore iniziale
                temp.matrix[i][j][k] = matrix[i + row_start][j + col_start][k + depth_start];
            }
        }
    }
    return temp;
}

Tensor Tensor::concat(const Tensor &rhs, int axis)
{

    if (axis == 0)
    { // rows
        if (c == rhs.c && d == rhs.d)
        {
            Tensor temp{r + rhs.r, c, d, 0.0};
            /* Copiare il primo tensore in temp */

            for (int k = 0; k < d; k++)
            {
                for (int i = 0; i < r; i++)
                {
                    for (int j = 0; j < c; j++)
                    {
                        temp.matrix[i][j][k] = matrix[i][j][k];
                    }
                }
            }
            /* Copiare il secondo tensore in temp */
            for (int k = 0; k < rhs.d; k++)
            {
                for (int i = 0; i < rhs.r; i++)
                {
                    for (int j = 0; j < rhs.c; j++)
                    {
                        temp.matrix[i + r][j][k] = rhs.matrix[i][j][k]; //occhio a dove si copia in temp
                    }
                }
            }
            return temp;
        }
        else
        {
            throw concat_wrong_dimension();
        }
    }
    else
    { //columns
        if (r == rhs.r && d == rhs.d)
        {
            Tensor temp{r, c + rhs.c, d, 0.0};

            for (int k = 0; k < d; k++)
            {
                for (int i = 0; i < r; i++)
                {
                    for (int j = 0; j < c; j++)
                    {
                        temp.matrix[i][j][k] = matrix[i][j][k];
                    }
                }
            }

            for (int k = 0; k < rhs.d; k++)
            {
                for (int i = 0; i < rhs.r; i++)
                {
                    for (int j = 0; j < rhs.c; j++)
                    {
                        temp.matrix[i][j + c][k] = rhs.matrix[i][j][k];
                    }
                }
            }
            return temp;
        }
        else
        {
            throw concat_wrong_dimension();
        }
    }
}

Tensor Tensor::convolve(const Tensor &f)
{

    Tensor temp{(this->r - f.r + 1), (this->c - f.c + 1), d, 0.0};

    float ris;

    for (int k = 0; k < temp.d; k++)
    {
        for (int i = 0; i < temp.r; i++)
        {
            for (int j = 0; j < temp.c; j++)
            {
                ris = 0; //Metto il valore zero in ogni casella del filtro
                //Mi dice cosa c'è dentro ogni casella finale

                for (int m = 0; m < f.r; m++)
                {
                    for (int n = 0; n < f.c; n++)
                    {
                        ris += (this->matrix[m + i][n + j][k] * f.matrix[m][n][k]);
                    }
                }
                temp.matrix[i][j][k] = ris;
            }
        }
    }
    return temp;
}

//utility
float ***Tensor::get_matrix(int i, int j, int k, float v)
{

    float ***temp = new float **[r];

    for (int i = 0; i < r; i++)
    {
        temp[i] = new float *[c];
        for (int j = 0; j < c; j++)
        {
            temp[i][j] = new float[d];
            for (int k = 0; k < d; k++)
            {
                temp[i][j][k] = v;
            }
        }
    }
    return temp;
}

int Tensor::rows()
{
    return r;
};

int Tensor::cols()
{
    return c;
};

int Tensor::depth()
{
    return d;
};

float Tensor::getMax(int k)
{
    float max;
    max = matrix[0][0][k];
    for (int j = 0; j < c; j++)
    {
        for (int i = 0; i < r; i++)
        {
            if (matrix[i][j][k] > max)
                max = matrix[i][j][k];
        }
    }
    return max;
};

float Tensor::getMin(int k)
{
    float min;
    min = matrix[0][0][k];
    for (int j = 0; j < c; j++)
    {
        for (int i = 0; i < r; i++)
        {
            if (matrix[i][j][k] < min)
                min = matrix[i][j][k];
        }
    }
    return min;
}

void Tensor::showSize()
{
    cout << "rows" << r << "colums" << c << "depth" << d;
}

//IOSTREAM

ostream &operator<<(ostream &stream, const Tensor &obj)
{
    for (int k = 0; k < obj.d; k++)
    {
        for (int i = 0; i < obj.r; i++)
        {
            stream << '[';
            for (int j = 0; j < obj.c; j++)
            {
                stream << obj.matrix[i][j][k];
                if (j != (obj.c - 1))
                    stream << ',';
            }
            stream << ']' << endl;
        }
        stream << endl;
    }

    return stream;
}

void Tensor::read_file(string filename)
{
    if (matrix != nullptr)
        this->~Tensor();
    ifstream ifile;
    ifile.open(filename, ios::in);
    if (!ifile.is_open())
    {
        throw unable_to_read_file();
    }
    string str_r, str_c, str_d;
    getline(ifile, str_r);
    getline(ifile, str_c);
    getline(ifile, str_d);
    r = stof(str_r);
    c = stof(str_c);
    d = stof(str_d);

    matrix = get_matrix(r, c, d, 0);

    string value;
    for (int k = 0; k < d; k++)
    {
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                getline(ifile, value);
                matrix[i][j][k] = stof(value);
            }
        }
    }
    ifile.close();
}

void Tensor::write_file(string filename)
{
    ofstream ifile;
    ifile.open(filename);
    if (!ifile.is_open())
    {
        throw unable_to_write_file();
    }
    ifile << to_string(r) << endl;
    ifile << to_string(c) << endl;
    ifile << to_string(d) << endl;

    string value;
    for (int k = 0; k < d; k++)
    {
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {

                /* Serve un casting ad intero*/
                ifile << to_string(matrix[i][j][k]) << endl;
            }
        }
    }
    ifile.close();
}
