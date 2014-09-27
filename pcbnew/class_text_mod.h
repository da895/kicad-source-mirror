/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2004 Jean-Pierre Charras, jaen-pierre.charras@gipsa-lab.inpg.com
 * Copyright (C) 1992-2011 KiCad Developers, see AUTHORS.txt for contributors.
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

/**
 * @file class_text_mod.h
 * @brief Footprint text class description.
 */


#ifndef TEXT_MODULE_H_
#define TEXT_MODULE_H_


#include <eda_text.h>
#include <class_board_item.h>


class LINE_READER;
class EDA_RECT;
class EDA_DRAW_PANEL;
class MODULE;
class MSG_PANEL_ITEM;


#define UMBILICAL_COLOR   LIGHTBLUE


class TEXTE_MODULE : public BOARD_ITEM, public EDA_TEXT
{
public:
    /** Text module type: there must be only one (and only one) for each
     * of the reference and value texts in one module; others could be
     * added for the user (DIVERS is French for 'others'). Reference and
     * value always live on silkscreen (on the module side); other texts
     * are planned to go on whatever layer the user wants (except
     * copper, probably) */
    enum TEXT_TYPE
    {
        TEXT_is_REFERENCE = 0,
        TEXT_is_VALUE = 1,
        TEXT_is_DIVERS = 2
    };

    TEXTE_MODULE( MODULE* parent, TEXT_TYPE text_type = TEXT_is_DIVERS );

    // Do not create a copy constructor.  The one generated by the compiler is adequate.

    ~TEXTE_MODULE();

    static inline bool ClassOf( const EDA_ITEM* aItem )
    {
        return aItem && PCB_MODULE_TEXT_T == aItem->Type();
    }

    virtual const wxPoint& GetPosition() const
    {
        return m_Pos;
    }

    virtual void SetPosition( const wxPoint& aPos )
    {
        m_Pos = aPos;
        SetLocalCoord();
    }

    void Move( const wxPoint& aMoveVector )
    {
        m_Pos += aMoveVector;
        SetLocalCoord();
    }

    void Rotate( const wxPoint& aRotCentre, double aAngle );

    void Flip( const wxPoint& aCentre );

    /// Rotate text during module rotation transform, in footprint editor
    void RotateTransformWithModule( const wxPoint& aOffset, double aAngle );

    /// Flip entity during module flip
    void FlipWithModule( int aOffset );

    /// Mirror text during module mirroring transform, in footprint editor
    /// the text itself is not mirrored, only position.
    void MirrorTransformWithModule( int aOffset );

    /// move text during module mirroring transform, in footprint editor
    void MoveTransformWithModule( const wxPoint& aMoveVector );

    /// @deprecated it seems (but the type is used to 'protect'
    //  reference and value from deletion, and for identification)
    void SetType( TEXT_TYPE aType )     { m_Type = aType; }
    TEXT_TYPE GetType() const           { return m_Type; }

    void SetVisible( bool isVisible )   { m_NoShow = !isVisible; }
    bool IsVisible() const              { return !m_NoShow; }

    // The Pos0 accessors are for module-relative coordinates
    void SetPos0( const wxPoint& aPos ) { m_Pos0 = aPos; SetDrawCoord(); }
    const wxPoint& GetPos0() const      { return m_Pos0; }

    void Copy( TEXTE_MODULE* source ); // copy structure

    int GetLength() const;        // text length

    /**
     * @return the text rotation for drawings and plotting
     * the footprint rotation is taken in account
     */
    double GetDrawRotation() const;

    // Virtual function
    const EDA_RECT GetBoundingBox() const;

    ///> Set absolute coordinates.
    void SetDrawCoord();

    ///> Set relative coordinates.
    void SetLocalCoord();

    /* drawing functions */
    void Draw( EDA_DRAW_PANEL* panel,
               wxDC*           DC,
               GR_DRAWMODE     aDrawMode,
               const wxPoint&  offset = ZeroOffset );

    /**
     * Function DrawUmbilical
     * draws a line from the TEXTE_MODULE origin
     * to parent MODULE origin.
     * @param aPanel = the current DrawPanel
     * @param aDC = the current device context
     * @param aDrawMode = drawing mode, typically GR_XOR
     * @param aOffset = offset for TEXTE_MODULE
     */
    void DrawUmbilical( EDA_DRAW_PANEL* aPanel,
                        wxDC*           aDC,
                        GR_DRAWMODE     aDrawMode,
                        const wxPoint&  aOffset = ZeroOffset );

    void GetMsgPanelInfo( std::vector< MSG_PANEL_ITEM >& aList );

    bool HitTest( const wxPoint& aPosition ) const;

    wxString GetClass() const
    {
        return wxT( "MTEXT" );
    }

    wxString GetSelectMenuText() const;

    BITMAP_DEF GetMenuImage() const { return  footprint_text_xpm; }

    EDA_ITEM* Clone() const;

    /// @copydoc VIEW_ITEM::ViewBBox()
    virtual const BOX2I ViewBBox() const;

    /// @copydoc VIEW_ITEM::ViewGetLayers()
    virtual void ViewGetLayers( int aLayers[], int& aCount ) const;

#if defined(DEBUG)
    virtual void Show( int nestLevel, std::ostream& os ) const { ShowDummy( os ); }    // override
#endif

private:
    /* Note: orientation in 1/10 deg relative to the footprint
     * Physical orient is m_Orient + m_Parent->m_Orient
     */

    TEXT_TYPE m_Type;       ///< 0=ref, 1=val, etc.
    bool      m_NoShow;     ///< true = invisible

    wxPoint   m_Pos0;       ///< text coordinates relatives to the footprint anchor, orient 0.
                            ///< text coordinate ref point is the text centre
};

#endif // TEXT_MODULE_H_
