yum install glog-devel gtest-devel gflags-devel boost-devel -y
cmake ../ -DBOOST_DIR=/usr/include/boost -DGTEST_DIR=/usr/include/gtest -DGLOG_DIR=/usr/include/glog -DCMAKE_CXX_COMPILER=g++ -DCMAKE_INSTALL_PREFIX=dist
