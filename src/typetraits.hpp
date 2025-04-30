/***************************************************************************
                               typetraits.hpp
                             -------------------
    begin                : July 22 2002
    copyright            : (C) 2002-2006 by Marc Schellens
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

// to be included only from datatypes.hpp

#ifndef TYPETRAITS_HPP_
#define TYPETRAITS_HPP_

#include <vector>
#include <valarray>

#include "basegdl.hpp"
#include "dstructdesc.hpp"
#include "gdlarray.hpp"

// define type parameterization here
struct SpDByte: public BaseGDL
{
  SpDByte();
  explicit SpDByte( const dimension& dim_);

  BaseGDL *GetTag() const override;
  BaseGDL *GetInstance() const override;
  BaseGDL *GetEmptyInstance() const override;

  SizeT NBytes() const override
  {
	return (this->N_Elements() * sizeof( Ty));
  }

  static const DType  t;
  static const std::string str; 
  static const DByte zero;

  static constexpr bool IS_INTEGER = true;
  //static constexpr bool IS_SIGNED = false;
  //static constexpr bool IS_NUMERIC = true;
  //static constexpr bool IS_FLOAT = false;
  static constexpr bool IS_COMPLEX = false;
  static constexpr bool IS_POD = true;
  //static constexpr bool IS_CONVERTABLE = true;

  typedef DByte Ty;
  typedef GDLArray<Ty,IS_POD>    DataT;

  // for SFINAE
  template <typename ReturnType>
  struct IfInteger { typedef ReturnType type; };
  template <typename ReturnType>
  struct IfFloat {};
  template <typename ReturnType>
  struct IfComplex {};
  template <typename ReturnType>
  struct IfOther {};

  DType Type() const override;
  const std::string &TypeStr() const override;

  ~SpDByte() override;
};

struct SpDInt: public BaseGDL
{
  SpDInt();
  explicit SpDInt( const dimension& dim_);

  BaseGDL *GetTag() const override;
  BaseGDL *GetInstance() const override;
  BaseGDL *GetEmptyInstance() const override;

  SizeT NBytes() const override
  {
	return (this->N_Elements() * sizeof( Ty));
  }

  static const DType t;
  static const std::string str; 
  static const DInt zero;

  static constexpr bool IS_INTEGER = true;
  //static constexpr bool IS_SIGNED = true;
  //static constexpr bool IS_NUMERIC = true;
  //static constexpr bool IS_FLOAT = false;
  static constexpr bool IS_COMPLEX = false;
  static constexpr bool IS_POD = true;
  //static constexpr bool IS_CONVERTABLE = true;

  typedef DInt Ty;
  typedef GDLArray<Ty,IS_POD>    DataT;

  template <typename ReturnType>
  struct IfInteger { typedef ReturnType type; };
  template <typename ReturnType>
  struct IfFloat {};
  template <typename ReturnType>
  struct IfComplex {};
  template <typename ReturnType>
  struct IfOther {};

  DType Type() const override;
  const std::string &TypeStr() const override;

  ~SpDInt() override;
};

struct SpDUInt: public BaseGDL
{
  SpDUInt();
  explicit SpDUInt( const dimension& dim_);

  BaseGDL *GetTag() const override;
  BaseGDL *GetInstance() const override;
  BaseGDL *GetEmptyInstance() const override;

  SizeT NBytes() const override;

  static const DType t;
  static const std::string str; 
  static const DUInt zero;

  static constexpr bool IS_INTEGER = true;
  //static constexpr bool IS_SIGNED = false;
  //static constexpr bool IS_NUMERIC = true;
  //static constexpr bool IS_FLOAT = false;
  static constexpr bool IS_COMPLEX = false;
  static constexpr bool IS_POD = true;
  //static constexpr bool IS_CONVERTABLE = true;

  typedef DUInt Ty;
  typedef GDLArray<Ty,IS_POD>    DataT;

  template <typename ReturnType>
  struct IfInteger { typedef ReturnType type; };
  template <typename ReturnType>
  struct IfFloat {};
  template <typename ReturnType>
  struct IfComplex {};
  template <typename ReturnType>
  struct IfOther {};

  DType Type() const override;
  const std::string &TypeStr() const override;

  ~SpDUInt() override;
};

struct SpDLong: public BaseGDL
{
  SpDLong();
  explicit SpDLong( const dimension& dim_);

  BaseGDL *GetTag() const override;
  BaseGDL *GetInstance() const override;
  BaseGDL *GetEmptyInstance() const override;

  SizeT NBytes() const override
  {
	return (this->N_Elements() * sizeof( Ty));
  }

  static const DType  t;
  static const std::string str; 
  static const DLong  zero;

  static constexpr bool IS_INTEGER = true;
  //static constexpr bool IS_SIGNED = true;
  //static constexpr bool IS_NUMERIC = true;
  //static constexpr bool IS_FLOAT = false;
  static constexpr bool IS_COMPLEX = false;
  static constexpr bool IS_POD = true;
  //static constexpr bool IS_CONVERTABLE = true;

  typedef DLong Ty;
  typedef GDLArray<Ty,IS_POD>    DataT;

  template <typename ReturnType>
  struct IfInteger { typedef ReturnType type; };
  template <typename ReturnType>
  struct IfFloat {};
  template <typename ReturnType>
  struct IfComplex {};
  template <typename ReturnType>
  struct IfOther {};

  DType Type() const override;
  const std::string &TypeStr() const override;

  ~SpDLong() override;
};

struct SpDULong: public BaseGDL
{
  SpDULong();
  explicit SpDULong( const dimension& dim_);

  BaseGDL *GetTag() const override;
  BaseGDL *GetInstance() const override;
  BaseGDL *GetEmptyInstance() const override;

  SizeT NBytes() const override
  {
	return (this->N_Elements() * sizeof( Ty));
  }

  static const DType  t;
  static const std::string str; 
  static const DULong zero;

  static constexpr bool IS_INTEGER = true;
  //static constexpr bool IS_SIGNED = false;
  //static constexpr bool IS_NUMERIC = true;
  //static constexpr bool IS_FLOAT = false;
  static constexpr bool IS_COMPLEX = false;
  static constexpr bool IS_POD = true;
  //static constexpr bool IS_CONVERTABLE = true;

  typedef DULong Ty;
  typedef GDLArray<Ty,IS_POD>    DataT;

  template <typename ReturnType>
  struct IfInteger { typedef ReturnType type; };
  template <typename ReturnType>
  struct IfFloat {};
  template <typename ReturnType>
  struct IfComplex {};
  template <typename ReturnType>
  struct IfOther {};

  DType Type() const override;
  const std::string &TypeStr() const override;

  ~SpDULong() override;
};

struct SpDLong64: public BaseGDL
{
  SpDLong64();
  explicit  SpDLong64( const dimension& dim_);

  BaseGDL *GetTag() const override;
  BaseGDL *GetInstance() const override;
  BaseGDL *GetEmptyInstance() const override;

  SizeT NBytes() const override
  {
	return (this->N_Elements() * sizeof( Ty));
  }

  static const DType  t;
  static const std::string str; 
  static const DLong64  zero;

  static constexpr bool IS_INTEGER = true;
  //static constexpr bool IS_SIGNED = true;
  //static constexpr bool IS_NUMERIC = true;
  //static constexpr bool IS_FLOAT = false;
  static constexpr bool IS_COMPLEX = false;
  static constexpr bool IS_POD = true;
  //static constexpr bool IS_CONVERTABLE = true;

  typedef DLong64 Ty;
  typedef GDLArray<Ty,IS_POD>    DataT;

  template <typename ReturnType>
  struct IfInteger { typedef ReturnType type; };
  template <typename ReturnType>
  struct IfFloat {};
  template <typename ReturnType>
  struct IfComplex {};
  template <typename ReturnType>
  struct IfOther {};

  DType Type() const override;
  const std::string &TypeStr() const override;

  ~SpDLong64() override;
};

struct SpDULong64: public BaseGDL
{
  SpDULong64();
  explicit SpDULong64( const dimension& dim_);

  BaseGDL *GetTag() const override;
  BaseGDL *GetInstance() const override;
  BaseGDL *GetEmptyInstance() const override;

  SizeT NBytes() const override
  {
	return (this->N_Elements() * sizeof( Ty));
  }

  static const DType  t;
  static const std::string str; 
  static const DULong64 zero;

  static constexpr bool IS_INTEGER = true;
  //static constexpr bool IS_SIGNED = false;
  //static constexpr bool IS_NUMERIC = true;
  //static constexpr bool IS_FLOAT = false;
  static constexpr bool IS_COMPLEX = false;
  static constexpr bool IS_POD = true;
  //static constexpr bool IS_CONVERTABLE = true;

  typedef DULong64 Ty;
  typedef GDLArray<Ty,IS_POD>    DataT;

  template <typename ReturnType>
  struct IfInteger { typedef ReturnType type; };
  template <typename ReturnType>
  struct IfFloat {};
  template <typename ReturnType>
  struct IfComplex {};
  template <typename ReturnType>
  struct IfOther {};

  DType Type() const override;
  const std::string &TypeStr() const override;

  ~SpDULong64() override;
};

struct SpDFloat: public BaseGDL
{
  SpDFloat();
  explicit SpDFloat( const dimension& dim_);

  BaseGDL *GetTag() const override;
  BaseGDL *GetInstance() const override;
  BaseGDL *GetEmptyInstance() const override;

  SizeT NBytes() const override
  {
	return (this->N_Elements() * sizeof( Ty));
  }

  static const DType  t;
  static const std::string str; 
  static const DFloat zero;

  static constexpr bool IS_INTEGER = false;
  //static constexpr bool IS_SIGNED = true;
  //static constexpr bool IS_NUMERIC = true;
  //static constexpr bool IS_FLOAT = true;
  static constexpr bool IS_COMPLEX = false;
  static constexpr bool IS_POD = true;
  //static constexpr bool IS_CONVERTABLE = true;

  typedef DFloat Ty;
  typedef GDLArray<Ty,IS_POD>    DataT;

  template <typename ReturnType>
  struct IfInteger {};
  template <typename ReturnType>
  struct IfFloat { typedef ReturnType type; };
  template <typename ReturnType>
  struct IfComplex {};
  template <typename ReturnType>
  struct IfOther {};

  DType Type() const override;
  const std::string &TypeStr() const override;

  ~SpDFloat() override;
};

struct SpDDouble: public BaseGDL
{
  SpDDouble();
  explicit SpDDouble( const dimension& dim_);

  BaseGDL *GetTag() const override;
  BaseGDL *GetInstance() const override;
  BaseGDL *GetEmptyInstance() const override;

  SizeT NBytes() const override
  {
	return (this->N_Elements() * sizeof( Ty));
  }

  static const DType  t;
  static const std::string str; 
  static const DDouble zero;

  static constexpr bool IS_INTEGER = false;
  //static constexpr bool IS_SIGNED = true;
  //static constexpr bool IS_NUMERIC = true;
  //static constexpr bool IS_FLOAT = true;
  static constexpr bool IS_COMPLEX = false;
  static constexpr bool IS_POD = true;
  //static constexpr bool IS_CONVERTABLE = true;

  typedef DDouble Ty;
  typedef GDLArray<Ty,IS_POD>    DataT;

  template <typename ReturnType>
  struct IfInteger {};
  template <typename ReturnType>
  struct IfFloat { typedef ReturnType type; };
  template <typename ReturnType>
  struct IfComplex {};
  template <typename ReturnType>
  struct IfOther {};

  DType Type() const override;
  const std::string &TypeStr() const override;

  ~SpDDouble() override;
};

struct SpDString: public BaseGDL
{
  SpDString();
  explicit SpDString( const dimension& dim_);

  BaseGDL *GetTag() const override;
  BaseGDL *GetInstance() const override;
  BaseGDL *GetEmptyInstance() const override;

  SizeT NBytes() const override
  {
	return (this->N_Elements() * sizeof( Ty));
  }

  static constexpr bool IS_INTEGER = false;
  //static constexpr bool IS_SIGNED = false;
  //static constexpr bool IS_NUMERIC = false;
  //static constexpr bool IS_FLOAT = false;
  static constexpr bool IS_COMPLEX = false;
  static constexpr bool IS_POD = false;
  //static constexpr bool IS_CONVERTABLE = true;

  typedef DString Ty;
  typedef GDLArray<Ty,IS_POD>    DataT;

  static const DType    t;
  static const std::string str; 
  static const Ty       zero;

  template <typename ReturnType>
  struct IfInteger {};
  template <typename ReturnType>
  struct IfFloat {};
  template <typename ReturnType>
  struct IfComplex {};
  template <typename ReturnType>
  struct IfOther { typedef ReturnType type; };

  DType Type() const override;
  const std::string &TypeStr() const override;

  ~SpDString() override;
};

// attention: structs are special
struct SpDStruct: public BaseGDL
{
protected:
  DStructDesc* desc;

  //explicit SpDStruct( DStructDesc* desc_= nullptr);
  SpDStruct( DStructDesc* desc_, const dimension& dim_);

  void MakeOwnDesc()
  {
//     if( /* desc != NULL && */ desc->IsUnnamed()) desc = new DStructDesc( desc);
    assert(desc != nullptr);
    if( desc->IsUnnamed()) desc->AddRef();
  }

