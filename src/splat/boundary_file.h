//
//  boundary_file.hpp
//  splat
//
//  Created by Peter Work Watkins on 1/9/18.
//  Copyright © 2018 ke7ist. All rights reserved.
//

#ifndef boundary_file_h
#define boundary_file_h

#include <string>

#include "path.h"
#include "elevation_map.h"

class BoundaryFile
{
    private: Path &path;
public:
    BoundaryFile(Path &path)
    : path(path)
    {}
    
    void LoadBoundaries(const std::string &filename, ElevationMap &em);
};

#endif /* boundary_file_hpp */
