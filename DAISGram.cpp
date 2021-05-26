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

    Tensor res{data.rows(), data.cols(), 1, 0};

    Tensor Ca{data}, Cb{data};
    /*  data        RGB
        Ca          BRG
        Cb          GBR
    */
    Ca.swap_channel(0, 1);
    Ca.swap_channel(0, 2);

    Cb.swap_channel(0, 1);
    Cb.swap_channel(1, 2);

    res = (data + Ca + Cb) / 3;

    t.data = res;
    return t;
}

DAISGram DAISGram::warhol()
{
    DAISGram t;
    /*Tensor* top_right = new Tensor{data}; */
    Tensor top_right{data};
    Tensor bottom_left{data};
    Tensor bottom_right{data};

    top_right.swap_channel(0, 1);    //RED WITH GREEN
    bottom_left.swap_channel(1, 2);  //GREEN WITH BLUE
    bottom_right.swap_channel(0, 2); //RED WITH BLUE

    Tensor top_result = data.concat(top_right, 1);
    Tensor bottom_result = bottom_left.concat(bottom_right, 1);

    Tensor res = top_result.concat(bottom_result, 0);

    t.data = res;

    return t;
}

DAISGram DAISGram::sharpen()
{
    DAISGram temp;
    Tensor filter{3, 3, 3, 0};
    /*
    for(int k = 0; k < data.depth(); k++){
        filter(1,1,k) = 5;
        filter(0,1,k) = -1;
        filter(1,0,k) = -1;
        filter(1,2,k) = -1;
        filter(2,1,k) = -1;
    }*/
    filter.read_file("filter/sharp.txt");
    Tensor res;

    res = data.padding((filter.rows() - 1) / 2, (filter.cols() - 1) / 2);

    res = res.convolve(filter);
    res.clamp(0, 255);
    temp.data = res;

    return temp;
}

DAISGram DAISGram::emboss()
{
    DAISGram temp;
    Tensor filter{3, 3, 3, 0};
    /*
    for(int k = 0; k < data.depth(); k++){
        filter(1,1,k) = 5;
        filter(0,1,k) = -1;
        filter(1,0,k) = -1;
        filter(1,2,k) = -1;
        filter(2,1,k) = -1;
    }*/
    filter.read_file("filter/emboss.txt");
    Tensor res;

    res = data.padding((filter.rows() - 1) / 2, (filter.cols() - 1) / 2);

    res = res.convolve(filter);
    res.clamp(0, 255);
    temp.data = res;

    return temp;
}

DAISGram DAISGram::edge()
{
    DAISGram temp;
    temp.data = data;

    temp = temp.grayscale();

    Tensor filter{3, 3, 3, 0};
    /*
    for(int k = 0; k < data.depth(); k++){
        filter(1,1,k) = 5;
        filter(0,1,k) = -1;
        filter(1,0,k) = -1;
        filter(1,2,k) = -1;
        filter(2,1,k) = -1;
    }*/
    filter.read_file("filter/edge.txt");
    Tensor res;

    res = temp.data.padding((filter.rows() - 1) / 2, (filter.cols() - 1) / 2);

    res = res.convolve(filter);
    res.clamp(0, 255);
    temp.data = res;

    return temp;
}

DAISGram DAISGram::smooth(int h)
{
    DAISGram temp;
    Tensor filter{h, h, 3, (1 / ((float)h * (float)h))};

    Tensor res = data.padding((filter.rows() - 1) / 2, (filter.cols() - 1) / 2);

    res = res.convolve(filter);
    res.clamp(0, 255);
    temp.data = res;

    return temp;
}

DAISGram DAISGram::blend(const DAISGram &rhs, float alpha)
{
    DAISGram result;
    result.data = (data * alpha) + (rhs.data * (1 - alpha));

    return result;
}
DAISGram DAISGram::greenscreen(DAISGram &bkg, int rgb[], float threshold[])
{
    DAISGram result;
    bool check;
    Tensor temp{data};

    //scroll through the image
    for (int i = 0; i < data.rows(); i++)
    {
        for (int j = 0; j < data.cols(); j++)
        {
            check = true;
            for (int k = 0; k < data.depth(); k++)
            {
                //check if the pixel is in the range
                if (data(i, j, k) < (rgb[k] - threshold[k]) or data(i, j, k) > (rgb[k] + threshold[k]))
                    check = false;
            }
            if (check)
            { //change to the background
                for (int k = 0; k < data.depth(); k++)
                {
                    temp(i, j, k) = bkg.data(i, j, k);
                }
            }
        }
    }
    result.data = temp;
    return result;
}

DAISGram DAISGram::equalize()
{
    DAISGram result;
    Tensor temp{data};

    for(int k=0;k<data.depth();k++){
        int histogram[256] = {};
        float cdf[256] = {}, equalized[256] = {};
        for (int i = 0; i < data.rows(); i++)
        {
            for (int j = 0; j < data.cols(); j++)
            {
                histogram[(int)data(i, j, k)]++;
            }
        }

        for(int i=0;i<256;i++){
            for(int j=0;j<=i;j++){
                cdf[i]+=histogram[j];
            }
        }

        float cdf_min = 0;
        int index = 0;
        while (cdf_min == 0)
        {
            if (cdf[index] != 0.0)
                cdf_min = cdf[index];

            index++;
        }

        for (int i = 0; i < 256; i++)
        {
            equalized[i] = (((cdf[i] - cdf_min) / ((float)(data.rows() * data.cols()) - cdf_min)) * 255);
        }

        for (int i = 0; i < data.rows(); i++)
        {
            for (int j = 0; j < data.cols(); j++)
            {
                    temp(i, j, k) = equalized[(int)data(i, j, k)];
            }
        }
    }

    result.data = temp;
    return result;
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
