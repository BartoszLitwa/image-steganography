#pragma once
#include "Helpers.hpp"

bool Helpers::endsWith(std::string const& value, std::string const& ending)
{
    if (ending.size() > value.size()) 
        return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}