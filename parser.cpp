#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Function.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include <memory>
#include <iostream>

int main(int argc, char **argv) {
    // Check if input file is provided
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input.ll>" << std::endl;
        return 1;
    }

    // Create LLVM context
    llvm::LLVMContext context;
    llvm::SMDiagnostic error;

    // Parse the LLVM IR file
    std::unique_ptr<llvm::Module> module = llvm::parseIRFile(argv[1], error, context);

    // Check if parsing was successful
    if (!module) {
        std::cerr << "Error parsing IR file: " << argv[1] << std::endl;
        error.print(argv[0], llvm::errs());
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
    for (llvm::Function &func : *module) {
        functionCount++;
        std::cout << functionCount << ". Function Name: " << func.getName().str() << std::endl;
    }

    std::cout << "========================================" << std::endl;
    std::cout << "Total Functions: " << functionCount << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}