public:
  inline SizeT NTags() const { return desc->NTags();}

  inline DStructDesc* Desc() const { return desc;}
  inline void SetDesc(DStructDesc *newDesc) {
    if (desc != nullptr && desc->IsUnnamed())
      delete desc;
    desc=newDesc;
  }

  // GetTag returns a tag descriptor (SpType)
  BaseGDL *GetTag() const override;
  BaseGDL *GetInstance() const override;
  BaseGDL *GetEmptyInstance() const override;

  SizeT NBytes() const override
  {
    return ( this->N_Elements() * desc->NBytes());
  }

  static constexpr bool IS_INTEGER = false;
  //static constexpr bool IS_SIGNED = false;
  //static constexpr bool IS_NUMERIC = false;
  //static constexpr bool IS_FLOAT = false;
  static constexpr bool IS_COMPLEX = false;
  static constexpr bool IS_POD = false;
  //static constexpr bool IS_CONVERTABLE = false;

  typedef char Ty;
  typedef GDLArray<Ty,true> DataT; // we are using char here

  static const DType  t;
  static const std::string str; 
  static const Ty     zero;

  DType Type() const override;
  const std::string &TypeStr() const override;

  ~SpDStruct() override;
};


struct SpDPtr: public BaseGDL
{
  SpDPtr();
  explicit SpDPtr( const dimension& dim_);

