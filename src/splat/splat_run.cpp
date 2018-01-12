/****************************************************************************\
*	   SPLAT!: An RF Signal Path Loss And Terrain Analysis Tool          *
******************************************************************************
*	     Project started in 1997 by John A. Magliacane, KD2BD 	     *
*			  Last update: 07-Feb-2014			     *
******************************************************************************
*         Please consult the documentation for a complete list of	     *
*	     individuals who have contributed to this project. 		     *
******************************************************************************
*                                                                            *
*  This program is free software; you can redistribute it and/or modify it   *
*  under the terms of the GNU General Public License as published by the     *
*  Free Software Foundation; either version 2 of the License or any later    *
*  version.								     *
* 									     *
*  This program is distributed in the hope that it will useful, but WITHOUT  *
*  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or     *
*  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License     *
*  for more details.							     *
*									     *
\****************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <bzlib.h>
#include <vector>
#include <string>
#include "site.h"
#include "path.h"
#include "dem.h"
#include "itwom3.0.h"
#include "splat_run.h"

using namespace std;

const std::string SplatRun::splat_name = "SPLAT!";
const std::string SplatRun::splat_version = "1.4.2";
