/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 1992-2013 jp.charras at wanadoo.fr
 * Copyright (C) 2013 SoftPLC Corporation, Dick Hollenbeck <dick@softplc.com>
 * Copyright (C) 1992-2016 KiCad Developers, see AUTHORS.TXT for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */


#include <algorithm>
#include <fctsys.h>
#include <build_version.h>
#include <confirm.h>

#include <sch_edit_frame.h>
#include <xnode.h>
#include <connection_graph.h>
#include "netlist_exporter_kicad.h"

bool NETLIST_EXPORTER_KICAD::WriteNetlist( const wxString& aOutFileName, unsigned aNetlistOptions )
{
    try
    {
        FILE_OUTPUTFORMATTER formatter( aOutFileName );
        Format( &formatter, GNL_ALL );
    }

    catch( const IO_ERROR& ioe )
    {
        DisplayError( NULL, ioe.What() );
        return false;
    }

    return true;
}


void NETLIST_EXPORTER_KICAD::Format( OUTPUTFORMATTER* aOut, int aCtl )
{
    std::unique_ptr<XNODE> xroot( makeRoot( aCtl ) );

    xroot->Format( aOut, 0 );
}
