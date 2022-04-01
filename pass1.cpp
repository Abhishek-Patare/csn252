#include <bits/stdc++.h>
using namespace std;

#include "data_structures.cpp"
#include "lib.cpp"

int program_length;
string start_addr;

vector<string> parse_for_pass1(string s)
{
    vector<string> parsedLine;
    int i = 0;
    while (i < (int)s.length())
    {
        while (i < (int)s.length() && isBlank(s[i]))
            ++i;
        string tmp = "";
        if (i >= (int)s.length())
            break;
        while (i < (int)s.length() && !isBlank(s[i]))
        {
            tmp += s[i];
            ++i;
        }
        parsedLine.push_back(tmp);
    }
    if (parsedLine.size() <= 2)
    {
        parsedLine.insert(parsedLine.cbegin(), "\t");
    }
    return parsedLine;
}

vector<string> parse_for_pass2(string s)
{
    vector<string> parsedLine;
    int i = 0;
    while (i < (int)s.length())
    {
        while (i < (int)s.length() && isBlank(s[i]))
            ++i;
        string tmp = "";
        if (i >= (int)s.length())
            break;
        while (i < (int)s.length() && !isBlank(s[i]))
        {
            tmp += s[i];
            ++i;
        }
        parsedLine.push_back(tmp);
    }
    if (parsedLine.size() <= 4)
    {
        auto it = parsedLine.begin();
        for (int p = 0; p < 3; ++p)
            it++;
        parsedLine.insert(it, "\t");
    }
    return parsedLine;
}

int locctrDelta(vector<string> l)
{
    if (l[1][0] == '+')
        return 6; // format4 insruction
    if (OPTAB[l[1]] != nullptr)
    {
        return OPTAB[l[1]]->format;
    }
    if (l[1] == "RESW")
        return stoi(l[2]) * 3;
    if (l[1] == "RESB")
        return stoi(l[2]);
    if (l[1] == "WORD")
        return 3;
    if (l[1] == "BYTE")
    {
        if (l[2][0] == 'X')
            return 1;
        else
            return l[2].length() - 3;
    }
    // need to modify this for wrong code
    return 0;
}

textrecord *initializeTxtRecord(string start_adr)
{
    auto t = new textrecord;
    t->size = 0;
    t->record[0] = 'T';
    for (int i = 1; i < 7; ++i)
        t->record[i] = start_adr[i - 1];
    for (int i = 7; i < 69; ++i)
        t->record[i] = ' ';
    return t;
};
void resetRecord(textrecord *t, string start_adr)
{
    t->size = 0;
    t->record[0] = 'T';
    for (int i = 1; i < 7; ++i)
        t->record[i] = start_addr[i - 1];
    for (int i = 7; i < 69; ++i)
        t->record[i] = ' ';
}
void addtxtRecord(textrecord *t, string objcode)
{
    for (int i = 0; i < objcode.length(); ++i)
    {
        t->record[i + t->size] = objcode[i];
    }
    t->size += objcode.length();
}

