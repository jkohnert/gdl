/***************************************************************************
                          gdlexception.cpp  -  exception handling
                             -------------------
    begin                : July 22 2002
    copyright            : (C) 2002 by Marc Schellens
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

#include "gdlexception.hpp"

#include "dnode.hpp"
#include "initsysvar.hpp"
#include "gdljournal.hpp"
#include "dinterpreter.hpp"

//#define GDL_DEBUG
//#undef GDL_DEBUG

using namespace std;

DInterpreter *GDLException::interpreter = nullptr;

string GDLException::Name(BaseGDL *b) {
  if (interpreter != nullptr && !DInterpreter::CallStack().empty())
    return DInterpreter::Name(b);
  return "";
}

GDLException::GDLException(DLong eC, const string &s, bool pre, bool decorate) :
    ANTLRException(s),
    errorNode(static_cast<RefDNode>(antlr::nullAST)),
    errorCode(eC),
    col(0),
    prefix(pre),
    ioException(false) {
  if (decorate && interpreter != nullptr && !DInterpreter::CallStack().empty()) {
    EnvBaseT *e = DInterpreter::CallStack().back();
    errorNodeP = e->CallingNode();
    if (e->GetProName() != "$MAIN$")
      msg = std::runtime_error(e->GetProName() + ": " + s);
  }
  // note: This is for cases, when form a destructor is thrown
  // in these cases, program aborts
#ifdef GDL_DEBUG
  cerr << s << endl;
#endif
}
GDLException::GDLException(const string &s, bool pre, bool decorate) :
    ANTLRException(s),
    errorNode(static_cast<RefDNode>(antlr::nullAST)),
    errorCode(-1L),
    col(0),
    prefix(pre),
    ioException(false) {
  if (decorate && interpreter != nullptr && !DInterpreter::CallStack().empty()) {
    EnvBaseT *e = DInterpreter::CallStack().back();
    errorNodeP = e->CallingNode();
    if (e->GetProName() != "$MAIN$")
      msg = std::runtime_error(e->GetProName() + ": " + s);
  }
  // note: This is for cases, when form a destructor is thrown
  // in these cases, program aborts
#ifdef GDL_DEBUG
  cerr << s << endl;
#endif
}

GDLException::GDLException(const RefDNode &eN, const string &s) :
    ANTLRException(s),
    errorNode(eN),
    errorCode(-1L),
    col(0),
    prefix(true),
    ioException(false) {
  if (interpreter != nullptr && !DInterpreter::CallStack().empty()) {
    EnvBaseT *e = DInterpreter::CallStack().back();
    errorNodeP = e->CallingNode();
    if (e->GetProName() != "$MAIN$")
      msg = std::runtime_error(e->GetProName() + ": " + s);
  }
#ifdef GDL_DEBUG
  cerr << s << endl;
#endif
}
GDLException::GDLException(DLong eC, const RefDNode &eN, const string &s) :
    ANTLRException(s),
    errorNode(eN),
    errorCode(eC),
    col(0),
    prefix(true),
    ioException(false) {
  if (interpreter != nullptr && !DInterpreter::CallStack().empty()) {
    EnvBaseT *e = DInterpreter::CallStack().back();
    errorNodeP = e->CallingNode();
    if (e->GetProName() != "$MAIN$")
      msg = std::runtime_error(e->GetProName() + ": " + s);
  }
#ifdef GDL_DEBUG
  cerr << s << endl;
#endif
}

GDLException::GDLException(ProgNodeP eN, const string &s, bool decorate, bool overWriteNode) :
    ANTLRException(s),
    errorNode(static_cast<RefDNode>(antlr::nullAST)),
    errorNodeP(eN),
    errorCode(-1L),
    col(0),
    prefix(true),
    ioException(false) {
  if (overWriteNode && interpreter != nullptr && !DInterpreter::CallStack().empty()) {
    EnvBaseT *e = DInterpreter::CallStack().back();
    errorNodeP = e->CallingNode();
  }
  if (decorate && interpreter != nullptr && !DInterpreter::CallStack().empty()) {
    EnvBaseT *e = DInterpreter::CallStack().back();
    if (e->GetProName() != "$MAIN$")
      msg = std::runtime_error(e->GetProName() + ": " + s);
  }
#ifdef GDL_DEBUG
  cerr << s << endl;
#endif
}
GDLException::GDLException(DLong eC, ProgNodeP eN, const string &s, bool decorate, bool overWriteNode) :
    ANTLRException(s),
    errorNode(static_cast<RefDNode>(antlr::nullAST)),
    errorNodeP(eN),
    errorCode(eC),
    col(0),
    prefix(true),
    ioException(false) {
  if (overWriteNode && interpreter != nullptr && !DInterpreter::CallStack().empty()) {
    EnvBaseT *e = DInterpreter::CallStack().back();
    errorNodeP = e->CallingNode();
  }
  if (decorate && interpreter != nullptr && !DInterpreter::CallStack().empty()) {
    EnvBaseT *e = DInterpreter::CallStack().back();
    if (e->GetProName() != "$MAIN$")
      msg = std::runtime_error(e->GetProName() + ": " + s);
  }
#ifdef GDL_DEBUG
  cerr << s << endl;
#endif
}
GDLException::GDLException(SizeT l, SizeT c, const string &s, const string &file) :
    ANTLRException(s),
    errorNode(static_cast<RefDNode>(antlr::nullAST)),
    errorCode(-1L),
    line(l),
    col(c),
    prefix(true),
    ioException(false) {
  strncpy(filename, file.c_str(), 255);
  filename[255] = '\0';
  if (interpreter != nullptr && !DInterpreter::CallStack().empty()) {
    EnvBaseT *e = DInterpreter::CallStack().back();
    errorNodeP = e->CallingNode();
    if (e->GetProName() != "$MAIN$")
      msg = std::runtime_error(e->GetProName() + ": " + s);
  }
#ifdef GDL_DEBUG
  cerr << s << endl;
#endif
}

GDLException::GDLException(SizeT l, SizeT c, const string &s) :
    ANTLRException(s),
    errorNode(static_cast<RefDNode>(antlr::nullAST)),
    errorCode(-1L),
    line(l),
    col(c),
    prefix(true),
    ioException(false) {
  if (interpreter != nullptr && !DInterpreter::CallStack().empty()) {
    EnvBaseT *e = DInterpreter::CallStack().back();
    errorNodeP = e->CallingNode();
    if (e->GetProName() != "$MAIN$")
      msg = std::runtime_error(e->GetProName() + ": " + s);
  }
#ifdef GDL_DEBUG
  cerr << s << endl;
#endif
}
GDLException::GDLException(DLong eC, SizeT l, SizeT c, const string &s) :
    ANTLRException(s),
    errorNode(static_cast<RefDNode>(antlr::nullAST)),
    errorCode(eC),
    line(l),
    col(c),
    prefix(true),
    ioException(false) {
  if (interpreter != nullptr && !DInterpreter::CallStack().empty()) {
    EnvBaseT *e = DInterpreter::CallStack().back();
    errorNodeP = e->CallingNode();
    if (e->GetProName() != "$MAIN$")
      msg = std::runtime_error(e->GetProName() + ": " + s);
  }
  if (!iAmMaster)
    gdl_ipc_ClientSendReturn(3, s);
#ifdef GDL_DEBUG
  cerr << s << endl;
#endif
}

void Message(const string &s) {
  if (SysVar::Quiet() == 0) {
    cerr << SysVar::MsgPrefix() << s << endl;
    lib::write_journal_comment(SysVar::MsgPrefix() + s);
  }
}

void Warning(const std::string &s) {
  cerr << SysVar::MsgPrefix() << s << endl;
  lib::write_journal_comment(SysVar::MsgPrefix() + s);
}

void ThrowGDLException(const std::string &str) {
  throw GDLException(str);
}

void WarnAboutObsoleteRoutine(const string &name) {
  // no static here due to .RESET_SESSION
  DStructGDL *warnStruct = SysVar::Warn();
  // this static is ok as it will evaluate always to the same value
  static unsigned obs_routinesTag = warnStruct->Desc()->TagIndex("OBS_ROUTINES");
  if (((dynamic_cast<DByteGDL *>( warnStruct->GetTag(obs_routinesTag, 0)))[0]).LogTrue())
    Message("Routine compiled from an obsolete library: " + name);
  // TODO: journal / !QUIET??
}

void WarnAboutObsoleteRoutine(const RefDNode &eN, const string &name) {
// TODO: journal?
  // no static here due to .RESET_SESSION
  DStructGDL *warnStruct = SysVar::Warn();
  // this static is ok as it will evaluate always to the same value
  static unsigned obs_routinesTag = warnStruct->Desc()->TagIndex("OBS_ROUTINES");
  if (((dynamic_cast<DByteGDL *>( warnStruct->GetTag(obs_routinesTag, 0)))[0]).LogTrue()) {
    auto e = new GDLException(eN,
                              "Routine compiled from an obsolete library: " + name
    );
    Guard<GDLException> eGuard(e);
    GDLInterpreter::ReportCompileError(*e, "");
// TODO: file 
  }
}
