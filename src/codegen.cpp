#include "codegen.hpp"
#include <iostream>
#include <set>
#include <unordered_map>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>

using namespace std::string_literals;

std::vector<std::tuple<int64_t, size_t, size_t>> get_funs(const flags &fs, std::vector<token_t> &tokens, std::set<int64_t> &idl) {
	std::vector<std::tuple<int64_t, size_t, size_t>> out;
	std::unordered_map<int64_t, size_t> active_funs;
	unsigned int in_call = 0;
	for (size_t i = 0; i < tokens.size(); i++) {
		if (tokens[i].t == token_kind::id) {
			idl.insert(tokens[i].value);
 			if (!in_call) {
				auto j = active_funs.find(tokens[i].value);
				if (j == active_funs.end()) {
					active_funs[tokens[i].value] = i;
				} else {
					out.push_back({ tokens[i].value, j->second, i });
				}
			}
		}
		in_call += (tokens[i].t == token_kind::clls);
		if (!in_call && tokens[i].t == token_kind::clle) {
			std::cout << error_pref << "call end without call start in " << fs.file << " " << tokens[i].begin << std::endl;
		}
		in_call -= (tokens[i].t == token_kind::clle);
	}
	return out;
}

codegen::codegen() : irb(ctx) {  }
void codegen::gen(const flags &fs, std::vector<token_t> &tokens) {
	if (fs.tgver != "0.4.0E") {
		throw std::runtime_error("Unknown TG version.");
	}
	std::set<int64_t> idl;
	const auto funs = get_funs(fs, tokens, idl);
	modules.push_back(std::make_unique<llvm::Module>(fs.file, ctx));
	llvm::Module *mod = modules.back().get();

	llvm::Type *tvp = llvm::PointerType::get(ctx, 0);
	llvm::Type *tv = llvm::Type::getVoidTy(ctx);
	llvm::Type *td = llvm::Type::getDoubleTy(ctx);
	llvm::Type *ti64 = llvm::Type::getInt64Ty(ctx);
	llvm::FunctionType *vp_v_funt = llvm::FunctionType::get(tvp, { }, false);
	llvm::FunctionType *v_vp_funt = llvm::FunctionType::get(tv, { tvp }, false);
	llvm::FunctionType *vp_vp_funt = llvm::FunctionType::get(tvp, { tvp }, false);
	llvm::FunctionType *v_vp_vp_funt = llvm::FunctionType::get(tv, { tvp, tvp }, false);
	llvm::FunctionType *v_vp_i64_funt = llvm::FunctionType::get(tv, { tvp, ti64 }, false);
	llvm::FunctionType *d_vp_funt = llvm::FunctionType::get(td, { tvp }, false);
	llvm::FunctionType *v_vp_d_funt = llvm::FunctionType::get(tv, { tvp, td }, false);
	llvm::FunctionType *d_d_funt = llvm::FunctionType::get(td, { td }, false);
	llvm::FunctionType *d_v_funt = llvm::FunctionType::get(td, { }, false);
	llvm::FunctionType *i_v_funt = llvm::FunctionType::get(llvm::Type::getInt32Ty(ctx), { }, false);
	llvm::Function *cl_memory_poold1k_alloc = llvm::Function::Create(vp_vp_funt, llvm::Function::ExternalLinkage, 0, "corelib_memory_poold1k_alloc", mod);
	llvm::Function *cl_memory_poold1k_dealloc = llvm::Function::Create(v_vp_vp_funt, llvm::Function::ExternalLinkage, 0, "corelib_memory_poold1k_dealloc", mod);
	llvm::Function *cl_memoryd_movea = llvm::Function::Create(v_vp_i64_funt, llvm::Function::ExternalLinkage, 0, "corelib_memoryd_movea", mod);
	llvm::Function *cl_memoryd_swap = llvm::Function::Create(v_vp_funt, llvm::Function::ExternalLinkage, 0, "corelib_memoryd_swap", mod);
	llvm::Function *cl_memoryd_seta = llvm::Function::Create(v_vp_d_funt, llvm::Function::ExternalLinkage, 0, "corelib_memoryd_seta", mod);
	llvm::Function *cl_memoryd_geta = llvm::Function::Create(d_vp_funt, llvm::Function::ExternalLinkage, 0, "corelib_memoryd_geta", mod);
	llvm::Function *cl_memoryd_getb = llvm::Function::Create(d_vp_funt, llvm::Function::ExternalLinkage, 0, "corelib_memoryd_getb", mod);
	llvm::Function *cl_memoryd_idxa = llvm::Function::Create(v_vp_funt, llvm::Function::ExternalLinkage, 0, "corelib_memoryd_idxa", mod);
	llvm::Function *cl_memoryd_cios[] = {
		llvm::Function::Create(v_vp_funt, llvm::Function::ExternalLinkage, 0, "corelib_memoryd_scanna", mod),
		llvm::Function::Create(v_vp_funt, llvm::Function::ExternalLinkage, 0, "corelib_memoryd_printna", mod),
		llvm::Function::Create(v_vp_funt, llvm::Function::ExternalLinkage, 0, "corelib_memoryd_scanca", mod),
		llvm::Function::Create(v_vp_funt, llvm::Function::ExternalLinkage, 0, "corelib_memoryd_printca", mod)
	};
	llvm::Function *cl_floor = llvm::Function::Create(d_d_funt, llvm::Function::ExternalLinkage, 0, "corelib_floord", mod);
	llvm::Function *cl_ceil = llvm::Function::Create(d_d_funt, llvm::Function::ExternalLinkage, 0, "corelib_ceild", mod);
	llvm::Function *cl_rand = llvm::Function::Create(d_v_funt, llvm::Function::ExternalLinkage, 0, "corelib_randd", mod);
	llvm::Function *cl_flrpow = llvm::Function::Create(d_d_funt, llvm::Function::ExternalLinkage, 0, "corelib_flrpowd", mod);

	llvm::FunctionType *ft = llvm::FunctionType::get(tv, { tvp, tvp, tvp }, false);
	std::unordered_map<int64_t, llvm::Function *> functions;
	for (int64_t i : idl) {
		llvm::Function *f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, 0, "tg_fun_"+std::to_string(static_cast<uint64_t>(i)), mod);
		f->arg_begin()[0].setName("memory_pool");
		f->arg_begin()[1].setName("global");
		f->arg_begin()[2].setName("local");
		functions[i] = f;
	}
	for (const auto &[id, begin, end] : funs) {
		llvm::Function *f = functions[id];
		if (!f->empty()) {
			std::cerr << error_pref << "function " << id << " redefinition in " << fs.file << " " << tokens[begin].begin << std::endl;
			continue;
		}
		std::vector<std::array<llvm::BasicBlock *, 2>> wloops;
		std::vector<std::array<llvm::BasicBlock *, 2>> dwloops;
		std::vector<bool> loopt;
		bool global = false;
		llvm::BasicBlock *bb = llvm::BasicBlock::Create(ctx, "entry", f);
		irb.SetInsertPoint(bb);
		size_t ci = begin+1;
		llvm::Argument *arg_pool = &f->arg_begin()[0];
		llvm::Argument *arg_global = &f->arg_begin()[1];
		llvm::Argument *arg_local = &f->arg_begin()[2];
		llvm::Value *amem = global ? arg_global : arg_local;
		int64_t rep = 1;
		bool repv = false;
		if (opts.show_instrs) std::cout << "fun " << id << ":" << std::endl;
		std::vector<llvm::Value *> frame_stack = { arg_global };
		while (ci != end) {
			token_t t = tokens[ci++];
			if (opts.show_instrs) std::cout << "\t" << op_names[size_t(t.t)] << " " << t.value << std::endl;
			switch (t.t) {
			case token_kind::mdf:{
				llvm::Value *a = irb.CreateCall(cl_memoryd_geta, { amem }, "opa");
				llvm::Value *r;
				if (repv)
					r = irb.CreateFMul(a, llvm::ConstantFP::get(td, rep + t.value), "rv_mdf_res");
				else
					r = irb.CreateFAdd(a, llvm::ConstantFP::get(td, rep * t.value), "mdf_res");
				irb.CreateCall(cl_memoryd_seta, { amem, r });
				}break;
			case token_kind::mathb:{
				llvm::Value *a = irb.CreateCall(cl_memoryd_geta, { amem }, "opa");
				llvm::Value *b = irb.CreateCall(cl_memoryd_getb, { amem }, "opb");
				llvm::Value *r;
				unsigned int opid = t.value;
				if (rep < 0) {
					rep = -rep;
					opid ^= 1;
				}
				switch (opid) {
				default:
					if (rep != 1) { b = irb.CreateFMul(b, llvm::ConstantFP::get(td, rep)); }
					if (repv) { b = irb.CreateFMul(b, a); }
					r = irb.CreateFAdd(a, b, "add_res");
					break;
				case 1:
					if (rep != 1) { b = irb.CreateFMul(b, llvm::ConstantFP::get(td, rep)); }
					if (repv) { b = irb.CreateFMul(b, a); }
					r = irb.CreateFSub(a, b, "sub_res");
					break;
				case 2:
					if (repv) { b = irb.CreateCall(cl_flrpow, { b, a }); }
					for (int64_t i = 0; i < rep; i++) { a = irb.CreateFMul(a, b, "mul_res"); } r = a;
					break;
				case 3:
					if (repv) { b = irb.CreateCall(cl_flrpow, { b, a }); }
					for (int64_t i = 0; i < rep; i++) { a = irb.CreateFDiv(a, b, "div_res"); } r = a;
					break;
				}
				irb.CreateCall(cl_memoryd_seta, { amem, r });
				}break;
			case token_kind::mathu:{
				if (rep != 1 || repv) { t.begin.message(fs, error_pref+"flr and ceil aren't repeatable commands"s); }
				llvm::Value *a = irb.CreateCall(cl_memoryd_geta, { amem }, "opa");
				llvm::Value *r;
				switch (t.value) {
				default: r = irb.CreateCall(cl_floor, { a }, "floor_res"); break;
				case 1: r = irb.CreateCall(cl_ceil, { a }, "ceil_res"); break;
				}
				irb.CreateCall(cl_memoryd_seta, { amem, r });
				}break;
			case token_kind::rnd:{
				if (rep != 1 || repv) { t.begin.message(fs, error_pref+"rnd isn't a repeatable command"s); }
				llvm::Value *rndv = irb.CreateCall(cl_rand, { }, "random_value");
				irb.CreateCall(cl_memoryd_seta, { amem, rndv });
				}break;
			case token_kind::sft:
				if (repv) {
					llvm::Value *a = irb.CreateCall(cl_memoryd_geta, { amem }, "repv");
					a = irb.CreateCall(cl_floor, { a }, "floor_res");
					a = irb.CreateFPToSI(a, ti64);
					a = irb.CreateMul(a, llvm::ConstantInt::get(ti64, llvm::APInt(64, t.value * rep, true)));
					irb.CreateCall(cl_memoryd_movea, { amem, a });
				} else {
					irb.CreateCall(cl_memoryd_movea, { amem, llvm::ConstantInt::get(ti64, llvm::APInt(64, t.value * rep, true)) });
				}
				break;
			case token_kind::swp:
				if (rep != 1 || repv) { t.begin.message(fs, error_pref+"swp isn't a repeatable command"s); }
				irb.CreateCall(cl_memoryd_swap, { amem });
				break;
			case token_kind::cio:
				if ((t.value & 1) == 0 && (rep != 1 || repv)) { t.begin.message(fs, error_pref+"in_n and in_c aren't a repeatable command"s); rep = 1; repv = false; }
				if (rep < 0) { t.begin.message(fs, error_pref+"Negative repetition of outn and outc commands isn't allowed"s); rep = 0; }
				if (repv) {
					llvm::Value *a = irb.CreateCall(cl_memoryd_geta, { amem }, "repv");
					a = irb.CreateCall(cl_floor, { a }, "floor_res");
					a = irb.CreateFPToSI(a, ti64, "repvi");
					llvm::Value *cmp = irb.CreateICmpSGT(a, llvm::ConstantInt::get(ti64, llvm::APInt(64, 0, true)), "cmpres");
					llvm::BasicBlock *prev = irb.GetInsertBlock();
					llvm::BasicBlock *lbb = llvm::BasicBlock::Create(ctx, "rprintloop", f);
					llvm::BasicBlock *albb = llvm::BasicBlock::Create(ctx, "afterloop", f);
					irb.CreateCondBr(cmp, lbb, albb);
					irb.SetInsertPoint(lbb);
					llvm::PHINode *phi = irb.CreatePHI(ti64, 2, "i");
					phi->addIncoming(a, prev);
					llvm::Value *i = irb.CreateSub(phi, llvm::ConstantInt::get(ti64, llvm::APInt(64, 1, true)), "i_sub");
					phi->addIncoming(i, lbb);
					for (int64_t i = 0; i < rep; i++) {
						irb.CreateCall(cl_memoryd_cios[t.value], { amem });
					}
					cmp = irb.CreateICmpSGT(i, llvm::ConstantInt::get(ti64, llvm::APInt(64, 0, true)), "cmpres");
					irb.CreateCondBr(cmp, lbb, albb);
					irb.SetInsertPoint(albb);
				} else {
					for (int64_t i = 0; i < rep; i++) {
						irb.CreateCall(cl_memoryd_cios[t.value], { amem });
					}
				}
				break;
			case token_kind::lps:{
				if (rep != 1 || repv) { t.begin.message(fs, error_pref+"lps isn't a repeatable command"s); }
				loopt.push_back(t.value);
				auto &lpstack = t.value ? dwloops : wloops;
				lpstack.push_back({llvm::BasicBlock::Create(ctx, "loop", f), llvm::BasicBlock::Create(ctx, "afterloop", f)});
				if (t.value) {
					irb.CreateBr(lpstack.back()[0]);
				} else {
					llvm::Value *cond = irb.CreateCall(cl_memoryd_geta, { amem }, "cond");
					llvm::Value *cmpr = irb.CreateFCmpONE(cond, llvm::ConstantFP::get(td, 0.0), "cmpres");
					irb.CreateCondBr(cmpr, lpstack.back()[0], lpstack.back()[1]);
				}
				irb.SetInsertPoint(lpstack.back()[0]);
				}break;
			case token_kind::lpe:{
				if (rep != 1 || repv) { t.begin.message(fs, error_pref+"lpe isn't a repeatable command"s); }
				loopt.pop_back();
				auto &lpstack = t.value ? dwloops : wloops;
				llvm::Value *cond = irb.CreateCall(cl_memoryd_geta, { amem }, "cond");
				llvm::Value *cmpr = irb.CreateFCmpONE(cond, llvm::ConstantFP::get(td, 0.0), "cmpres");
				irb.CreateCondBr(cmpr, lpstack.back()[0], lpstack.back()[1]);
				irb.SetInsertPoint(lpstack.back()[1]);
				lpstack.pop_back();
				}break;
			case token_kind::idx:
				if (rep != 1 || repv) { t.begin.message(fs, error_pref+"idx isn't a repeatable command"s); }
				irb.CreateCall(cl_memoryd_idxa, { amem });
				break;
			case token_kind::tst:{
				if (rep < 1 || repv) { t.begin.message(fs, error_pref+"teq, tlt and tgt can't be repeated by-value or with non-negative repetition amount"s); break; }
				if (rep > int64_t(loopt.size())) { t.begin.message(fs, error_pref+"teq, tlt and tgt repetition can be only as high as is current loop depth+1"s); break; }
				llvm::Value *tva = irb.CreateCall(cl_memoryd_geta, { amem }, "testvala");
				llvm::Value *tvb = irb.CreateCall(cl_memoryd_getb, { amem }, "testvalb");
				llvm::Value *r;
				switch (t.value) {
				case -1: r = irb.CreateFCmpOLT(tva, tvb); break;
				default: r = irb.CreateFCmpOEQ(tva, tvb); break;
				case 1:  r = irb.CreateFCmpOGT(tva, tvb); break;
				}
				if (loopt.empty() || rep-1 == int64_t(loopt.size())) {
					llvm::BasicBlock *nbb = llvm::BasicBlock::Create(ctx, "aftertst", f);
					llvm::BasicBlock *trbb = llvm::BasicBlock::Create(ctx, "tstret", f);
					irb.CreateCondBr(r, trbb, nbb);
					irb.SetInsertPoint(trbb);
					irb.CreateRetVoid();
					irb.SetInsertPoint(nbb);
				} else {
					auto &lpstack = loopt.rbegin()[rep-1] ? dwloops : wloops;
					llvm::BasicBlock *nbb = llvm::BasicBlock::Create(ctx, "aftertst", f);
					irb.CreateCondBr(r, lpstack.rbegin()[rep-1][1], nbb);
					irb.SetInsertPoint(nbb);
				}
				}break;
			case token_kind::gtrf:{
				if (rep != 1 || repv) { t.begin.message(fs, error_pref+"gtrf isn't a repeatable command"s); }
				llvm::Value *ogl = irb.CreateCall(cl_memoryd_geta, { frame_stack.back() }, "old_global");
				llvm::Value *oloc = irb.CreateCall(cl_memoryd_geta, { arg_local }, "old_local");
				irb.CreateCall(cl_memoryd_seta, { frame_stack.back(), oloc });
				irb.CreateCall(cl_memoryd_seta, { arg_local, ogl });
				}break;
			case token_kind::gswp:
				if (rep != 1 || repv) { std::cout << error_pref << "gswp isn't a repeatable command " << fs.file << t.begin << std::endl; }
				global = !global;
				amem = global ? frame_stack.back() : arg_local;
				break;
			case token_kind::clls:{
				if (rep != 1 || repv) { t.begin.message(fs, error_pref+"clls isn't a repeatable command"s); }
				frame_stack.push_back(irb.CreateCall(cl_memory_poold1k_alloc, { arg_pool }, "sublocal"));
				if (global)
					amem = frame_stack.back();
				}break;
			case token_kind::clle:
				if (rep != 1 || repv) { t.begin.message(fs, error_pref+"clle isn't a repeatable command"s); }
				if (frame_stack.size() < 2) { t.begin.message(fs, error_pref+"clle without clls"s); break; }
				irb.CreateCall(cl_memory_poold1k_dealloc, { arg_pool, frame_stack.back() });
				frame_stack.pop_back();
				if (global)
					amem = frame_stack.back();
				break;
			case token_kind::rptv:
				if (repv) {
					t.begin.message(fs, error_pref+"by-value repetition of by-value repetition is not supported"s);
				}
				repv = true;
				break;
			case token_kind::rpt: rep *= t.value; break;
			case token_kind::id:
				if (!frame_stack.empty()) {
					if (rep != 1 || repv) { t.begin.message(fs, error_pref+"call isn't a repeatable command"s); }
					irb.CreateCall(functions[t.value], { arg_pool, arg_global, frame_stack.back() });
				}
				break;
			default: break; // eof
			}
			if (t.t != token_kind::rpt && t.t != token_kind::rptv) {
				rep = 1;
				repv = false;
			}
		}
		if (frame_stack.size() > 1) {
			tokens[end].begin.message(fs, error_pref+"there are unclosed clls frames (which cause memory and memory pool leaks)"s);
		}
		irb.CreateRetVoid();
		if (id == 79) {
			llvm::Function *cl_memory_poold1k_new = llvm::Function::Create(vp_v_funt, llvm::Function::ExternalLinkage, 0, "corelib_memory_poold1k_new", mod);
			llvm::Function *cl_memory_poold1k_free = llvm::Function::Create(v_vp_funt, llvm::Function::ExternalLinkage, 0, "corelib_memory_poold1k_free", mod);
			llvm::Function *main = llvm::Function::Create(i_v_funt, llvm::Function::ExternalLinkage, 0, "main", mod);
			llvm::BasicBlock *mbb = llvm::BasicBlock::Create(ctx, "entry", main);
			irb.SetInsertPoint(mbb);
			llvm::Value *pool = irb.CreateCall(cl_memory_poold1k_new, { }, "pool");
			llvm::Value *global_mem = irb.CreateCall(cl_memory_poold1k_alloc, { pool }, "global");
			llvm::Value *local_mem = irb.CreateCall(cl_memory_poold1k_alloc, { pool }, "local");
			irb.CreateCall(f, { pool, global_mem, local_mem });
			irb.CreateCall(cl_memory_poold1k_dealloc, { pool, local_mem });
			irb.CreateCall(cl_memory_poold1k_dealloc, { pool, global_mem });
			irb.CreateCall(cl_memory_poold1k_free, { pool });
			irb.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), llvm::APInt::getZero(32)));
		}
		if (opts.show_ir)
			f->print(llvm::errs());
	}
	llvm::verifyModule(*mod, &llvm::errs());
}
void codegen::optimize() {
	if (opts.verbose) std::cout << "running O2 optimization passes" << std::endl;
	llvm::LoopAnalysisManager LAM;
	llvm::FunctionAnalysisManager FAM;
	llvm::CGSCCAnalysisManager CGAM;
	llvm::ModuleAnalysisManager MAM;
	llvm::PassBuilder PB;
	PB.registerModuleAnalyses(MAM);
	PB.registerCGSCCAnalyses(CGAM);
	PB.registerFunctionAnalyses(FAM);
	PB.registerLoopAnalyses(LAM);
	PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);
	llvm::ModulePassManager MPM = PB.buildPerModuleDefaultPipeline(llvm::OptimizationLevel::O2);
	for (auto &mod : modules) {
		MPM.run(*mod, MAM);
	}
}
void codegen::to_obj(const flags &, const std::string &file) {
	std::string tt = llvm::sys::getDefaultTargetTriple();
	if (opts.verbose) std::cout << "target triple is " << tt << std::endl;
	llvm::InitializeAllTargetInfos();
	llvm::InitializeAllTargets();
	llvm::InitializeAllTargetMCs();
	llvm::InitializeAllAsmParsers();
	llvm::InitializeAllAsmPrinters();
	std::string err;
	auto trg = llvm::TargetRegistry::lookupTarget(tt, err);
	if (!trg) {
		std::cerr << error_pref << err << std::endl;
		return;
	}
	std::string cpu = "generic";
	std::string features = "";
	llvm::TargetOptions opt;
	auto trgm = trg->createTargetMachine(tt, cpu, features, opt, llvm::Reloc::PIC_);
	for (auto &mod : modules) {
		mod->setDataLayout(trgm->createDataLayout());
		mod->setTargetTriple(tt);
	}
	if (opts.optimize) {
		optimize();
	}
	std::error_code ec;
	llvm::raw_fd_ostream dest(file, ec, llvm::sys::fs::OF_None);
	if (ec) {
		opts.rcode = 1;
		std::cerr << error_pref << " failed to open output file - " << ec.message();
		return;
	}
	llvm::legacy::PassManager pass;
	auto FileType = llvm::CodeGenFileType::ObjectFile;
	if (trgm->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
		opts.rcode = 1;
		std::cerr << error_pref << "llvm target machine can't emit a file of this type" << std::endl;
		return;
	}
	for (auto &mod : modules) {
		pass.run(*mod);
	}
	dest.flush();
	if (opts.verbose) std::cout << "object file emit pass completed" << std::endl;
}
