#include <iostream>
#include <string>

#include "dais_exc.h"
#include "tensor.h"
#include "libbmp.h"
#include "DAISGram.h"

using namespace std;

DAISGram::DAISGram()
{
}

DAISGram::~DAISGram()
{
}

/**
 * Load a bitmap from file
 *
 * @param filename String containing the path of the file
 */
void DAISGram::load_image(string filename)
{
    BmpImg img = BmpImg();

    img.read(filename.c_str());

    const int h = img.get_height();
    const int w = img.get_width();

    data = Tensor(h, w, 3, 0.0);

    for (int i = 0; i < img.get_height(); i++)
    {
        for (int j = 0; j < img.get_width(); j++)
        {
            data(i, j, 0) = (float)img.red_at(j, i);
            data(i, j, 1) = (float)img.green_at(j, i);
            data(i, j, 2) = (float)img.blue_at(j, i);
        }
    }
}

/**
 * Save a DAISGram object to a bitmap file.
 * 
 * Data is clamped to 0,255 before saving it.
 *
 * @param filename String containing the path where to store the image.
 */
void DAISGram::save_image(string filename)
{

    data.clamp(0, 255);

    BmpImg img = BmpImg(getCols(), getRows());

    img.init(getCols(), getRows());

    for (int i = 0; i < getRows(); i++)
    {
        for (int j = 0; j < getCols(); j++)
        {
            img.set_pixel(j, i, (unsigned char)data(i, j, 0), (unsigned char)data(i, j, 1), (unsigned char)data(i, j, 2));
        }
    }

    img.write(filename);
}

int DAISGram::getRows()
{
    return data.rows();
}

int DAISGram::getCols()
{
    return data.cols();
}

int DAISGram::getDepth()
{
    return data.depth();
}

DAISGram DAISGram::brighten(float bright)
{

    DAISGram t;

    Tensor temp{data};

    temp = data + bright;

    t.data = temp;

    return t;
}

DAISGram DAISGram::grayscale()
{

    DAISGram t;

    /*
    Tensor res;
    for(int k = 0; k < data.depth(); k++){
        Tensor temp = data.subset(0, (data.rows()-1), 0, (data.cols() -1), k, k);
        res = res + temp;
    }
    res = res /data.depth();
    */

    Tensor Red = data.subset(0, (data.rows() - 1), 0, (data.cols() - 1), 0, 0);
    Tensor Green = data.subset(0, (data.rows() - 1), 0, (data.cols() - 1), 1, 1);
    Tensor Blue = data.subset(0, (data.rows() - 1), 0, (data.cols() - 1), 2, 2);

    Tensor res = (Red + Green + Blue) / 3;

    t.data = res;
    return t;
}

DAISGram DAISGram::warhol(){
    DAISGram t;
    /*Tensor* top_right = new Tensor{data}; */
    Tensor top_right{data};
    Tensor bottom_left{data};
    Tensor bottom_right{data};

    top_right.swap_channel(0,1);
    bottom_left.swap_channel(1,2);
    bottom_right.swap_channel(0,2);



    Tensor top_result = data.concat(top_right, 1);
    Tensor bottom_result = bottom_left.concat(bottom_right, 1);

    Tensor res = top_result.concat(bottom_result,0);

    t.data = res;
    
    return t;
}

/**
 * Generate Random Image
 * 
 * Generate a random image from nois
 * 
 * @param h height of the image
 * @param w width of the image
 * @param d number of channels
 * @return returns a new DAISGram containing the generated image.
 */
void DAISGram::generate_random(int h, int w, int d)
{
    data = Tensor(h, w, d, 0.0);
    data.init_random(128, 50);
    data.rescale(255);
}
