#pragma once

#ifndef SORTER_INCLUDE_CLI_HPP_
#define SORTER_INCLUDE_CLI_HPP_

#include "config.hpp"

class Cli {
public:
    static AppConfig parse(int argc, char *argv[]);
};

#endif // SORTER_INCLUDE_CLI_HPP_
