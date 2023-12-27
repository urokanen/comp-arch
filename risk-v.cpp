#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <map>
#include <string>

typedef unsigned int u_int;

using namespace std;
vector<uint8_t> arr;
const int ELEMENT_SIZE = 16;
u_int shoff, shnum, shstrndx, symtab_start, symtab_size, text_start, text_size, entry;
u_int strtab_position, shstrtab_position, mark;
map <u_int, string> mp;

struct element {
    u_int VAL, SIZE, INFO, VIS, IND;
    string NAME;

    element(u_int VAL, u_int SIZE, u_int INFO, u_int VIS, u_int IND, string NAME) {
        this->VAL = VAL;
        this->SIZE = SIZE;
        this->INFO = INFO;
        this->VIS = VIS;
        this->IND = IND;
        this->NAME = NAME;
    }

    string get_BIND() {
        switch (((INFO) >> 4)) {
            case 0:
                return "LOCAL";
            case 1:
                return "GLOBAL";
            case 2:
                return "WEAK";
            case 10:
                return "LOOS";
            case 12:
                return "HIOS";
            case 13:
                return "LOPROC";
            default:
                return "HIPROC";
        }
    }

    string get_TYPE() {
        switch (((INFO) & 0xf)) {
            case 0:
                return "NOTYPE";
            case 1:
                return "OBJECT";
            case 2:
                return "FUNC";
            case 3:
                return "SECTION";
            case 4:
                return "FILE";
            case 5:
                return "COMMON";
            case 6:
                return "TLS";
            case 10:
                return "LOOS";
            case 12:
                return "HIOS";
            case 13:
                return "LOPROC";
            default:
                return "HIPROC";
        }
    }

    string get_VIS() {
        switch (VIS) {
            case 0:
                return "DEFAULT";
            case 1:
                return "INTERNAL";
            case 2:
                return "HIDDEN";
            default:
                return "PROTECTED";
        }
    }

    string get_IND() {
        switch (IND) {
            case 0:
                return "UNDEF";
            case 0xff00:
                return "LORESERVE";
            case 0xff1f:
                return "HIPROC";
            case 0xff20:
                return "LOOS";
            case 0xff3f:
                return "HIOS";
            case 0xfff1:
                return "ABS";
            case 0xfff2:
                return "COMMON";
            default:
                return to_string(IND);
        }
    }
};

string get_reg(u_int val) {
    if (!val) {
        return "zero";
    } else if (val == 1) {
        return "ra";
    } else if (val == 2) {
        return "sp";
    } else if (val == 3) {
        return "gp";
    } else if (val == 4) {
        return "tp";
    } else if (val <= 7) {
        return "t" + to_string(val - 5);
    } else if (val <= 9) {
        return "s" + to_string(val - 8);
    } else if (val <= 17) {
        return "a" + to_string(val - 10);
    } else if (val <= 27) {
        return "s" + to_string(val - 16);
    } else if (val <= 31) {
        return "t" + to_string(val - 25);
    } else {
        return "inv_reg";
    }
}

vector<element *> symtab;

u_int calc_4byte(u_int start_index) {
    u_int res = 0;
    for (int j = 3; j >= 0; j--) {
        res = res * (1 << 8) + arr[start_index + j];
    }
    return res;
}

string parse_section_name(u_int ind) {
    int link = 0;
    for (int j = 3; j >= 0; j--) {
        link = link * (1 << 8) + arr[ind + j];
    }
    string ans;
    while (arr[shstrtab_position + link]) {
        ans.push_back((char) arr[shstrtab_position + link++]);
    }
    return ans;
}

void parse_section_header() {
    shstrtab_position = calc_4byte(shoff + shstrndx * 40 + 16);
    int link = -1;
    for (int j = 0; j < shnum; j++) {
        u_int ind = shoff + j * 40;
        string s = parse_section_name(ind);
        u_int offset = calc_4byte(ind + 16);
        u_int length = calc_4byte(ind + 20);
        if (s == ".text") {
            entry = calc_4byte(ind + 12);
            text_start = offset;
            text_size = length;
        } else if (s == ".symtab") {
            symtab_start = offset;
            symtab_size = length;
            link = calc_4byte(ind + 24);
        }
    }
    strtab_position = calc_4byte(shoff + link * 40 + 16);
}

