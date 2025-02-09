/***************************************************************************
                              gdlexception.hpp
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

#ifndef GDLEXCEPTION_HPP_
#define GDLEXCEPTION_HPP_

#include <string>
#include <iostream>

#include "prognode.hpp"

#include <antlr/ANTLRException.hpp>

//using namespace std;

class EnvUDT;

class GDLException : public antlr::ANTLRException {
  static DInterpreter *interpreter;

  const RefDNode errorNode;
  ProgNodeP errorNodeP = nullptr;
  const DLong errorCode;
  SizeT line = 0;
  const SizeT col;
  // Note: Windows allows longer filenames...
  char filename[256] = ""; //the filename (or "" for interactive $MAIN$) where the problem arose
  const bool prefix;

  bool arrayExprIndexesFailed = false;

protected:
  bool ioException;

private:
  EnvUDT *targetEnv = nullptr; // where to stop (depending on ON_ERROR)

public:
  static DInterpreter *Interpreter() { return interpreter; }
  static void SetInterpreter(DInterpreter *i) { interpreter = i; }

  static std::string Name(BaseGDL *b);

  void SetErrorNodeP(ProgNodeP p) { errorNodeP = p; }

  bool GetArrayexprIndexeeFailed() const { return arrayExprIndexesFailed; }
  void SetArrayexprIndexeeFailed(bool b) { arrayExprIndexesFailed = b; }

  GDLException() :
      ANTLRException(),
      errorNode(static_cast<RefDNode>(antlr::nullAST)),
      errorNodeP(nullptr),
      errorCode(-1),
      col(0),
      prefix(true),
      ioException(false) {}
  explicit GDLException(DLong eC) :
      ANTLRException(),
      errorNode(static_cast<RefDNode>(antlr::nullAST)),
      errorNodeP(nullptr),
      errorCode(eC),
      col(0),
      prefix(true),
      ioException(false) {}
  explicit GDLException(const std::string &s, bool pre = true, bool decorate = true);
  GDLException(const RefDNode &eN, const std::string &s);
  GDLException(ProgNodeP eN, const std::string &s, bool decorate = true, bool overWriteNode = true);
  GDLException(SizeT l, SizeT c, const std::string &s, const std::string &file);
  GDLException(SizeT l, SizeT c, const std::string &s);

  GDLException(DLong eC, const std::string &s, bool pre = true, bool decorate = true);
  GDLException(DLong eC, const RefDNode &eN, const std::string &s);
  GDLException(DLong eC, ProgNodeP eN, const std::string &s, bool decorate = true, bool overWriteNode = true);
  GDLException(DLong eC, SizeT l, SizeT c, const std::string &s);

  ~GDLException() noexcept override = default;

  DLong ErrorCode() const { return errorCode; }

  std::string toString() const override {
    return msg.what();
  }
  std::string getFilename() const {
    return filename;
  }
  SizeT getLine() const {
    if (line != 0)
      return line;
    if (errorNodeP != nullptr)
      return errorNodeP->getLine();
    if (errorNode != static_cast<RefDNode>(antlr::nullAST))
      return errorNode->getLine();
    return 0;
  }

  void SetLine(SizeT l) { line = l; }

  SizeT getColumn() const {
    //    if( errorNode != static_cast<RefDNode>(antlr::nullAST))
    //      return errorNode->getColumn();
    return col;
  }

  bool Prefix() const {
    return prefix;
  }

  void SetTargetEnv(EnvUDT *tEnv) {
    targetEnv = tEnv;
  }

  EnvUDT *GetTargetEnv() {
    return targetEnv;
  }

  bool IsIOException() const { return ioException; }
};

// for ON_IOERROR
class GDLIOException : public GDLException {
public:
  GDLIOException() :
      GDLException() { ioException = true; }

  explicit GDLIOException(const std::string &s, bool pre = true) :
      GDLException(s, pre) { ioException = true; }

  GDLIOException(ProgNodeP eN, const std::string &s) :
      GDLException(eN, s) { ioException = true; }

  explicit GDLIOException(DLong eC) :
      GDLException(eC) { ioException = true; }

  GDLIOException(DLong eC, const std::string &s, bool pre = true) :
      GDLException(eC, s, pre) { ioException = true; }

  GDLIOException(DLong eC, ProgNodeP eN, const std::string &s) :
      GDLException(eC, eN, s) { ioException = true; }
};

// warnings ignore !QUIET
void Warning(const std::string &s);

// messages honor !QUIET
void Message(const std::string &s);

void WarnAboutObsoleteRoutine(const std::string &name);
void WarnAboutObsoleteRoutine(const RefDNode &eN, const std::string &name);

#endif

