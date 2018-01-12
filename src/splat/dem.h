//
//  dem.h
//  splat
//
//  Created by Peter Work Watkins on 4/8/16.
//  Copyright © 2016 ke7ist. All rights reserved.
//

#ifndef dem_h
#define dem_h

#include <vector>

class Dem
{
public:
    int min_north;
    int max_north;
    int min_west;
    int max_west;
    int max_el;
    int min_el;
    std::vector<short> data;
    std::vector<unsigned char> mask;
    std::vector<unsigned char> signal;
    
public:
    Dem(int size)
    :data(std::vector<short>(size * size)),
    mask(std::vector<unsigned char>(size * size)),
    signal(std::vector<unsigned char>(size * size)),
    min_north(0),
    max_north(0),
    min_west(0),
    max_west(0),
    max_el(0),
    min_el(0)
    {}
    
    ~Dem();
};

#endif /* dem_h */
