#pragma once
#include <vector>
#include <string>

namespace mathutils {

double mean(const std::vector<double>& data);
double variance(const std::vector<double>& data);
double stdev(const std::vector<double>& data);
double median(std::vector<double> data);   // takes by value (sorts in place)
double percentile(std::vector<double> data, double p);

struct SummaryStats {
    double min, max, mean, median, stdev;
    size_t count;
    std::string to_string() const;
};

SummaryStats summarize(const std::vector<double>& data);

} // namespace mathutils
