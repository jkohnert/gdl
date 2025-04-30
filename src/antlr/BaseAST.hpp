#ifndef INC_BaseAST_hpp_
#define INC_BaseAST_hpp_

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: BaseAST.hpp,v 1.1.1.1 2004-12-09 15:10:20 m_schellens Exp $
 */

#include <antlr/config.hpp>
#include <antlr/AST.hpp>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

class ANTLR_API BaseAST;
typedef ASTRefCount<BaseAST> RefBaseAST;

class ANTLR_API BaseAST : public AST {
public:
  BaseAST();
  BaseAST(const BaseAST &other);

  ~BaseAST() override;

  /// Return the class name
  const char *typeName() const override;

  /// Clone this AST node.
  RefAST clone() const override;

  /// Is node t equal to this in terms of token type and text?
  bool equals(RefAST t) const override;

  /** Is t an exact structural and equals() match of this tree. The
    * 'this' reference is considered the start of a sibling list.
    */
  bool equalsList(RefAST t) const override;

  /** Is 't' a subtree of this list? The siblings of the root are NOT ignored.
   */
  bool equalsListPartial(RefAST t) const override;

  /** Is tree rooted at 'this' equal to 't'?  The siblings of 'this' are
   * ignored.
   */
  bool equalsTree(RefAST t) const override;

  /** Is 't' a subtree of the tree rooted at 'this'? The siblings of
   * 'this' are ignored.
   */
  bool equalsTreePartial(RefAST t) const override;

  /** Walk the tree looking for all exact subtree matches.  Return
   *  an ASTEnumerator that lets the caller walk the list
   *  of subtree roots found herein.
   */
  ANTLR_USE_NAMESPACE(std)vector<RefAST> findAll(RefAST t) override;

  /** Walk the tree looking for all subtrees.  Return
   *  an ASTEnumerator that lets the caller walk the list
   *  of subtree roots found herein.
   */
  ANTLR_USE_NAMESPACE(std)vector<RefAST> findAllPartial(RefAST t) override;

  /// Add a node to the end of the child list for this node
  void addChild(RefAST c) override;
  /** Get the number of child nodes of this node (shallow e.g. not of the
   * whole tree it spans).
   */
  size_t getNumberOfChildren() const override;

  /// Get the first child of this node; null if no children
  RefAST getFirstChild() const override {
    return {down};
  }
  /// Get  the next sibling in line after this one
  RefAST getNextSibling() const override {
    return {right};
  }

  /// Get the token text for this node
  ANTLR_USE_NAMESPACE(std)string getText() const override {
    return "";
  }
  /// Get the token type for this node
  int getType() const override {
    return 0;
  }

  /// Remove all children
  virtual void removeChildren() {
    down = dynamic_cast<BaseAST *>(static_cast<AST *>(nullAST));
  }

  /// Set the first child of a node.
  void setFirstChild(RefAST c) override {
    down = dynamic_cast<BaseAST *>(static_cast<AST *>(c));
  }

  /// Set the next sibling after this one.
  void setNextSibling(RefAST n) override {
    right = dynamic_cast<BaseAST *>(static_cast<AST *>(n));
  }

  /// Set the token text for this node
  void setText(const ANTLR_USE_NAMESPACE(std)string &txt) override;

  /// Set the token type for this node
  void setType(int type) override;

#ifdef ANTLR_SUPPORT_XML
  /** print attributes of this node to 'out'. Override to customize XML
   * output.
   * @param out the stream to write the AST attributes to.
   */
  virtual bool attributesToStream( ANTLR_USE_NAMESPACE(std)ostream& out ) const;
  /** Write this subtree to a stream. Overload this one to customize the XML
   * output for AST derived AST-types
   * @param output stream
   */
  virtual void toStream( ANTLR_USE_NAMESPACE(std)ostream &out ) const;
#endif

  /// Return string representation for the AST
  ANTLR_USE_NAMESPACE(std)string toString() const override;

  /// Print out a child sibling tree in LISP notation
  ANTLR_USE_NAMESPACE(std)string toStringList() const override;
  ANTLR_USE_NAMESPACE(std)string toStringTree() const override;

  static const char *const TYPE_NAME;
protected:
  RefBaseAST down;
  RefBaseAST right;
private:
  void doWorkForFindAll(ANTLR_USE_NAMESPACE(std)vector<RefAST> &v,
                        const RefAST &target,
                        bool partialMatch);
};

/** Is node t equal to this in terms of token type and text?
 */
inline bool BaseAST::equals(RefAST t) const {
  if (!t)
    return false;
  return ((getType() == t->getType()) && (getText() == t->getText()));
}

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_BaseAST_hpp_
