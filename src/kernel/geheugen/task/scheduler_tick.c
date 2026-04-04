#include "geheugen.h"

uint64_t _contextRIP;
uint64_t _contextCS;
uint64_t _contextRFLAGS;
uint64_t _contextRSP;
uint64_t _contextSS;
uint64_t _contextRDX;
uint64_t _contextRCX;
uint64_t _contextRBX;
uint64_t _contextRAX;
uint64_t _contextRBP;
uint64_t _contextRDI;
uint64_t _contextRSI;
uint64_t _contextR8;
uint64_t _contextR9;
uint64_t _contextR10;
uint64_t _contextR11;
uint64_t _contextR12;
uint64_t _contextR13;
uint64_t _contextR14;
uint64_t _contextR15;
uint64_t _contextGS;
uint64_t _contextFS;
uint64_t _contextES;
uint64_t _contextDS;
uint64_t _contextCPL;

void scheduler_tick() {
    ct[current_task].context.rip = _contextRIP;
    ct[current_task].context.cs = _contextCS;
    ct[current_task].context.rflags = _contextRFLAGS;
    ct[current_task].context.rsp = _contextRSP;
    ct[current_task].context.ss = _contextSS;
    ct[current_task].context.rdx = _contextRDX;
    ct[current_task].context.rcx = _contextRCX;
    ct[current_task].context.rbx = _contextRBX;
    ct[current_task].context.rax = _contextRAX;
    ct[current_task].context.rbp = _contextRBP;
    ct[current_task].context.rdi = _contextRDI;
    ct[current_task].context.rsi = _contextRSI;
    ct[current_task].context.r8 = _contextR8;
    ct[current_task].context.r9 = _contextR9;
    ct[current_task].context.r10 = _contextR10;
    ct[current_task].context.r11 = _contextR11;
    ct[current_task].context.r12 = _contextR12;
    ct[current_task].context.r13 = _contextR13;
    ct[current_task].context.r14 = _contextR14;
    ct[current_task].context.r15 = _contextR15;
    ct[current_task].context.gs = _contextGS;
    ct[current_task].context.fs = _contextFS;
    ct[current_task].context.es = _contextES;
    ct[current_task].context.ds = _contextDS;
    ct[current_task].context.cpl = _contextCPL;
    ct[current_task].timerswatch = 1;

    current_task = (current_task + 1) % max_task;

    acknowledge_interrupt();
    
    _contextRIP = ct[current_task].context.rip;
    _contextCS = ct[current_task].context.cs;
    _contextRFLAGS = ct[current_task].context.rflags;
    _contextRSP = ct[current_task].context.rsp;
    _contextSS = ct[current_task].context.ss;
    _contextRDX = ct[current_task].context.rdx;
    _contextRCX = ct[current_task].context.rcx;
    _contextRBX = ct[current_task].context.rbx;
    _contextRAX = ct[current_task].context.rax;
    _contextRBP = ct[current_task].context.rbp;
    _contextRDI = ct[current_task].context.rdi;
    _contextRSI = ct[current_task].context.rsi;
    _contextR8 = ct[current_task].context.r8;
    _contextR9 = ct[current_task].context.r9;
    _contextR10 = ct[current_task].context.r10;
    _contextR11 = ct[current_task].context.r11;
    _contextR12 = ct[current_task].context.r12;
    _contextR13 = ct[current_task].context.r13;
    _contextR14 = ct[current_task].context.r14;
    _contextR15 = ct[current_task].context.r15;
    _contextGS = ct[current_task].context.gs;
    _contextFS = ct[current_task].context.fs;
    _contextES = ct[current_task].context.es;
    _contextDS = ct[current_task].context.ds;
    _contextCPL = ct[current_task].context.cpl;
}
