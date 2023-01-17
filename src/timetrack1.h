//---------------------------------------------------------------------------

#ifndef timetrack1H
#define timetrack1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "AdvGrid.hpp"
#include "BaseGrid.hpp"
#include <Grids.hpp>
#include "Trayicon.h"
#include <ImgList.hpp>
#include <Menus.hpp>
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <ComCtrls.hpp>
#include "LMDAboutDlg.hpp"
#include "LMDBaseDialog.hpp"
#include "LMDContainerComponent.hpp"
#include "LMDCustomComponent.hpp"
#include "ChartLink.hpp"
#include "lmdcont.hpp"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
  TTimer *Timer1;
  TAdvStringGrid *Grid;
  TTrayIcon *TrayIcon1;
  TImageList *ImageList1;
  TPopupMenu *PopupMenu1;
  TMenuItem *Exit1;
  TMenuItem *Restor1;
  TMainMenu *MainMenu1;
  TMenuItem *File1;
  TMenuItem *Exit2;
  TMenuItem *Actions1;
  TMenuItem *ShowHideChart1;
  TStatusBar *StatusBar1;
  TMenuItem *Timer2;
  TMenuItem *Start1;
  TMenuItem *Stop1;
  TMenuItem *N2;
  TMenuItem *AutoStop;
  TMenuItem *Help1;
  TMenuItem *About1;
  TLMDAboutDlg *AboutDlg;
  TMenuItem *N3;
  TMenuItem *ResetSessionTime1;
  TMenuItem *ResetTotalTime1;
  TMenuItem *ShowHideCumulativeChart1;
  TTimer *Timer3;
  void __fastcall GetFgAppName(TObject *Sender);
  void __fastcall Restor1Click(TObject *Sender);
  void __fastcall s1Click(TObject *Sender);
  void __fastcall Load1Click(TObject *Sender);
  void __fastcall Exit1Click(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall TrayIcon1Restore(TObject *Sender);
  void __fastcall Load2Click(TObject *Sender);
  void __fastcall Exit2Click(TObject *Sender);
  void __fastcall Start1Click(TObject *Sender);
  void __fastcall Stop1Click(TObject *Sender);
  void __fastcall AutoStopClick(
          TObject *Sender);
  void __fastcall ShowHideChart1Click(TObject *Sender);
  void __fastcall About1Click(TObject *Sender);
  void __fastcall Save1Click(TObject *Sender);
  void __fastcall ResetSessionTime1Click(TObject *Sender);
  void __fastcall ResetTotalTime1Click(TObject *Sender);
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall ShowHideCumulativeChart1Click(TObject *Sender);
  void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
  void __fastcall Timer3Timer(TObject *Sender);
private:	// User declarations
public:		// User declarations

  __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
