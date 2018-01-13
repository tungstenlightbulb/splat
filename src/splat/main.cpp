/** @file main.cpp
 *
 * SPLAT!: An RF Signal Path Loss And Terrain Analysis Tool
 * Project started in 1997 by John A. Magliacane (KD2BD)
 * File created by Peter Watkins (KE7IST) 6/17/15.
 * Derived from original project code.
 * Splat!
 * @copyright 1997 - 2018 John A. Magliacane (KD2BD) and contributors.
 * See revision control history for contributions.
 * This file is covered by the LICENSE.md file in the root of this project.
 */

#include <iostream>
#include <cmath>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <fstream>
#include <bzlib.h>
#include "site.h"
#include "path.h"
#include "dem.h"
#include "pat_file.h"
#include "splat_run.h"
#include "lrp.h"
#include "sdf.h"
#include "elevation_map.h"
#include "utilities.h"
#include "region.h"
#include "ppm.h"
#include "itwom3.0.h"
#include "anf.h"
#include "boundary_file.h"
#include "city_file.h"
#include "udt.h"
#include "report.h"
#include "gnuplot.h"
#include "kml.h"


using namespace std;

void check_allocation(void *ptr, string name, const SplatRun &sr);

int main(int argc, const char * argv[])
{
    int		x, y, z=0, min_lat, min_lon, max_lat, max_lon,
    rxlat, rxlon, txlat, txlon, west_min, west_max,
    north_min, north_max;
    
    char *env=NULL;
    string mapfile,
    elevation_file, height_file,
    longley_file, terrain_file,
    udt_file, ani_filename,
    ano_filename, logfile, maxpages_str;
    
    vector<std::string> city_file;
    vector<std::string> boundary_file;
    
    vector<Site> tx_site;
    Site rx_site;
    
    SplatRun sr;
    
    sr.maxpages = 16;
    sr.arraysize = -1;
    
    sr.hd_mode = false;
    sr.coverage = false;
    sr.LRmap = false;
    sr.terrain_plot = false;
    sr.elevation_plot = false;
    sr.height_plot = false;
    sr.map = false;
    sr.longley_plot = false;
    sr.norm = false;
    sr.topomap = false;
    sr.geo = false;
    sr.kml = false;
    sr.pt2pt_mode = false;
    sr.area_mode = false;
    sr.ngs = false;
    sr.nolospath = false;
    sr.nositereports = false;
    sr.fresnel_plot = true;
    sr.command_line_log = false;
    sr.rxsite = false;
    sr.metric = false;
    sr.dbm = false;
    sr.smooth_contours = false;
    
    sr.altitude        =  0.0;
    sr.altitudeLR      =  0.0;
    sr.tx_range        =  0.0;
    sr.rx_range        =  0.0;
    sr.deg_range       =  0.0;
    sr.deg_limit       =  0.0;
    sr.max_range       =  0.0;
    sr.forced_erp      = -1.0;
    sr.fzone_clearance =  0.6;
    
    
    if (argc==1)
    {
        cout << "\n\t\t --==[ " << SplatRun::splat_name << " v" << SplatRun::splat_version << " Available Options... ]==--\n\n"
        "       -t txsite(s).qth (max of 4 with -c, max of 30 with -L)\n"
        "       -r sr.rxsite.qth\n"
        "       -c plot LOS coverage of TX(s) with an RX antenna at X feet/meters AGL\n"
        "       -L plot path loss map of TX based on an RX at X feet/meters AGL\n"
        "       -s filename(s) of city/site file(s) to import (5 max)\n"
        "       -b filename(s) of cartographic boundary file(s) to import (5 max)\n"
        "       -p filename of terrain profile graph to plot\n"
        "       -e filename of terrain elevation graph to plot\n"
        "       -h filename of terrain height graph to plot\n"
        "       -H filename of normalized terrain height graph to plot\n"
        "       -l filename of path loss graph to plot\n"
        "       -o filename of topographic map to generate (.ppm)\n"
        "       -u filename of user-defined terrain file to import\n"
        "       -d sdf file directory path (overrides path in ~/.splat_path file)\n"
        "       -m earth radius multiplier\n"
        "       -n do not plot LOS paths in .ppm maps\n"
        "       -N do not produce unnecessary site or obstruction reports\n"
        "       -f frequency for Fresnel zone calculation (MHz)\n"
        "       -R modify default range for -c or -L (miles/kilometers)\n"
        "      -sc display smooth rather than quantized contour levels\n"
        "      -db threshold beyond which contours will not be displayed\n"
        "      -nf do not plot Fresnel zones in height plots\n"
        "      -fz Fresnel zone clearance percentage (default = 60)\n"
        "      -gc ground clutter height (feet/meters)\n"
        "     -ngs display greyscale topography as white in .ppm files\n"
        "     -erp override ERP in .lrp file (Watts)\n"
        "     -ano name of alphanumeric output file\n"
        "     -ani name of alphanumeric input file\n"
        "     -udt name of user defined terrain input file\n"
        "     -kml generate Google Earth (.kml) compatible output\n"
        "     -geo generate an Xastir .geo georeference file (with .ppm output)\n"
        "     -dbm plot signal power level contours rather than field strength\n"
        "     -log copy command line string to this output file\n"
        "   -gpsav preserve gnuplot temporary working files after SPLAT! execution\n"
        "  -metric employ metric rather than imperial units for all user I/O\n"
        "  -olditm invoke Longley-Rice rather than the default ITWOM model\n"
        "-maxpages [" << sr.maxpages << "] Maximum Analysis Region capability: 1, 4, 9, 16, 25, 36, 49, 64 \n"
        "      -hd Enable HD mode."
        "\n"
        "If that flew by too fast, consider piping the output through 'less':\n"
        "\n\tsplat | less\n\n"
        "Type 'man splat', or see the documentation for more details.\n\n";
        
        return 1;
    }
    
    y=argc-1;
    
    sr.olditm=false;
    sr.kml = false;
    sr.geo = false;
    sr.dbm = false;
    sr.gpsav=false;
    sr.metric = false;
    sr.clutter=0.0;
    sr.forced_erp=-1.0;
    sr.forced_freq=0.0;
    sr.fzone_clearance=0.6;
    sr.contour_threshold=0;
    sr.rx_site.lat=91.0;
    sr.rx_site.lon=361.0;
    sr.smooth_contours = false;
    sr.earthradius = EARTHRADIUS;
    
//    for (x=0; x<4; x++)
//    {
//        tx_site[x].lat=91.0;
//        tx_site[x].lon=361.0;
//    }
    
    /* Scan for command line arguments */
    
    for (x=1; x<=y; x++)
    {
        if (strcmp(argv[x],"-R")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                sscanf(argv[z],"%lf",&sr.max_range);
                
                if (sr.max_range<0.0)
                    sr.max_range=0.0;
                
                if (sr.max_range>1000.0)
                    sr.max_range=1000.0;
            }
        }
        
        if (strcmp(argv[x],"-m")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                sscanf(argv[z],"%lf",&sr.er_mult);
                
                if (sr.er_mult<0.1)
                    sr.er_mult=1.0;
                
                if (sr.er_mult>1.0e6)
                    sr.er_mult=1.0e6;
                
                sr.earthradius*=sr.er_mult;
            }
        }
        
        if (strcmp(argv[x],"-gc")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                sscanf(argv[z],"%lf",&sr.clutter);
                
                if (sr.clutter<0.0)
                    sr.clutter=0.0;
            }
        }
        
        if (strcmp(argv[x],"-fz")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                sscanf(argv[z],"%lf",&sr.fzone_clearance);
                
                if (sr.fzone_clearance<0.0 || sr.fzone_clearance>100.0)
                    sr.fzone_clearance=60.0;
                
                sr.fzone_clearance/=100.0;
            }
        }
        
        if (strcmp(argv[x],"-o")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
                mapfile = argv[z];
            sr.map = true;
        }
        
        if (strcmp(argv[x],"-log")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
                logfile = argv[z];
            
            sr.command_line_log = true;
        }
        
        if (strcmp(argv[x],"-udt")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
                udt_file = argv[z];
        }
        
        if (strcmp(argv[x],"-c")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                sscanf(argv[z],"%lf",&sr.altitude);
                sr.map       = true;
                sr.coverage  = true;
                sr.area_mode = true;
            }
        }
        
        if (strcmp(argv[x],"-db")==0 || strcmp(argv[x],"-dB")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0]) /* A minus argument is legal here */
                sscanf(argv[z],"%d",&sr.contour_threshold);
        }
        
        if (strcmp(argv[x],"-p")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                terrain_file = argv[z];
                sr.terrain_plot = true;
                sr.pt2pt_mode   = true;
            }
        }
        
        if (strcmp(argv[x],"-e")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                elevation_file = argv[z];
                sr.elevation_plot = true;
                sr.pt2pt_mode     = true;
            }
        }
        
        if (strcmp(argv[x],"-h")==0 || strcmp(argv[x],"-H")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                height_file = argv[z];
                sr.height_plot = true;
                sr.pt2pt_mode  = true;
            }
            
            sr.norm = strcmp(argv[x],"-H") == 0 ? true : false;
        }
        
        if (strcmp(argv[x],"-metric")==0)
            sr.metric = true;
        
        if (strcmp(argv[x],"-gpsav")==0)
            sr.gpsav=true;
        
        if (strcmp(argv[x],"-geo")==0)
            sr.geo = true;
        
        if (strcmp(argv[x],"-kml")==0)
            sr.kml = true;
        
        if (strcmp(argv[x],"-nf")==0)
            sr.fresnel_plot = false;
        
        if (strcmp(argv[x],"-ngs")==0)
            sr.ngs = true;
        
        if (strcmp(argv[x],"-n")==0)
            sr.nolospath = true;
        
        if (strcmp(argv[x],"-dbm")==0)
            sr.dbm = true;
        
        if (strcmp(argv[x],"-sc")==0)
            sr.smooth_contours = true;
        
        if (strcmp(argv[x],"-olditm")==0)
            sr.olditm=true;
        
        if (strcmp(argv[x],"-N")==0)
        {
            sr.nolospath     = true;
            sr.nositereports = true;
        }
        
        if (strcmp(argv[x],"-d")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
                sr.sdf_path = argv[z];
        }
        
        if (strcmp(argv[x],"-t")==0)
        {
            /* Read Transmitter Location */
            
            z=x+1;
            
            while (z<=y && argv[z][0] && argv[z][0]!='-' && tx_site.size()<30)
            {
                string txfile = argv[z];
                tx_site.push_back(Site(txfile));
                z++;
            }
            
            z--;
        }
        
        if (strcmp(argv[x],"-L")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                sscanf(argv[z],"%lf",&sr.altitudeLR);
                sr.map       = true;
                sr.LRmap     = true;
                sr.area_mode = true;
                
                if (sr.coverage)
                    fprintf(stdout,"c and L are exclusive options, ignoring L.\n");
            }
        }
        
        if (strcmp(argv[x],"-l")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                longley_file = argv[z];
                sr.longley_plot = true;
                sr.pt2pt_mode   = true;
            }
        }
        
        if (strcmp(argv[x],"-r")==0)
        {
            /* Read Receiver Location */
            
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                string rxfile = argv[z];
                rx_site.LoadQTH(rxfile);
                sr.rxsite     = true;
                sr.pt2pt_mode = true;
            }
        }
        
        if (strcmp(argv[x],"-s")==0)
        {
            /* Read city file(s) */
            
            z=x+1;
            
            while (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                city_file.push_back(argv[z]);
                z++;
            }
            
            z--;
        }
        
        if (strcmp(argv[x],"-b")==0)
        {
            /* Read Boundary File(s) */
            
            z=x+1;
            
            while (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                boundary_file.push_back(argv[z]);
                z++;
            }
            
            z--;
        }
        
        if (strcmp(argv[x],"-f")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                sscanf(argv[z],"%lf",&(sr.forced_freq));
                
                if (sr.forced_freq<20.0)
                    sr.forced_freq=0.0;
                
                if (sr.forced_freq>20.0e3)
                    sr.forced_freq=20.0e3;
            }
        }
        
        if (strcmp(argv[x],"-erp")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                sscanf(argv[z],"%lf",&(sr.forced_erp));
                
                if (sr.forced_erp<0.0)
                    sr.forced_erp=-1.0;
            }
        }
        
        if (strcmp(argv[x],"-ano")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
                ano_filename = argv[z];
        }
        
        if (strcmp(argv[x],"-ani")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
                ani_filename = argv[z];
        }
        
        if (strcmp(argv[x],"-maxpages")==0)
        {
            z=x+1;
            
            if (z<=y && argv[z][0] && argv[z][0]!='-')
            {
                maxpages_str = argv[z];
                if (sscanf(maxpages_str.c_str(), "%d", &sr.maxpages) != 1)
                {
                    cerr << "\n" << 7 << "*** ERROR: Could not parse maxpages: " << maxpages_str << "\n\n";
                    exit (-1);
                }
            }
        }
        
        if (strcmp(argv[x],"-hd")==0)
        {
            sr.hd_mode = true;
        }
    }
    
    /* Perform some error checking on the arguments
     and switches parsed from the command-line.
     If an error is encountered, print a message
     and exit gracefully. */
    
    if (tx_site.size()==0)
    {
        fprintf(stderr,"\n%c*** ERROR: No transmitter site(s) specified!\n\n",7);
        exit (-1);
    }
    
    for (x=0, y=0; x<tx_site.size(); x++)
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
    
    if (!sr.coverage && !sr.LRmap && ani_filename.empty() && rx_site.lat==91.0 && rx_site.lon==361.0)
    {
        if (sr.max_range!=0.0 && tx_site.size()!=0)
        {
            /* Plot topographic map of radius "sr.max_range" */
            
            sr.map     = false;
            sr.topomap = true;
        }
        
        else
        {
            fprintf(stderr,"\n%c*** ERROR: No receiver site found or specified!\n\n",7);
            exit (-1);
        }
    }
    
    switch (sr.maxpages)
    {
        case 1:
            if (!sr.hd_mode)
            {
                fprintf(stderr,"\n%c*** ERROR: -maxpages must be >= 4 if not in HD mode!\n\n",7);
                exit (-1);
            }
            sr.arraysize = 5092;
            break;
        case 4:
            sr.arraysize = sr.hd_mode ? 14844 : 4950;
            break;

        case 9:
            sr.arraysize = sr.hd_mode ? 32600 : 10870;
            break;

        case 16:
            sr.arraysize = sr.hd_mode ? 57713 : 19240;
            break;

        case 25:
            sr.arraysize = sr.hd_mode ? 90072 : 30025;
            break;

        case 36:
            sr.arraysize = sr.hd_mode ? 129650 : 43217;
            break;

        case 49:
            sr.arraysize = sr.hd_mode ? 176437 : 58813;
            break;

        case 64:
            sr.arraysize = sr.hd_mode ? 230430 : 76810;
            break;
        default:
            fprintf(stderr,"\n%c*** ERROR: -maxpages must be one of 1, 4, 9, 16, 25, 36, 49, 64\n\n",7);
            exit (-1);
    }
    
    sr.ippd = sr.hd_mode ? 3600 : 1200; /* pixels per degree (integer) */
    


    

    
    int degrees=(int)sqrt((int)sr.maxpages);
    
    cout << "This invocation of " << SplatRun::splat_name << " supports analysis over a region of " << degrees << " square\n";
    
    if (degrees==1)
        fprintf(stdout,"degree");
    else
        fprintf(stdout,"degrees");
    
    fprintf(stdout," of terrain, and computes signal levels using ITWOM Version %.1f.\n\n",ITWOMVersion());

    
    sr.ppd=(double)sr.ippd;	/* pixels per degree (double)  */
    sr.dpp=1.0/sr.ppd;		/* degrees per pixel */
    sr.mpi=sr.ippd-1;		/* maximum pixel index per degree */
    

    
    /* No major errors were detected.  Whew!  :-) */
    
    /* Adjust input parameters if -metric option is used */
    
    if (sr.metric)
    {
        sr.altitudeLR/=METERS_PER_FOOT;	/* meters --> feet */
        sr.max_range/=KM_PER_MILE;		/* kilometers --> miles */
        sr.altitude/=METERS_PER_FOOT;	/* meters --> feet */
        sr.clutter/=METERS_PER_FOOT;	/* meters --> feet */
    }
    
    /* If no SDF path was specified on the command line (-d), check
     for a path specified in the $HOME/.splat_path file.  If the
     file is not found, then sr.sdf_path[] remains NULL, and the
     current working directory is assumed to contain the SDF
     files. */
    
    if (sr.sdf_path.empty())
    {
        env=getenv("HOME");
        std::string config_path = env;
        config_path += "/.splat_path";
        fstream fs;
        fs.open(config_path.c_str(), fstream::in);
        
        if (fs)
        {
            getline(fs, sr.sdf_path);
            fs.close();
        }
    }
    
    /* Ensure a trailing '/' is present in sr.sdf_path */
    
    if (!sr.sdf_path.empty() && (*sr.sdf_path.rbegin() != '/'))
    {
        sr.sdf_path += '/';
    }
    Sdf sdf(sr.sdf_path, sr);
    
    // Now print the header:
    cout << "\n\t\t--==[ Welcome To " << SplatRun::splat_name << " v" << SplatRun::splat_version << " ]==--\n\n";
    
    double *elev = new double[sr.arraysize+10];
    check_allocation(elev, "elev", sr);
    
    Path *path_p = new Path(sr.arraysize, sr.ppd);
    check_allocation(path_p, "path_p", sr);
    
    ElevationMap *em_p = new ElevationMap(*path_p, sr);
    check_allocation(em_p, "em_p", sr);
    
    Lrp lrp(sr.forced_erp, sr.forced_freq);
    Ppm ppm(sr, mapfile, tx_site, *em_p);
    BoundaryFile bf(*path_p);
    CityFile cf;
    Region region;
    
    if (!ani_filename.empty())
    {
        // TODO: Here's an instance where reading the LRParms may say to load
        // a PAT file but it's never used. Refactor this.
        PatFile pat = PatFile();
        
        // TODO: Why only the first TX site?
        bool loadPat;
        string patFilename;
        lrp.ReadLRParm(tx_site[0],0, loadPat, patFilename); /* Get ERP status */
        if (loadPat)
        {
            pat.LoadPAT(patFilename);
        }
        Anf anf(lrp, sr);

        y = anf.LoadANO(ani_filename, sdf, *em_p);
        
        for (x=0; x<tx_site.size(); x++)
            em_p->PlaceMarker(tx_site[x]);
        
        if (sr.rxsite)
            em_p->PlaceMarker(rx_site);
        
        if (boundary_file.size() > 0)
        {
            for (x=0; x<boundary_file.size(); x++)
                bf.LoadBoundaries(boundary_file[x], *em_p);
            
            fprintf(stdout,"\n");
            fflush(stdout);
        }
        
        if (city_file.size() > 0)
        {
            for (x=0; x<city_file.size(); x++)
                cf.LoadCities(city_file[x], *em_p);
            
            fprintf(stdout,"\n");
            fflush(stdout);
        }
        


        
        if (lrp.erp==0.0)
        {
            ppm.WritePPMLR(region);
        }
        else
        {
            if (sr.dbm)
                ppm.WritePPMDBM(region);
            else
                ppm.WritePPMSS(region);
        }
        
        exit(0);
    }
    
    x=0;
    y=0;
    
    min_lat=90;
    max_lat=-90;
    
    min_lon=(int)floor(tx_site[0].lon);
    max_lon=(int)floor(tx_site[0].lon);
    
    for (y=0, z=0; z<tx_site.size(); z++)
    {
        txlat=(int)floor(tx_site[z].lat);
        txlon=(int)floor(tx_site[z].lon);
        
        if (txlat<min_lat)
            min_lat=txlat;
        
        if (txlat>max_lat)
            max_lat=txlat;
        
        if (Utilities::LonDiff(txlon,min_lon)<0.0)
            min_lon=txlon;
        
        if (Utilities::LonDiff(txlon,max_lon)>=0.0)
            max_lon=txlon;
    }
    
    if (sr.rxsite)
    {
        rxlat=(int)floor(rx_site.lat);
        rxlon=(int)floor(rx_site.lon);
        
        if (rxlat<min_lat)
            min_lat=rxlat;
        
        if (rxlat>max_lat)
            max_lat=rxlat;
        
        if (Utilities::LonDiff(rxlon,min_lon)<0.0)
            min_lon=rxlon;
        
        if (Utilities::LonDiff(rxlon,max_lon)>=0.0)
            max_lon=rxlon;
    }
    
    /* Load the required SDF files */
    
    em_p->LoadTopoData(max_lon, min_lon, max_lat, min_lat, sdf);
    
    if (sr.area_mode || sr.topomap)
    {
        for (z=0; z<tx_site.size(); z++)
        {
            /* "Ball park" estimates used to load any additional
             SDF files required to conduct this analysis. */
            
            sr.tx_range=sqrt(1.5 * (tx_site[z].alt + em_p->GetElevation(tx_site[z])));
            
            if (sr.LRmap)
                sr.rx_range=sqrt(1.5*sr.altitudeLR);
            else
                sr.rx_range=sqrt(1.5*sr.altitude);
            
            /* sr.deg_range determines the maximum
             amount of topo data we read */
            
            sr.deg_range=(sr.tx_range+sr.rx_range)/57.0;
            
            /* sr.max_range regulates the size of the
             analysis.  A small, non-zero amount can
             be used to shrink the size of the analysis
             and limit the amount of topo data read by
             SPLAT!  A large number will increase the
             width of the analysis and the size of
             the map. */
            
            if (sr.max_range==0.0)
                sr.max_range=sr.tx_range+sr.rx_range;
            
            sr.deg_range=sr.max_range/57.0;
            
            /* Prevent the demand for a really wide coverage
             from allocating more "pages" than are available
             in memory. */
            
            switch (sr.maxpages)
            {
                case 1: sr.deg_limit=0.125;
                    break;
                    
                case 2: sr.deg_limit=0.25;
                    break;
                    
                case 4: sr.deg_limit=0.5;
                    break;
                    
                case 9: sr.deg_limit=1.0;
                    break;
                    
                case 16: sr.deg_limit=1.5;  /* WAS 2.0 */
                    break;
                    
                case 25: sr.deg_limit=2.0;  /* WAS 3.0 */
                    break;
                    
                case 36: sr.deg_limit=2.5;	 /* New! */
                    break;
                    
                case 49: sr.deg_limit=3.0;  /* New! */
                    break;
                    
                case 64: sr.deg_limit=3.5;  /* New! */
                    break;
            }
            
            if (fabs(tx_site[z].lat)<70.0)
                sr.deg_range_lon=sr.deg_range/cos(DEG2RAD*tx_site[z].lat);
            else
                sr.deg_range_lon=sr.deg_range/cos(DEG2RAD*70.0);
            
            /* Correct for squares in degrees not being square in miles */
            
            if (sr.deg_range>sr.deg_limit)
                sr.deg_range=sr.deg_limit;
            
            if (sr.deg_range_lon>sr.deg_limit)
                sr.deg_range_lon=sr.deg_limit;
            
            north_min=(int)floor(tx_site[z].lat-sr.deg_range);
            north_max=(int)floor(tx_site[z].lat+sr.deg_range);
            
            west_min=(int)floor(tx_site[z].lon-sr.deg_range_lon);
            
            while (west_min<0)
                west_min+=360;
            
            while (west_min>=360)
                west_min-=360;
            
            west_max=(int)floor(tx_site[z].lon+sr.deg_range_lon);
            
            while (west_max<0)
                west_max+=360;
            
            while (west_max>=360)
                west_max-=360;
            
            if (north_min<min_lat)
                min_lat=north_min;
            
            if (north_max>max_lat)
                max_lat=north_max;
            
            if (Utilities::LonDiff(west_min,min_lon)<0.0)
                min_lon=west_min;
            
            if (Utilities::LonDiff(west_max,max_lon)>=0.0)
                max_lon=west_max;
        }
        
        /* Load any additional SDF files, if required */ 
        
        em_p->LoadTopoData(max_lon, min_lon, max_lat, min_lat, sdf);
    }
    
    if (!udt_file.empty())
    {
        Udt udt(sr);
        udt.LoadUDT(udt_file, *em_p);
    }
    
    
    /***** Let the SPLATting begin! *****/
    
    Report report(*em_p, sr, *path_p);
    
    if (sr.pt2pt_mode)
    {
        em_p->PlaceMarker(rx_site);
        
        string ext;
        if (sr.terrain_plot)
        {
            /* Extract extension (if present)
             from "terrain_file" */

            ext = Utilities::DivideExtension(terrain_file, "png");
        }
        
        if (sr.elevation_plot)
        {
            /* Extract extension (if present)
             from "elevation_file" */
            ext = Utilities::DivideExtension(elevation_file, "png");
        }
        
        if (sr.height_plot)
        {
            /* Extract extension (if present)
             from "height_file" */

            ext = Utilities::DivideExtension(height_file, "png");
        }
        
        if (sr.longley_plot)
        {
            /* Extract extension (if present)
             from "longley_file" */

            ext = Utilities::DivideExtension(longley_file, "png");
        }
        
        for (x=0; x<tx_site.size() && x<4; x++)
        {
            em_p->PlaceMarker(tx_site[x]);
            
            if (!sr.nolospath)
            {
                switch (x)
                {
                    case 0:
                        em_p->PlotPath(tx_site[x],rx_site,1);
                        break;
                        
                    case 1:
                        em_p->PlotPath(tx_site[x],rx_site,8);
                        break;
                        
                    case 2:
                        em_p->PlotPath(tx_site[x],rx_site,16);
                        break;
                        
                    case 3:
                        em_p->PlotPath(tx_site[x],rx_site,32);
                }
            }
            

            
            if (!sr.nositereports)
                report.SiteReport(tx_site[x]);
            
            if (sr.kml)
            {
                Kml kml(*path_p, *em_p, sr);
                kml.WriteKML(tx_site[x],rx_site);
            }
            
            // If there's more than one TX site, put a dash-tx_site_number on the end before the
            // extension.
            std::string filename;
            std::ostringstream oss;
            if (tx_site.size()>1)
                oss << "-" << x + 1;
            oss << "." << ext;
            
            PatFile pat;
            if (!sr.nositereports)
            {
                filename = longley_file + oss.str();
                bool longly_file_exists = !longley_file.empty();
                
                bool loadPat;
                string patFilename;
                lrp.ReadLRParm(tx_site[x], longly_file_exists, loadPat, patFilename);
                if (loadPat)
                {
                    pat.LoadPAT(patFilename);
                }
                report.PathReport(tx_site[x], rx_site, filename, longly_file_exists, elev, pat, lrp);
            }
            else
            {
                bool loadPat;
                string patFilename;
                lrp.ReadLRParm(tx_site[x],1, loadPat, patFilename);
                if (loadPat)
                {
                    pat.LoadPAT(patFilename);
                }
                report.PathReport(tx_site[x],rx_site,filename, true,elev, pat, lrp);
            }
            
            GnuPlot gnuPlot(*path_p, sr);
            
            if (sr.terrain_plot)
            {
                filename = terrain_file + oss.str();
                gnuPlot.GraphTerrain(tx_site[x],rx_site,filename, *em_p);
            }
            
            if (sr.elevation_plot)
            {
                filename = elevation_file + oss.str();
                gnuPlot.GraphElevation(tx_site[x],rx_site,filename, *em_p);
            }
            
            if (sr.height_plot)
            {
                filename = height_file + oss.str();
                gnuPlot.GraphHeight(tx_site[x],rx_site,filename,sr.fresnel_plot, sr.norm, *em_p, lrp);
            }
        }
    }
    
    if (sr.area_mode && !sr.topomap)
    {
        for (x=0; x<tx_site.size(); x++)
        {
            PatFile pat;
            if (sr.coverage)
            {
                em_p->PlotLOSMap(tx_site[x],sr.altitude);
            }
            else
            {
                bool loadPat;
                string patFilename;
                char flag = lrp.ReadLRParm(tx_site[x],1, loadPat, patFilename);
                if (loadPat)
                {
                    pat.LoadPAT(patFilename);
                }
                
                if (flag)
                {
                    em_p->PlotLRMap(tx_site[x],sr.altitudeLR,ano_filename,elev,pat,lrp);
                }
            }
            
            report.SiteReport(tx_site[x]);
        }
    }
    
    if (sr.map || sr.topomap)
    {
        /* Label the map */
        
        if (!sr.kml)
        {
            for (x=0; x<tx_site.size(); x++)
                em_p->PlaceMarker(tx_site[x]);
        }
        
        if (city_file.size() > 0)
        {
            CityFile cityFile;
            
            for (y=0; y<city_file.size(); y++)
                cityFile.LoadCities(city_file[y], *em_p);
            
            fprintf(stdout,"\n");
            fflush(stdout);
        }
        
        /* Load city and county boundary data files */
        
        if (boundary_file.size() > 0)
        {
            BoundaryFile boundaryFile(*path_p);
            
            for (y=0; y<boundary_file.size(); y++)
                boundaryFile.LoadBoundaries(boundary_file[y], *em_p);
            
            fprintf(stdout,"\n");
            fflush(stdout);
        }
        
        /* Plot the map */
        
        if (sr.coverage || sr.pt2pt_mode || sr.topomap)
            ppm.WritePPM();
        
        else
        {
            if (lrp.erp==0.0)
                ppm.WritePPMLR(region);
            else
                if (sr.dbm)
                    ppm.WritePPMDBM(region);
                else
                    ppm.WritePPMSS(region);
        }
    }
    
    if (sr.command_line_log && !logfile.empty())
    {
        fstream fs;
        fs.open(logfile.c_str(), fstream::out);
        
        // TODO: Should we fail silently if we can't open the logfile. Shouldn't we WARN?
        if (fs)
        {
            for (x=0; x<argc; x++)
                fs << argv[x] << " ";
            
            fs << endl;
            
            fs.close();
            
            cout << "\nCommand-line parameter log written to: \"" << logfile << "\"\n";
        }
    }
    
    cout << endl;
    
    /* That's all, folks! */
    
 
    delete em_p;
    
    delete path_p;
    
    // TODO: Why can't we clear. It complains about items already being deleted?!
    //dem.clear();
    delete[] elev;
    
    return 0;
}

void check_allocation(void *ptr, string name, const SplatRun &sr)
{
    if (ptr == NULL)
    {
        cerr << "\n\a*** ERROR: Could not allocate memory for " << name << " with -maxpages == " << sr.maxpages << "\n\n";
        exit (-1);
    }
}
