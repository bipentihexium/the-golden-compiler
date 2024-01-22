#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include "cmake.hpp"
#include "codegen.hpp"
#include "options.hpp"
#include "parser/lex.hpp"

options opts;
int main(int argc, const char **argv) {
	boost::program_options::options_description opt_desc("Options");
	opt_desc.add_options()
		("help", "show help")
		("version", "show version")
		("verbose,v", "show verbose output")
		("ver", boost::program_options::value<std::string>(), "set TG version")
		("output,o", boost::program_options::value<std::string>()->default_value("maumivu.o"), "set output file")
		("input-file", boost::program_options::value<std::vector<std::string>>(), "add input file")
		("show-ir,ir", "show LLVM IR of compiled modules")
		("show-instrs", "show TG instructions (for debugging the compiler)")
		("optimize", "run LLVM O2 optimization passes on the IR")
		("debug-info,g", "generate debug info")
		("jit,j", "run compiled code through jit immediately")
		("jit-console,console", "run jit compiled console")
	;
	boost::program_options::positional_options_description pos_desc;
	pos_desc.add("input-file", -1);
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(opt_desc).positional(pos_desc).run(), vm);
	boost::program_options::notify(vm);
	if (vm.contains("help")) {
		std::cout << "Usage: tgc [options] <input files...>" << std::endl;
		opt_desc.print(std::cout);
		return 0;
	} else if (vm.contains("version")) {
		std::cout << "The golden compiler version " << version_str << std::endl;
		return 0;
	}
	if (vm.contains("jit-console")) {
		std::cerr << error_pref << "JIT console isn't implemented yet" << std::endl;
		return 1;
	}
	if (!vm.contains("input-file")) {
		std::cerr << error_pref << "no input files." << std::endl;
		return 1;
	}
	opts.rcode = 0;
	for (int i = 1; i < argc; i++)
		opts.args += std::string(i > 1 ? " " : "") + argv[i];
	opts.verbose = vm.contains("verbose");
	opts.show_ir = vm.contains("show-ir");
	opts.show_instrs = vm.contains("show-instrs");
	opts.jit = vm.contains("jit");
	if (opts.jit) {
		std::cerr << error_pref << "JIT isn't implemented yet" << std::endl;
	}
	opts.optimize = vm.contains("optimize");
	opts.debug_info = vm.contains("debug-info");
	flags fs{"", "", vm.contains("ver") ? vm["ver"].as<std::string>() : "0.4.0E"};
	std::vector<std::string> ifs = vm["input-file"].as<std::vector<std::string>>();
	codegen cg;
	for (const auto &fn : ifs) {
		if (opts.verbose) std::cout << "reading " << fn << std::endl;
		flags fflags(fs);
		fflags.file = fn;
		{
			std::ifstream f(fn);
			if (!f.good()) {
				opts.rcode = 1;
				std::cerr << error_pref << "unable to open file '" << fn << "'" << std::endl;
				continue;
			}
			std::stringstream ss;
			ss << f.rdbuf();
			fflags.filec = ss.str();
		}
		if (opts.verbose) std::cout << "compiling " << fn << std::endl;
		auto ts = lex(fflags);
		if (opts.verbose) std::cout << "lexing complete for " << fn << std::endl;
		cg.gen(fflags, ts);
		if (opts.verbose) std::cout << "codegen complete for " << fn << std::endl;
	}
	cg.to_obj(fs, vm["output"].as<std::string>());
	return opts.rcode;
}
