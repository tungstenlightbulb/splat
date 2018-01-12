//
//  city_file.h
//  splat
//
//  Created by Peter Work Watkins on 1/9/18.
//  Copyright © 2018 ke7ist. All rights reserved.
//

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
