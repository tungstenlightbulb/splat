//
//  pat_file.h
//  splat
//
//  Created by Peter Work Watkins on 1/7/18.
//  Copyright © 2018 ke7ist. All rights reserved.
//

#ifndef pat_file_h
#define pat_file_h

#include <stdio.h>

class PatFile
{
public:
        bool got_elevation_pattern;
    bool got_azimuth_pattern;
        float antenna_pattern[361][1001];

public:
void LoadPAT(const std::string &filename);
};

#endif /* pat_file_h */
