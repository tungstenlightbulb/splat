/** @file site.h
 *
 * File created by Peter Watkins (KE7IST) 8/12/16.
 * Derived from original project code.
 * Splat!
 * @copyright 1997 - 2018 John A. Magliacane (KD2BD) and contributors.
 * See revision control history for contributions.
 * This file is covered by the LICENSE.md file in the root of this project.
 */

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
