#pragma once
#include <string>

struct options {
	bool verbose;
	bool show_ir;
	bool show_instrs;
	bool jit;
	bool optimize;
	int rcode;
};
extern options opts;
struct flags {
	std::string file;
	std::string filec;
	std::string tgver;
};

constexpr const char *warn_pref = "\x1b[33mwarning: \x1b[0m";
constexpr const char *error_pref = "\x1b[31merror: \x1b[0m";
