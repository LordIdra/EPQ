#include "generator/registers.hpp"
#include <generator/programs.hpp>

#include <generator/assembly.hpp>



namespace programs {
    using namespace assembly;

    auto IsEqual(const int r1, const int r2) -> int {
        const string labelNext = GenerateLabel("IsEqual_Next");
        const string labelTrue = GenerateLabel("IsEqual_True");
        const string labelEnd  = GenerateLabel("IsEqual_End");
        const int r3 = registers::Allocate();
        const int r4 = registers::Allocate();
        
        SUB(r1, r2, r3);
        BRP(labelNext, r3);
        SET(registers::FALSE, r4);

        SUB(r2, r1, r3);
        LabelLatestInstruction(labelNext);
        BRP(labelTrue, r3);
        SET(registers::FALSE, r4);
        BRA(labelEnd);

        SET(registers::TRUE, r4);
        LabelLatestInstruction(labelTrue);

        NOP();
        LabelLatestInstruction(labelEnd);

        return r4;
    }

    auto IsLessOrEqual(const int r1, const int r2) -> int {
        const string labelFirstPositive = GenerateLabel("IsLessOrEqual_FirstPositive");
        const string labelFirstNegative = GenerateLabel("IsLessOrEqual_FirstNegative");
        const string labelFirstLessOrEqual = GenerateLabel("IsLessOrEqual_FirstLessOrEqual");
        const string labelMSBSame = GenerateLabel("IsLessOrEqual_End");
        const string labelEnd = GenerateLabel("IsLessOrEqual_End");
        const int r3 = registers::Allocate();
        const int r4 = registers::Allocate();

        BRP(labelFirstPositive, r1);
        BRA(labelFirstNegative);

        BRP(labelMSBSame, r2);
        LabelLatestInstruction(labelFirstPositive);
        SET(registers::FALSE, r3);
        BRA(labelEnd);

        BRP(labelMSBSame, r2);
        LabelLatestInstruction(labelFirstNegative);
        SET(registers::TRUE, r3);
        BRA(labelEnd);

        SUB(r1, r2, r4);
        LabelLatestInstruction(labelMSBSame);
        BRP(labelFirstLessOrEqual, r4); // r2 bigger
        SET(registers::TRUE, r3);
        BRA(labelEnd);

        SET(registers::FALSE, r3);
        LabelLatestInstruction(labelFirstLessOrEqual);
        
        NOP();
        LabelLatestInstruction(labelEnd);

        return r3;
    }

    auto IsGreaterOrEqual(const int r1, const int r2) -> int {
        const string labelFirstPositive = GenerateLabel("IsGreaterOrEqual_FirstPositive");
        const string labelFirstNegative = GenerateLabel("IsGreaterOrEqual_FirstNegative");
        const string labelSecondGreaterOrEqual = GenerateLabel("IsGreaterOrEqual_FirstGreaterOrEqual");
        const string labelMSBSame = GenerateLabel("IsGreaterOrEqual_End");
        const string labelEnd = GenerateLabel("IsGreaterOrEqual_End");
        const int r3 = registers::Allocate();
        const int r4 = registers::Allocate();

        BRP(labelFirstPositive, r1);
        BRA(labelFirstNegative);

        BRP(labelMSBSame, r2);
        LabelLatestInstruction(labelFirstPositive);
        SET(registers::TRUE, r3);
        BRA(labelEnd);

        BRP(labelMSBSame, r2);
        LabelLatestInstruction(labelFirstNegative);
        SET(registers::FALSE, r3);
        BRA(labelEnd);

        SUB(r1, r2, r4);
        LabelLatestInstruction(labelMSBSame);
        BRP(labelSecondGreaterOrEqual, r4); // r2 bigger
        SET(registers::FALSE, r3);
        BRA(labelEnd);

        SET(registers::TRUE, r3);
        LabelLatestInstruction(labelSecondGreaterOrEqual);

        NOP();
        LabelLatestInstruction(labelEnd);

        return r3;
    }
}