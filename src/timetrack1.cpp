//---------------------------------------------------------------------------
//(c) sivann 5/2004
#include <vcl.h>
#pragma hdrstop

#include "timetrack1.h"

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

#include "Unit2.h"
#include "About.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGrid"
#pragma link "BaseGrid"
#pragma link "Trayicon"
#pragma link "LMDAboutDlg"
#pragma link "LMDBaseDialog"
#pragma link "LMDContainerComponent"
#pragma link "LMDCustomComponent"
#pragma link "ChartLink"
#pragma link "lmdcont"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
BOOL GetWindowFileName (HWND hwnd,LPSTR fn,int fnlen);
void ExitApp();
//---------------------------------------------------------------------------
int wantidle=1;

//---------------------------------------------------------------------------
BOOL GetWindowFileName (HWND hwnd,LPSTR fn,int fnlen)
{
    HANDLE         hProcessSnap ;
    BOOL           bRet         ;
    PROCESSENTRY32 pe32         = {0};
    DWORD pid;

    GetWindowThreadProcessId( hwnd, &pid);

    // Take a snapshot of all processes currently in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == (HANDLE)-1)
        return (FALSE);

    // Size of the PROCESSENTRY32 structure must be filled out before use.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Walk the snapshot of processes and for each process, get information
    // to display.
    if (Process32First(hProcessSnap, &pe32)) {
      //BOOL          bGotModule = FALSE;
      //MODULEENTRY32 me32       = {0};
      
      do {
          //bGotModule = GetProcessModule(pe32.th32ProcessID,
          //             pe32.th32ModuleID,&me32,sizeof(MODULEENTRY32));
          // if (bGotModule && (pe32.th32ProcessID == pid)) {
          if (pe32.th32ProcessID == pid) {
             strncpy(fn,pe32.szExeFile,fnlen);
             break;
           }
      } while (Process32Next(hProcessSnap, &pe32));
      bRet = TRUE;
    }
    else
        bRet = FALSE;    // Couldn't walk the list of processes.

    // Don't forget to clean up the snapshot object...
    CloseHandle (hProcessSnap);
    return (bRet);
}
//------------------------------------------------
DWORD _stdcall getlastenentTime()
{   bool res;
    LASTINPUTINFO lInput;
    LASTINPUTINFO* lInputp = &lInput;
    lInput.cbSize = sizeof(LASTINPUTINFO);
    res=GetLastInputInfo(lInputp);
    DWORD idleTime = lInput.dwTime;
    if (res) return idleTime;
    else return 0;
}
//------------------------------------------------

//------------------------------------------------
void __fastcall TForm1::GetFgAppName(TObject *Sender)
{
HWND fghandle;
char buff[512];
DWORD pid;
DWORD lastevent;
DWORD idletime;
int row;
TFindParams findparams;
TPoint res;
long len;
long ts;

 buff[0]=0;

 lastevent=getlastenentTime();
 idletime = (GetTickCount()-lastevent) / 1000; //secs

 StatusBar1->SimpleText="Idle: "+AnsiString(idletime) + "/" + AnsiString(120);
 if (wantidle && (idletime>120)) {
   StatusBar1->SimpleText=StatusBar1->SimpleText+"  paused due to 120 seconds of inactivity";
   TrayIcon1->IconIndex=1;
   return;
 }
 TrayIcon1->IconIndex=0;
 fghandle=GetForegroundWindow();
 GetWindowFileName(fghandle,buff,256);
 if (strlen(buff)<3) strcpy(buff,"unknown");

 Form1->Grid->Col=0;
 findparams << fnFindInCurrentCol << fnMatchStart	<< fnMatchFull;
 res=Form1->Grid->FindFirst(buff, findparams);
 if (res.y>0)
   row=res.y;
 else {
   row=Grid->RowCount-1;
   len=Form1->Grid->Cells[0][row].Length();
   if ((row>1) || (row==1 && len!=0))
     {Grid->RowCount++;row++;}

 }
 Form1->Grid->Cells[0][row]=buff;
 //session time +1
 ts=atol(AnsiString(Form1->Grid->Cells[1][row]).c_str());
 ts++;
 Form1->Grid->Cells[1][row]=ts;

 //total time +1
 ts=atol(AnsiString(Form1->Grid->Cells[2][row]).c_str());
 ts++;
 Form1->Grid->Cells[2][row]=ts;


}
//---------------------------------------------------------------------------
void LoadGrid()
{
  if (FileExists("timetrack.csv"))
    Form1->Grid->LoadFromCSV("timetrack.csv");

}

