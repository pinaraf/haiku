/*
 * pwg.cpp
 * Copyright 2025 P.Ducroquet
 */


#include "pwg.h"

#include "DbgMsg.h"
#if 0
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include <Alert.h>
#include <Bitmap.h>
#include <File.h>
#include <Path.h>

#include "DbgMsg.h"
#include "FilterIO.h"
#include "Halftone.h"
#include "JobData.h"
#include "PackBits.h"
#include "PPDParser.h"
#include "PrinterData.h"
#include "PSCap.h"
#include "PSData.h"
#include "UIDriver.h"
#include "ValidRect.h"
#endif

#define dbg_msg(x) WriteSpoolData((x), strlen((x)))

PWGDriver::PWGDriver(BMessage* message, PrinterData* printerData,
	const PrinterCap* printerCap)
	:
	GraphicsDriver(message, printerData, printerCap)
{
}


bool
PWGDriver::StartDocument()
{
	DBGMSG(("> startDocument\n"));
	dbg_msg("> startDocument\n");
	return true;
}


bool
PWGDriver::StartPage(int page)
{
	DBGMSG(("> startPage\n"));
	dbg_msg("> startPage\n");
	return true;
}


bool
PWGDriver::EndPage(int)
{
	DBGMSG(("> endPage\n"));
	dbg_msg("> endPage\n");
	return true;
}


bool
PWGDriver::EndDocument(bool)
{
	DBGMSG(("> endDocument\n"));
	dbg_msg("> endDocument\n");
	return true;
}


bool
PWGDriver::NextBand(BBitmap* bitmap, BPoint* offset)
{
	DBGMSG(("> nextBand\n"));
	dbg_msg("> nextBand\n");
	
	offset->x = -1.0;
	offset->y = -1.0;
	return true;
}


