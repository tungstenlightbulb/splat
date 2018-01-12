//
//  pat_file.cpp
//  splat
//
//  Created by Peter Work Watkins on 1/7/18.
//  Copyright © 2018 ke7ist. All rights reserved.
//

#include <string>
#include <cmath>
#include "site.h"
#include "splat_run.h"
#include "pat_file.h"

using namespace std;



void PatFile::LoadPAT(const string &filename)
{
    /* This function reads and processes antenna pattern (.az
     and .el) files that correspond in name to previously
     loaded SPLAT! .lrp files.  */
    
    int	a, b, w, x, y, z, last_index, next_index, span;
    char	string[255], azfile[255], elfile[255], *pointer=NULL;
    float	az, xx, elevation, amplitude, rotation, valid1, valid2,
    delta, azimuth[361], azimuth_pattern[361], el_pattern[10001],
    elevation_pattern[361][1001], slant_angle[361], tilt,
    mechanical_tilt=0.0, tilt_azimuth, tilt_increment, sum;
    FILE	*fd=NULL;
    unsigned char read_count[10001];
    
    for (x=0; filename[x]!='.' && filename[x]!=0 && x<250; x++)
    {
        azfile[x]=filename[x];
        elfile[x]=filename[x];
    }
    
    azfile[x]='.';
    azfile[x+1]='a';
    azfile[x+2]='z';
    azfile[x+3]=0;
    
    elfile[x]='.';
    elfile[x+1]='e';
    elfile[x+2]='l';
    elfile[x+3]=0;
    
    rotation=0.0;
    
    got_azimuth_pattern=false;
    got_elevation_pattern=false;
    
    /* Load .az antenna pattern file */
    
    fd=fopen(azfile,"r");
    
    if (fd!=NULL)
    {
        /* Clear azimuth pattern array */
        
        for (x=0; x<=360; x++)
        {
            azimuth[x]=0.0;
            read_count[x]=0;
        }
        
        
        /* Read azimuth pattern rotation
         in degrees measured clockwise
         from true North. */
        
        fgets(string,254,fd);
        pointer=strchr(string,';');
        
        if (pointer!=NULL)
            *pointer=0;
        
        sscanf(string,"%f",&rotation);
        
        
        /* Read azimuth (degrees) and corresponding
         normalized field radiation pattern amplitude
         (0.0 to 1.0) until EOF is reached. */
        
        fgets(string,254,fd);
        pointer=strchr(string,';');
        
        if (pointer!=NULL)
            *pointer=0;
        
        sscanf(string,"%f %f",&az, &amplitude);
        
        do
        {
            x=(int)rintf(az);
            
            if (x>=0 && x<=360 && fd!=NULL)
            {
                azimuth[x]+=amplitude;
                read_count[x]++;
            }
            
            fgets(string,254,fd);
            pointer=strchr(string,';');
            
            if (pointer!=NULL)
                *pointer=0;
            
            sscanf(string,"%f %f",&az, &amplitude);
            
        } while (feof(fd)==0);
        
        fclose(fd);
        
        
        /* Handle 0=360 degree ambiguity */
        
        if ((read_count[0]==0) && (read_count[360]!=0))
        {
            read_count[0]=read_count[360];
            azimuth[0]=azimuth[360];
        }
        
        if ((read_count[0]!=0) && (read_count[360]==0))
        {
            read_count[360]=read_count[0];
            azimuth[360]=azimuth[0];
        }
        
        /* Average pattern values in case more than
         one was read for each degree of azimuth. */
        
        for (x=0; x<=360; x++)
        {
            if (read_count[x]>1)
                azimuth[x]/=(float)read_count[x];
        }
        
        /* Interpolate missing azimuths
         to completely fill the array */
        
        last_index=-1;
        next_index=-1;
        
        for (x=0; x<=360; x++)
        {
            if (read_count[x]!=0)
            {
                if (last_index==-1)
                    last_index=x;
                else
                    next_index=x;
            }
            
            if (last_index!=-1 && next_index!=-1)
            {
                valid1=azimuth[last_index];
                valid2=azimuth[next_index];
                
                span=next_index-last_index;
                delta=(valid2-valid1)/(float)span;
                
                for (y=last_index+1; y<next_index; y++)
                    azimuth[y]=azimuth[y-1]+delta;
                
                last_index=y;
                next_index=-1;
            }
        }
        
        /* Perform azimuth pattern rotation
         and load azimuth_pattern[361] with
         azimuth pattern data in its final form. */
        
        for (x=0; x<360; x++)
        {
            y=x+(int)rintf(rotation);
            
            if (y>=360)
                y-=360;
            
            azimuth_pattern[y]=azimuth[x];
        }
        
        azimuth_pattern[360]=azimuth_pattern[0];
        
        got_azimuth_pattern=true;
    }
    
    /* Read and process .el file */
    
    fd=fopen(elfile,"r");
    
    if (fd!=NULL)
    {
        for (x=0; x<=10000; x++)
        {
            el_pattern[x]=0.0;
            read_count[x]=0;
        }
        
        /* Read mechanical tilt (degrees) and
         tilt azimuth in degrees measured
         clockwise from true North. */
        
        fgets(string,254,fd);
        pointer=strchr(string,';');
        
        if (pointer!=NULL)
            *pointer=0;
        
        sscanf(string,"%f %f",&mechanical_tilt, &tilt_azimuth);
        
        /* Read elevation (degrees) and corresponding
         normalized field radiation pattern amplitude
         (0.0 to 1.0) until EOF is reached. */
        
        fgets(string,254,fd);
        pointer=strchr(string,';');
        
        if (pointer!=NULL)
            *pointer=0;
        
        sscanf(string,"%f %f", &elevation, &amplitude);
        
        while (feof(fd)==0)
        {
            /* Read in normalized radiated field values
             for every 0.01 degrees of elevation between
             -10.0 and +90.0 degrees */
            
            x=(int)rintf(100.0*(elevation+10.0));
            
            if (x>=0 && x<=10000)
            {
                el_pattern[x]+=amplitude;
                read_count[x]++;
            }
            
            fgets(string,254,fd);
            pointer=strchr(string,';');
            
            if (pointer!=NULL)
                *pointer=0;
            
            sscanf(string,"%f %f", &elevation, &amplitude);
        }
        
        fclose(fd);
        
        /* Average the field values in case more than
         one was read for each 0.01 degrees of elevation. */
        
        for (x=0; x<=10000; x++)
        {
            if (read_count[x]>1)
                el_pattern[x]/=(float)read_count[x];
        }
        
        /* Interpolate between missing elevations (if
         any) to completely fill the array and provide
         radiated field values for every 0.01 degrees of
         elevation. */
        
        last_index=-1;
        next_index=-1;
        
        for (x=0; x<=10000; x++)
        {
            if (read_count[x]!=0)
            {
                if (last_index==-1)
                    last_index=x;
                else
                    next_index=x;
            }
            
            if (last_index!=-1 && next_index!=-1)
            {
                valid1=el_pattern[last_index];
                valid2=el_pattern[next_index];
                
                span=next_index-last_index;
                delta=(valid2-valid1)/(float)span;
                
                for (y=last_index+1; y<next_index; y++)
                    el_pattern[y]=el_pattern[y-1]+delta;
                
                last_index=y;
                next_index=-1;
            }
        }
        
        /* Fill slant_angle[] array with offset angles based
         on the antenna's mechanical beam tilt (if any)
         and tilt direction (azimuth). */
        
        if (mechanical_tilt==0.0)
        {
            for (x=0; x<=360; x++)
                slant_angle[x]=0.0;
        }
        
        else
        {
            tilt_increment=mechanical_tilt/90.0;
            
            for (x=0; x<=360; x++)
            {
                xx=(float)x;
                y=(int)rintf(tilt_azimuth+xx);
                
                while (y>=360)
                    y-=360;
                
                while (y<0)
                    y+=360;
                
                if (x<=180)
                    slant_angle[y]=-(tilt_increment*(90.0-xx));
                
                if (x>180)
                    slant_angle[y]=-(tilt_increment*(xx-270.0));
            }
        }
        
        slant_angle[360]=slant_angle[0];   /* 360 degree wrap-around */
        
        for (w=0; w<=360; w++)
        {
            tilt=slant_angle[w];
            
            /** Convert tilt angle to
             an array index offset **/
            
            y=(int)rintf(100.0*tilt);
            
            /* Copy shifted el_pattern[10001] field
             values into elevation_pattern[361][1001]
             at the corresponding azimuth, downsampling
             (averaging) along the way in chunks of 10. */
            
            for (x=y, z=0; z<=1000; x+=10, z++)
            {
                for (sum=0.0, a=0; a<10; a++)
                {
                    b=a+x;
                    
                    if (b>=0 && b<=10000)
                        sum+=el_pattern[b];
                    if (b<0)
                        sum+=el_pattern[0];
                    if (b>10000)
                        sum+=el_pattern[10000];
                }
                
                elevation_pattern[w][z]=sum/10.0;
            }
        }
        
        got_elevation_pattern=true;
    }
    
    for (x=0; x<=360; x++)
    {
        for (y=0; y<=1000; y++)
        {
            if (got_elevation_pattern)
                elevation=elevation_pattern[x][y];
            else
                elevation=1.0;
            
            if (got_azimuth_pattern)
                az=azimuth_pattern[x];
            else
                az=1.0;
            
            antenna_pattern[x][y]=az*elevation;
        }
    }
}
