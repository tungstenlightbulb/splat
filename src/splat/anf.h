/** @file anf.h
 *
 * File created by Peter Watkins (KE7IST) 1/9/18.
 * Derived from original project code.
 * Splat!
 * @copyright 1997 - 2018 John A. Magliacane (KD2BD) and contributors.
 * See revision control history for contributions.
 * This file is covered by the LICENSE.md file in the root of this project.
 */

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
