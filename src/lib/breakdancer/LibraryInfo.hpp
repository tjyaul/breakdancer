#pragma once

#include "BamConfig.hpp"
#include "BamSummary.hpp"

#include <map>
#include <stdint.h>
#include <string>
#include <vector>


class LibraryInfo {
public:
    //leaving the underscore until such a time as we add encapsulation
    BamConfig const& _cfg;
    BamSummary const& _summary;

    LibraryInfo(BamConfig const& cfg, BamSummary const& summary)
        : _cfg(cfg)
        , _summary(summary)
    {
    }

    size_t const& index_for_readgroup(std::string const& rg) const {
        return _cfg.library_config_by_name(_cfg.readgroup_library(rg)).index;
    }
};
