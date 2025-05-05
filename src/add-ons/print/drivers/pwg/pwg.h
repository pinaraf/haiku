/*
 * pwg.h
 * Copyright 2025 P.Ducroquet
 */
#ifndef __PWG_H
#define __PWG_H


#include "GraphicsDriver.h"


enum WhenEnum : uint32_t {
	Never = 0,
	AfterDocument = 1,
	AfterJob = 2,
	AfterSet = 3,
	AfterPage = 4
};

using Boolean = uint32_t;

struct Dimension {
	uint32_t x;
	uint32_t y;
};

enum EdgeEnum : uint32_t {
	ShortEdgeFirst = 0,
	LongEdgeFirst = 1
};

enum MediaPositionEnum : uint32_t {
};

enum OrientationEnum : uint32_t {
	Portrait = 0,
	Landscape = 1,
	ReversePortrait = 2,
	ReverseLandscape = 3
};

enum ColorOrderEnum : uint32_t {
	Chunky = 0
};

enum ColorSpaceEnum : uint32_t {
};

// What is this one?
using SrgbColor = uint32_t;

enum PrintQualityEnum : uint32_t {
	Default = 0,
	Draft = 3,
	Normal = 4,
	High = 5
};

struct PWGPageHeader {
	char PwgRaster[64];
	char MediaColor[64];
	char MediaType[64];
	char PrintContentOptimize[64];
	char padding1[12];
	WhenEnum CutMedia;
	Boolean Duplex;
	Dimension HWResolution;
	char padding2[16];
	Boolean InsertSheet;
	WhenEnum Jog;
	EdgeEnum LeadingEdge;
	char padding3[12];
	MediaPositionEnum MediaPosition;	
	uint32_t MediaWeightMetric;
	char padding4[8];
	uint32_t NumCopies;
	OrientationEnum Orientation;
	char padding5[4];
	Dimension PageSize;
	char padding6[8];
	Boolean Tumble;
	uint32_t Width;
	uint32_t Height;
	char padding7[4];
	uint32_t BitsPerColor;
	uint32_t BitsPerPixel;
	uint32_t BytesPerLine;
	ColorOrderEnum ColorOrder;
	ColorSpaceEnum ColorSpace;
	char padding8[16];
	uint32_t NumColors;
	char padding9[28];
	uint32_t TotalPageCount;
	int32_t CrossFeedTransform;
	int32_t FeedTransform;
	uint32_t ImageBoxLeft;
	uint32_t ImageBoxTop;
	uint32_t ImageBoxRight;
	uint32_t ImageBoxBottom;
	SrgbColor AlternatePrimary;
	PrintQualityEnum PrintQuality;
	char padding10[20];
	uint32_t VendorIdentifier;
	uint32_t VendorLength;
	char VendorData[1088];
	char padding11[64];
	char RenderingIntent[64];
	char PageSizeName[64];
};

static_assert(offsetof(PWGPageHeader, InsertSheet) == 300, "Invalid offset of field InsertSheet");
static_assert(offsetof(PWGPageHeader, ColorSpace) == 400, "Invalid offset of field ColorSpace");
static_assert(offsetof(PWGPageHeader, TotalPageCount) == 452, "Invalid offset of field TotalPageCount");
static_assert(offsetof(PWGPageHeader, AlternatePrimary) == 480, "Invalid offset of field AlternatePrimary");
static_assert(offsetof(PWGPageHeader, VendorData) == 516, "Invalid offset of field VendorData");
static_assert(sizeof(PWGPageHeader) == 1796, "Invalid size for the PWGPageHeader structure");

class PWGDriver : public GraphicsDriver {
public:
	PWGDriver(BMessage* message, PrinterData* printerData,
		const PrinterCap* printerCap);

protected:
	virtual	bool		StartDocument();
	virtual	bool		StartPage(int page);
	virtual	bool		NextBand(BBitmap* bitmap, BPoint* offset);
	virtual	bool		EndPage(int page);
	virtual	bool		EndDocument(bool success);

private:
	void dumpLine(int repeatCount, uint32_t *lineData, int length);
};

#endif // __PWG_H
