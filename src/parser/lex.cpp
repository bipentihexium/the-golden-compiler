#include "lex.hpp"
#include <iomanip>
#include <iostream>
#include <string>
#include "../options.hpp"

using namespace std::string_literals;

#include <iterator>
#include <string>
#include <stdexcept>
namespace zmn {
	enum class token_kind {
		tINCR, tDECR, tADD, tSUB, tMUL, tDIV, tFLR, tCEIL, tRND, tLFT, tRGT, tSWP, tIN_N, tOUTN, tIN_C, tOUTC, tDLPS, tDLPE, tLPS, tLPE, tIDX, tTEQ, tTLT, tTGT, tGTRF, tGSWP, tCLLS, tCLLE, tFLAG_VERSION, tFLAG_IMPL_PRINT_MEM, tFLAG_IMPL_PRINT_ALL_MEM, tFLAG_IMPL_PRINT_GLOBAL, tFLAG_IMPL_PRINT_ON, tFLAG_IMPL_PRINT_OFF, tFLAG_IMPL_UNKNOWN, tFLAG_UNKNOWN, tRPTV, tRPT, tID, tEOF, tCOMMENT, tNEWLINE, tCOMMENT2, tUNSET, tERROR
	};
	constexpr const char *token_kind_to_str(const token_kind tk) {
		constexpr const char *strs[] = {
			"INCR","DECR","ADD","SUB","MUL","DIV","FLR","CEIL","RND","LFT","RGT","SWP","IN_N","OUTN","IN_C","OUTC","DLPS","DLPE","LPS","LPE","IDX","TEQ","TLT","TGT","GTRF","GSWP","CLLS","CLLE","FLAG_VERSION","FLAG_IMPL_PRINT_MEM","FLAG_IMPL_PRINT_ALL_MEM","FLAG_IMPL_PRINT_GLOBAL","FLAG_IMPL_PRINT_ON","FLAG_IMPL_PRINT_OFF","FLAG_IMPL_UNKNOWN","FLAG_UNKNOWN","RPTV","RPT","ID","EOF","COMMENT","NEWLINE","COMMENT2","UNSET","ERROR",
		};
		return strs[static_cast<unsigned int>(tk)];
	}
	struct lex {
	public:bool fail = false;
	flags *fs;
	std::vector<token_t> *out;
	void pusht(::token_kind t, int64_t val=0) const {
		out->push_back(::token_t{t, val, {span_begin(), line_begin(), col_begin()}, {span_end(), line_end(), col_end()}});
	}

		token_kind token;

