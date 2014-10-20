// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h> 
#include <tchar.h>
#include <stdlib.h>
#include <windows.h>
#include <atlbase.h> 
#include <atlstr.h> 
#include "gdal_priv.h"

//Decide wether to save the records of function calls or not.
#define _LOG_CALLS_

static FILE* fout = fopen("./TileLog.txt", "w");

#define LOG(buf) \
{\
			if (fout)\
				fprintf(fout, "%s ", buf); \
}


// TODO: reference additional headers your program requires here
