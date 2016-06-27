#!/usr/bin/env python
from pathlib import Path
import subprocess
import urllib.request
from numpy import isclose

rdir = Path(__file__).parents[1]

ofn='WNYE-DT_25-to-Oyster_Bay.txt'
url='http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/North_America/N40W074.hgt.zip'
sdffn=Path('40:41:73:74.sdf')

if not sdffn.is_file():
    urllib.request.urlretrieve(url,'N40W074.hgt.zip')

    subprocess.run(['unzip','N40W074.hgt.zip'])
    subprocess.run([str(rdir/'build/utils/srtm2sdf'),'N40W074.hgt'])

print(rdir)

subprocess.run([str(rdir/'build/splat/splat'),'-metric',
                '-t',str(rdir/'tests/wnye.qth'),
                '-o','topo.ppm',
                '-r',str(rdir/'tests/oyster.qth'),
                '-e','el.png','-p','tests/terr.png'])
#%% load output
out = {}
with open(ofn,'r',encoding='iso-8859-1') as f:
    for line in f:
        l = line.split(':')
        if l[0].startswith('Free space path loss'):
            out['fspl'] =    float(l[1].split()[0])
        elif l[0].startswith('Signal power level'):
            out['rxpow'] =   float(l[1].split()[0])
        elif l[0].startswith('ITWOM error number'):
            out['err'] =     int(l[1].split()[0])
        elif l[0].startswith('Attenuation due to terrain shielding'):
            out['terrain'] = float(l[1].split()[0])
        elif l[0].startswith('ITWOM Version 3.0 path loss'):
            out['itwom'] =   float(l[1].split()[0])

#%% check output
assert isclose(out['rxpow'],-67.51)
assert isclose(out['fspl']+out['terrain'],out['itwom'])