  BaseGDL *GetTag() const override;
  BaseGDL *GetInstance() const override;
  BaseGDL *GetEmptyInstance() const override;

  SizeT NBytes() const override
  {
	return (this->N_Elements() * sizeof( Ty));
  }

  static constexpr bool IS_INTEGER = false;
  //static constexpr bool IS_SIGNED = false;
  //static constexpr bool IS_NUMERIC = false;
  //static constexpr bool IS_FLOAT = false;
  static constexpr bool IS_COMPLEX = false;
  static constexpr bool IS_POD = false; // due to ref counting
  //static constexpr bool IS_CONVERTABLE = false;

  typedef DPtr Ty;
  typedef GDLArray<Ty,true>    DataT; // on this level, DPtr is POD

  template <typename ReturnType>
  struct IfInteger {};
  template <typename ReturnType>
  struct IfFloat {};
  template <typename ReturnType>
  struct IfComplex {};
  template <typename ReturnType>
  struct IfOther { typedef ReturnType type; };

  static const DType    t;
  static const std::string str; 
  static const Ty       zero;

  DType Type() const override;
  const std::string &TypeStr() const override;

  ~SpDPtr() override;
};

// objects are pointer to structs
struct SpDObj: public BaseGDL
{
  SpDObj();
  explicit SpDObj( const dimension& dim_);

