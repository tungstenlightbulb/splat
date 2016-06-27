.. image:: https://travis-ci.org/scienceopen/splat.svg?branch=master
    :target: https://travis-ci.org/scienceopen/splat

======================================================================
SPLAT!: A Terrestrial RF Path and Terrain Analysis Tool for Unix/Linux
======================================================================

.. contents::

Quick Run
=========
::

	cd build
	cmake ../src
	make
	make install

    wget -nc http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/North_America/N40W074.hgt.zip
    unzip N40W074.hgt.zip
    


Requirements
============
SPLAT! builds with CMake version 2.6 or greater.

SPLAT! requires the libbzip2-1.0.1 (or later) compression library and
header files for successful compilation.  bzip2/libbzip2 is available
at: 
http://www.bzip2.org/

SPLAT! also requires the zlib general purpose compression library.
The latest version of zlib may be found at: http://www.gzip.org/zlib/
If you should decide to build zlib, it is important to invoke::

	./configure -s

to build zlib as a shared library.

If you wish to plot terrain and elevation profiles through SPLAT!,
you will need gnuplot.  The latest version of gnuplot is available
at: 
http://www.gnuplot.info/


Installation Instructions
=========================
Unpack the tar.gz file::

	tar xf splat-1.4.2.tar.gz

generating a subdirectory named splat-1.4.2.

Next, build via the commands::

	cd build

	cmake ..

	make

To install SPLAT!::

	make install

Before running SPLAT!, carefully read the documentation (man splat) for 
information on the use of the program. Some sample data files are 
located under the splat-1.4.2/sample_data directory.

REMEMBER: Topography data must be downloaded and SPLAT Data Files must
be generated using the included srtm2sdf, postdownload, or usgs2sdf
utilities before using SPLAT!  Instructions for doing so are included
in the documentation.

It is a good practice to create a working directory for SPLAT! use
under your home directory::

	mkdir $HOME/splat-work

	cd $HOME/splat-work

before invoking SPLAT!

In this manner, all associated SPLAT! working files can be kept in a
common directory.

It is important to realize that when analyzing regional coverage
areas of transmitters, repeaters, or cell sites, SPLAT! Data Files
need to be available for the entire region surrounding the site(s)
being analyzed.  SPLAT! Data Files can be placed under your SPLAT!
working directory, or under a separate directory specified in your
$HOME/.splat_path file so SPLAT! can easily find them.

Please read the README file under the utils directory for information
on the utilities included with SPLAT!.

Please read the documentation under the splat-1.4.2/docs directory,
or consult the program's man page for more information and examples
of SPLAT! use.

The latest announcements and information regarding SPLAT! software
can be found on the SPLAT! Web Site:

	http://www.qsl.net/kd2bd/splat.html

--
John A. Magliacane, KD2BD
XXX XXXX

