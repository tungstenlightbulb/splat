//
//  path.h
//  splat
//
//  Created by Peter Work Watkins on 4/8/16.
//  Copyright © 2016 ke7ist. All rights reserved.
//

#ifndef path_h
#define path_h

#include <vector>

class Site;
class ElevationMap;

// Forward-declare ElevationMap since ElevationMap uses Path
class ElevationMap;

class Path
{
private:
    double ppd;
    int arraysize;
    
public:
    std::vector<double> lat;
    std::vector<double> lon;
    std::vector<double> elevation;
    std::vector<double> distance;
    int length;
    
public:
    Path(int size, double ppd)
    :lat(std::vector<double>(size)),
    lon(std::vector<double>(size)),
    elevation(std::vector<double>(size)),
    distance(std::vector<double>(size)),
    arraysize(size),
    length(0),
    ppd(ppd)
    {}
    
    void ReadPath(const Site &source, const Site &destination, const ElevationMap &em);
    
    ~Path();
};

#endif /* path_h */