  BaseGDL *GetTag() const override;
  BaseGDL *GetInstance() const override;
  BaseGDL *GetEmptyInstance() const override;

  SizeT NBytes() const override
  {
	return (this->N_Elements() * sizeof( Ty));
  }

  static constexpr bool IS_INTEGER = false;
  //static constexpr bool IS_SIGNED = false;
  //static constexpr bool IS_NUMERIC = false;
  //static constexpr bool IS_FLOAT = false;
  static constexpr bool IS_COMPLEX = false;
  static constexpr bool IS_POD = false; // due to ref counting
  //static constexpr bool IS_CONVERTABLE = false;

  typedef DObj Ty;
  typedef GDLArray<Ty, true>    DataT; // on this level, DObj is POD

  template <typename ReturnType>
  struct IfInteger {};
  template <typename ReturnType>
  struct IfFloat {};
  template <typename ReturnType>
  struct IfComplex {};
  template <typename ReturnType>
  struct IfOther { typedef ReturnType type; };

  static const DType    t;
  static const std::string str; 
  static const Ty       zero;

  DType Type() const override;
  const std::string &TypeStr() const override;

  ~SpDObj() override;
};

struct SpDComplex: public BaseGDL
{
  SpDComplex();
  explicit SpDComplex( const dimension& dim_);

