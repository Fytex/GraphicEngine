#ifndef COLOR_H
#define COLOR_H

#include <vector>


class Color{
    public:
        int R, G, B;


        Color();

        Color(int R, int G, int B);
        std::vector<float> getVectorf(void);
        
};

#endif
