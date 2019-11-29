/*
 * Copyright 2000-2014 NeuStar, Inc. All rights reserved.
 * NeuStar, the Neustar logo and related names and logos are registered
 * trademarks, service marks or tradenames of NeuStar, Inc. All other
 * product names, company names, marks, logos and symbols may be trademarks
 * of their respective owners.
 */

/*
 * test_console_reporter.cpp
 *
 *  Created on: Jul 2, 2014
 *      Author: vpoliboy
 */

#include <gtest/gtest.h>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/thread.hpp>
#include <boost/foreach.hpp>
#include "cppmetrics/core/influxdb_reporter.h"

namespace cppmetrics {
namespace core {

namespace {

TEST(influxdbreporter, test)
{
	influxdb_cpp::server_info si("127.0.0.1", 8086, "test");
	MetricRegistryPtr metric_registry(new MetricRegistry());
	std::string region = "testRegion";
	std::string server = "testServer";
	std::string node = "testNode";
	InfluxdbReporter influxdb_reporter(metric_registry, si, region, server, node);
	influxdb_reporter.start(boost::chrono::milliseconds(999));

	for (size_t i = 0; i < 100; ++i)
	{
		size_t sleep_time = std::rand() % 100 + 10;
		metric_registry->timer("testtimer")->update(boost::chrono::milliseconds(sleep_time));
		metric_registry->counter("testcount")->increment();
		metric_registry->meter("testmeter")->mark();

		boost::this_thread::sleep(boost::posix_time::milliseconds(sleep_time));
	}

	boost::this_thread::sleep(boost::posix_time::milliseconds(9 * 1000));
}

}
}
}

