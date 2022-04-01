#include <iostream>
#include <string>
#include <bits/stdc++.h>
using namespace  std;


struct instruction{
    string opcode;
    int format;
};

struct symbol{
    string value;
    int flags;
    
};

struct block{
    int num;
    int address;
    int length;
};

struct literal{
    string value;
    string address;
};

struct textrecord{
    char record[69];
    int size;
};

//constructors
instruction* to_instr(string opcode,int format){
    auto *t = new instruction;
    t->opcode = opcode;
    t->format = format;
    return t;
}   


///need to work on the flags
symbol* to_sym(string val,int flags){
    auto i = new symbol;
    i->value = val;
    i->flags = flags;
    return i;
}
block* to_blk(int id,int address,int length){
    auto t = new block;
    t->num = id;
    t->address = address;
    t->length = length;
    return t;
}

literal* to_lit(string value,string address){
    auto t = new literal;
    t->value = value;
    t->address = address;
    return t;
}

map<string,instruction*>OPTAB{
    {"ADD",to_instr("18",3)},
    {"ADDF",to_instr("58",3)},
    {"ADDR",to_instr("90",2)},
    {"AND",to_instr("40",3)},
    {"CLEAR",to_instr("B4",2)},
    {"COMP",to_instr("28",3)},
    {"COMPF",to_instr("88",3)},
    {"COMPR",to_instr("A0",2)},
    {"DIV",to_instr("24",3)},
    {"DIVF",to_instr("64",3)},
    {"FIX",to_instr("C4",1)},
    {"FLOAT",to_instr("C0",1)},
    {"HIO",to_instr("F4",1)},
    {"J",to_instr("3C",3)},
    {"JEQ",to_instr("30",3)},
    {"JGT",to_instr("34",3)},
    {"JLT",to_instr("38",3)},
    {"JSUB",to_instr("48",3)},
    {"LDA",to_instr("00",3)},
    {"LDB",to_instr("68",3)},
    {"LDCH",to_instr("50",3)},
    {"LDF",to_instr("70",3)},
    {"LDL",to_instr("08",3)},
    {"LDS",to_instr("6C",3)},
    {"LDT",to_instr("74",3)},
    {"LDX",to_instr("04",3)},
    {"LPS",to_instr("D0",3)},
    {"MUL",to_instr("20",3)},
    {"MULF",to_instr("60",3)},
    {"MULR",to_instr("98",2)},
    {"NORM",to_instr("C8",1)},
    {"OR",to_instr("44",3)},
    {"RD",to_instr("D8",3)},
    {"RMO",to_instr("AC",2)},
    {"RSUB",to_instr("4C",3)},
    {"SHIFTL",to_instr("A4",2)},
    {"SHIFTR",to_instr("A8",2)},
    {"SIO",to_instr("F0",1)},
    {"SSK",to_instr("EC",3)},
    {"STA",to_instr("0C",3)},
    {"STB",to_instr("78",3)},
    {"STCH",to_instr("54",3)},
    {"STF",to_instr("80",3)},
    {"STI",to_instr("D4",3)},
    {"STL",to_instr("14",3)},
    {"STS",to_instr("7C",3)},
    {"STSW",to_instr("E8",3)},
    {"STT",to_instr("84",3)},
    {"STX",to_instr("10",3)},
    {"SUB",to_instr("1C",3)},
    {"SUBF",to_instr("5C",3)},
    {"SUBR",to_instr("94",2)},
    {"SVC",to_instr("B0",2)},
    {"TD",to_instr("E0",3)},
    {"TIO",to_instr("F8",1)},
    {"TIX",to_instr("2C",3)},
    {"TIXR",to_instr("B8",3)},
    {"WD",to_instr("DC",3)},
};



map<string,symbol*>STAB{
    {"A",to_sym("0",0)},
    {"X",to_sym("1",0)},
    {"L",to_sym("2",0)},
    {"B",to_sym("3",0)},
    {"S",to_sym("4",0)},
    {"T",to_sym("5",0)},
    {"F",to_sym("6",0)},
    {"PC",to_sym("6",0)},
    {"SW",to_sym("9",0)},

};


map<string,block*>BLOCKTAB;
map<string,literal*>LITTAB;







