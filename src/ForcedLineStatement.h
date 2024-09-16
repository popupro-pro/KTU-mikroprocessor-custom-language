#pragma once
#include "Modules.h"
#include "Utils.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class ForcedLineStatement {
    void ALUFunction(std::string const& function) {
        A[(int)Modules::Register::S_Data] = 1;
        if (function.find('1') != std::string::npos) {
            if (function.find('+') != std::string::npos) {
                if (function.find('A') != std::string::npos) {
                    ALU[(int)Modules::ALU::Rp1] = 1;
                } else {
                    ALU[(int)Modules::ALU::Lp1] = 1;
                }
            } else {
                if (function.find('A') != std::string::npos) {
                    ALU[(int)Modules::ALU::Rm1] = 1;
                } else {
                    ALU[(int)Modules::ALU::Lm1] = 1;
                }
            }
            return;
        }
        if (function.find('N') != std::string::npos) {
            if (function.find('A') != std::string::npos) {
                ALU[(int)Modules::ALU::NotR] = 1;
            } else {
                ALU[(int)Modules::ALU::NotL] = 1;
            }
            return;
        }
        if (function.find('X') != std::string::npos) {
            ALU[(int)Modules::ALU::LxorR] = 1;
            return;
        }
        ALU[(int)Modules::ALU::LpR] = 1;
    }
    void AssignFunction(std::string const& part) {
        std::vector<std::string> BothSides = Utils::split(part, "=");
        if (BothSides[0].find('A') != std::string::npos &&
            BothSides[0].length() > 1) {
            std::cout << "Tried to assign A and something else";
            return;
        }

        if (BothSides[0].find('A') != std::string::npos) {
            ALUFunction(BothSides[1]);
        }
        if (BothSides[0].find('B') != std::string::npos) {
            B[(int)Modules::Register::S_Data] = 1;
        }
        if (BothSides[0].find('C') != std::string::npos) {
            C[(int)Modules::Register::S_Data] = 1;
        }
        if (BothSides[0].find('D') != std::string::npos) {
            D[(int)Modules::Register::S_Data] = 1;
        }
        if (BothSides[0].find('E') != std::string::npos) {
            E[(int)Modules::Register::S_Data] = 1;
        }
        if (BothSides[0].find('F') != std::string::npos) {
            F[(int)Modules::Register::S_Data] = 1;
        }

        if (BothSides[1].find("DIN") != std::string::npos) {
            MUX[(int)Modules::MUX::Din] = 1;
        } else if (BothSides[1].find('B') != std::string::npos) {
            MUX[(int)Modules::MUX::RegB] = 1;
        } else if (BothSides[1].find('C') != std::string::npos) {
            MUX[(int)Modules::MUX::RegC] = 1;
        } else if (BothSides[1].find('D') != std::string::npos) {
            MUX[(int)Modules::MUX::RegD] = 1;
        } else if (BothSides[1].find('E') != std::string::npos) {
            MUX[(int)Modules::MUX::RegE] = 1;
        } else if (BothSides[1].find('F') != std::string::npos) {
            MUX[(int)Modules::MUX::RegF] = 1;
        } else {
            MUX[(int)Modules::MUX::RegA] = 1;
        }
    }
    void ResetFunction(std::string const& part) {
        std::vector<std::string> Resets =
            Utils::split(part.substr(4, part.length() - 5), ",");
        if (std::find(Resets.begin(), Resets.end(), "A") != Resets.end()) {
            Reset[(int)Modules::RST::RegA] = 1;
        }
        if (std::find(Resets.begin(), Resets.end(), "B") != Resets.end()) {
            Reset[(int)Modules::RST::RegB] = 1;
        }
        if (std::find(Resets.begin(), Resets.end(), "C") != Resets.end()) {
            Reset[(int)Modules::RST::RegC] = 1;
        }
        if (std::find(Resets.begin(), Resets.end(), "D") != Resets.end()) {
            Reset[(int)Modules::RST::RegD] = 1;
        }
        if (std::find(Resets.begin(), Resets.end(), "E") != Resets.end()) {
            Reset[(int)Modules::RST::RegE] = 1;
        }
        if (std::find(Resets.begin(), Resets.end(), "F") != Resets.end()) {
            Reset[(int)Modules::RST::RegF] = 1;
        }
        if (std::find(Resets.begin(), Resets.end(), "CNT") != Resets.end()) {
            Reset[(int)Modules::RST::CNT] = 1;
        }
        if (std::find(Resets.begin(), Resets.end(), "ROM") != Resets.end()) {
            Reset[(int)Modules::RST::ROM] = 1;
        }
        if (std::find(Resets.begin(), Resets.end(), "FLAG") != Resets.end()) {
            Reset[(int)Modules::RST::Flag] = 1;
        }
    }
    void FlagFunction(std::string const& part) {
        std::map<std::string, int> const flags{
            {"0", (int)Modules::Flag::FALSE}, {"CNT", (int)Modules::Flag::CNT},
            {"AH", (int)Modules::Flag::AH},   {"AL", (int)Modules::Flag::AL},
            {"BH", (int)Modules::Flag::BH},   {"BL", (int)Modules::Flag::BL},
            {"CH", (int)Modules::Flag::CH},   {"CL", (int)Modules::Flag::CL},
            {"DH", (int)Modules::Flag::DH},   {"DL", (int)Modules::Flag::DL},
            {"EH", (int)Modules::Flag::EH},   {"EL", (int)Modules::Flag::EL},
            {"FH", (int)Modules::Flag::FH},   {"FL", (int)Modules::Flag::FL},
            {"ALU", (int)Modules::Flag::ALU}};

        std::string token = part.substr(3, part.length() - 4);

        Utils::AssignIntToBoolArray(Flag, 4, flags.at(token));
    }
    void ShiftFunction(std::string const& part) {
        int shiftAssign = 0;
        std::string shift = part.substr(0, 3);

        std::map<std::string, int> const shifts{
            {"LL1", (int)Modules::Register::LL1},
            {"LR1", (int)Modules::Register::LR1},
            {"AL1", (int)Modules::Register::AL1},
            {"AR1", (int)Modules::Register::AR1},
            {"CL1", (int)Modules::Register::CL1},
            {"CR1", (int)Modules::Register::CR1}};
        shiftAssign = shifts.at(part.substr(0, 3));

        std::vector<std::string> registers =
            Utils::split(part.substr(4, part.length() - 5), ",");
        if (std::find(registers.begin(), registers.end(), "A") !=
            registers.end()) {
            A[shiftAssign] = 1;
        }
        if (std::find(registers.begin(), registers.end(), "B") !=
            registers.end()) {
            B[shiftAssign] = 1;
        }
        if (std::find(registers.begin(), registers.end(), "C") !=
            registers.end()) {
            C[shiftAssign] = 1;
        }
        if (std::find(registers.begin(), registers.end(), "D") !=
            registers.end()) {
            D[shiftAssign] = 1;
        }
        if (std::find(registers.begin(), registers.end(), "E") !=
            registers.end()) {
            E[shiftAssign] = 1;
        }
        if (std::find(registers.begin(), registers.end(), "F") !=
            registers.end()) {
            F[shiftAssign] = 1;
        }
    }
    void AssignComment(std::string const& part) { Comment = part; }
    void NextLineFunction(std::string const& part) {
        std::string number_string = part.substr(5);
        int number = std::stoi(number_string);
        nextLineNr = number;
        Utils::AssignIntToBoolArray(Adress, 8, number);
    }
    void ThisLineFunction(std::string const& part) {
        int number = std::stoi(part);
        lineNr = number;
    }

    void PartInterpreter(std::string const& part) {
        if (part.substr(0, 2) == "--") {
            AssignComment(part);
            return;
        }
        if (part.find("=") != std::string::npos) {
            AssignFunction(part);
            return;
        }
        if (part == "CNT--") {
            CNT = true;
            return;
        }
        if (part.find("RST") != std::string::npos) {
            ResetFunction(part);
            return;
        }
        if (part.find("LS") != std::string::npos) {
            IsFlag = true;
            FlagFunction(part);
            return;
        }
        if (part.find("NEXT:") != std::string::npos) {
            NextLineFunction(part);
            return;
        }
        if (part.find("DONE") != std::string::npos) {
            DataOut = 1;
            Utils::AssignIntToBoolArray(Adress, 8, lineNr);
            nextLineNr = lineNr;
            return;
        }
        std::string shiftCheck = part.substr(0, 3);
        if (shiftCheck == "LL1" || shiftCheck == "LR1" || shiftCheck == "AL1" ||
            shiftCheck == "AR1" || shiftCheck == "CL1" || shiftCheck == "CR1") {
            ShiftFunction(part);
            return;
        }
        ThisLineFunction(part);
    }

  public:
    Modules::Processor type{};
    int lineNr{-1};
    int nextLineNr{-1};
    std::string lineName{};
    bool IsFlag{};
    bool MUX[7]{};
    bool A[7]{};
    bool B[7]{};
    bool C[7]{};
    bool D[7]{};
    bool E[7]{};
    bool F[7]{};
    bool ALU[8]{};
    bool Reset[9]{};
    bool DataOut{};
    bool CNT{};
    bool Flag[4]{};
    bool Adress[8]{};
    std::string Comment{};
    ForcedLineStatement(std::string line, Modules::Processor type) {
        this->type = type;
        std::vector<std::string> parts = Utils::split(line, ";");
        for (std::string part : parts) {
            part = part.substr(part.find_first_not_of(" "));
            if (part.substr(0, 2) != "--") {
                std::transform(part.begin(), part.end(), part.begin(),
                               ::toupper);
                part.erase(std::remove(part.begin(), part.end(), ' '),
                           part.end());
            }
            PartInterpreter(part);
        }
        if (nextLineNr < 0) {
            Utils::AssignIntToBoolArray(Adress, 8, lineNr + 1);
        }
    }
    std::string ToString() const {
        std::string rez = std::to_string(lineNr) + "=> \"";
        if (type == Modules::Processor::Forced) {
            rez += Utils::BoolArrayToString(MUX, 7);
            rez += Utils::BoolArrayToString(A, 7);
            rez += Utils::BoolArrayToString(B, 7);
            rez += Utils::BoolArrayToString(C, 7);
            rez += Utils::BoolArrayToString(D, 7);
            rez += Utils::BoolArrayToString(E, 7);
            rez += Utils::BoolArrayToString(F, 7);
            rez += Utils::BoolArrayToString(ALU, 8);
            rez += Utils::BoolArrayToString(Reset, 9);
            rez += std::to_string(DataOut);
            rez += std::to_string(CNT);
            rez += Utils::BoolArrayToString(Flag, 4);
            rez += Utils::BoolArrayToString(Adress, 8);
        } else if (type == Modules::Processor::Natural) {
            rez += std::to_string(IsFlag);
            if (IsFlag) {
                rez += Utils::BoolArrayToString(Flag, 4);
                rez += Utils::BoolArrayToString(Adress, 8);
                rez += "00";
            } else {
                rez += Utils::BoolArrayToString(MUX, 7);
                rez += Utils::BoolArrayToString(A, 7);
            }
            rez += Utils::BoolArrayToString(B, 7);
            rez += Utils::BoolArrayToString(C, 7);
            rez += Utils::BoolArrayToString(D, 7);
            rez += Utils::BoolArrayToString(E, 7);
            rez += Utils::BoolArrayToString(F, 7);
            rez += Utils::BoolArrayToString(ALU, 8);
            rez += Utils::BoolArrayToString(Reset, 9);
            rez += std::to_string(DataOut);
            rez += std::to_string(CNT);
        }
        rez += "\", " + Comment;
        return rez;
    }
};
