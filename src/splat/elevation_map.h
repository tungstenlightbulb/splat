//
//  elevation_map.h
//  splat
//
//  Created by Peter Work Watkins on 8/12/16.
//  Copyright © 2016 ke7ist. All rights reserved.
//

#ifndef elevation_map_h
#define elevation_map_h

#include <string>
#include <vector>
#include <stdio.h>

class Path;
class SplatRun;
class Dem;
class Sdf;
class Site;
class Lrp;
class PatFile;

class ElevationMap
{
  
private:
    Path &path;
    
    const SplatRun &sr;

    
public:
    std::vector<Dem> dem;
    int min_north;
    int max_north;
    int min_west;
    int max_west;
    int max_elevation;
    int min_elevation;
    
public:
    ElevationMap(Path &path, const SplatRun &sr);
    
    void LoadTopoData(int max_lon, int min_lon, int max_lat, int min_lat, Sdf &sdf);
    
    int PutMask(double lat, double lon, int value);
    
    int OrMask(double lat, double lon, int value);
    
    int GetMask(double lat, double lon) const;
    
    double haat(const Site &antenna) const;
    
    double GetElevation(const Site &location) const;
    
    int AddElevation(double lat, double lon, double height);
    
    double ElevationAngle(const Site &source, const Site &destination) const;
    
    double ElevationAngle2(const Site &source, const Site &destination, double er) const;
    
    double AverageTerrain(const Site &source, double azimuthx, double start_distance, double end_distance) const;
    
    void PlaceMarker(const Site &location);
    
    void PlotPath(const Site &source, const Site &destination, char mask_value);
    
    void PlotLOSMap(const Site &source, double altitude);
    
    void PlotLRMap(const Site &source, double altitude, const std::string &plo_filename, double elev[], const PatFile &pat, const Lrp &lrp);
    
    int PutSignal(double lat, double lon, unsigned char signal);
    
    unsigned char GetSignal(double lat, double lon) const;
    
    ~ElevationMap();
    
private:
    void PlotLRPath(const Site &source, const Site &destination, unsigned char mask_value, FILE *fd, double elev[], const PatFile &pat, const Lrp &lrp);

    bool FindMask(double lat, double lon, int &x, int &y, int &indx) const;
};

#endif /* elevation_map_h */
