#pragma once

#include "frame.hpp"
#include "class_loader.hpp"
#include "array_obj.hpp"
#include "string_obj.hpp"
#include "class_obj.hpp"
#include "utils.hpp"

#include <stack>
#include <algorithm>

void ldc2_w(stack<Frame>*);

void dstore_1(stack<Frame>*);

void dstore_3(stack<Frame>*);

void getstatic(stack<Frame>*);

void dsub(stack<Frame>*);

void dadd(stack<Frame>*);

void invokevirtual(stack<Frame>*);

void dmul(stack<Frame>*);

void ddiv(stack<Frame>*);

void dneg(stack<Frame>*);

void drem(stack<Frame>*);

void return_instruction(stack<Frame>*);

void bipush(stack<Frame>*);

void istore_1(stack<Frame>*);

void iload_1(stack<Frame>*);

void nop(stack<Frame>*);

void aconst_null(stack<Frame>*);

void iconst_m1(stack<Frame>*);

void iconst_0(stack<Frame>*);

void iconst_1(stack<Frame>*);

void iconst_2(stack<Frame>*);

void iconst_3(stack<Frame>*);

void iconst_4(stack<Frame>*);

void iconst_5(stack<Frame>*);

void lconst_0(stack<Frame>*);

void lconst_1(stack<Frame>*);

void fconst_0(stack<Frame>*);

void fconst_1(stack<Frame>*);

void fconst_2(stack<Frame>*);

void dconst_0(stack<Frame>*);

void dconst_1(stack<Frame>*);

void sipush(stack<Frame>*);

void ldc(stack<Frame>*);

void ldc_w(stack<Frame>*);

void iload(stack<Frame>*);

void lload(stack<Frame>*);

void fload(stack<Frame>*);

void dload(stack<Frame>*);

void aload(stack<Frame>*);

void iload_0(stack<Frame>*);

void iload_2(stack<Frame>*);

void iload_3(stack<Frame>*);

void lload_0(stack<Frame>*);

void lload_1(stack<Frame>*);

void lload_2(stack<Frame>*);

void lload_3(stack<Frame>*);

void fload_0(stack<Frame>*);

void fload_1(stack<Frame>*);

void fload_2(stack<Frame>*);

void fload_3(stack<Frame>*);

void dload_0(stack<Frame>*);

void dload_1(stack<Frame>*);

void dload_2(stack<Frame>*);

void dload_3(stack<Frame>*);

void aload_0(stack<Frame>*);

void aload_1(stack<Frame>*);

void aload_2(stack<Frame>*);

void aload_3(stack<Frame>*);

void iaload(stack<Frame>*);

void laload(stack<Frame>*);

void faload(stack<Frame>*);

void daload(stack<Frame>*);

void aaload(stack<Frame>*);

void baload(stack<Frame>*);

void caload(stack<Frame>*);

void saload(stack<Frame>*);

void istore(stack<Frame>*);

void lstore(stack<Frame>*);

void fstore(stack<Frame>*);

void dstore(stack<Frame>*);

void astore(stack<Frame>*);

void istore_0(stack<Frame>*);

void istore_2(stack<Frame>*);

void istore_3(stack<Frame>*);

void lstore_0(stack<Frame>*);

void lstore_1(stack<Frame>*);

void lstore_2(stack<Frame>*);

void lstore_3(stack<Frame>*);

void fstore_0(stack<Frame>*);

void fstore_1(stack<Frame>*);

void fstore_2(stack<Frame>*);

void fstore_3(stack<Frame>*);

void dstore_0(stack<Frame>*);

void dstore_2(stack<Frame>*);

void astore_0(stack<Frame>*);

void astore_1(stack<Frame>*);

void astore_2(stack<Frame>*);

void astore_3(stack<Frame>*);

void iastore(stack<Frame>*);

void lastore(stack<Frame>*);

void fastore(stack<Frame>*);

void dastore(stack<Frame>*);

void aastore(stack<Frame>*);

void bastore(stack<Frame>*);

void castore(stack<Frame>*);

void sastore(stack<Frame>*);

void pop(stack<Frame>*);

void pop2(stack<Frame>*);

