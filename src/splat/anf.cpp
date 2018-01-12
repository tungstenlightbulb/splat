//
//  anf.cpp
//  splat
//
//  Created by Peter Work Watkins on 1/9/18.
//  Copyright © 2018 ke7ist. All rights reserved.
//

#include <string>
#include <cstring>
#include <cmath>
#include "elevation_map.h"
#include "anf.h"
#include "sdf.h"
#include "lrp.h"
#include "splat_run.h"

using namespace std;

int Anf::LoadANO(const string &filename, Sdf &sdf, ElevationMap &em)
{
    /* This function reads a SPLAT! alphanumeric output
     file (-ani option) for analysis and/or map generation. */
    
    int	error=0, max_west, min_west, max_north, min_north;
    char	string[80], *pointer=NULL;
    double	latitude=0.0, longitude=0.0, azimuth=0.0, elevation=0.0,
    ano=0.0;
    FILE	*fd;
    
    fd=fopen(filename.c_str(),"r");
    
    if (fd!=NULL)
    {
        fgets(string,78,fd);
        pointer=strchr(string,';');
        
        if (pointer!=NULL)
            *pointer=0;
        
        sscanf(string,"%d, %d",&max_west, &min_west);
        
        fgets(string,78,fd);
        pointer=strchr(string,';');
        
        if (pointer!=NULL)
            *pointer=0;
        
        sscanf(string,"%d, %d",&max_north, &min_north);
        
        fgets(string,78,fd);
        pointer=strchr(string,';');
        
        if (pointer!=NULL)
            *pointer=0;
        
        em.LoadTopoData(max_west-1, min_west, max_north-1, min_north, sdf);
        
        fprintf(stdout,"\nReading \"%s\"... ",filename.c_str());
        fflush(stdout);
        
        fgets(string,78,fd);
        sscanf(string,"%lf, %lf, %lf, %lf, %lf",&latitude, &longitude, &azimuth, &elevation, &ano);
        
        while (feof(fd)==0)
        {
            if (lrp.erp==0.0)
            {
                /* Path loss */
                
                if (sr.contour_threshold==0 || (fabs(ano)<=(double)sr.contour_threshold))
                {
                    ano=fabs(ano);
                    
                    if (ano>255.0)
                        ano=255.0;
                    
                    em.PutSignal(latitude,longitude,((unsigned char)round(ano)));
                }
            }
            
            if (lrp.erp!=0.0 && sr.dbm)
            {
                /* signal power level in dBm */
                
                if (sr.contour_threshold==0 || (ano>=(double)sr.contour_threshold))
                {
                    ano=200.0+rint(ano);
                    
                    if (ano<0.0)
                        ano=0.0;
                    
                    if (ano>255.0)
                        ano=255.0;
                    
                    em.PutSignal(latitude,longitude,((unsigned char)round(ano)));
                }
            }
            
            if (lrp.erp!=0.0 && !sr.dbm)
            {
                /* field strength dBuV/m */
                
                if (sr.contour_threshold==0 || (ano>=(double)sr.contour_threshold))
                {
                    ano=100.0+rint(ano);
                    
                    if (ano<0.0)
                        ano=0.0;
                    
                    if (ano>255.0)
                        ano=255.0;
                    
                    em.PutSignal(latitude,longitude,((unsigned char)round(ano)));
                }
            }
            
            fgets(string,78,fd);
            sscanf(string,"%lf, %lf, %lf, %lf, %lf",&latitude, &longitude, &azimuth, &elevation, &ano);
        }
        
        fclose(fd);
        
        fprintf(stdout," Done!\n");
        fflush(stdout);
    }
    
    else
        error=1;
    
    return error;
}