string parse_symtab_name(int &index) {
    unsigned int ans = 0;
    for (int i = 3; i >= 0; i--) {
        ans = ans * (1 << 8) + (u_int) arr[index + i];
    }
    index += 4;
    string s;
    if (ans) {
        while (arr[strtab_position + ans]) {
            s.push_back((char) arr[strtab_position + ans++]);
        }
    }
    return s;
}

u_int parse_symtab_4byte(int &index) {
    unsigned int ans = 0;
    for (int i = 3; i >= 0; i--) {
        ans = ans * (1 << 8) + (u_int) arr[index + i];
    }
    index += 4;
    return ans;
}

unsigned int parse_ind(int &index) {
    unsigned int ans = arr[index + 1] * (1 << 8) + arr[index];
    index += 2;
    return ans;
}

void parse_symtab() {
    int index = symtab_start;
    for (int j = 0; j < symtab_size / ELEMENT_SIZE; j++) {
        string name = parse_symtab_name(index);
        unsigned int val = parse_symtab_4byte(index);
        unsigned int size = parse_symtab_4byte(index);
        auto info = (unsigned int) arr[index++];
        auto vis = (unsigned int) arr[index++];
        auto ind = parse_ind(index);
        symtab.push_back(new element(val, size, info, vis, ind, name));
        mp[val] = name;
    }
}

void print_symtab() {
    printf("\n.symtab\n");
    printf("\nSymbol Value              Size Type     Bind     Vis       Index Name\n");
    for (int i = 0; i < symtab.size(); i++) {
        printf("[%4i] 0x%-15X %5i %-8s %-8s %-8s %6s %s\n", i, symtab[i]->VAL, symtab[i]->SIZE,
               symtab[i]->get_TYPE().c_str(),
               symtab[i]->get_BIND().c_str(), symtab[i]->get_VIS().c_str(), symtab[i]->get_IND().c_str(),
               symtab[i]->NAME.c_str());
    }
}

void parse_elf_header() {
    for (int j = 0; j < 4; j++) {
        shoff = shoff * (1 << 8) + arr[35 - j];
    }
    shnum = arr[49] * (1 << 8) + arr[48];
    shstrndx = arr[51] * (1 << 8) + arr[50];
}

void print_3args(u_int addr, u_int command, const string& name, const string& r1, const string& r2, const string& r3) {
    printf("   %05x:\t%08x\t%7s\t%s, %s, %s\n", addr, command, name.c_str(), r1.c_str(), r2.c_str(), r3.c_str());
}

void print_b_type(u_int addr, u_int command, const string& name, const string& r1, const string& r2, u_int value, const string& s) {
    printf("   %05x:\t%08x\t%7s\t%s, %s, 0x%x, <%s>\n", addr, command, name.c_str(), r1.c_str(), r2.c_str(), value, s.c_str());
}

void print_inst(u_int addr, u_int command, const string& name, const string& r1, const string& r2, int d) {
    printf("   %05x:\t%08x\t%7s\t%s, %d(%s)\n", addr, command, name.c_str(), r1.c_str(), d, r2.c_str());
}

void print_2args(u_int addr, u_int command, const string& name, const string& r1, u_int d) {
    printf("   %05x:\t%08x\t%7s\t%s, 0x%x\n", addr, command, name.c_str(), r1.c_str(), d);
}

void print_no_args(u_int addr, u_int command, const string& name) {
    printf("   %05x:\t%08x\t%7s\n", addr, command, name.c_str());
}

void print_invalid(u_int addr, u_int command) {
    string s = "invalid_instruction";
    printf("   %05x:\t%08x\t%-7s\n", addr, command, s.c_str());
}

void print_fence(u_int addr, u_int command, const string& name, const string& s1, const string& s2) {
    printf("   %05x:\t%08x\t%7s\t%s, %s\n", addr, command, name.c_str(), s1.c_str(), s2.c_str());
}

void print_j_type(u_int addr, u_int command, const string& name, const string& r1, u_int value, const string& s) {
    printf("   %05x:\t%08x\t%7s\t%s, 0x%x <%s>\n", addr, command, name.c_str(), r1.c_str(), value, s.c_str());
}

int convert12(int value) {
    return ((value << 20) >> 20);
}

u_int parse_b_type(u_int command, u_int addr) {
    u_int imm = (command >> 8) % (1 << 4);
    int imm2 = ((int) command >> 25);
    imm2 = ((imm2 << 4) + (int)imm) << 1;
    int bit = ((int) command >> 7) % 2;
    imm2 |= (bit & (1 << 11));
    return imm2 + addr;
}

