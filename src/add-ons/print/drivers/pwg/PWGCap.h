/*
 * pwgcap.h
 * Copyright 2025 P.Ducroquet
 */

#ifndef __PWGCAP_H
#define __PWGCAP_H


#include "PrinterCap.h"


class PWGCap : public PrinterCap {
public:
					PWGCap(const PrinterData* printer_data);
	virtual	int		CountCap(CapID) const;
	virtual bool	Supports(CapID) const;
	virtual const	BaseCap **GetCaps(CapID) const;
};

#endif // __PWGCAP_H
