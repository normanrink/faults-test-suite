
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"

using namespace llvm;

namespace {
  struct NoEncInlinePass : public FunctionPass {
    NoEncInlinePass() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override;

    static char ID;
  };
}

char NoEncInlinePass::ID = 0;

bool NoEncInlinePass::runOnFunction(Function &F) {
  if (F.getName().startswith_lower("___enc_")) {
    F.addFnAttr(Attribute::NoInline);
    return true;
  }

  return false;
}

static RegisterPass<NoEncInlinePass> X("NoEncInline",
                                       "",
                                       false,
                                       false);

Pass *createNoEncInlinePass() {
  return new NoEncInlinePass();
}
