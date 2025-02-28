/*
 * pwg.h
 * Copyright 2025 P.Ducroquet
 */
#ifndef __PWG_H
#define __PWG_H


#include "GraphicsDriver.h"



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
};

#endif // __PWG_H
