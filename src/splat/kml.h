//
//  kml.hpp
//  splat
//
//  Created by Peter Work Watkins on 1/9/18.
//  Copyright © 2018 ke7ist. All rights reserved.
//

#ifndef kml_h
#define kml_h

#include <stdio.h>

#include "elevation_map.h"
#include "splat_run.h"

class Kml
{
private:
    Path &path;
    const ElevationMap &em;
    const SplatRun &sr;
public:
    Kml(Path &path, const ElevationMap &em, const SplatRun &sr)
    : path(path),
    em(em),
    sr(sr)
    {}
    
        void WriteKML(const Site &source, const Site &destination);
};

#endif /* kml_h */
