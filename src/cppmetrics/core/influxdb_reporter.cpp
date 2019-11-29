#include <glog/logging.h>
#include "influxdb_reporter.h"
#include "utils.h"

namespace cppmetrics {
namespace core {

InfluxdbReporter::InfluxdbReporter(MetricRegistryPtr registry, influxdb_cpp::server_info& influxdb,
	std::string& region, std::string& server, std::string& node, boost::chrono::milliseconds rate_unit) :
	ScheduledReporter(registry, rate_unit),
	influxdb_(influxdb),
	region_(region),
	server_(server),
	node_(node)
{
}

InfluxdbReporter::~InfluxdbReporter()
{
}

void InfluxdbReporter::report(core::CounterMap counter_map,
	core::HistogramMap histogram_map,
	core::MeteredMap meter_map,
	core::TimerMap timer_map,
	core::GaugeMap gauge_map)
{
	if (!gauge_map.empty())
	{
		BOOST_FOREACH(const core::GaugeMap::value_type & entry, gauge_map)
		{
			reportGauge(entry);
		}
	}

	if (!counter_map.empty())
	{
		BOOST_FOREACH(const core::CounterMap::value_type & entry, counter_map)
		{
			reportCounter(entry);
		}
	}

	if (!histogram_map.empty())
	{
		BOOST_FOREACH(const core::HistogramMap::value_type & entry, histogram_map)
		{
			reportHistogram(entry);
		}
	}

	if (!meter_map.empty())
	{
		BOOST_FOREACH(const core::MeteredMap::value_type & entry, meter_map)
		{
			reportMeter(entry);
		}
	}

	if (!timer_map.empty())
	{
		BOOST_FOREACH(const core::TimerMap::value_type & entry, timer_map)
		{
			reportTimer(entry);
		}
	}
}

void InfluxdbReporter::reportMeter(const core::MeteredMap::value_type& entry)
{
	uint64_t timestamp = get_millis_from_epoch();

	const MeteredPtr meter = entry.second;
	std::string resp;
	int ret = influxdb_cpp::builder()
		.meas(entry.first)
		.tag("region", region_)
		.tag("server", server_)
		.tag("node", node_)
		.field("count",(long long)meter->getCount())
		.field("mean", convertRateUnit(meter->getMeanRate()))
		.field("oneMinute",convertRateUnit(meter->getOneMinuteRate()))
		.field("fiveMinute",convertRateUnit(meter->getFiveMinuteRate()))
		.field("fifteenMinute",convertRateUnit(meter->getFifteenMinuteRate()))
		.timestamp(timestamp)
		.post_http(influxdb_, &resp);

	if (ret != 0)
	{
		LOG(ERROR) << "ret = " << ret << " resp(" << resp << ")";
	}
}

void InfluxdbReporter::reportCounter(const core::CounterMap::value_type& entry)
{
	uint64_t timestamp = get_millis_from_epoch();

	std::string resp;
	int ret = influxdb_cpp::builder()
		.meas(entry.first)
		.tag("region", region_)
		.tag("server", server_)
		.tag("node", node_)
		.field("value", (long long)entry.second->getCount())
		.timestamp(timestamp)
		.post_http(influxdb_, &resp);

	if (ret != 0)
	{
		LOG(ERROR) << "ret = " << ret << " resp(" << resp << ")";
	}

	LOG(ERROR) << entry.first << " ret:" << ret;
}

void InfluxdbReporter::reportGauge(const core::GaugeMap::value_type& entry)
{
	uint64_t timestamp = get_millis_from_epoch();

	std::string resp;
	int ret = influxdb_cpp::builder()
		.meas(entry.first)
		.tag("region", region_)
		.tag("server", server_)
		.tag("node", node_)
		.field("value", (long long)entry.second->getValue())
		.timestamp(timestamp)
		.post_http(influxdb_, &resp);

	if (ret != 0)
	{
		LOG(ERROR) << "ret = " << ret << " resp(" << resp << ")";
	}
}

void InfluxdbReporter::reportHistogram(const core::HistogramMap::value_type& entry)
{
	uint64_t timestamp = get_millis_from_epoch();

	const HistogramPtr histogram = entry.second;
    SnapshotPtr snapshot = histogram->getSnapshot();

	std::string resp;
	int ret = influxdb_cpp::builder()
		.meas(entry.first)
		.tag("region", region_)
		.tag("server", server_)
		.tag("node", node_)
		.field("count", (long long)histogram->getCount())
		.field("min", snapshot->getMin())
		.field("max", snapshot->getMax())
		.field("mean", snapshot->getMean())
		.field("stddev", snapshot->getStdDev())
		.field("median", snapshot->getMedian())
		.field("75thPer", snapshot->get75thPercentile())
		.field("95thPer", snapshot->get95thPercentile())
		.field("98thPer", snapshot->get98thPercentile())
		.field("99thPer", snapshot->get99thPercentile())
		.field("999thPer", snapshot->get999thPercentile())
		.timestamp(timestamp)
		.post_http(influxdb_, &resp);

	if (ret != 0)
	{
		LOG(ERROR) << "ret = " << ret << " resp(" << resp << ")";
	}
}

void InfluxdbReporter::reportTimer(const core::TimerMap::value_type& entry)
{
	uint64_t timestamp = get_millis_from_epoch();

	TimerPtr timer = entry.second;
    SnapshotPtr snapshot = timer->getSnapshot();

	std::string resp;
	int ret = influxdb_cpp::builder()
		.meas(entry.first)
		.tag("region", region_)
		.tag("server", server_)
		.tag("node", node_)
		.field("count", (long long)timer->getCount())
		.field("min", snapshot->getMin())
		.field("max", snapshot->getMax())
		.field("mean", snapshot->getMean())
		.field("stddev", snapshot->getStdDev())
		.field("median", snapshot->getMedian())
		.field("75thPer", snapshot->get75thPercentile())
		.field("95thPer", snapshot->get95thPercentile())
		.field("98thPer", snapshot->get98thPercentile())
		.field("99thPer", snapshot->get99thPercentile())
		.field("999thPer", snapshot->get999thPercentile())
		.timestamp(timestamp)
		.post_http(influxdb_, &resp);

	if (ret != 0)
	{
		LOG(ERROR) << "ret = " << ret << " resp(" << resp << ")";
	}
}

} /* namespace core */
} /* namespace cppmetrics */