void dup(stack<Frame>*);

void dup_x1(stack<Frame>*);

void dup_x2(stack<Frame>*);

void dup2(stack<Frame>*);

void dup2_x1(stack<Frame>*);

void dup2_x2(stack<Frame>*);

void swap(stack<Frame>*);

void iadd(stack<Frame>*);

void ladd(stack<Frame>*);

void fadd(stack<Frame>*);

void isub(stack<Frame>*);

void lsub(stack<Frame>*);

void fsub(stack<Frame>*);

void imul(stack<Frame>*);

void lmul(stack<Frame>*);

void fmul(stack<Frame>*);

void idiv(stack<Frame>*);

void ldiv_instruction(stack<Frame>*);

void fdiv(stack<Frame>*);

void irem(stack<Frame>*);

void lrem(stack<Frame>*);

void frem(stack<Frame>*);

void ineg(stack<Frame>*);

void lneg(stack<Frame>*);

void fneg(stack<Frame>*);

void ishl(stack<Frame>*);

void lshl(stack<Frame>*);

void ishr(stack<Frame>*);

void lshr(stack<Frame>*);

void iushr(stack<Frame>*);

void lushr(stack<Frame>*);

void iand(stack<Frame>*);

void land(stack<Frame>*);

void ior(stack<Frame>*);

void lor(stack<Frame>*);

void ixor(stack<Frame>*);

void lxor(stack<Frame>*);

void iinc(stack<Frame>*);

void i2l(stack<Frame>*);

void i2f(stack<Frame>*);

void i2d(stack<Frame>*);

void l2i(stack<Frame>*);

void l2f(stack<Frame>*);

void l2d(stack<Frame>*);

void f2i(stack<Frame>*);

void f2l(stack<Frame>*);

void f2d(stack<Frame>*);

void d2i(stack<Frame>*);

void d2l(stack<Frame>*);

void d2f(stack<Frame>*);

void i2b(stack<Frame>*);

void i2c(stack<Frame>*);

void i2s(stack<Frame>*);

void lcmp(stack<Frame>*);

void fcmpl(stack<Frame>*);

void fcmpg(stack<Frame>*);

void dcmpl(stack<Frame>*);

void dcmpg(stack<Frame>*);

void ifeq(stack<Frame>*);

void ifne(stack<Frame>*);

void iflt(stack<Frame>*);

void ifge(stack<Frame>*);

void ifgt(stack<Frame>*);

void ifle(stack<Frame>*);

void if_icmpeq(stack<Frame>*);

void if_icmpne(stack<Frame>*);

void if_icmplt(stack<Frame>*);

void if_icmpge(stack<Frame>*);

void if_icmpgt(stack<Frame>*);

void if_icmple(stack<Frame>*);

void if_acmpeq(stack<Frame>*);

void if_acmpne(stack<Frame>*);

void goto_instruction(stack<Frame>*);

void jsr(stack<Frame>*);

void ret(stack<Frame>*);

void tableswitch(stack<Frame>*);

void lookupswitch(stack<Frame>*);

void ireturn(stack<Frame>*);

void lreturn(stack<Frame>*);

void freturn(stack<Frame>*);

void dreturn(stack<Frame>*);

void areturn(stack<Frame>*);

void putstatic(stack<Frame>*);

void getfield(stack<Frame>*);

void putfield(stack<Frame>*);

void invokespecial(stack<Frame>*);

void invokestatic(stack<Frame>*);

void invokeinterface(stack<Frame>*);

void invokedynamic(stack<Frame>*);

void new_instruction(stack<Frame>*);

void newarray(stack<Frame>*);

void anewarray(stack<Frame>*);

void arraylength(stack<Frame>*);

void athrow(stack<Frame>*);

void checkcast(stack<Frame>*);

void instanceof(stack<Frame>*);

void monitorenter(stack<Frame>*);

void monitorexit(stack<Frame>*);

void wide(stack<Frame>*);

void multianewarray(stack<Frame>*);

void ifnull(stack<Frame>*);

void ifnonnull(stack<Frame>*);

void goto_w(stack<Frame>*);

void jsr_w(stack<Frame>*);