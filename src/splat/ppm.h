//
//  ppm.h
//  splat
//
//  Created by Peter Work Watkins on 1/8/18.
//  Copyright © 2018 ke7ist. All rights reserved.
//

#ifndef ppm_h
#define ppm_h

#include <string>
#include <vector>

class SplatRun;
class ElevationMap;
class Site;
class Region;

class Ppm {
private:
    const SplatRun &sr;
    const ElevationMap &em;
    std::string &filename;
    const std::vector<Site> &xmtr;

public:
    Ppm(const SplatRun &sr, std::string &filename, const std::vector<Site> &xmtr, const ElevationMap &em)
    : sr(sr),
    filename(filename),
    xmtr(xmtr),
    em(em)
    {}
    
void WritePPM();

void WritePPMLR(Region &region);

void WritePPMSS(Region &region);

void WritePPMDBM(Region &region);
    
private:
    
    void WriteKmlForPpm(const std::string &groundOverlayName, const std::string &description, bool writeScreenOverlay, const std::string &kmlfile, const std::string &mapfile, double north, double south, double east, double west, const std::string &ckfile);
    static void WriteGeo(const std::string &geofile, const std::string &mapfile, double north, double south, double east, double west, unsigned int width, unsigned int height);

};

#endif /* ppm_h */
