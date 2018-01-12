//
//  utilities.h
//  splat
//
//  Created by Peter Work Watkins on 8/12/16.
//  Copyright © 2016 ke7ist. All rights reserved.
//

#ifndef utilities_h
#define utilities_h

#include <string>

#ifndef PI
#define PI 3.141592653589793
#endif

#define GAMMA 2.5

#ifndef PI
#define PI 3.141592653589793
#endif

#ifndef TWOPI
#define TWOPI 6.283185307179586
#endif

#ifndef HALFPI
#define HALFPI 1.570796326794896
#endif

#define DEG2RAD 1.74532925199e-02

class Utilities
{
public:
    static int interpolate(int y0, int y1, int x0, int x1, int n);

    static double arccos(double x, double y);

    static int ReduceAngle(double angle);

    static double LonDiff(double lon1, double lon2);

    static std::string dec2dms(double decimal);
    
    static double ReadBearing(const std::string &input);
    
    static std::string PathLeaf(const std::string &path);
    
    static std::string Extension(const std::string &path);
    
    static std::string Basename(const std::string &path);
    
    static std::string::size_type ExtensionIdx(const std::string &path);
    
    static std::string DivideExtension(std::string &path, const std::string &default_extension);
    
    static void Chomp(std::string &str);
};

#endif /* utilities_h */
