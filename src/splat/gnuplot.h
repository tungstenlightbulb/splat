//
//  gnuplot.h
//  splat
//
//  Created by Peter Work Watkins on 1/8/18.
//  Copyright © 2018 ke7ist. All rights reserved.
//

#ifndef gnuplot_h
#define gnuplot_h

#include <stdio.h>

class GnuPlot
{
private:
    const SplatRun &sr;
    
    Path &path;
    
public:
    GnuPlot(Path &path, const SplatRun &sr)
    : path(path),
    sr(sr)
    {}
    
void GraphTerrain(const Site &source, const Site &destination, const std::string &name, const ElevationMap &em);

void GraphElevation(const Site &source, const Site &destination, const std::string &name, const ElevationMap &em);

void GraphHeight(const Site &source, const Site &destination, const std::string &name, bool fresnel_plot, bool normalized, const ElevationMap &em, const Lrp &lrp);
};

#endif /* gnuplot_h */
