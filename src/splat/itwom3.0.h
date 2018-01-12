//
//  itwom3.0.h
//  splat
//
//  Created by Peter Work Watkins on 6/17/15.
//  Copyright (c) 2015 ke7ist. All rights reserved.
//

#ifndef splat_itwom3_0_h
#define splat_itwom3_0_h

double ITWOMVersion();

void point_to_point_ITM(double elev[], double tht_m, double rht_m, double eps_dielect, double sgm_conductivity, double eno_ns_surfref, double frq_mhz, int radio_climate, int pol, double conf, double rel, double &dbloss, char *strmode, int &errnum);

void point_to_point(double elev[], double tht_m, double rht_m, double eps_dielect, double sgm_conductivity, double eno_ns_surfref, double frq_mhz, int radio_climate, int pol, double conf, double rel, double &dbloss, char *strmode, int &errnum);

#endif
