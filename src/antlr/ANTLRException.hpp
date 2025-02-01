#ifndef INC_ANTLRException_hpp_
#define INC_ANTLRException_hpp_

/* ANTLR Translator Generator
 * Project led by Terence Parr at http://www.jGuru.com
 * Software rights: http://www.antlr.org/license.html
 *
 * $Id: ANTLRException.hpp,v 1.1.1.1 2004-12-09 15:10:20 m_schellens Exp $
 */

#include <antlr/config.hpp>
#include <stdexcept>

#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
namespace antlr {
#endif

class ANTLR_API ANTLRException : std::runtime_error
{
public:
	/// Create ANTLR base exception without error message
	ANTLRException() : std::runtime_error("")
	{
	}
	/// Create ANTLR base exception with error message
	explicit ANTLRException(const ANTLR_USE_NAMESPACE(std)string& s)
	: std::runtime_error(s)
	{
	}
	~ANTLRException() noexcept override = default;

	/** Return complete error message with line/column number info (if present)
	 * @note for your own exceptions override this one. Call getMessage from
	 * here to get the 'clean' error message stored in the text attribute.
	 */
	virtual ANTLR_USE_NAMESPACE(std)string toString() const
	{
		return what();
	}

	/** Return error message without additional info (if present)
	 * @note when making your own exceptions classes override toString
	 * and call in toString getMessage which relays the text attribute
	 * from here.
	 */
	virtual ANTLR_USE_NAMESPACE(std)string getMessage() const
	{
		return what();
	}
};
#ifdef ANTLR_CXX_SUPPORTS_NAMESPACE
}
#endif

#endif //INC_ANTLRException_hpp_
