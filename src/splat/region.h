//
//  region.h
//  splat
//
//  Created by Peter Work Watkins on 1/8/18.
//  Copyright © 2018 ke7ist. All rights reserved.
//

#ifndef region_h
#define region_h

class Site;

class Region
{
public:
    unsigned char color[32][3];
    int level[32];
    int levels;
    
    
    
    void LoadLossColors(const Site &xmtr);
    void LoadDBMColors(const Site &xmtr);
    void LoadSignalColors(const Site &xmtr);
};

#endif /* region_h */