u_int parse_j_type(u_int command, u_int addr) {
    u_int copy = command;
    copy = copy >> 12;
    u_int part1 = copy % (1 << 8);
    copy = copy >> 9;
    u_int part2 = copy % (1 << 10);
    int pref = ((int) command & (1 << 31)) >> 12;
    part1 = ((part1 << 11) | part2) << 1;
    part1 |= pref;
    if (command & (1 << 20)) {
        part1 |= (1 << 11);
    }
    return part1 + addr;
}

void print_command(u_int command, u_int addr) {
    u_int opcode = command % (1 << 7);
    if (opcode == 0b110011) {
        u_int pref = command >> 25;
        u_int copy = command;
        copy = copy >> 7;
        u_int rd = copy % (1 << 5);
        copy = copy >> 5;
        u_int type = copy % (1 << 3);
        copy = copy >> 3;
        u_int rs1 = copy % (1 << 5);
        copy = copy >> 5;
        u_int rs2 = copy % (1 << 5);
        if (pref == 1) {
            switch (type) {
                case 0:
                    print_3args(addr, command, "mul", get_reg(rd), get_reg(rs1), get_reg(rs2));
                    return;
                case 1:
                    print_3args(addr, command, "mulh", get_reg(rd), get_reg(rs1), get_reg(rs2));
                    return;
                case 2:
                    print_3args(addr, command, "mulhsu", get_reg(rd), get_reg(rs1), get_reg(rs2));
                    return;
                case 3:
                    print_3args(addr, command, "mulhu", get_reg(rd), get_reg(rs1), get_reg(rs2));
                    return;
                case 4:
                    print_3args(addr, command, "div", get_reg(rd), get_reg(rs1), get_reg(rs2));
                    return;
                case 5:
                    print_3args(addr, command, "divu", get_reg(rd), get_reg(rs1), get_reg(rs2));
                    return;
                case 6:
                    print_3args(addr, command, "rem", get_reg(rd), get_reg(rs1), get_reg(rs2));
                    return;
                default:
                    print_3args(addr, command, "remu", get_reg(rd), get_reg(rs1), get_reg(rs2));
                    return;
            }
        } else if (!pref) {
            switch (type) {
                case 0:
                    print_3args(addr, command, "add", get_reg(rd), get_reg(rs1), get_reg(rs2));
                    return;
                case 1:
                    print_3args(addr, command, "sll", get_reg(rd), get_reg(rs1), get_reg(rs2));
                    return;
                case 2:
                    print_3args(addr, command, "slt", get_reg(rd), get_reg(rs1), get_reg(rs2));
                    return;
                case 3:
                    print_3args(addr, command, "sltu", get_reg(rd), get_reg(rs1), get_reg(rs2));
                    return;
                case 4:
                    print_3args(addr, command, "xor", get_reg(rd), get_reg(rs1), get_reg(rs2));
                    return;
                case 5:
                    print_3args(addr, command, "srl", get_reg(rd), get_reg(rs1), get_reg(rs2));
                    return;
                case 6:
                    print_3args(addr, command, "or", get_reg(rd), get_reg(rs1), get_reg(rs2));
                    return;
                default:
                    print_3args(addr, command, "and", get_reg(rd), get_reg(rs1), get_reg(rs2));
                    return;
            }
        } else if (pref == 0b100000) {
            switch (type) {
                case 0:
                    print_3args(addr, command, "sub", get_reg(rd), get_reg(rs1), get_reg(rs2));
                    return;
                case 5:
                    print_3args(addr, command, "sra", get_reg(rd), get_reg(rs1), get_reg(rs2));
                    return;
                default:
                    print_invalid(addr, command);
                    return;
            }
        } else {
            print_invalid(addr, command);
            return;
        }
    } else if (opcode == 0b10011) {
        u_int copy = command;
        copy = copy >> 7;
        u_int rd = copy % (1 << 5);
        copy = copy >> 5;
        u_int type = copy % (1 << 3);
        copy = copy >> 3;
        u_int rs1 = copy % (1 << 5);
        copy = copy >> 5;
        int value = convert12(copy);
        u_int val2 = copy % (1 << 5);
        copy = copy >> 5;
        switch (type) {
            case 0:
                print_3args(addr, command, "addi", get_reg(rd), get_reg(rs1), to_string(value));
                return;
            case 1:
                print_3args(addr, command, "slli", get_reg(rd), get_reg(rs1), to_string(val2));
                return;
            case 2:
                print_3args(addr, command, "slti", get_reg(rd), get_reg(rs1), to_string(value));
                return;
            case 3:
                print_3args(addr, command, "sltiu", get_reg(rd), get_reg(rs1), to_string(value));
                return;
            case 4:
                print_3args(addr, command, "xori", get_reg(rd), get_reg(rs1), to_string(value));
                return;
            case 5:
                if (copy == 0b100000) {
                    print_3args(addr, command, "srai", get_reg(rd), get_reg(rs1), to_string(val2));
                } else if (!copy) {
                    print_3args(addr, command, "srli", get_reg(rd), get_reg(rs1), to_string(val2));
                } else {
                    print_invalid(addr, command);
                }
                return;
            case 6:
                print_3args(addr, command, "ori", get_reg(rd), get_reg(rs1), to_string(value));
                return;
            default:
                print_3args(addr, command, "andi", get_reg(rd), get_reg(rs1), to_string(value));
                return;
        }
    } else if (opcode == 0b100011) {
        u_int copy = command;
        copy = copy >> 7;
        int imm = (int) copy % (1 << 5);
        copy = copy >> 5;
        u_int type = copy % (1 << 3);
        copy = copy >> 3;
        u_int rs1 = copy % (1 << 5);
        copy = copy >> 5;
        u_int rs2 = copy % (1 << 5);
        copy = copy >> 5;
        imm = ((int) copy << 5) + imm;
        imm = ((imm << 20) >> 20);
        switch (type) {
            case 0:
                print_inst(addr, command, "sb", get_reg(rs2), get_reg(rs1), imm);
                return;
            case 1:
                print_inst(addr, command, "sh", get_reg(rs2), get_reg(rs1), imm);
                return;
            case 2:
                print_inst(addr, command, "sw", get_reg(rs2), get_reg(rs1), imm);
                return;
            default:
                print_invalid(addr, command);
                return;
        }
    } else if (opcode == 0b11) {
        u_int copy = command;
        copy = copy >> 7;
        u_int rd = copy % (1 << 5);
        copy = copy >> 5;
        u_int type = copy % (1 << 3);
        copy = copy >> 3;
        u_int rs1 = copy % (1 << 5);
        copy = copy >> 5;
        int imm = convert12(copy);
        switch (type) {
            case 0:
                print_inst(addr, command, "lb", get_reg(rd), get_reg(rs1), imm);
                return;
            case 1:
                print_inst(addr, command, "lh", get_reg(rd), get_reg(rs1), imm);
                return;
            case 2:
                print_inst(addr, command, "lw", get_reg(rd), get_reg(rs1), imm);
                return;
            case 4:
                print_inst(addr, command, "lbu", get_reg(rd), get_reg(rs1), imm);
                return;
            case 5:
                print_inst(addr, command, "lhu", get_reg(rd), get_reg(rs1), imm);
                return;
            default:
                print_invalid(addr, command);
                return;
        }
    } else if (opcode == 0b1100011) {
        u_int copy = command;
        copy = copy >> 12;
        u_int type = copy % (1 << 3);
        copy = copy >> 3;
        u_int rs1 = copy % (1 << 5);
        copy = copy >> 5;
        u_int rs2 = copy % (1 << 5);
        u_int value = parse_b_type(command, addr);
        string b_mark = mp[value];
        switch (type) {
            case 0:
                print_b_type(addr, command, "beq", get_reg(rs1), get_reg(rs2), value, b_mark);
                return;
            case 1:
                print_b_type(addr, command, "bne", get_reg(rs1), get_reg(rs2), value, b_mark);
                return;
            case 4:
                print_b_type(addr, command, "blt", get_reg(rs1), get_reg(rs2), value, b_mark);
                return;
            case 5:
                print_b_type(addr, command, "bge", get_reg(rs1), get_reg(rs2), value, b_mark);
                return;
            case 6:
                print_b_type(addr, command, "bltu", get_reg(rs1), get_reg(rs2), value, b_mark);
                return;
            case 7:
                print_b_type(addr, command, "bgeu", get_reg(rs1), get_reg(rs2), value, b_mark);
                return;
            default:
                print_invalid(addr, command);
                return;
        }
    } else if (opcode == 0b1100111) {
        u_int copy = command;
        copy = copy >> 7;
        u_int rd = copy % (1 << 5);
        copy = copy >> 5;
        u_int type = copy % (1 << 3);
        copy = copy >> 3;
        u_int rs1 = copy % (1 << 5);
        copy = copy >> 5;
        int res = ((int) (copy << 20) >> 20);
        if (!type) {
            print_inst(addr, command, "jalr", get_reg(rd), get_reg(rs1), res);
            return;
        } else {
            print_invalid(addr, command);
            return;
        }
    } else if (opcode == 0b110111) {
        u_int copy = command;
        copy = copy >> 7;
        u_int rd = copy % (1 << 5);
        copy = copy >> 5;
        if (copy & (1 << 19)) {
            copy -= (1 << 20);
        }
        print_2args(addr, command, "lui", get_reg(rd), copy);
        return;
    } else if (opcode == 0b10111) {
        u_int copy = command;
        copy = copy >> 7;
        u_int rd = copy % (1 << 5);
        copy = copy >> 5;
        if (copy & (1 << 19)) {
            copy -= (1 << 20);
        }
        print_2args(addr, command, "auipc", get_reg(rd), copy);
        return;
    } else if (opcode == 0b1111) {
        u_int copy = command;
        copy = copy >> 7;
        u_int rd = copy % (1 << 5);
        copy = copy >> 5;
        u_int type = copy % (1 << 3);
        copy = copy >> 3;
        u_int rs1 = copy % (1 << 5);
        copy = copy >> 5;
        if (type == 0) {
            if (!rd && !rs1 && copy == 0b10000) {
                print_no_args(addr, command, "pause");
            } else if (!rd && !rs1 && copy == 0b100000110011) {
                print_no_args(addr, command, "fence.tso");
            } else {
                string predecessor, successor;
                if (copy & (1 << 3)) {
                    successor.push_back('i');
                }
                if (copy & (1 << 2)) {
                    successor.push_back('o');
                }
                if (copy & (1 << 1)) {
                    successor.push_back('r');
                }
                if (copy & 1) {
                    successor.push_back('w');
                }
                if (copy & (1 << 7)) {
                    predecessor.push_back('i');
                }
                if (copy & (1 << 6)) {
                    predecessor.push_back('o');
                }
                if (copy & (1 << 5)) {
                    predecessor.push_back('r');
                }
                if (copy & (1 << 4)) {
                    predecessor.push_back('w');
                }
                print_fence(addr, command, "fence", predecessor, successor);
            }
        } else {
            print_invalid(addr, command);
            return;
        }
    } else if (opcode == 0b1110011) {
        u_int copy = command;
        copy = copy >> 7;
        if (!copy) {
            print_no_args(addr, command, "ecall");
            return;
        } else if (!(copy & (1 << 20))) {
            print_no_args(addr, command, "ebreak");
            return;
        } else {
            print_invalid(addr, command);
            return;
        }
    } else if (opcode == 0b1101111) {
        u_int copy = command;
        copy = copy >> 7;
        u_int rd = copy % (1 << 5);
        u_int value = parse_j_type(command, addr);
        print_j_type(addr, command, "jal", get_reg(rd), value, mp[value]);
    } else {
        print_invalid(addr, command);
        return;
    }
}