void executePass1(ifstream &fin, ofstream &fout)
{
    int line = 0;
    string block = "";
    int locctr = 0;
    int lastAddr =0;

    string s;
    getline(fin, s);
    line++;
    vector<string> p = parse_for_pass1(s);
    if (p[1] == "START")
    {
        start_addr = p[2];
        locctr = hex_toInt(start_addr);
    }

    bool ismultilineComment = false;
    while (getline(fin, s))
    {
        line++;
        /////deal with comments;
        ismultilineComment = (s[0] == '/' && s[1] == '*');
        if (ismultilineComment && (s[s.length() - 1] == '/' && s[s.length() - 2] == '*'))
        {
            ismultilineComment = false;
            continue;
        }
        if ((s[0] == '/' && s[1] == '/'))
            continue;
        vector<string> l = parse_for_pass1(s);
        if (l[1] == "END")
            break;
        if (l[3] == "END")
            break;
        


        //blocks
        //currently assuming that default block is labelled as default i,e USE DEFAULT
        if(l[1] == "USE"){
            if(l[2]=="")l[2]="DEFAULT";
            if(block!=""){
                BLOCKTAB[block]->length = locctr;
            }
            block = l[2];
            if(BLOCKTAB[l[3]]==nullptr){
                BLOCKTAB[l[3]] = to_blk(BLOCKTAB.size(),0,0);
                locctr = 0;
            }else{
                locctr = BLOCKTAB[block]->length;
            }
        }


        if (l[0] != "\t")
        {
            if (STAB[l[0]] != nullptr)
            {
                // error
            }
            else
            {
                // insert it in symtab
                STAB[l[0]] = to_sym(int_tohex(locctr,5),0);
            }
        }
        // need to update later
        fout << locctr << " "  << "\t" << l[0] << "\t" << l[1] << "\t" << l[2] << endl;
        locctr += locctrDelta(l);
        lastAddr = max(lastAddr,locctr);
    }
    program_length = locctr - hex_toInt(start_addr);
}
string format1Obj(string &ins)
{
    return OPTAB[ins]->opcode;
}
string format2Obj(string &ins, string &operand)
{
    string opcode, r1, r2;
    opcode = OPTAB[ins]->opcode;
    r1 = int_tohex((STAB[operand.substr(0, operand.find(','))]->value[0] - '0'), 1);
    r2 = int_tohex((STAB[operand.substr(operand.find(',') + 1)]->value[0] - '0'), 1);
    return opcode + r1 + r2;
}
string format3Obj(string &ins, string &operand, string &addr, int base, bool b)
{
    vector<bool> flag(6),opcode(6);
    string opcodeAndFlags, displacement;
    opcode = hex_tobool(OPTAB[ins.substr(1)]->opcode,6);
    flag[5]=1;
    flag[2] = (operand.find(',') < operand.length() - 1 && operand[operand.find(',') + 1] == 'X');

    int pc = hex_toInt(addr) + 3;
    if (operand[0] == '#')
    {

        

        if (STAB[operand.substr(1)] != nullptr)
        {



        //when will this happen???????????
        //equ????
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //     flag[0] = true;
        //     flag[1] = true;
        //     int opr = stoi(operand.substr(1));
        //     if ((opr - pc) >= -2048 && (pc - opr) < 2048)
        //     {
        //         flag[4] = true;
        //         displacement = int_tohex(opr - pc, 3);
        //     }
        //     else if (b)
        //     {
        //         if (b <= opr && (opr - b) < 4096)
        //         {
        //             flag[3] = true;
        //             displacement = int_tohex(opr - b, 3);
        //         }
        //         else
        //         {
        //             // err
        //         }
        //     }
        }
        else
        {
            int opr = stoi(operand.substr(1));
            if ((opr - pc) >= -2048 && (pc - opr) < 2048)
            {
                flag[4] = true;
                displacement = int_tohex(opr - pc, 3);
            }
            else if (b&&(base <= opr && (opr - base) < 4096))
            {
                    flag[3] = true;
                    displacement = int_tohex(opr - base, 3);
            }
            else{
                //err;
            }


        }
    }
    else if (operand[0] == '@')
    {
        flag[0] = 1;
        int opr = stoi(operand.substr(1));
        if ((opr - pc) >= -2048 && (pc - opr) < 2048)
        {
            flag[4] = true;
            displacement = int_tohex(opr - pc, 3);
        }
        else if (b&&(base <= opr && (opr - base) < 4096))
        {
                flag[3] = true;
                displacement = int_tohex(opr - base, 3);
        }
        else{
            //err;
        }
    }
    else
    {
        flag[0]=flag[1]=1;

        int opr = stoi(operand.substr(1));
        if ((opr - pc) >= -2048 && (pc - opr) < 2048)
        {
            flag[4] = true;
            displacement = int_tohex(opr - pc, 3);
        }
        else if (b&&(base <= opr && (opr - base) < 4096))
        {
                flag[3] = true;
                displacement = int_tohex(opr - base, 3);
        }
        else{
            //err;
        }
    }
    opcodeAndFlags= bool_tohex(opcode,flag,3);
    return opcodeAndFlags+displacement;
}
string format4Obj(string &ins, string &operand)
{
    vector<bool> flag(6),opcode(6);
    string opcodeAndFlags, displacement;
    opcode = hex_tobool(OPTAB[ins.substr(1)]->opcode,6);
    flag[5]=1;
    flag[2] = (operand.find(',') < operand.length() - 1 && operand[operand.find(',') + 1] == 'X');
    if (operand[0] == '#')
    {
        if (STAB[operand.substr(1)] != nullptr)
        {
            displacement = format_to_size(STAB[operand.substr(1)]->value, 5, '0');
        }
        else
        {
            displacement = int_tohex(stoi(operand.substr(1)), 5);
            flag[1]=1;
        }
    }
    else if (operand[0] == '@')
    {
        flag[0] = true;
        displacement = format_to_size(STAB[operand.substr(1)]->value, 5, '0');
    }
    else
    {
        flag[0] = 1;
        flag[1] = 1;
        displacement = format_to_size(STAB[operand]->value, 5, '0');
    }
    opcodeAndFlags= bool_tohex(opcode,flag,3);
    return opcodeAndFlags+displacement;
}

void executePass2(ifstream &fin, ofstream &f1out, ofstream &f2out)
{
    int line = 0;
    int base=0;
    bool b = false;
    string s;
    getline(fin, s);
    vector<string> p = parse_for_pass2(s);
    line++;
    // f2out << "H^" << format_to_size(p[2], 6, ' ') << "^" << start_addr << "^" << int_tohex(program_length, 3) << endl;
    bool ismultilineComment = false;
    while (getline(fin, s))
    {
        line++;
        /////deal with comments;
        ismultilineComment = (s[0] == '/' && s[1] == '*');
        if (ismultilineComment && (s[s.length() - 1] == '/' && s[s.length() - 2] == '*'))
        {
            ismultilineComment = false;
            continue;
        }
    
        if ((s[0] == '/' && s[1] == '/'))
            continue;
        vector<string> l = parse_for_pass2(s);
        if (l[3] == "END")
            break;
        string objcode;
        if (l[3][0] == '+')
        {
            objcode = format4Obj(l[3], l[4]);
        }
        else if (OPTAB[l[3]] != nullptr)
        {
            switch (OPTAB[l[3]]->format)
            {
            case 1:
                objcode = format1Obj(l[3]);
                break;
            case 2:
                objcode = format2Obj(l[3], l[4]);
                break;
            case 3:
                objcode = format3Obj(l[3], l[4], l[4], base, b);
                break;
            }
        }
        else if (l[3] == "WORD")
        {
            objcode = format_to_size(l[4], 6, '0');
        }
        else if (l[3] == "BYTE")
        {
            if (l[4][0] == 'X')
                objcode = l[4].substr(2, 4);
            else
            {
                objcode = string_tohex(l[4].substr(2, l[4].length() - 1), l[4].length() - 3);
            }
        }
        else if (l[3] == "RESW" || l[3] == "RESB")
        {
            /* code */
            ///dummmy code change the below code later
            objcode = "";
        }

        //deal with objectcode;
        f1out<<s<<"\t"<<objcode;

    }
    // f2out << "E" << format_to_size(start_addr, 6, '0') << endl;
}
