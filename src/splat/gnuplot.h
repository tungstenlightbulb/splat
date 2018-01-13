/** @file gnuplot.h
 *
 * File created by Peter Watkins (KE7IST) 1/8/18.
 * Derived from original project code.
 * Splat!
 * @copyright 1997 - 2018 John A. Magliacane (KD2BD) and contributors.
 * See revision control history for contributions.
 * This file is covered by the LICENSE.md file in the root of this project.
 */

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
