//---------------------------------------------------------------------------
// ����� ��� ������ � ������� � WIN32.
// �����: ������� �.
//---------------------------------------------------------------------------
#ifndef SaturnFileH
#define SaturnFileH
//#include <windows.h> 
//#include <vcl.h>

/*
-------------------------- �������� ���������� ------------------------------
DWORD Access (������ � ����� (�������� ����������� �� OR)):

0             - �� ��� ������� (������ � ����������)
GENERIC_READ  - �� ������
GENERIC_WRITE - �� ������
-----------------------------------------------------------------------------
DWORD ShareMode (���������� ����� (�������� ����������� �� OR)):

0                 - ���� �� ����� �����������
FILE_SHARE_DELETE - For Windows NT only
FILE_SHARE_READ   - ���� ����� ���� ������ ���� ���� ���-�� ��� ��� ������
FILE_SHARE_WRITE  - ���� ����� ���� ������ ���� ���� ���-�� ��� ��� �����
-----------------------------------------------------------------------------
DWORD CreationDistribution (��� ��������� ����):

CREATE_NEW        - Creates a new file. The function fails if the specified file
                    already exists.
CREATE_ALWAYS     - Creates a new file. The function overwrites the file if it exists.
OPEN_EXISTING     - Opens the file. The function fails if the file does not exist.
OPEN_ALWAYS       - Opens the file, if it exists. If the file does not exist,
                    the function creates the file as if dwCreationDistribution
                    were CREATE_NEW.
TRUNCATE_EXISTING - Opens the file. Once opened, the file is truncated so that
                    its size is zero bytes. The calling process must open the
                    file with at least GENERIC_WRITE access. The function fails
                    if the file does not exist.
-----------------------------------------------------------------------------
DWORD FlagsAndAttributes (�������� ����� (�������� ����������� �� OR)):

FILE_ATTRIBUTE_NORMAL - ������������ ������ (��. Help [CreateFile])

FILE_FLAG_RANDOM_ACCESS   - ����� ��������� ���� ����������� (����������� �����������)
FILE_FLAG_SEQUENTIAL_SCAN - ����� ��������� ���� ���������������
FILE_FLAG_DELETE_ON_CLOSE - ������������ ������� ���� ����� �� ��������
FILE_FLAG_WRITE_THROUGH - �� ���������� ������
������ � ������ FILE_FLAG_WRITE_THROUGH ����������� ��������� �������� ���� !!!
-----------------------------------------------------------------------------
LONG Dist (����� ���� ������������� � �������������):

DWORD MoveMethod (�� ���� ���������� ��������� �����):
FILE_BEGIN   - �� ������
FILE_CURRENT - �� ������� ������� ���������
FILE_END     - �� �����
*/
// ������������ ����� ����� �����
#define MN 260
// �������� �������� ����� ��� ������
#define READ_FILE           GENERIC_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL
// �������� ����� ��� ������ � ����� ����
#define WRITE_FILE_NEW      GENERIC_WRITE,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL

class TFile
{
private:
  HANDLE handle;    //
  bool Error;       // ��������� �������� ����������� � ������� ?
  bool Exceptions;  // �������� ��������� ��� ������� ?
  DWORD LastError;  // GetLastError
  DWORD bytes;      // ���� ���������/�������� � ��������� ��������

public:
  __fastcall TFile(bool enableExceptions = false);
  __fastcall TFile(char * FileName, DWORD Access, DWORD ShareMode,
                   DWORD CreationDistribution,
                   DWORD FlagsAndAttributes,
                   bool enableExceptions = false);
  __fastcall TFile(String FileName, DWORD Access, DWORD ShareMode,
                   DWORD CreationDistribution,
                   DWORD FlagsAndAttributes,
                   bool enableExceptions = false);

  __fastcall ~TFile();

public:
  bool __fastcall Open(char * FileName, DWORD Access, DWORD ShareMode,
                       DWORD CreationDistribution, DWORD FlagsAndAttributes);
  bool __fastcall Open(String FileName, DWORD Access, DWORD ShareMode,
                       DWORD CreationDistribution, DWORD FlagsAndAttributes);
  bool __fastcall Close(void);
  bool __fastcall IsOpen(void);

  bool __fastcall Write(BYTE * data, DWORD size);
  bool __fastcall Read(BYTE * data, DWORD size);

  bool __fastcall Flush(void);

  // ���������� ��������� ��������� � ����� ������������ ��� ������
  bool __fastcall SetPointer(LONG Dist);
  // ���������� ��������� ��������� � �����
  bool __fastcall SetPointer(LONG Dist, DWORD MoveMethod);
  // �������� ��������� ��������� � �����
  LONG __fastcall GetPointer(void);

  // �������� ������ �����
  DWORD __fastcall GetSize(void);
  // ���������� ����� ����� �� ������� ������� ���������
  // (����� ����������� � ��������� �����)
  // ���� ������ ���� ������ ��� GENERIC_WRITE
  bool __fastcall SetEnd(void);

  // �������� ������ string � �������� "\r\n"
  bool __fastcall WriteString(char * string);
  bool __fastcall WriteString(AnsiString string);
  //bool __fastcall WriteString(String string);
  bool WriteHex(char * title, void * data, DWORD size);

  // ��������� �� ����� ������ (�� \r\n)
  bool __fastcall ReadString(char * data, DWORD size);
  bool __fastcall ReadString(AnsiString & data, DWORD maxsize);

  DWORD __fastcall GetDosTime(void);
  bool __fastcall SetDosTime(DWORD t);

  // �������� ��������� ��������� �����
  bool __fastcall GetError(void) { return Error; }
  HANDLE __fastcall GetHandle(void) { return handle; }

  // �������� ���������� � �����
  bool GetInfo(BY_HANDLE_FILE_INFORMATION * p);

  // The file handle must have GENERIC_READ access right
  double GetModificationTime(void);

  // �������� ��������� �� ������
  String GetErrorMessage(void);

public:
  __property LONG Pointer = {read=GetPointer, write=SetPointer };
  __property DWORD Bytes = {read=bytes};

  __property DWORD ErrorCode = {read=LastError};
  __property String ErrorMessage = {read=GetErrorMessage};

public:
  bool operator ! () { return GetError(); }

  TFile & operator << (char * string) { Write((BYTE *)string, lstrlen(string)); return *this; }
  TFile & operator << (AnsiString string) { Write((BYTE *)string.c_str(), string.Length()); return *this; }
  TFile & operator << (char i) { Write((BYTE *)&i, sizeof(i)); return *this; }
  TFile & operator << (int i)  { Write((BYTE *)&i, sizeof(i)); return *this; }
  TFile & operator << (double i)  { Write((BYTE *)&i, sizeof(i)); return *this; }

  TFile & operator >> (char & i) { Read((BYTE *)&i, sizeof(i)); return *this; }
  TFile & operator >> (BYTE & i) { Read((BYTE *)&i, sizeof(i)); return *this; }
  TFile & operator >> (int & i) { Read((BYTE *)&i, sizeof(i)); return *this; }
};
//---------------------------------------------------------------------------
// szName �������� ������� ������ ����� ?
bool IsPath(char * szName);
// �������� � out ����� ���������� ��������� ����� name
// ULONGLONG==__int64==FILETIME
// ������� � TDateTime: TDateTime  TFileTimeToDateTime((ULONGLONG *));
bool GetFileModificationTime(AnsiString name, ULONGLONG & out);
//---------------------------------------------------------------------------
extern char CR[];
//---------------------------------------------------------------------------
#endif

