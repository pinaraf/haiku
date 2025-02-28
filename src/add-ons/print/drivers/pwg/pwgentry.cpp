/*
 * pwgentry.cpp
 * Copyright 2025 P.Ducroquet
 */


#include <Entry.h>
#include <Path.h>

#include "PrinterDriver.h"
#include "pwg.h"
#include "PWGCap.h"


class PWGPrinterDriver : public PrinterDriver {
public:
	PWGPrinterDriver(BNode* printerFolder)
	:
	PrinterDriver(printerFolder)
	{
	}

	const char*	GetSignature() const
				{
					return "application/x-vnd.pwg-compatible";
				}

	const char*	GetDriverName() const
				{ 
					return "PWG";
				}

	const char*	GetVersion() const
				{
					return "0.1";
				}

	const char*	GetCopyright() const
				{
					return "PWG driver Copyright © 2025 Pierre Ducroquet.\n";
				}

	char*		AddPrinter(char *printerName)
				{
					// Accept any printer
					return printerName;
				}

	PrinterData*	InstantiatePrinterData(BNode* node)
					{
						return new PrinterData(node);
					}

	PrinterCap*		InstantiatePrinterCap(PrinterData* printerData) 
					{
						return new PWGCap(printerData);
					}

	GraphicsDriver*	InstantiateGraphicsDriver(BMessage* settings,
						PrinterData* printerData, PrinterCap* printerCap)
					{
						return new PWGDriver(settings, printerData, printerCap);
					}
};


PrinterDriver *
instantiate_printer_driver(BNode* printerFolder)
{
	return new PWGPrinterDriver(printerFolder);
}
