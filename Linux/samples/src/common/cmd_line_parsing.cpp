// Copyright (C) 2023-2024 Arm Technology (China) Co. Ltd.
//
// SPDX-License-Identifier: Apache-2.0


/**
 * @file  cmd_line_parsing.cpp
 * @brief AIPU UMD test implementation file: command line parsing
 */

#include <iostream>
#include <sstream>
#include <iomanip>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include "cmd_line_parsing.h"
#include "helper.h"
#include "dbg.hpp"

std::shared_ptr<SemOp> semOp_sp = nullptr;
std::mutex m_mtx;

static struct option opts[] = {
    { "bin", required_argument, NULL, 'b' },
    { "idata", required_argument, NULL, 'i' },
    { "check", required_argument, NULL, 'c' },
    { "dump_dir", required_argument, NULL, 'd' },
    { "sim", optional_argument, NULL, 's' },
    { "x2_arch", required_argument, NULL, 'a' },
    { "log_level", optional_argument, NULL, 'l' },
    { "dump_opt", optional_argument, NULL, 'o' },
    { "verbose", optional_argument, NULL, 'v' },
    { "time", required_argument, NULL, 't' },
    { "shape", required_argument, NULL, 'r' },
    { "weight_dir", required_argument, NULL, 'w' },
    { NULL, 0, NULL, 0 }
};

void help(void)
{
    std::string help_info =
        "usage: ./test -s sim -b aipu.bin -i input0.bin,input1.bin -c output.bin -d ./output [-l 0-3] [-v] [-r]\n"
        "   -s: aipu v1/v2 simulator path\n"
        "   -b: aipu.bin\n"
        "   -i: input bins\n"
        "   -c: output bin\n"
        "   -d: output data path\n"
        "   -a: aipu v3 arch (X2_1204/X2_1204MP3), aipu v3_1 arch(X3_1304/X3_1304MP2)\n"
        "   -o: dump options for text/weight/in/out on board(hex form: ff)\n"
        "   -t: test flush or finish job time(flush | finish), only for basic_time_test\n"
        "   -l: simulator log level(0-3)\n"
        "   -v: simulator verbose(0, 1)\n"
        "   -r: dynamic real input shape(eg: 1,480,640,3;if multi tensors, use'/' for isolation: 1,480,640,3/1,480,640,3)\n"
        "   -w: extra weight bin path,(note: weight bin name is like extra_weight_{0-9}.bin)\n";

    std::cout << help_info;
    exit(0);
}

int init_test_bench(int argc, char* argv[], cmd_opt_t* opt, const char* test_case)
{
    int ret = 0;
    extern char *optarg;
    int opt_idx = 0;
    int c = 0;
    char* temp = nullptr;
    char* optarg_bkup = nullptr;
    char* dest = nullptr;
    uint32_t size = 0;
    std::stringstream dump_opt;

    if (nullptr == opt)
    {
        AIPU_ERR()("invalid null pointer!\n");
        return -1;
    }

    while (1)
    {
        c = getopt_long(argc, argv, "hs:C:b:i:c:d:a:s:z:q:k:x:o:l:t:r:w:v", opts, &opt_idx);
        if (-1 == c)
            break;

        optarg_bkup = optarg;
        switch (c)
        {
        case 0:
            if (opts[opt_idx].flag != 0)
                break;

            fprintf (stdout, "option %s", opts[opt_idx].name);
            if (optarg)
                printf (" with arg %s", optarg);
            printf ("\n");
            break;

        case 'b':
            temp = strtok(optarg_bkup, ",");
            opt->bin_files.push_back(temp);
            while (temp)
            {
                temp = strtok(nullptr, ",");
                if (temp != nullptr)
                    opt->bin_files.push_back(temp);
            }
            break;

        case 'i':
            temp = strtok(optarg_bkup, ",");
            opt->input_files.push_back(temp);
            while (temp)
            {
                temp = strtok(nullptr, ",");
                if (temp != nullptr)
                    opt->input_files.push_back(temp);
            }

            for (uint32_t i = 0; i < opt->input_files.size(); i++)
            {
                ret = load_file_helper(opt->input_files[i].c_str(), &dest, &size);
                if (ret != 0)
                    goto finish;

                opt->inputs.push_back(dest);
                opt->inputs_size.push_back(size);
            }
            break;

        case 'c':
            temp = strtok(optarg_bkup, ",");
            opt->gt_files.push_back(temp);
            while (temp)
            {
                temp = strtok(nullptr, ",");
                if (temp != nullptr)
                    opt->gt_files.push_back(temp);
            }

            for (uint32_t i = 0; i < opt->gt_files.size(); i++)
            {
                ret = load_file_helper(opt->gt_files[i].c_str(), &dest, &size);
                if (ret != 0)
                    goto finish;

                opt->gts.push_back(dest);
                opt->gts_size.push_back(size);
            }
            break;

        case 'd':
            strcpy(opt->dump_dir, optarg);
            break;

        case 'a':
            opt->npu_arch_desc = optarg;
            break;

        case 's':
            strcpy(opt->simulator, optarg);
            break;

        case 'o':
            dump_opt << optarg;
            dump_opt >> std::hex >> opt->dump_opt;
            break;

        case 'l':
            opt->log_level_set = true;
            opt->log_level = atoi(optarg);
            break;

        case 'v':
            opt->verbose = true;
            break;

        case 't':
            if (!strncmp(optarg, "flush", 5))
                opt->flush_time = true;
            else
                opt->flush_time = false;
            break;

        case 'r':
            strcpy(opt->input_shape, optarg);
            break;

        case 'w':
            opt->extra_weight_dir = optarg;
            break;

        case 'h':
            help();
            break;

        case '?':
            break;

        default:
            break;
        }
    }

    semOp_sp = std::make_shared<SemOp>();

finish:
    if (ret != 0)
        deinit_test_bench(opt);

    return ret;
}

int deinit_test_bench(cmd_opt_t* opt)
{
    if (opt == nullptr)
        return 0;

    for (uint32_t i = 0; i < opt->inputs.size(); i++)
        unload_file_helper(opt->inputs[i]);

    opt->input_files.clear();
    opt->inputs_size.clear();
    opt->inputs.clear();

    for (uint32_t i = 0; i < opt->gts.size(); i++)
        unload_file_helper(opt->gts[i]);

    opt->gt_files.clear();
    opt->gts_size.clear();
    opt->gts.clear();
    return 0;
}