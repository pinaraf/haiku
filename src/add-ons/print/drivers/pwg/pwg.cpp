/*
 * pwg.cpp
 * Copyright 2025 P.Ducroquet
 */


#include "pwg.h"

#include "DbgMsg.h"
#include <inttypes.h>

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
#endif
#include "ValidRect.h"

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
	WriteSpoolData("RaS2", 4);
	return true;
}


bool
PWGDriver::StartPage(int page)
{
	// TODO: map from job data to the header
	PWGPageHeader header;
	memset(&header, 97, sizeof(PWGPageHeader)); // 'a' everywhere
	// memset(&header, 0, sizeof(PWGPageHeader));
	DBGMSG(("> startPage\n"));
	WriteSpoolData(&header, sizeof(PWGPageHeader));
	return true;
}


bool
PWGDriver::EndPage(int)
{
	DBGMSG(("> endPage\n"));
	return true;
}


bool
PWGDriver::EndDocument(bool)
{
	DBGMSG(("> endDocument\n"));
	return true;
}


bool
PWGDriver::NextBand(BBitmap* bitmap, BPoint* offset)
{
	DBGMSG(("> nextBand\n"));

	BRect bounds = bitmap->Bounds();

	DBGMSG((" this band has %d bytes\n", bitmap->BitsLength()));

	RECT rc;
	rc.left = (int)bounds.left;
	rc.top = (int)bounds.top;
	rc.right = (int)bounds.right;
	rc.bottom = (int)bounds.bottom;

	int height = rc.bottom - rc.top + 1;

	int x = (int)offset->x;
	int y = (int)offset->y;

	int page_height = GetPageHeight();

	if (y + height > page_height) {
		height = page_height - y;
	}

	rc.bottom = height - 1;

	if (get_valid_rect(bitmap, &rc)) {
		DBGMSG(("validate rect = %d, %d, %d, %d\n",
                                rc.left, rc.top, rc.right, rc.bottom));
		x = rc.left;
		y += rc.top;
		
		int width = rc.right - rc.left + 1;
		int height = rc.bottom - rc.top + 1;
		
		DBGMSG(("x = %d\n", x));
		DBGMSG(("y = %d\n", y));
		DBGMSG(("width = %d\n", width));
		DBGMSG(("height = %d\n", height));
		
		DBGMSG(("color_space = %d\n", bitmap->ColorSpace()));
		DBGMSG(("bpr = %d\n", bitmap->BytesPerRow()));
		
		
		if (bitmap->ColorSpace() == B_RGB32) {
			DBGMSG(("B_RGB32, let's go\n"));
			// Skip lines before rc.top
			
			uint32_t *bitmapData = (uint32_t *) bitmap->Bits();
			// Handle valid lines
			int pixelsPerLine = bounds.right - bounds.left + 1;
			int validBytesPerRow = (rc.right - rc.left) * 4;
			DBGMSG(("validBytesPerRow = %d\n", validBytesPerRow));
			uint32_t *lineData = new uint32_t[validBytesPerRow / 4];
			memset(lineData, 0, validBytesPerRow);
			int lineRepeatCount = -1;
			
			for (int line = rc.top ; line < rc.bottom ; line++) {
				//DBGMSG(("Checking line %d\n", line));
				if ((memcmp(lineData, &(bitmapData[line*pixelsPerLine + rc.left]), validBytesPerRow) == 0) && (lineRepeatCount < 255))
				{
					lineRepeatCount++;
				} else {
					// flush previous line if needed
					if (lineRepeatCount > -1) {
						dumpLine(lineRepeatCount, lineData, validBytesPerRow / 4);
					}
					// Store new line info
					memcpy(lineData, &(bitmapData[line*pixelsPerLine + rc.left]), validBytesPerRow);
					lineRepeatCount = 0;
				}
			}
			if (lineRepeatCount > -1) {
				// flush final lines
				dumpLine(lineRepeatCount, lineData, validBytesPerRow / 4);
			}
			
			delete[] lineData;
			
			// Skip lines after rc.bottom
		}

	} else {
		DBGMSG(("Could not get a valid rect, weird\n"));
	}

	
	offset->x = -1.0;
	offset->y = -1.0;


	return true;
}

void PWGDriver::dumpLine(int repeatCount, uint32_t *lineData, int length)
{
	uint8_t truncatedRepeatCount = repeatCount;
	WriteSpoolData(&truncatedRepeatCount, 1);

	DBGMSG(("Repeat %d:", repeatCount));
	uint32_t currentByte = lineData[0];
	int repeat = 0;
	for (int col = 1 ; col < length ; col++) {
		if ((currentByte == lineData[col]) && (repeat < 255)) {
			repeat++;
		} else {
			truncatedRepeatCount = repeat;
			WriteSpoolData(&truncatedRepeatCount, 1);
			WriteSpoolData(((char*)&currentByte)+1, 3);
			
			DBGMSG(("%d * %x ; " B_PRIu32, repeat, currentByte));
			currentByte = lineData[col];
			repeat = 0;
		}
		//DBGMSG(("%x" B_PRIu32, lineData[col]));
	}
	truncatedRepeatCount = repeat;
	WriteSpoolData(&truncatedRepeatCount, 1);
	WriteSpoolData(((char*)&currentByte)+1, 3);
	DBGMSG(("%d * %x" B_PRIu32, repeat, currentByte));
	DBGMSG(("\n"));
}


/*
****************
page_width  = 2363
page_height = 3388
band_count  = 1
band_height = 3388
****************
nup        = 1
page_count = 1
reverse    = false
> startDocument
page index = 0
> startPage
picture_count = 1
construct PictureData
1: current seek position = 0x651
picture_data::point = 0.000000, 0.000000
picture_data::rect = 14.400001, 14.400001, 0.000000, 3.000000
2: current seek position = 0x669
3: current seek position = 0x82d
> nextBand
validate rect = 17, 20, 131, 58
x = 17
y = 20
width = 115
height = 39
> endPage
> endDocument
	************ START - DUMP BMessage ***********
	address: 0xc916ad9e00
	msg->what: okok
	************ END - DUMP BMessage ***********
 */
