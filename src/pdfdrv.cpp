/*
 * Grace - GRaphing, Advanced Computation and Exploration of data
 *
 * Home page: http://plasma-gate.weizmann.ac.il/Grace/
 *
 * Copyright (c) 1996-2004 Grace Development Team
 *
 * Maintained by Evgeny Stambulchik
 *
 * Modified by Andreas Winter 2008-2012
 *
 *                           All Rights Reserved
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * Grace PDF driver
 */

///#include <config.h>

#include "defines.h"


#include "device.h"
#include "devlist.h"
#include "svgdrv.h"


static Device_entry dev_pdf = {
    DEVICE_FILE,
    "PDF",
    svginitgraphics,
    NULL,
    NULL,
    "pdf",
    TRUE,
    TRUE,
    {DEFAULT_PAGE_WIDTH, DEFAULT_PAGE_HEIGHT, 72},
    NULL
};

int register_pdf_drv(void)
{    
    return register_device(dev_pdf);
}

