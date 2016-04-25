//
//  path.h
//  splat
//
//  Created by Peter Work Watkins on 4/8/16.
//  Copyright © 2016 ke7ist. All rights reserved.
//

#ifndef path_h
#define path_h

#include <stdio.h>

class Path
{
public:
    std::vector<double> lat;
    std::vector<double> lon;
    std::vector<double> elevation;
    std::vector<double> distance;
    int length;
    
public:
    Path(int size)
    :lat(std::vector<double>(size)),
    lon(std::vector<double>(size)),
    elevation(std::vector<double>(size)),
    distance(std::vector<double>(size)),
    length(0)
    {}
    
    ~Path();
};

#endif /* path_h */
