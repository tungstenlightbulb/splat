//
//  lrp.h
//  splat
//
//  Created by Peter Work Watkins on 1/8/18.
//  Copyright © 2018 ke7ist. All rights reserved.
//

#ifndef lrp_h
#define lrp_h

#include <stdio.h>

class Lrp
{
private:
     double forced_erp;
    double forced_freq;
    
public:
    Lrp(double forced_erp, double forced_freq)
    :forced_erp(forced_erp),
    forced_freq(forced_freq)
    {}
    
    double eps_dielect;
        double sgm_conductivity;
        double eno_ns_surfref;
        double frq_mhz;
        double conf;
        double rel;
        double erp;
        int radio_climate;
        int pol;
    
    char ReadLRParm(const Site &txsite, char forced_read, bool &loadPat, std::string &filename);
};

#endif /* lrp_h */
