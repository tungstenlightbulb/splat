//
//  site.h
//  splat
//
//  Created by Peter Work Watkins on 8/12/16.
//  Copyright © 2016 ke7ist. All rights reserved.
//

#ifndef site_h
#define site_h

#include <string>

/**
 Represents a site (a location having a lattitude, longitude, and altitude).
 */
class Site
{
public:
    double lat;
    double lon;
    float alt;
    std::string name;
    std::string filename;
    
public:
    Site();
    Site(const std::string &filename);
    
    double Distance(const Site &site2) const;
    
    double Azimuth(const Site &destination) const;
    
    /**
     Populates the Site by loading a QTH file.

     @param filename The path to the QTH file.
     */
    void LoadQTH(const std::string &filename);
};


#endif /* site_h */
