//
//  main.cpp
//  splat
//
//  Created by Peter Work Watkins on 6/17/15.
//  Copyright (c) 2015 ke7ist. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <vector>
#include "path.h"
#include "dem.h"
#include "splat.h"
#include "itwom3.0.h"

int main(int argc, const char * argv[])
{
    bool hd_mode = false;
    
    int		x, y, z=0, min_lat, min_lon, max_lat, max_lon,
    rxlat, rxlon, txlat, txlon, west_min, west_max,
    north_min, north_max;
    
    unsigned char	coverage=0, LRmap=0, terrain_plot=0,
    elevation_plot=0, height_plot=0, map=0,
    longley_plot=0, cities=0, bfs=0, txsites=0,
    norm=0, topomap=0, geo=0, kml=0, pt2pt_mode=0,
    area_mode=0, max_txsites, ngs=0, nolospath=0,
    nositereports=0, fresnel_plot=1, command_line_log=0;
    
    char		mapfile[255], header[80], city_file[5][255],
    elevation_file[255], height_file[255],
    longley_file[255], terrain_file[255],
    string[255], rxfile[255], *env=NULL,
    txfile[255], boundary_file[5][255],
    udt_file[255], rxsite=0, ani_filename[255],
    ano_filename[255], ext[20], logfile[255], maxpages_str[255];
    
    double		altitude=0.0, altitudeLR=0.0, tx_range=0.0,
    rx_range=0.0, deg_range=0.0, deg_limit=0.0,
    deg_range_lon, er_mult;
    
    struct		site tx_site[32], rx_site;
    
    FILE		*fd;
    
    strncpy(splat_version,"1.4.2\0",6);
    
    strncpy(splat_name,"SPLAT!\0",7);
    
    strncpy(dashes,"---------------------------------------------------------------------------\0",76);
    
    if (argc==1)
    {
        fprintf(stdout,"\n\t\t --==[ %s v%s Available Options... ]==--\n\n",splat_name, splat_version);
        
        fprintf(stdout,"       -t txsite(s).qth (max of 4 with -c, max of 30 with -L)\n");
        fprintf(stdout,"       -r rxsite.qth\n");
        fprintf(stdout,"       -c plot LOS coverage of TX(s) with an RX antenna at X feet/meters AGL\n");
        fprintf(stdout,"       -L plot path loss map of TX based on an RX at X feet/meters AGL\n");
        fprintf(stdout,"       -s filename(s) of city/site file(s) to import (5 max)\n");
        fprintf(stdout,"       -b filename(s) of cartographic boundary file(s) to import (5 max)\n");
        fprintf(stdout,"       -p filename of terrain profile graph to plot\n");
        fprintf(stdout,"       -e filename of terrain elevation graph to plot\n");
        fprintf(stdout,"       -h filename of terrain height graph to plot\n");
        fprintf(stdout,"       -H filename of normalized terrain height graph to plot\n");
        fprintf(stdout,"       -l filename of path loss graph to plot\n");
        fprintf(stdout,"       -o filename of topographic map to generate (.ppm)\n");
        fprintf(stdout,"       -u filename of user-defined terrain file to import\n");
        fprintf(stdout,"       -d sdf file directory path (overrides path in ~/.splat_path file)\n");
        fprintf(stdout,"       -m earth radius multiplier\n");
        fprintf(stdout,"       -n do not plot LOS paths in .ppm maps\n");
        fprintf(stdout,"       -N do not produce unnecessary site or obstruction reports\n");
        fprintf(stdout,"       -f frequency for Fresnel zone calculation (MHz)\n");
        fprintf(stdout,"       -R modify default range for -c or -L (miles/kilometers)\n");
        fprintf(stdout,"      -sc display smooth rather than quantized contour levels\n");
        fprintf(stdout,"      -db threshold beyond which contours will not be displayed\n");
        fprintf(stdout,"      -nf do not plot Fresnel zones in height plots\n");
        fprintf(stdout,"      -fz Fresnel zone clearance percentage (default = 60)\n");
        fprintf(stdout,"      -gc ground clutter height (feet/meters)\n");
        fprintf(stdout,"     -ngs display greyscale topography as white in .ppm files\n");
        fprintf(stdout,"     -erp override ERP in .lrp file (Watts)\n");
        fprintf(stdout,"     -ano name of alphanumeric output file\n");
        fprintf(stdout,"     -ani name of alphanumeric input file\n");
        fprintf(stdout,"     -udt name of user defined terrain input file\n");
        fprintf(stdout,"     -kml generate Google Earth (.kml) compatible output\n");
        fprintf(stdout,"     -geo generate an Xastir .geo georeference file (with .ppm output)\n");
        fprintf(stdout,"     -dbm plot signal power level contours rather than field strength\n");
        fprintf(stdout,"     -log copy command line string to this output file\n");
        fprintf(stdout,"   -gpsav preserve gnuplot temporary working files after SPLAT! execution\n");
        fprintf(stdout,"  -metric employ metric rather than imperial units for all user I/O\n");
        fprintf(stdout,"  -olditm invoke Longley-Rice rather than the default ITWOM model\n");
        fprintf(stdout,"-maxpages [%d] Maximum Analysis Region capability: 1, 4, 9, 16, 25, 36, 49, 64 \n", maxpages);
        fprintf(stdout,"      -hd Enable HD mode.");
        fprintf(stdout,"\n");
        fprintf(stdout,"If that flew by too fast, consider piping the output through 'less':\n");
        
        fprintf(stdout,"\n\tsplat | less\n\n");
        
        fprintf(stdout,"Type 'man splat', or see the documentation for more details.\n\n");
        fflush(stdout);
        
        return 1;
    }
    
    y=argc-1;
    
    olditm=0;
    kml=0;
    geo=0;
    dbm=0;
    gpsav=0;
    metric=0;
    rxfile[0]=0;
    txfile[0]=0;
    string[0]=0;
    mapfile[0]=0;
    clutter=0.0;
    forced_erp=-1.0;
    forced_freq=0.0;
    elevation_file[0]=0;
    terrain_file[0]=0;
    sdf_path[0]=0;
    udt_file[0]=0;
    max_txsites=30;
    fzone_clearance=0.6;
    contour_threshold=0;
    rx_site.lat=91.0;
    rx_site.lon=361.0;
    longley_file[0]=0;
    ano_filename[0]=0;
    ani_filename[0]=0;
    smooth_contours=0;
    earthradius=EARTHRADIUS;
    

    
    sprintf(header,"\n\t\t--==[ Welcome To %s v%s ]==--\n\n", splat_name, splat_version);
    
    for (x=0; x<4; x++)
    {
        tx_site[x].lat=91.0;
        tx_site[x].lon=361.0;
    }
    
    /* Scan for command line arguments */
    
    for (x=1; x<=y; x++)
    {
        if (strcmp(argv[x],"-R")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                sscanf(argv[z],"%lf",&max_range);
                
                if (max_range<0.0)
                    max_range=0.0;
                
                if (max_range>1000.0)
                    max_range=1000.0;
            }
        }
        
        if (strcmp(argv[x],"-m")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                sscanf(argv[z],"%lf",&er_mult);
                
                if (er_mult<0.1)
                    er_mult=1.0;
                
                if (er_mult>1.0e6)
                    er_mult=1.0e6;
                
                earthradius*=er_mult;
            }
        }
        
        if (strcmp(argv[x],"-gc")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                sscanf(argv[z],"%lf",&clutter);
                
                if (clutter<0.0)
                    clutter=0.0;
            }
        }
        
        if (strcmp(argv[x],"-fz")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                sscanf(argv[z],"%lf",&fzone_clearance);
                
                if (fzone_clearance<0.0 || fzone_clearance>100.0)
                    fzone_clearance=60.0;
                
                fzone_clearance/=100.0;
            }
        }
        
        if (strcmp(argv[x],"-o")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
                strncpy(mapfile,argv[z],253);
            map=1;
        }
        
        if (strcmp(argv[x],"-log")==0)
        {
            z=x+1;
            
            logfile[0]=0;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
                strncpy(logfile,argv[z],253);
            
            command_line_log=1;
        }
        
        if (strcmp(argv[x],"-udt")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
                strncpy(udt_file,argv[z],253);
        }
        
        if (strcmp(argv[x],"-c")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                sscanf(argv[z],"%lf",&altitude);
                map=1;
                coverage=1;
                area_mode=1;
                max_txsites=4;
            }
        }
        
        if (strcmp(argv[x],"-db")==0 || strcmp(argv[x],"-dB")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0]) /* A minus argument is legal here */
                sscanf(argv[z],"%d",&contour_threshold);
        }
        
        if (strcmp(argv[x],"-p")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                strncpy(terrain_file,argv[z],253);
                terrain_plot=1;
                pt2pt_mode=1;
            }
        }
        
        if (strcmp(argv[x],"-e")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                strncpy(elevation_file,argv[z],253);
                elevation_plot=1;
                pt2pt_mode=1;
            }
        }
        
        if (strcmp(argv[x],"-h")==0 || strcmp(argv[x],"-H")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                strncpy(height_file,argv[z],253);
                height_plot=1;
                pt2pt_mode=1;
            }
            
            if (strcmp(argv[x],"-H")==0)
                norm=1;
            else
                norm=0;
        }
        
        if (strcmp(argv[x],"-metric")==0)
            metric=1;
        
        if (strcmp(argv[x],"-gpsav")==0)
            gpsav=1;
        
        if (strcmp(argv[x],"-geo")==0)
            geo=1;
        
        if (strcmp(argv[x],"-kml")==0)
            kml=1;
        
        if (strcmp(argv[x],"-nf")==0)
            fresnel_plot=0;
        
        if (strcmp(argv[x],"-ngs")==0)
            ngs=1;
        
        if (strcmp(argv[x],"-n")==0)
            nolospath=1;
        
        if (strcmp(argv[x],"-dbm")==0)
            dbm=1;
        
        if (strcmp(argv[x],"-sc")==0)
            smooth_contours=1;
        
        if (strcmp(argv[x],"-olditm")==0)
            olditm=1;
        
        if (strcmp(argv[x],"-N")==0)
        {
            nolospath=1;
            nositereports=1;
        }
        
        if (strcmp(argv[x],"-d")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
                strncpy(sdf_path,argv[z],253);
        }
        
        if (strcmp(argv[x],"-t")==0)
        {
            /* Read Transmitter Location */
            
            z=x+1;
            
            while (z<=y && argv[z][0] && argv[z][0]!='-' && txsites<30)
            {
                strncpy(txfile,argv[z],253);
                tx_site[txsites]=LoadQTH(txfile);
                txsites++;
                z++;
            }
            
            z--;
        }
        
        if (strcmp(argv[x],"-L")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                sscanf(argv[z],"%lf",&altitudeLR);
                map=1;
                LRmap=1;
                area_mode=1;
                
                if (coverage)
                    fprintf(stdout,"c and L are exclusive options, ignoring L.\n");
            }
        }
        
        if (strcmp(argv[x],"-l")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                strncpy(longley_file,argv[z],253);
                longley_plot=1;
                pt2pt_mode=1;
            }
        }
        
        if (strcmp(argv[x],"-r")==0)
        {
            /* Read Receiver Location */
            
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                strncpy(rxfile,argv[z],253);
                rx_site=LoadQTH(rxfile);
                rxsite=1;
                pt2pt_mode=1;
            }
        }
        
        if (strcmp(argv[x],"-s")==0)
        {
            /* Read city file(s) */
            
            z=x+1;
            
            while (z<=y && argv[z][0] && argv[z][0]!='-' && cities<5)
            {
                strncpy(city_file[cities],argv[z],253);
                cities++;
                z++;
            }
            
            z--;
        }
        
        if (strcmp(argv[x],"-b")==0)
        {
            /* Read Boundary File(s) */
            
            z=x+1;
            
            while (z<=y && argv[z][0] && argv[z][0]!='-' && bfs<5)
            {
                strncpy(boundary_file[bfs],argv[z],253);
                bfs++;
                z++;
            }
            
            z--;
        }
        
        if (strcmp(argv[x],"-f")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                sscanf(argv[z],"%lf",&forced_freq);
                
                if (forced_freq<20.0)
                    forced_freq=0.0;
                
                if (forced_freq>20.0e3)
                    forced_freq=20.0e3;
            }
        }
        
        if (strcmp(argv[x],"-erp")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                sscanf(argv[z],"%lf",&forced_erp);
                
                if (forced_erp<0.0)
                    forced_erp=-1.0;
            }
        }
        
        if (strcmp(argv[x],"-ano")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
                strncpy(ano_filename,argv[z],253);
        }
        
        if (strcmp(argv[x],"-ani")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
                strncpy(ani_filename,argv[z],253);
        }
        
        if (strcmp(argv[x],"-maxpages")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                strncpy(maxpages_str,argv[z],sizeof(maxpages_str));
                maxpages_str[sizeof(maxpages_str) - 1] = '\0';
                if (sscanf(maxpages_str, "%d", &maxpages) != 1)
                {
                    fprintf(stderr,"\n%c*** ERROR: Could not parse maxpages: %s\n\n", 7, maxpages_str);
                    exit (-1);
                }
            }
        }
        
        if (strcmp(argv[x],"-hd")==0)
        {
            hd_mode = true;
        }
    }
    
    /* Perform some error checking on the arguments
     and switches parsed from the command-line.
     If an error is encountered, print a message
     and exit gracefully. */
    
    if (txsites==0)
    {
        fprintf(stderr,"\n%c*** ERROR: No transmitter site(s) specified!\n\n",7);
        exit (-1);
    }
    
    for (x=0, y=0; x<txsites; x++)
    {
        if (tx_site[x].lat==91.0 && tx_site[x].lon==361.0)
        {
            fprintf(stderr,"\n*** ERROR: Transmitter site #%d not found!",x+1);
            y++;
        }
    }
    
    if (y)
    {
        fprintf(stderr,"%c\n\n",7);
        exit (-1);
    }
    
    if ((coverage+LRmap+ani_filename[0])==0 && rx_site.lat==91.0 && rx_site.lon==361.0)
    {
        if (max_range!=0.0 && txsites!=0)
        {
            /* Plot topographic map of radius "max_range" */
            
            map=0;
            topomap=1;
        }
        
        else
        {
            fprintf(stderr,"\n%c*** ERROR: No receiver site found or specified!\n\n",7);
            exit (-1);
        }
    }
    
    switch (maxpages)
    {
        case 1:
            if (!hd_mode)
            {
                fprintf(stderr,"\n%c*** ERROR: -maxpages must be >= 4 if not in HD mode!\n\n",7);
                exit (-1);
            }
            arraysize = 5092;
            break;
        case 4:
            arraysize = hd_mode ? 14844 : 4950;
            break;

        case 9:
            arraysize = hd_mode ? 32600 : 10870;
            break;

        case 16:
            arraysize = hd_mode ? 57713 : 19240;
            break;

        case 25:
            arraysize = hd_mode ? 90072 : 30025;
            break;

        case 36:
            arraysize = hd_mode ? 129650 : 43217;
            break;

        case 49:
            arraysize = hd_mode ? 176437 : 58813;
            break;

        case 64:
            arraysize = hd_mode ? 230430 : 76810;
            break;
        default:
            fprintf(stderr,"\n%c*** ERROR: -maxpages must be one of 1, 4, 9, 16, 25, 36, 49, 64\n\n",7);
            exit (-1);
    }
    
    ippd = hd_mode ? 3600 : 1200; /* pixels per degree (integer) */
    
    std::vector<Dem> dem(maxpages, Dem(ippd));
    
    double *elev = new double[arraysize+10];
    if (elev == NULL)
    {
        fprintf(stderr,"\n%c*** ERROR: Could not allocate memory for elev with -maxpages == %d\n\n",7, maxpages);
        exit (-1);
    }
    
    path = new Path(arraysize);
    if (path == NULL)
    {
        fprintf(stderr,"\n%c*** ERROR: Could not allocate memory for path with -maxpages == %d\n\n",7, maxpages);
        exit (-1);
    }
    path->length=0;
    
    LR = new struct LR();
    if (LR == NULL)
    {
        fprintf(stderr,"\n%c*** ERROR: Could not allocate memory for LR with -maxpages == %d\n\n",7, maxpages);
        exit (-1);
    }
    
    int degrees=(int)sqrt((int)maxpages);
    
    fprintf(stdout,"This invocation of %s supports analysis over a region of %d square\n",splat_name,degrees);
    
    if (degrees==1)
        fprintf(stdout,"degree");
    else
        fprintf(stdout,"degrees");
    
    fprintf(stdout," of terrain, and computes signal levels using ITWOM Version %.1f.\n\n",ITWOMVersion());

    
    for (x=0; x < maxpages; x++)
    {
        dem[x].min_el=32768;
        dem[x].max_el=-32768;
        dem[x].min_north=90;
        dem[x].max_north=-90;
        dem[x].min_west=360;
        dem[x].max_west=-1;
    }
    
    ppd=(double)ippd;	/* pixels per degree (double)  */
    dpp=1.0/ppd;		/* degrees per pixel */
    mpi=ippd-1;		/* maximum pixel index per degree */
    
    /* No major errors were detected.  Whew!  :-) */
    
    /* Adjust input parameters if -metric option is used */
    
    if (metric)
    {
        altitudeLR/=METERS_PER_FOOT;	/* meters --> feet */
        max_range/=KM_PER_MILE;		/* kilometers --> miles */
        altitude/=METERS_PER_FOOT;	/* meters --> feet */
        clutter/=METERS_PER_FOOT;	/* meters --> feet */
    }
    
    /* If no SDF path was specified on the command line (-d), check
     for a path specified in the $HOME/.splat_path file.  If the
     file is not found, then sdf_path[] remains NULL, and the
     current working directory is assumed to contain the SDF
     files. */
    
    if (sdf_path[0]==0)
    {
        env=getenv("HOME");
        snprintf(string,253,"%s/.splat_path",env);
        fd=fopen(string,"r");
        
        if (fd!=NULL)
        {
            fgets(string,253,fd);
            
            /* Remove <CR> and/or <LF> from string */
            
            for (x=0; string[x]!=13 && string[x]!=10 && string[x]!=0 && x<253; x++);
            string[x]=0;
            
            strncpy(sdf_path,string,253);
            
            fclose(fd);
        }
    }
    
    /* Ensure a trailing '/' is present in sdf_path */
    
    if (sdf_path[0])
    {
        size_t x=strlen(sdf_path);
        
        if (sdf_path[x-1]!='/' && x!=0)
        {
            sdf_path[x]='/';
            sdf_path[x+1]=0;
        }
    }
    
    fprintf(stdout,"%s",header);
    fflush(stdout);
    
    if (ani_filename[0])
    {
        ReadLRParm(tx_site[0],0); /* Get ERP status */
        y=LoadANO(ani_filename,dem);
        
        for (x=0; x<txsites && x<max_txsites; x++)
            PlaceMarker(tx_site[x],dem);
        
        if (rxsite)
            PlaceMarker(rx_site,dem);
        
        if (bfs)
        {
            for (x=0; x<bfs; x++)
                LoadBoundaries(boundary_file[x],dem);
            
            fprintf(stdout,"\n");
            fflush(stdout);
        }
        
        if (cities)
        {
            for (x=0; x<cities; x++)
                LoadCities(city_file[x], dem);
            
            fprintf(stdout,"\n");
            fflush(stdout);
        }
        
        if (LR->erp==0.0)
            WritePPMLR(mapfile,geo,kml,ngs,tx_site,txsites, dem);
        else
        {
            if (dbm)
                WritePPMDBM(mapfile,geo,kml,ngs,tx_site,txsites,dem);
            else
                WritePPMSS(mapfile,geo,kml,ngs,tx_site,txsites,dem);
        }
        
        exit(0);
    }
    
    x=0;
    y=0;
    
    min_lat=90;
    max_lat=-90;
    
    min_lon=(int)floor(tx_site[0].lon);
    max_lon=(int)floor(tx_site[0].lon);
    
    for (y=0, z=0; z<txsites && z<max_txsites; z++)
    {
        txlat=(int)floor(tx_site[z].lat);
        txlon=(int)floor(tx_site[z].lon);
        
        if (txlat<min_lat)
            min_lat=txlat;
        
        if (txlat>max_lat)
            max_lat=txlat;
        
        if (LonDiff(txlon,min_lon)<0.0)
            min_lon=txlon;
        
        if (LonDiff(txlon,max_lon)>=0.0)
            max_lon=txlon;
    }
    
    if (rxsite)
    {
        rxlat=(int)floor(rx_site.lat);
        rxlon=(int)floor(rx_site.lon);
        
        if (rxlat<min_lat)
            min_lat=rxlat;
        
        if (rxlat>max_lat)
            max_lat=rxlat;
        
        if (LonDiff(rxlon,min_lon)<0.0)
            min_lon=rxlon;
        
        if (LonDiff(rxlon,max_lon)>=0.0)
            max_lon=rxlon;
    }
    
    /* Load the required SDF files */
    
    LoadTopoData(max_lon, min_lon, max_lat, min_lat,dem);
    
    if (area_mode || topomap)
    {
        for (z=0; z<txsites && z<max_txsites; z++)
        {
            /* "Ball park" estimates used to load any additional
             SDF files required to conduct this analysis. */
            
            tx_range=sqrt(1.5*(tx_site[z].alt+GetElevation(tx_site[z],dem)));
            
            if (LRmap)
                rx_range=sqrt(1.5*altitudeLR);
            else
                rx_range=sqrt(1.5*altitude);
            
            /* deg_range determines the maximum
             amount of topo data we read */
            
            deg_range=(tx_range+rx_range)/57.0;
            
            /* max_range regulates the size of the
             analysis.  A small, non-zero amount can
             be used to shrink the size of the analysis
             and limit the amount of topo data read by
             SPLAT!  A large number will increase the
             width of the analysis and the size of
             the map. */
            
            if (max_range==0.0)
                max_range=tx_range+rx_range;
            
            deg_range=max_range/57.0;
            
            /* Prevent the demand for a really wide coverage
             from allocating more "pages" than are available
             in memory. */
            
            switch (maxpages)
            {
                case 1: deg_limit=0.125;
                    break;
                    
                case 2: deg_limit=0.25;
                    break;
                    
                case 4: deg_limit=0.5;
                    break;
                    
                case 9: deg_limit=1.0;
                    break;
                    
                case 16: deg_limit=1.5;  /* WAS 2.0 */
                    break;
                    
                case 25: deg_limit=2.0;  /* WAS 3.0 */
                    break;
                    
                case 36: deg_limit=2.5;	 /* New! */
                    break;
                    
                case 49: deg_limit=3.0;  /* New! */
                    break;
                    
                case 64: deg_limit=3.5;  /* New! */
                    break;
            }
            
            if (fabs(tx_site[z].lat)<70.0)
                deg_range_lon=deg_range/cos(DEG2RAD*tx_site[z].lat);
            else
                deg_range_lon=deg_range/cos(DEG2RAD*70.0);
            
            /* Correct for squares in degrees not being square in miles */
            
            if (deg_range>deg_limit)
                deg_range=deg_limit;
            
            if (deg_range_lon>deg_limit)
                deg_range_lon=deg_limit;
            
            north_min=(int)floor(tx_site[z].lat-deg_range);
            north_max=(int)floor(tx_site[z].lat+deg_range);
            
            west_min=(int)floor(tx_site[z].lon-deg_range_lon);
            
            while (west_min<0)
                west_min+=360;
            
            while (west_min>=360)
                west_min-=360;
            
            west_max=(int)floor(tx_site[z].lon+deg_range_lon);
            
            while (west_max<0)
                west_max+=360;
            
            while (west_max>=360)
                west_max-=360;
            
            if (north_min<min_lat)
                min_lat=north_min;
            
            if (north_max>max_lat)
                max_lat=north_max;
            
            if (LonDiff(west_min,min_lon)<0.0)
                min_lon=west_min;
            
            if (LonDiff(west_max,max_lon)>=0.0)
                max_lon=west_max;
        }
        
        /* Load any additional SDF files, if required */ 
        
        LoadTopoData(max_lon, min_lon, max_lat, min_lat,dem);
    }
    
    if (udt_file[0])
        LoadUDT(udt_file,dem);
    
    
    /***** Let the SPLATting begin! *****/
    
    if (pt2pt_mode)
    {
        PlaceMarker(rx_site,dem);
        
        if (terrain_plot)
        {
            /* Extract extension (if present)
             from "terrain_file" */
            
            size_t y=strlen(terrain_file);
            
            for (x=(int)y-1; x>0 && terrain_file[x]!='.'; x--);
            
            if (x>0)  /* Extension found */
            {
                for (z=x+1; z<=y && (z-(x+1))<10; z++)
                    ext[z-(x+1)]=tolower(terrain_file[z]);
                
                ext[z-(x+1)]=0;	    /* Ensure an ending 0 */
                terrain_file[x]=0;  /* Chop off extension */
            }
            
            else
                strncpy(ext,"png\0",4);
        }
        
        if (elevation_plot)
        {
            /* Extract extension (if present)
             from "elevation_file" */
            
            size_t y=strlen(elevation_file);
            
            for (x=(int)y-1; x>0 && elevation_file[x]!='.'; x--);
            
            if (x>0)  /* Extension found */
            {
                for (z=x+1; z<=y && (z-(x+1))<10; z++)
                    ext[z-(x+1)]=tolower(elevation_file[z]);
                
                ext[z-(x+1)]=0;       /* Ensure an ending 0 */
                elevation_file[x]=0;  /* Chop off extension */
            }
            
            else
                strncpy(ext,"png\0",4);
        }
        
        if (height_plot)
        {
            /* Extract extension (if present)
             from "height_file" */
            
            size_t y=strlen(height_file);
            
            for (x=(int)y-1; x>0 && height_file[x]!='.'; x--);
            
            if (x>0)  /* Extension found */
            {
                for (z=x+1; z<=y && (z-(x+1))<10; z++)
                    ext[z-(x+1)]=tolower(height_file[z]);
                
                ext[z-(x+1)]=0;    /* Ensure an ending 0 */
                height_file[x]=0;  /* Chop off extension */
            }
            
            else
                strncpy(ext,"png\0",4);
        }
        
        if (longley_plot)
        {
            /* Extract extension (if present)
             from "longley_file" */
            
            size_t y = strlen(longley_file);
            
            for (x= (int) y -1; x>0 && longley_file[x]!='.'; x--);
            
            if (x>0)  /* Extension found */
            {
                for (z=x+1; z<=y && (z-(x+1))<10; z++)
                    ext[z-(x+1)]=tolower(longley_file[z]);
                
                ext[z-(x+1)]=0;     /* Ensure an ending 0 */
                longley_file[x]=0;  /* Chop off extension */
            }
            
            else
                strncpy(ext,"png\0",4);
        }
        
        for (x=0; x<txsites && x<4; x++)
        {
            PlaceMarker(tx_site[x],dem);
            
            if (nolospath==0)
            {
                switch (x)
                {
                    case 0:
                        PlotPath(tx_site[x],rx_site,1,dem);
                        break;
                        
                    case 1:
                        PlotPath(tx_site[x],rx_site,8,dem);
                        break;
                        
                    case 2:
                        PlotPath(tx_site[x],rx_site,16,dem);
                        break;
                        
                    case 3:
                        PlotPath(tx_site[x],rx_site,32,dem);
                }
            }
            
            if (nositereports==0)
                SiteReport(tx_site[x], dem);
            
            if (kml)
                WriteKML(tx_site[x],rx_site,dem);
            
            if (txsites>1)
                snprintf(string,250,"%s-%c.%s%c",longley_file,'1'+x,ext,0);
            else
                snprintf(string,250,"%s.%s%c",longley_file,ext,0);
            
            if (nositereports==0)
            {
                if (longley_file[0]==0)
                {
                    ReadLRParm(tx_site[x],0);
                    PathReport(tx_site[x],rx_site,string,0,elev,dem);
                }
                
                else
                {
                    ReadLRParm(tx_site[x],1);
                    PathReport(tx_site[x],rx_site,string,longley_file[0],elev,dem);
                }
            }
            
            if (terrain_plot)
            {
                if (txsites>1)
                    snprintf(string,250,"%s-%c.%s%c",terrain_file,'1'+x,ext,0);
                else
                    snprintf(string,250,"%s.%s%c",terrain_file,ext,0);
                
                GraphTerrain(tx_site[x],rx_site,string,dem);
            }
            
            if (elevation_plot)
            {
                if (txsites>1)
                    snprintf(string,250,"%s-%c.%s%c",elevation_file,'1'+x,ext,0);
                else
                    snprintf(string,250,"%s.%s%c",elevation_file,ext,0);
                
                GraphElevation(tx_site[x],rx_site,string,dem);
            }
            
            if (height_plot)
            {
                if (txsites>1)
                    snprintf(string,250,"%s-%c.%s%c",height_file,'1'+x,ext,0);
                else
                    snprintf(string,250,"%s.%s%c",height_file,ext,0);
                
                GraphHeight(tx_site[x],rx_site,string,fresnel_plot,norm,dem);
            }
        }
    }
    
    if (area_mode && topomap==0)
    {
        for (x=0; x<txsites && x<max_txsites; x++)
        {
            if (coverage)
                PlotLOSMap(tx_site[x],altitude,dem);
            
            else if (ReadLRParm(tx_site[x],1))
                PlotLRMap(tx_site[x],altitudeLR,ano_filename,elev,dem);
            
            SiteReport(tx_site[x], dem);
        }
    }
    
    if (map || topomap)
    {
        /* Label the map */
        
        if (kml==0)
        {
            for (x=0; x<txsites && x<max_txsites; x++)
                PlaceMarker(tx_site[x],dem);
        }
        
        if (cities)
        {
            
            for (y=0; y<cities; y++)
                LoadCities(city_file[y],dem);
            
            fprintf(stdout,"\n");
            fflush(stdout);
        }
        
        /* Load city and county boundary data files */
        
        if (bfs)
        {
            for (y=0; y<bfs; y++)
                LoadBoundaries(boundary_file[y],dem);
            
            fprintf(stdout,"\n");
            fflush(stdout);
        }
        
        /* Plot the map */
        
        if (coverage || pt2pt_mode || topomap)
            WritePPM(mapfile,geo,kml,ngs,tx_site,txsites,dem);
        
        else
        {
            if (LR->erp==0.0)
                WritePPMLR(mapfile,geo,kml,ngs,tx_site,txsites,dem);
            else
                if (dbm)
                    WritePPMDBM(mapfile,geo,kml,ngs,tx_site,txsites,dem);
                else
                    WritePPMSS(mapfile,geo,kml,ngs,tx_site,txsites,dem);
        }
    }
    
    if (command_line_log && strlen(logfile)>0)
    {
        fd=fopen(logfile,"w");
        
        if (fd!=NULL)
        {
            for (x=0; x<argc; x++)
                fprintf(fd,"%s ",argv[x]);
            
            fprintf(fd,"\n");
            
            fclose(fd);
            
            fprintf(stdout,"\nCommand-line parameter log written to: \"%s\"\n",logfile);
            
        }
    }
    
    printf("\n");
    
    /* That's all, folks! */
 
    // TODO: Why can't we delete? It complains about items already being deleted?!
    //delete path;
    delete LR;
    // TODO: Why can't we clear. It complains about items already being deleted?!
    //dem.clear();
    delete[] elev;
    
    return 0;
}
