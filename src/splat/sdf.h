//
//  sdf.h
//  splat
//
//  Created by Peter Work Watkins on 1/8/18.
//  Copyright © 2018 ke7ist. All rights reserved.
//

#ifndef sdf_h
#define sdf_h

#include <string>
#include <bzlib.h>

class ElevationMap;
class SplatRun;

class Sdf
{
private:
    std::string sdf_path;
    int	bzerror;
    const SplatRun &sr;
    
public:
    Sdf(const std::string &path, const SplatRun &sr)
    :sdf_path(path),
    sr(sr)
    {}
int LoadSDF_SDF(const std::string &name, ElevationMap &em);

int LoadSDF_BZ(const std::string &name, ElevationMap &em);

char LoadSDF(const std::string &name, ElevationMap &em);
    
    
private:
    char *BZfgets(BZFILE *bzfd, unsigned length);
};

#endif /* sdf_h */
