//---------------------------------------------------------------------------

#ifndef AboutH
#define AboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "LMDAboutDlg.hpp"
#include "LMDBaseDialog.hpp"
#include "LMDContainerComponent.hpp"
#include "LMDCustomComponent.hpp"
//---------------------------------------------------------------------------
class TAboutFrm : public TForm
{
__published:	// IDE-managed Components
private:	// User declarations
public:		// User declarations
  __fastcall TAboutFrm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutFrm *AboutFrm;
//---------------------------------------------------------------------------
#endif
