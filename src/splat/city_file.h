/** @file city_file.h
 *
 * File created by Peter Watkins (KE7IST) 1/9/18.
 * Derived from original project code.
 * Splat!
 * @copyright 1997 - 2018 John A. Magliacane (KD2BD) and contributors.
 * See revision control history for contributions.
 * This file is covered by the LICENSE.md file in the root of this project.
 */

#ifndef city_file_h
#define city_file_h

#include <string>

class ElevationMap;

class CityFile
{
public:
        void LoadCities(const std::string &filename, ElevationMap &em);
};

#endif /* city_file_hpp */
