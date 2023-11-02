#include <stdio.h>
#include "loader.h"
#include "reflet.h"
#include "config.h"

static void help();
static bool parse_arg(const char* arg, reflet* vm);

int main(int argc, char** argv){
    reflet* vm = reflet_init();
    bool rom_loaded = false;
    for (int i=1; i<argc; i++) {
        rom_loaded |= parse_arg(argv[i], vm);
    }
    if (!rom_loaded) {
        fprintf(stderr, "Error, no program given.\n");
        return RET_INVALID_ARGS;
    }
    reflet_run(vm);
    reflet_free(vm);
    return 0;
}

static void help(){
    printf("reflet-sim, a virtual machine to run reflet ISA machine code.\n"
           "Usage: reflet-sim [options] <binary file to execute>\n"
           "Available options:\n"
           "    -r/--ram-size <size>          : Size in bytes of the available RAM.\n"
           "    -w/--word-size <size>         : Size in bits of the processor's words.\n"
           "    -c/--config-file <file>       : Config file used to describe the VM.\n"
           "    -!/--ignore-first-line <file> : Ignore chars up to the first new line in input binary.\n"
           "    -x/--extended-io              : Use extended IOs instead of basic ones.\n"
           "    -l/--log-file <file>          : Use the given file as a log file.\n"
          );
}

/*
 * Parse an argument. Returns true if the argument is the ROM file.
 * Is there is an error, this function exits.
 */
static bool parse_arg(const char* arg, reflet* vm) {
    static bool reading_config_file = false;
    static bool reading_word_size   = false;
    static bool reading_ram_size    = false;
    static bool rom_loaded          = false;
    static bool ignore_first_line   = false;
    static bool reading_log_file    = false;
    if (reading_config_file) {
        reading_config_file = false;
        applyConfig(vm, arg);
    } else if (reading_word_size) {
        reading_word_size = false;
        word_t word_size;
        if (sscanf(arg, "%" WORD_P, &word_size) == 1) {
            if (word_size%8) {
                fprintf(stderr, "Error in arguments, word size is of invalid size.\n");
                exit(RET_CONFIG);
            }
            vm->config->word_size = word_size;
            SET_WORD_SIZE(vm);
        } else {
            fprintf(stderr, "Error in arguments, word size should be a number.\n");
            exit(RET_CONFIG);
        }
    } else if (reading_ram_size) {
        reading_ram_size = false;
        word_t ram_size;
        if (sscanf(arg, "%" WORD_P, &ram_size) == 1) {
            vm->config->ram_size = ram_size;
        } else {
            fprintf(stderr, "Error in arguments, ram size should be a number.\n");
            exit(RET_CONFIG);
        }
    } else if (reading_log_file) {
        if(vm->debug->enable){
            fprintf(stderr, "Error, multiple log files opened.\n");
            exit(RET_CONFIG);
        }
        vm->debug->enable = true;
        if((vm->debug->file = fopen(arg, "w")) == NULL){
            fprintf(stderr, "Error, unable to open log file '%s'.\n", arg);
            exit(RET_CONFIG);
        }
        reading_log_file = false;
    } else {
        if (!strcmp(arg, "--config-file") || !strcmp(arg, "-c")) {
            reading_config_file = true;
        } else if (!strcmp(arg, "--ignore-first-line") || !strcmp(arg, "-!")) {
            ignore_first_line = true;
        } else if (!strcmp(arg, "--word-size") || !strcmp(arg, "-w")) {
            reading_word_size = true;
        } else if (!strcmp(arg, "--ram-size") || !strcmp(arg, "-r")) {
            reading_ram_size = true;
        } else if (!strcmp(arg, "--extended-io") || !strcmp(arg, "-x")) {
            vm->config->extended_io = true;
        } else if (!strcmp(arg, "help") || !strcmp(arg, "--help") || !strcmp(arg, "-h")) {
            help();
            exit(0);
        } else if (!strcmp(arg, "--log-file") || !strcmp(arg, "-l")) {
            reading_log_file = true;
        } else if (!rom_loaded) {
            bool loading = load_file(arg, vm, ignore_first_line);
            if(!loading){
                fprintf(stderr, "Unable to set the simulator up.\n");
                exit(RET_NO_VM);
            }
            rom_loaded = true;
            return true;
        } else {
            fprintf(stderr, "Warning: '%s' argument is unused.\n", arg);
        }
    }
    return false;
}