void parse_text() {
    for (int j = 0; j < text_size / 4; j++) {
        u_int command = calc_4byte(text_start + j * 4);
        u_int addr = entry + 4 * j;
        u_int value = 0;
        bool f = false;
        if (command % (1 << 7) == 0b1101111) {
            value = parse_j_type(command, addr);
            f = true;
        } else if (command % (1 << 7) == 0b1100011) {
            value = parse_b_type(command, addr);
            f = true;
        }
        if (f && mp[value] == "") {
            string s;
            s.push_back('L');
            s += to_string(mark++);
            mp[value] = s;
        }
    }
    printf(".text\n");
    for (int j = 0; j < text_size / 4; j++) {
        if (!mp[entry + 4 * j].empty()) {
            printf("\n%08x \t<%s>:\n", entry + 4 * j, mp[entry + 4 * j].c_str());
        }
        print_command(calc_4byte(text_start + j * 4), entry + 4 * j);
    }
    printf("\n");
}

int main(int argc, char **argv) {
    if (argc == 3) {
        mark = 0;
        ifstream in;
        in.open(argv[1], ios::binary);
        while (in.good()) {
            arr.push_back(in.get());
        }
        arr.pop_back();
        in.close();
        freopen(argv[2], "w", stdout);
        parse_elf_header();
        parse_section_header();
        parse_symtab();
        parse_text();
        print_symtab();
    } else {
        cerr << "incorrect input data\n";
    }
}
