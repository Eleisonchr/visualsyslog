#ifndef __SENDMAIL_H__
#define __SENDMAIL_H__

#include <IdSSLOpenSSL.hpp>
#include <IdExplicitTLSClientServerBase.hpp>
#include <IdSMTP.hpp>
#include <IdReplySMTP.hpp>

#include "unilist.h"
#include "letter.h"

/* ��� �������� ������ ����� ������� �������� ������� TLetter (��. letter.h)
 * � ��������� ��� �������� �����:
 *
 *   server - ����� ������� � ������ ������������ �� SMTP
 *   port   - ���� �������
 *   username - ��� ������������, ��� ��������������
 *   password - ������, ��� ��������������
 *   ssl - 0 ��� (������������ �����������),
 *         1 ������������ SSL ����� (������ 465 ����),
 *         2 ������������ TLS ����� ���� ��� ������ �������� (������ 587 ����)
 *
 *   sender - e-mail ����� �����������
 *   sender_name - ��� �����������
 *   recipient - e-mail ����� ����������
 *   subject - ���� ������
 *   message - ���������
 *
 *   callback - �������, ������� ����� ������� �� �������� ������ ��� ������.
 *              ���������:
 *                self - ������ TLetter; � ��� ���� error
 *                       ����� ����� � ��������� ������ ���� result == false
 *   closure - �� �� ����� ��� � callback, �� � ����������� ��������� �� ������.
 *             ��������� �������� �������, ���������� �������� �������.
 * 
 * ����� ����� ����� ������� ����������� ������� TSendmailThread::Send()
 * ��� ������� ����� �������� �����, �������� ������ � ������� �� �������� �
 * ������ ���������� ���������.
 * � ���������� �������� ������ ����� ������ � ������� callback / closure
 * ��� ���������� ���������� ����� ������� ����������� ������� TSendmailThread::Exit()
 * ��� ���������� ������ ������ �������� �����.
 */

//---------------------------------------------------------------------------
class TSendmailThread : public TThread
{
private:
  TIdSMTP * smtp;             // indy
  TIdSSLIOHandlerSocketOpenSSL * SSLIOHandler;
  TUniList<TLetter> letters;  // list of letters
  TLetter * SyncLetter;       // Pointer for sync()

private:
  bool SendInternal(TLetter * p);
  void __fastcall sync(void);
  void __fastcall iiso(char &VHeaderEncoding, AnsiString &VCharSet);

protected:
  void __fastcall Execute();

public:
  __fastcall TSendmailThread(void);
  __fastcall ~TSendmailThread();
  static void __fastcall Send(TLetter * p);
  static void __fastcall Exit(void);
};
//---------------------------------------------------------------------------
#endif

