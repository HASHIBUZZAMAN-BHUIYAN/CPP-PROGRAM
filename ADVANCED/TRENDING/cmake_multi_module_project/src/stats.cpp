#include "mathutils/stats.h"
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <iomanip>

namespace mathutils {

double mean(const std::vector<double>& data) {
    if (data.empty()) throw std::invalid_argument("empty data");
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}

double variance(const std::vector<double>& data) {
    double m = mean(data);
    double sum_sq = 0;
    for (double x : data) sum_sq += (x - m) * (x - m);
    return sum_sq / data.size();
}

double stdev(const std::vector<double>& data) {
    return std::sqrt(variance(data));
}

double median(std::vector<double> data) {
    if (data.empty()) throw std::invalid_argument("empty data");
    std::sort(data.begin(), data.end());
    size_t n = data.size();
    return (n % 2 == 0) ? (data[n/2 - 1] + data[n/2]) / 2.0 : data[n/2];
}

double percentile(std::vector<double> data, double p) {
    if (data.empty()) throw std::invalid_argument("empty data");
    std::sort(data.begin(), data.end());
    double idx = p / 100.0 * (data.size() - 1);
    size_t lo = static_cast<size_t>(idx);
    size_t hi = lo + 1 < data.size() ? lo + 1 : lo;
    double frac = idx - lo;
    return data[lo] * (1 - frac) + data[hi] * frac;
}

SummaryStats summarize(const std::vector<double>& data) {
    if (data.empty()) throw std::invalid_argument("empty data");
    std::vector<double> sorted = data;
    std::sort(sorted.begin(), sorted.end());
    return {
        sorted.front(),
        sorted.back(),
        mean(data),
        median(data),
        stdev(data),
        data.size()
    };
}

std::string SummaryStats::to_string() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    oss << "n=" << count
        << " min=" << min << " max=" << max
        << " mean=" << mean << " median=" << median
        << " stdev=" << stdev;
    return oss.str();
}

} // namespace mathutils
