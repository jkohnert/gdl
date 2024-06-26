/***************************************************************************
                       plotting_erase.cpp  -  GDL routines for plotting
                             -------------------
    begin                : July 22 2002
    copyright            : (C) 2002-2011 by Marc Schellens et al.
    email                : m_schellens@users.sf.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "includefirst.hpp"
#include "plotting.hpp"

namespace lib {

  using namespace std;

  class erase_call : public plotting_routine_call {
  private:

    bool handle_args(EnvT* e) {
      if (nParam() > 1) e->Throw("Incorrect number of arguments.");
      return false;
    }

    bool prepareDrawArea(EnvT* e, GDLGStream* actStream) {
      return false;
    }

  private:

    void applyGraphics(EnvT* e, GDLGStream* actStream) {
      bool printer = (((*static_cast<DLongGDL*> (SysVar::D()->GetTag(SysVar::D()->Desc()->TagIndex("FLAGS"), 0)))[0] & 512) == 512);
      if (printer) {
        actStream->eop();
        return;
      }
      DLong chan = 0;
      static int chanIx = e->KeywordIx("CHANNEL");
      if (e->KeywordPresent(chanIx)) {
        e->AssureLongScalarKWIfPresent(chanIx, chan);
        if ((chan > 3) || (chan < 0)) e->Throw("Value of Channel is out of allowed range.");
      } else { //get !P.CHANNEL value
        DStructGDL* pStruct = SysVar::P(); //MUST NOT BE STATIC, due to .reset 
        chan = (*static_cast<DLongGDL*>
          (pStruct->GetTag(pStruct->Desc()->TagIndex("CHANNEL"), 0)))[0];
      }

      DStructGDL* dStruct = SysVar::D(); //MUST NOT BE STATIC, due to .reset 
      DLong MaxColorIdx;
      MaxColorIdx = (*static_cast<DLongGDL*>
        (dStruct->GetTag(dStruct->Desc()->TagIndex("N_COLORS"), 0)))[0];

      DLong bColor = -1;
      static int bColorIx = e->KeywordIx("COLOR");

      if (nParam() == 0) {
        if (e->KeywordPresent(bColorIx)) {
          e->AssureLongScalarKWIfPresent(bColorIx, bColor);
        } else
          // we have to read back !p.background value
        {
          DStructGDL* pStruct = SysVar::P(); //MUST NOT BE STATIC, due to .reset 
          bColor = (*static_cast<DLongGDL*>
            (pStruct->GetTag(pStruct->Desc()->TagIndex("BACKGROUND"), 0)))[0];
        }
      } else {
        e->AssureLongScalarPar(0, bColor);
      }
      if (bColor > MaxColorIdx) bColor = MaxColorIdx;
      if (bColor < 0) bColor = 0;
      DLong decomposed = GraphicsDevice::GetDevice()->GetDecomposed();
      actStream->Background(bColor, decomposed);
      if (chan > 0) actStream->Clear(chan - 1);
      else actStream->Clear();
    }

  private:

    virtual void post_call(EnvT*, GDLGStream*) { }

  };

  void erase(EnvT* e) {
    erase_call erase;
    erase.call(e, 0);
  }

} // namespace
