#pragma once

#include "ReadCountsByLib.hpp"
#include "ReadRegionData.hpp"
#include "BasicRegion.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iterator>
#include <map>
#include <numeric>
#include <stdint.h>
#include <string>
#include <vector>

#include <boost/unordered_map.hpp>

class Options;
class BamConfig;
class IBamReader;

class BreakDancer {
public:
    typedef breakdancer::Read ReadType;
    typedef BasicRegion::ReadVector ReadVector;
    typedef std::vector<BasicRegion*> RegionData;
    typedef std::vector<ReadCountsByLib> RoiReadCounts;
    typedef std::map<std::string, std::vector<int> > ReadsToRegionsMap;

    BreakDancer(
        Options const& opts,
        BamConfig const& cfg,
        IBamReader& merged_reader,
        int max_read_window_size
        );

    void push_read(ReadType& aln, bam_header_t const* bam_header);
    void build_connection(bam_header_t const* bam_header);
    int sum_of_region_sizes(std::vector<int> const& region_ids) const;

    void set_max_read_window_size(int val) {
        _max_read_window_size = val;
    }

    void process_breakpoint(bam_header_t const* bam_header);
    void process_final_region(bam_header_t const* bam_header);

    void run();

private:
    uint32_t _region_lib_counts(size_t region_idx, std::string const& lib, RoiReadCounts const& x) const {
        if (region_idx >= x.size())
            return 0;
        RoiReadCounts::value_type::const_iterator found = x[region_idx].find(lib);
        if (found != x[region_idx].end())
            return found->second;
        return 0;
    }

private:
    ReadRegionData _rdata;
    Options const& _opts;
    BamConfig const& _cfg;
    IBamReader& _merged_reader;
    int _max_read_window_size;

    bool _collecting_normal_reads;
    int _nnormal_reads;
    int _ntotal_nucleotides;
    int _max_readlen;
    int _buffer_size;

    int _region_start_tid;
    int _region_start_pos;
    int _region_end_tid; // global (chr, should be int in samtools)
    int _region_end_pos; // global

    ReadVector reads_in_current_region;

public:
    std::map<std::string, float> read_density;

    void process_sv(std::vector<int> const& snodes, std::set<int>& free_nodes, bam_header_t const* bam_header);
};