  BaseGDL *GetTag() const override;
  BaseGDL *GetInstance() const override;
  BaseGDL *GetEmptyInstance() const override;

  SizeT NBytes() const override
  {
	return (this->N_Elements() * sizeof( Ty));
  }

  static constexpr bool IS_INTEGER = false;
  //static constexpr bool IS_SIGNED = true;
  //static constexpr bool IS_NUMERIC = true;
  //static constexpr bool IS_FLOAT = false;
  static constexpr bool IS_COMPLEX = true;
  static constexpr bool IS_POD = false;
  //static constexpr bool IS_CONVERTABLE = true;

  typedef DComplex Ty;
  typedef GDLArray<Ty, TreatPODComplexAsPOD> DataT; // ATTENTION: strictly complex is non-pod

  template <typename ReturnType>
  struct IfInteger {};
  template <typename ReturnType>
  struct IfFloat {};
  template <typename ReturnType>
  struct IfComplex { typedef ReturnType type; };
  template <typename ReturnType>
  struct IfOther {};

  static const DType  t;
  static const std::string str; 
  static const DComplex zero;

  DType Type() const override;
  const std::string &TypeStr() const override;

  ~SpDComplex() override;
};

struct SpDComplexDbl: public BaseGDL
{
  SpDComplexDbl();
  explicit SpDComplexDbl( const dimension& dim_);

  BaseGDL *GetTag() const override;
  BaseGDL *GetInstance() const override;
  BaseGDL *GetEmptyInstance() const override;

  SizeT NBytes() const override
  {
	return (this->N_Elements() * sizeof( Ty));
  }

  //static constexpr bool IS_SIGNED = true;
  //static constexpr bool IS_NUMERIC = true;
  static constexpr bool IS_INTEGER = false;
  //static constexpr bool IS_FLOAT = false;
  static constexpr bool IS_COMPLEX = true;
  static constexpr bool IS_POD = false;
  //static constexpr bool IS_CONVERTABLE = true;

  typedef DComplexDbl Ty;
  typedef GDLArray<Ty, TreatPODComplexAsPOD>    DataT; // ATTENTION: srictly complex is non-pod

  template <typename ReturnType>
  struct IfInteger {};
  template <typename ReturnType>
  struct IfFloat {};
  template <typename ReturnType>
  struct IfComplex { typedef ReturnType type; };
  template <typename ReturnType>
  struct IfOther {};

  static const DType  t;
  static const std::string str; 
  static const DComplexDbl zero;

  DType Type() const override;
  const std::string &TypeStr() const override;

  ~SpDComplexDbl() override;
};


#endif
