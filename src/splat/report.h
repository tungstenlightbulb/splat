//
//  report.h
//  splat
//
//  Created by Peter Work Watkins on 1/8/18.
//  Copyright © 2018 ke7ist. All rights reserved.
//

#ifndef report_h
#define report_h

#include <stdio.h>

class Report
{
private:
    const std::string dashes;
    
    const ElevationMap &em;
    const SplatRun &sr;
    Path &path;
    
public:
    Report(const ElevationMap &em, const SplatRun &sr, Path &path)
    : em(em), sr(sr), path(path),
    dashes("---------------------------------------------------------------------------")
    {}
    
    void PathReport(const Site &source, const Site &destination, const std::string &name, bool graph_it, double elev[], const PatFile &pat, const Lrp &lrp);
    
    void SiteReport(const Site &xmtr);
    
private:
    void ObstructionAnalysis(const Site &xmtr, const Site &rcvr, double f, FILE *outfile);
};

#endif /* report_h */
