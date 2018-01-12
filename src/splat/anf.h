//
//  anf.h
//  splat
//
//  Created by Peter Work Watkins on 1/9/18.
//  Copyright © 2018 ke7ist. All rights reserved.
//

#ifndef anf_h
#define anf_h

#include <string>

class Sdf;
class Lrp;

class Anf
{
private:
    const Lrp &lrp;
    const SplatRun &sr;
    
public:
    Anf(const Lrp &lrp, const SplatRun &sr)
    :lrp(lrp),
    sr(sr)
    {}
    
    int LoadANO(const std::string &filename, Sdf &sdf, ElevationMap &em);
};

#endif /* anf_h */
