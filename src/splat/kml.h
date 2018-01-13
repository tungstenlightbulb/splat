/** @file kml.h
 *
 * File created by Peter Watkins (KE7IST) 1/9/18.
 * Derived from original project code.
 * Splat!
 * @copyright 1997 - 2018 John A. Magliacane (KD2BD) and contributors.
 * See revision control history for contributions.
 * This file is covered by the LICENSE.md file in the root of this project.
 */

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
