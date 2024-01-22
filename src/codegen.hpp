#pragma once
#include <memory>
#include <vector>
#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include "parser/lex.hpp"

struct codegen {
public:
	llvm::LLVMContext ctx;
	llvm::IRBuilder<> irb;
	std::vector<std::unique_ptr<llvm::Module>> modules;
	std::vector<std::unique_ptr<llvm::DIBuilder>> dibs;

	codegen();
	void gen(const flags &fs, std::vector<token_t> &tokens);
	void to_obj(const flags &fs, const std::string &file);
private:
	void optimize();
};
