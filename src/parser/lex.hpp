#pragma once
#include <stdint.h>
#include <ostream>
#include <vector>
#include "../options.hpp"

enum class token_kind {
	mdf,
	mathb, mathu, rnd,
	sft, swp,
	cio,
	lps, lpe,
	idx, tst,
	gtrf, gswp,
	clls, clle,
	rptv, rpt,
	id,
	eof
};
constexpr const char *op_names[] = {
	"mdf", "mathb", "mathu", "rnd", "sft", "swp", "cio", "lps", "lpe", "idx", "tst", "gtrf", "gswp", "clls", "clle", "rptv", "rpt", "id", "eof"
};
struct cloc {
	unsigned int idx;
	unsigned int line;
	unsigned int col;

	void message(const flags &fs, const std::string &msg) const;
};
struct token_t {
	token_kind t;
	int64_t value;
	cloc begin;
	cloc end;
};

std::vector<token_t> lex(flags &fs);
std::ostream &operator<<(std::ostream &os, const cloc &cl);
