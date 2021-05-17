
    cdf[pos] = 1.0 * count / number_of_pixels;
    equalized[pos] = round(cdf[pos] * (L - 1));
