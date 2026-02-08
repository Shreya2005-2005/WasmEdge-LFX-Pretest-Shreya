#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Function.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include <memory>
#include <iostream>

// Using namespace alias instead of 'using namespace llvm' - better practice for namespace management
namespace ll = llvm;

int main(int argc, char **argv) {
    // Check if input file is provided
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input.ll>" << std::endl;
        return 1;
    }

    // Create LLVM context
    ll::LLVMContext context;
    ll::SMDiagnostic error;

    // Parse the LLVM IR file
    std::unique_ptr<ll::Module> module = ll::parseIRFile(argv[1], error, context);

    // Check if parsing was successful
    if (!module) {
        std::cerr << "Error parsing IR file: " << argv[1] << std::endl;
        error.print(argv[0], ll::errs());
        return 1;
    }

    // Print module information
    std::cout << "========================================" << std::endl;
    std::cout << "LLVM Module Parser - Pre-test 2" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Module: " << module->getName().str() << std::endl;
    std::cout << "Source File: " << module->getSourceFileName() << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Functions found in the module:" << std::endl;
    std::cout << "========================================" << std::endl;

    // Counter for functions
    int functionCount = 0;

    // Iterate through all functions in the module
    for (ll::Function &func : *module) {
        functionCount++;
        
        // Print function name
        std::cout << functionCount << ". Function Name: " << func.getName().str() << std::endl;
        
        // Print additional information about the function
        std::cout << "   - Return Type: ";
        func.getReturnType()->print(ll::outs());
        std::cout << std::endl;
        
        std::cout << "   - Number of Arguments: " << func.arg_size() << std::endl;
        std::cout << "   - Number of Basic Blocks: " << func.size() << std::endl;
        
        // Count instructions and track instruction types
        int instructionCount = 0;
        int num_adds = 0, num_subs = 0, num_muls = 0, num_divs = 0;
        int num_calls = 0, num_loads = 0, num_stores = 0;
        
        for (ll::BasicBlock &bb : func) {
            for (ll::Instruction &inst : bb) {
                instructionCount++;
                unsigned opcode = inst.getOpcode();
                
                // Track instruction types for security and gas estimation
                if (opcode == ll::Instruction::Add) num_adds++;
                else if (opcode == ll::Instruction::Sub) num_subs++;
                else if (opcode == ll::Instruction::Mul) num_muls++;
                else if (opcode == ll::Instruction::UDiv || opcode == ll::Instruction::SDiv) num_divs++;
                else if (opcode == ll::Instruction::Call) num_calls++;
                else if (opcode == ll::Instruction::Load) num_loads++;
                else if (opcode == ll::Instruction::Store) num_stores++;
            }
        }
        
        std::cout << "   - Number of Instructions: " << instructionCount << std::endl;
        
        // Check if the function logic actually matches its name (Security Check)
        // This helps catch potentially malicious code where function names are misleading
        std::string funcName = func.getName().str();
        if (funcName == "add" && num_adds == 0 && num_muls > 0) {
            std::cout << "   [WARNING] SECURITY: Possible Malicious Logic Mismatch" << std::endl;
            std::cout << "       Function named 'add' contains mul instructions but no add instructions!" << std::endl;
        } else if (funcName == "multiply" && num_muls == 0 && num_adds > 0) {
            std::cout << "   [WARNING] SECURITY: Possible Malicious Logic Mismatch" << std::endl;
            std::cout << "       Function named 'multiply' contains add instructions but no mul instructions!" << std::endl;
        }
        
        // Estimate 'Gas' - inspired by WasmEdge's resource tracking
        // Different operations have different computational costs
        int predicted_resource_units = 0;
        predicted_resource_units += (num_adds + num_subs) * 1;      // add/sub: 1 point each
        predicted_resource_units += (num_muls + num_divs) * 5;      // mul/div: 5 points each
        predicted_resource_units += (num_calls + num_loads + num_stores) * 10;  // call/load/store: 10 points each
        
        std::cout << "   [GAS ESTIMATE] Total Estimated Gas Cost: " << predicted_resource_units << " units" << std::endl;
        std::cout << "       (add/sub=1, mul/div=5, call/load/store=10)" << std::endl;
        
        // AOT Optimization suggestions based on function complexity
        // Small functions are good candidates for inlining in WasmEdge AOT compilation
        if (instructionCount < 5 && instructionCount > 0) {
            std::cout << "   [AOT TIP] Optimization: Suggest Inlining for WasmEdge AOT performance" << std::endl;
            std::cout << "       (Function has only " << instructionCount << " instructions)" << std::endl;
        }
        
        if (func.size() > 3) {
            std::cout << "   [COMPLEXITY] Note: Monitor for Branching overhead" << std::endl;
            std::cout << "       (Function has " << func.size() << " basic blocks)" << std::endl;
        }
        
        std::cout << std::endl;
    }

    std::cout << "========================================" << std::endl;
    std::cout << "Total Functions: " << functionCount << std::endl;
    std::cout << "========================================" << std::endl;

    // TODO: Future improvement - add support for analyzing floating-point operation costs
    // Also considering adding SIMD instruction analysis for better performance predictions

    return 0;
}
