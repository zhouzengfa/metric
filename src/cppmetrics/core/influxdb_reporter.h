
#ifndef INFLUXDB_REPORTER_H_
#define INFLUXDB_REPORTER_H_

#include "cppmetrics/core/scheduled_reporter.h"
#include "cppmetrics/core/influxdb.hpp"

namespace cppmetrics {
namespace core {

class InfluxdbReporter: public ScheduledReporter, boost::noncopyable {
public:

    /**
     * Creates a {@link ConsoleReporter} instance. Uses the given registry.
     * @param registry The metric registry.
     * @param ostr The output stream used for printing the values.
     * @param rate_unit The conversion unit user for the rate metrics.
     */
    InfluxdbReporter(MetricRegistryPtr registry,
		influxdb_cpp::server_info& influxdb, std::string& region, std::string& server, std::string& node,
            boost::chrono::milliseconds rate_unit = boost::chrono::seconds(1));
    virtual ~InfluxdbReporter();

    /**
     * Reports all the metrics from the registry periodically to the graphite server.
     * @param gauge_map     all of the gauges in the registry
     * @param counter_map   all of the counters in the registry
     * @param histogram_map all of the histograms in the registry
     * @param meter_map     all of the meters in the registry
     * @param timer_map     all of the timers in the registry
     */
    virtual void report(core::CounterMap counter_map,
            core::HistogramMap histogram_map,
            core::MeteredMap meter_map,
            core::TimerMap timer_map,
            core::GaugeMap gauge_map);

private:
    void reportGauge(const core::GaugeMap::value_type& entry);
    void reportCounter(const core::CounterMap::value_type& entry);
    void reportHistogram(const core::HistogramMap::value_type& entry);
    void reportTimer(const core::TimerMap::value_type& entry);
    void reportMeter(const core::MeteredMap::value_type& entry);

private:
    influxdb_cpp::server_info influxdb_;
	std::string region_;
	std::string server_;
	std::string node_;
};

} /* namespace core */
} /* namespace cppmetrics */
#endif