void SaveGrid()
{
   Form1->Grid->SaveToCSV("timetrack.csv");
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Restor1Click(TObject *Sender)
{
    if(!Application->Terminated) {
        ShowWindow(Application->Handle, SW_SHOW);
        Visible = true;
        //TrayIcon1->Visible=false;
    }
    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TrayIcon1Restore(TObject *Sender)
{
    if(!Application->Terminated) {
        ShowWindow(Application->Handle, SW_SHOW);
        Visible = true;
//        TrayIcon1->Visible=false;
    }

}
//---------------------------------------------------------------------------
void __fastcall TForm1::s1Click(TObject *Sender)
{
  SaveGrid();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Load1Click(TObject *Sender)
{
  LoadGrid();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Exit1Click(TObject *Sender)
{
  ExitApp();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
    if(!Application->Terminated)
    {   //TrayIcon1->Visible=true;
        ShowWindow(Application->Handle, SW_HIDE);
        Visible = false;
        Action = caNone;
    }
}


//---------------------------------------------------------------------------
void ShowHideChart_S()
{
long ts;
long totalts=0;
  if (Form2->Visible) {
    Form2->Visible=false;
    return;
  }
  Form1->Grid->SortByColumn(1);
  Form2->Visible=true;
  Form2->Series1->Clear();
  for (int i=1;i<Form1->Grid->RowCount;i++) {
    ts=atol(AnsiString(Form1->Grid->Cells[1][i]).c_str());
    if (!ts) continue;
    totalts+=ts;
    Form2->Series1->Add(ts,Form1->Grid->Cells[0][i],clTeeColor);
  }
   Form2->Chart1->Title->Text->Text="Running Time Percentages of " + AnsiString(totalts) + " seconds";
}


//---------------------------------------------------------------------------
void ShowHideChart_T()
{
long ts;
long totalts=0;
int i;

  if (Form2->Visible) {
    Form2->Visible=false;
    return;
  }

  Form1->Grid->SortByColumn(2);
  Form2->Visible=true;
  Form2->Series1->Clear();

  for (i=1;i<Form1->Grid->RowCount;i++) {
    Form1->StatusBar1->SimpleText="RowCount="+AnsiString(Form1->Grid->RowCount)+" i="+AnsiString(i);
    ts=atol(AnsiString(Form1->Grid->Cells[2][i]).c_str());
    if (!ts) continue;
    totalts+=ts;
    Form2->Series1->Add(ts,Form1->Grid->Cells[0][i],clTeeColor);
  }
   Form2->Chart1->Title->Text->Text="Running Time Percentages of " + AnsiString(totalts) + " seconds";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ShowHideCumulativeChart1Click(TObject *Sender)
{
  ShowHideChart_T();  
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ShowHideChart1Click(TObject *Sender)
{
  ShowHideChart_S();
}

//---------------------------------------------------------------------------


void __fastcall TForm1::Start1Click(TObject *Sender)
{
  Timer1->Enabled=true;
  Stop1->Enabled=true;
  Start1->Enabled=false;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Stop1Click(TObject *Sender)
{
  Timer1->Enabled=false;
  Stop1->Enabled=false;
  Start1->Enabled=true;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::AutoStopClick(
      TObject *Sender)
{
  AutoStop->Checked=!AutoStop->Checked;
  wantidle=!wantidle;
}
//---------------------------------------------------------------------------



void __fastcall TForm1::About1Click(TObject *Sender)
{
  AboutDlg->Execute();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Save1Click(TObject *Sender)
{
  SaveGrid();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Load2Click(TObject *Sender)
{
  LoadGrid();  
}
//---------------------------------------------------------------------------
void ExitApp()
{
//reset session data
//  for (int i=1;i<Form1->Grid->RowCount;i++)
//    Form1->Grid->Cells[1][i]=0;
  SaveGrid();
  Application->Terminate();
}
  
void __fastcall TForm1::Exit2Click(TObject *Sender)
{
  ExitApp();
}

void __fastcall TForm1::ResetSessionTime1Click(TObject *Sender)
{

 if (MessageDlg("Are you sure?", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) != mrYes)
  return;

 //reset session data
 for (int i=1;i<Form1->Grid->RowCount;i++)
    Form1->Grid->Cells[1][i]=0;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ResetTotalTime1Click(TObject *Sender)
{

if (MessageDlg("Are you sure?", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) != mrYes)
 return;


 //reset total data
 for (int i=1;i<Form1->Grid->RowCount;i++) {
   Form1->Grid->Cells[1][i]=0;
   Form1->Grid->Cells[2][i]=0;
 }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
  LoadGrid();
  Timer3->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if (Key == VK_ESCAPE )
 {
   Form1->Close();
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer3Timer(TObject *Sender)
{
   Timer3->Enabled=false;

   ShowWindow(Application->Handle, SW_HIDE);
   Visible = false;
   //TrayIcon1->Visible=true;

}
//---------------------------------------------------------------------------



