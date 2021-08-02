#include "ast.hh"
#include "engine.hh"
#include "instruction.hh"
#include "parser.hh"
#include "type.hh"
#include "value.hh"

#include <iostream>
#include <vector>

using namespace plsm;

FunctionValue *printFunc() {
  Instruction *tmpInst = new CustomInstruction([](Engine *engine) {
    std::cout << engine->stackPop()->toString() << std::endl;
    engine->stackPush(new UndefinedValue());
    return 1;
  });
  return new FunctionValue(1, {tmpInst, new ReturnInstruction()});
}

void printUsage(char *arg0) {
  std::cout << "usage: " << arg0 << " <file>" << std::endl;
}

int main(int argc, char **argv) {
  if (argc <= 0) {
    return 1;
  } else if (argc <= 1) {
    printUsage(argv[0]);
    return 1;
  }

  std::u32string text = readFile(argv[1]);

  std::vector<Instruction *> insts = (new LLParser(text))->parse();

  auto types = Type::getStandardTypes();

  Engine *engine = new Engine({}, insts);
  engine->defineGlobal("print", printFunc());

  return engine->execute({});
}