		lex(const std::string &s) : token(token_kind::tEOF), l(1), c(1), el(1), ec(1), str(s+"\0"), p(str.begin()), ep(str.begin()), e(str.end()) { }
		std::string value() const { return std::string(p, ep); }
		unsigned int span_begin() const { return std::distance(str.begin(), p); }
		unsigned int span_end() const { return std::distance(str.begin(), ep); }
		unsigned int line_begin() const { return l; }
		unsigned int line_end() const { return el; }
		unsigned int col_begin() const { return c; }
		unsigned int col_end() const { return ec; }
		void advance() {
			switch (*ep) {
			case '\0': break;
			case '\n': ep++; ec = 1; el++; break;
			case '\t': ep++; ec += 4; break;
			default: ep++; ec++; break;
			}
		}
		token_kind next() {
			p = ep; l = el; c = ec;
			unsigned int state = 0;
			while (1) {
				switch(state) {
				case 0:switch (*ep) {
					case '\0': advance(); token = token_kind::tEOF;
						{ pusht(::token_kind::eof);}
						 return token_kind::tEOF;
					case '\t':
					case '\n':
					case '\v':
					case '\f':
					case '\r':
					case ' ':
					case 'A':
					case 'B':
					case 'C':
					case 'D':
					case 'E':
					case 'F':
					case 'G':
					case 'H':
					case 'I':
					case 'J':
					case 'K':
					case 'L':
					case 'M':
					case 'N':
					case 'O':
					case 'P':
					case 'Q':
					case 'R':
					case 'S':
					case 'T':
					case 'U':
					case 'V':
					case 'W':
					case 'X':
					case 'Y':
					case 'Z':
					case 'a':
					case 'b':
					case 'c':
					case 'd':
					case 'e':
					case 'f':
					case 'g':
					case 'h':
					case 'i':
					case 'j':
					case 'k':
					case 'l':
					case 'm':
					case 'n':
					case 'o':
					case 'p':
					case 'q':
					case 'r':
					case 's':
					case 't':
					case 'u':
					case 'v':
					case 'w':
					case 'x':
					case 'y':
					case 'z': advance(); token = token_kind::tCOMMENT2;  return token_kind::tCOMMENT2;
					case '!': advance(); token = token_kind::tINCR;
						{ pusht(::token_kind::mdf, 1);}
						 return token_kind::tINCR;
					case '"': advance(); state = 8; break;
					case '#': advance(); state = 5; break;
					case '$': advance(); state = 1; break;
					case '&': advance(); token = token_kind::tCEIL;
						{ pusht(::token_kind::mathu, 1);}
						 return token_kind::tCEIL;
					case '\'': advance(); token = token_kind::tGSWP;
						{ pusht(::token_kind::gswp);}
						 return token_kind::tGSWP;
					case '(': advance(); token = token_kind::tCLLS;
						{ pusht(::token_kind::clls);}
						 return token_kind::tCLLS;
					case ')': advance(); token = token_kind::tCLLE;
						{ pusht(::token_kind::clle);}
						 return token_kind::tCLLE;
					case '*': advance(); token = token_kind::tMUL;
						{ pusht(::token_kind::mathb, 2);}
						 return token_kind::tMUL;
					case '+': advance(); token = token_kind::tADD;
						{ pusht(::token_kind::mathb, 0);}
						 return token_kind::tADD;
					case ',': advance(); token = token_kind::tIN_C;
						{ pusht(::token_kind::cio, 2);}
						 return token_kind::tIN_C;
					case '-': advance(); token = token_kind::tSUB;
						{ pusht(::token_kind::mathb, 1);}
						 return token_kind::tSUB;
					case '.': advance(); token = token_kind::tOUTC;
						{ pusht(::token_kind::cio, 3);}
						 return token_kind::tOUTC;
					case '/': advance(); token = token_kind::tDIV;
						{ pusht(::token_kind::mathb, 3);}
						 return token_kind::tDIV;
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9': advance(); state = 7; break;
					case ':': advance(); state = 9; break;
					case ';': advance(); token = token_kind::tGTRF;
						{ pusht(::token_kind::gtrf);}
						 return token_kind::tGTRF;
					case '<': advance(); token = token_kind::tLFT;
						{ pusht(::token_kind::sft, -1);}
						 return token_kind::tLFT;
					case '>': advance(); token = token_kind::tRGT;
						{ pusht(::token_kind::sft, 1);}
						 return token_kind::tRGT;
					case '?': advance(); state = 4; break;
					case '@': advance(); state = 3; break;
					case '[': advance(); state = 2; break;
					case ']': advance(); token = token_kind::tLPE;
						{ pusht(::token_kind::lpe);}
						 return token_kind::tLPE;
					case '^': advance(); token = token_kind::tSWP;
						{ pusht(::token_kind::swp);}
						 return token_kind::tSWP;
					case '_': advance(); token = token_kind::tFLR;
						{ pusht(::token_kind::mathu, 0);}
						 return token_kind::tFLR;
					case '`': advance(); token = token_kind::tRND;
						{ pusht(::token_kind::rnd);}
						 return token_kind::tRND;
					case '|': advance(); state = 6; break;
					case '~': advance(); token = token_kind::tDECR;
						{ pusht(::token_kind::mdf, -1);}
						 return token_kind::tDECR;
					default: ep = p; ec = c; el = l; advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					}break;
				case 1:switch (*ep) {
					case ',': advance(); token = token_kind::tIN_N;
						{ pusht(::token_kind::cio, 0);}
						 return token_kind::tIN_N;
					case '.': advance(); token = token_kind::tOUTN;
						{ pusht(::token_kind::cio, 1);}
						 return token_kind::tOUTN;
					default: ep = p; ec = c; el = l; advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					}break;
				case 2:switch (*ep) {
					case '@': advance(); token = token_kind::tDLPS;
						{ pusht(::token_kind::lps, 1);}
						 return token_kind::tDLPS;
					default: token = token_kind::tLPS;
						{ pusht(::token_kind::lps);}
						 return token_kind::tLPS;
					}break;
				case 3:switch (*ep) {
					case ']': advance(); token = token_kind::tDLPE;
						{ pusht(::token_kind::lpe, 1);}
						 return token_kind::tDLPE;
					default: ep = p; ec = c; el = l; advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					}break;
				case 4:switch (*ep) {
					case '<': advance(); token = token_kind::tTLT;
						{ pusht(::token_kind::tst, -1);}
						 return token_kind::tTLT;
					case '=': advance(); token = token_kind::tTEQ;
						{ pusht(::token_kind::tst, 0);}
						 return token_kind::tTEQ;
					case '>': advance(); token = token_kind::tTGT;
						{ pusht(::token_kind::tst, 1);}
						 return token_kind::tTGT;
					case '?': advance(); token = token_kind::tIDX;
						{ pusht(::token_kind::idx);}
						 return token_kind::tIDX;
					default: ep = p; ec = c; el = l; advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					}break;
				case 5:switch (*ep) {
					case '\0':
					case '\n':
					case '#': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case 'I':
					case 'i': advance(); state = 11; break;
					case 'V':
					case 'v': advance(); state = 10; break;
					default: advance(); state = 12; break;
					}break;
				case 6:switch (*ep) {
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9': advance(); state = 13; break;
					case '|': advance(); token = token_kind::tRPTV;
						{ pusht(::token_kind::rptv);}
						 return token_kind::tRPTV;
					default: ep = p; ec = c; el = l; advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					}break;
				case 7:switch (*ep) {
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9': advance(); break;
					default: token = token_kind::tID;
						{ pusht(::token_kind::id, std::stoll(value()));}
						 return token_kind::tID;
					}break;
				case 8:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '"': advance(); token = token_kind::tCOMMENT;  return token_kind::tCOMMENT;
					default: advance(); break;
					}break;
				case 9:switch (*ep) {
					case '\n': advance(); token = token_kind::tNEWLINE;  return token_kind::tNEWLINE;
					default: ep = p; ec = c; el = l; advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					}break;
				case 10:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown flag"s);}
						 return token_kind::tFLAG_UNKNOWN;
					case 'E':
					case 'e': advance(); state = 14; break;
					default: advance(); state = 12; break;
					}break;
				case 11:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown flag"s);}
						 return token_kind::tFLAG_UNKNOWN;
					case 'M':
					case 'm': advance(); state = 15; break;
					default: advance(); state = 12; break;
					}break;
				case 12:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown flag"s);}
						 return token_kind::tFLAG_UNKNOWN;
					default: advance(); break;
					}break;
				case 13:switch (*ep) {
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9': advance(); break;
					case '|': advance(); token = token_kind::tRPT;
						{ std::string v = value().substr(1); v.pop_back(); pusht(::token_kind::rpt, std::stoll(v));}
						 return token_kind::tRPT;
					default: ep = p; ec = c; el = l; advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					}break;
				case 14:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown flag"s);}
						 return token_kind::tFLAG_UNKNOWN;
					case 'R':
					case 'r': advance(); state = 16; break;
					default: advance(); state = 12; break;
					}break;
				case 15:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown flag"s);}
						 return token_kind::tFLAG_UNKNOWN;
					case 'P':
					case 'p': advance(); state = 17; break;
					default: advance(); state = 12; break;
					}break;
				case 16:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown flag"s);}
						 return token_kind::tFLAG_UNKNOWN;
					case 'S':
					case 's': advance(); state = 18; break;
					default: advance(); state = 12; break;
					}break;
				case 17:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown flag"s);}
						 return token_kind::tFLAG_UNKNOWN;
					case 'L':
					case 'l': advance(); state = 19; break;
					default: advance(); state = 12; break;
					}break;
				case 18:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown flag"s);}
						 return token_kind::tFLAG_UNKNOWN;
					case 'I':
					case 'i': advance(); state = 20; break;
					default: advance(); state = 12; break;
					}break;
				case 19:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown flag"s);}
						 return token_kind::tFLAG_UNKNOWN;
					case ' ': advance(); state = 21; break;
					default: advance(); state = 12; break;
					}break;
				case 20:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown flag"s);}
						 return token_kind::tFLAG_UNKNOWN;
					case 'O':
					case 'o': advance(); state = 22; break;
					default: advance(); state = 12; break;
					}break;
				case 21:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown flag"s);}
						 return token_kind::tFLAG_UNKNOWN;
					case 'p': advance(); state = 23; break;
					default: advance(); state = 24; break;
					}break;
				case 22:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown flag"s);}
						 return token_kind::tFLAG_UNKNOWN;
					case 'N':
					case 'n': advance(); state = 25; break;
					default: advance(); state = 12; break;
					}break;
				case 23:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 'r': advance(); state = 26; break;
					default: advance(); state = 24; break;
					}break;
				case 24:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					default: advance(); break;
					}break;
				case 25:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown flag"s);}
						 return token_kind::tFLAG_UNKNOWN;
					case ' ': advance(); state = 27; break;
					default: advance(); state = 12; break;
					}break;
				case 26:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 'i': advance(); state = 28; break;
					default: advance(); state = 24; break;
					}break;
				case 27:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown flag"s);}
						 return token_kind::tFLAG_UNKNOWN;
					default: advance(); state = 29; break;
					}break;
				case 28:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 'n': advance(); state = 30; break;
					default: advance(); state = 24; break;
					}break;
				case 29:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_VERSION;
						{ fs->tgver = value().substr(9); fs->tgver.pop_back();}
						 return token_kind::tFLAG_VERSION;
					default: advance(); break;
					}break;
				case 30:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 't': advance(); state = 31; break;
					default: advance(); state = 24; break;
					}break;
				case 31:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case ' ': advance(); state = 32; break;
					default: advance(); state = 24; break;
					}break;
				case 32:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 'a': advance(); state = 34; break;
					case 'g': advance(); state = 35; break;
					case 'm': advance(); state = 33; break;
					case 'o': advance(); state = 36; break;
					default: advance(); state = 24; break;
					}break;
				case 33:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 'e': advance(); state = 37; break;
					default: advance(); state = 24; break;
					}break;
				case 34:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 'l': advance(); state = 38; break;
					default: advance(); state = 24; break;
					}break;
				case 35:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 'l': advance(); state = 39; break;
					default: advance(); state = 24; break;
					}break;
				case 36:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 'f': advance(); state = 41; break;
					case 'n': advance(); state = 40; break;
					default: advance(); state = 24; break;
					}break;
				case 37:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 'm': advance(); state = 42; break;
					default: advance(); state = 24; break;
					}break;
				case 38:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 'l': advance(); state = 43; break;
					default: advance(); state = 24; break;
					}break;
				case 39:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 'o': advance(); state = 44; break;
					default: advance(); state = 24; break;
					}break;
				case 40:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_PRINT_ON;
						{ pusht(::token_kind::dbg, -1);}
						 return token_kind::tFLAG_IMPL_PRINT_ON;
					default: advance(); state = 24; break;
					}break;
				case 41:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 'f': advance(); state = 45; break;
					default: advance(); state = 24; break;
					}break;
				case 42:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_PRINT_MEM;
						{ pusht(::token_kind::dbg, 0);}
						 return token_kind::tFLAG_IMPL_PRINT_MEM;
					default: advance(); state = 24; break;
					}break;
				case 43:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case ' ': advance(); state = 46; break;
					default: advance(); state = 24; break;
					}break;
				case 44:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 'b': advance(); state = 47; break;
					default: advance(); state = 24; break;
					}break;
				case 45:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_PRINT_OFF;
						{ pusht(::token_kind::dbg, -2);}
						 return token_kind::tFLAG_IMPL_PRINT_OFF;
					default: advance(); state = 24; break;
					}break;
				case 46:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 'm': advance(); state = 48; break;
					default: advance(); state = 24; break;
					}break;
				case 47:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 'a': advance(); state = 49; break;
					default: advance(); state = 24; break;
					}break;
				case 48:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 'e': advance(); state = 50; break;
					default: advance(); state = 24; break;
					}break;
				case 49:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 'l': advance(); state = 51; break;
					default: advance(); state = 24; break;
					}break;
				case 50:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_UNKNOWN;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, warn_pref+"unknown impl flag"s);}
						 return token_kind::tFLAG_IMPL_UNKNOWN;
					case 'm': advance(); state = 52; break;
					default: advance(); state = 24; break;
					}break;
				case 51:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_PRINT_GLOBAL;
						{ pusht(::token_kind::dbg, 2);}
						 return token_kind::tFLAG_IMPL_PRINT_GLOBAL;
					default: advance(); state = 24; break;
					}break;
				case 52:switch (*ep) {
					case '\0': ep = p; ec = c; el = l; advance(); advance(); token = token_kind::tERROR;
						{ cloc{span_begin(), line_begin(), col_begin()}.message(*fs, error_pref+"unexpected token '"s + value() + "'");}
						 return token_kind::tERROR;
					case '\n':
					case '#': advance(); token = token_kind::tFLAG_IMPL_PRINT_ALL_MEM;
						{ pusht(::token_kind::dbg, 1);}
						 return token_kind::tFLAG_IMPL_PRINT_ALL_MEM;
					default: advance(); state = 24; break;
					}break;
				default: throw std::runtime_error("Invalid lexer state!");
				}
			}
		}
	private:
		unsigned int l, c, el, ec;
		std::string str;
		std::string::const_iterator p, ep, e;
	};
}


void cloc::message(const flags &fs, const std::string &msg) const {
	std::cerr << msg << " - " << fs.file << " " << *this << "\n";
	int64_t i = idx;
	do { i--; } while (i >= 0 && fs.filec[i] != '\n');
	std::string ln;
	do { ln.push_back(fs.filec[++i]); } while (size_t(i) < fs.filec.size() && fs.filec[i] != '\n');
	std::cerr << std::setw(3) << line << " | " << ln;
	if (ln.empty() || ln.back() != '\n') { std::cerr << '\n'; }
	std::cerr << "    |" << std::string(col, ' ') << '^' << std::endl;
}

std::vector<token_t> lex(flags &fs) {
	std::vector<token_t> out;
	zmn::lex l(fs.filec.c_str());
	l.out = &out;
	l.fs = &fs;
	while (l.next() != zmn::token_kind::tEOF) { }
	if (l.fail) {
		throw std::runtime_error("Lexing failed.");
	}
	return out;
}
std::ostream &operator<<(std::ostream &os, const cloc &cl) {
	os << cl.line << ':' << cl.col;
	return os;
}

