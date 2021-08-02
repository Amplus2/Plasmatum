#pragma once

#include "types.hh"

#include <string>
#include <vector>

#include "type.hh"

namespace plsm {

class Engine;
class Instruction;

class Value {
public:
  Type *type;

  Value(Type *type) : type(type) {}

  virtual ~Value() = default;

  virtual inline std::string toString() = 0;

  plsm_float_t asFloat();
  plsm_int_t asInteger();
  plsm_bool_t asBoolean();

  virtual inline bool isTruthy() = 0;
  virtual inline bool isConstant() { return false; };

  virtual inline bool isFloat() { return false; }
  virtual inline bool isInteger() { return false; }
  virtual inline bool isBoolean() { return false; }
  virtual inline bool isFunction() { return false; }
};

class Constant : public Value {
public:
  Constant(Type *type) : Value(type) {}

  virtual ~Constant() = default;

  virtual inline bool isConstant() { return true; }
};

class UndefinedValue : public Constant {
public:
  UndefinedValue() : Constant(Type::getUndefinedType()) {}

  inline std::string toString() override { return "Undefined"; }

  inline bool isTruthy() override { return false; }
};

class IntegerValue : public Constant {
private:
  plsm_int_t value;

public:
  IntegerValue(plsm_int_t value)
      : Constant(Type::getIntegerType()), value(value) {}

  inline plsm_int_t getValue() { return value; }

  inline std::string toString() override { return std::to_string(value); }

  inline bool isTruthy() override { return value != 0; }
  inline bool isInteger() override { return true; }
};

class FloatValue : public Constant {
private:
  plsm_float_t value;

public:
  FloatValue(plsm_float_t value)
      : Constant(Type::getFloatType()), value(value) {}

  inline plsm_float_t getValue() { return value; }

  inline std::string toString() override { return std::to_string(value); }

  inline bool isTruthy() override { return value != 0.0; }
  inline bool isFloat() override { return true; }
};

class BooleanValue : public Constant {
private:
  plsm_bool_t value;

public:
  BooleanValue(plsm_bool_t value)
      : Constant(Type::getBooleanType()), value(value) {}

  inline plsm_bool_t getValue() { return value; }

  inline std::string toString() override { return value ? "True" : "False"; }

  inline bool isTruthy() override { return value == true; }
  inline bool isBoolean() override { return true; }
};

class FunctionValue : public Constant {
private:
  plsm_size_t argc;
  std::vector<Instruction *> instructions;

public:
  FunctionValue(plsm_size_t argc,
                const std::vector<Instruction *> &instructions)
      : Constant(Type::getFunctionType()), argc(argc),
        instructions(instructions) {}

  inline plsm_size_t getArgc() { return argc; }

  inline Instruction *getInstruction(plsm_size_t index) {
    return index >= instructions.size() ? nullptr : instructions[index];
  }

  void call(Engine *engine);

  inline std::string toString() override { return "Function Value"; }

  inline bool isTruthy() override { return true; }

  inline bool isFunction() override { return true; }
};

}
