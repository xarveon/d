#include "pin.H"
#include <iostream>
#include <fstream>
#include <map>

// Output log file
KNOB<std::string> log_filename(KNOB_MODE_WRITEONCE, "pintool", "o", "log.bin", "log file");
std::ofstream logfile;

constexpr ADDRINT write_decrypted = 0x47B2DD;

VOID log_decrypted(ADDRINT ecx)
{
    logfile.write((char*) &ecx, 4);
}

VOID insInstrumentation(INS ins, VOID* v)
{
    if (INS_Address(ins) == write_decrypted)
    {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)log_decrypted, IARG_REG_VALUE, REG_ECX, IARG_END);
    }
}

VOID finished(INT32 code, VOID* v)
{
    logfile.close();
}

int main(int argc, char* argv[])
{
    // Init PIN
    PIN_InitSymbols();

    // Parse command line
    if (PIN_Init(argc, argv))
        return -1;

    // Open log file
    logfile.open(log_filename.Value().c_str(), std::ios::binary);

    // Return if unable to open log file
    // No way to record errors up to this point
    if (!logfile)
        return -1;

    // Setup PIN instrumentation callbacks
    // IMG_AddInstrumentFunction(imgInstrumentation, 0);
    INS_AddInstrumentFunction(insInstrumentation, 0);
    PIN_AddFiniFunction(finished, 0);

    // Start analysis
    PIN_StartProgram();

    return 0;
}
