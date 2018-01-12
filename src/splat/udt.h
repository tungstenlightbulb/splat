//
//  udt.h
//  splat
//
//  Created by Peter Work Watkins on 1/9/18.
//  Copyright © 2018 ke7ist. All rights reserved.
//

#ifndef udt_h
#define udt_h

#include <string>

#include "splat_run.h"
#include "elevation_map.h"

class Udt
{
private:
    const SplatRun &sr;
public:
    Udt(const SplatRun &sr)
    : sr(sr)
    {}
    
    void LoadUDT(const std::string &filename, ElevationMap &em);
};

#endif /* udt_hpp */
