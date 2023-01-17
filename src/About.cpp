//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "About.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "LMDAboutDlg"
#pragma link "LMDBaseDialog"
#pragma link "LMDContainerComponent"
#pragma link "LMDCustomComponent"
#pragma resource "*.dfm"
TAboutFrm *AboutFrm;
//---------------------------------------------------------------------------
__fastcall TAboutFrm::TAboutFrm(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
