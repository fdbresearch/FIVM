#ifndef APPLICATION_RETAILER_HPP
#define APPLICATION_RETAILER_HPP

#include <sys/time.h>
#include "application_retailer_base.hpp"

void Application::on_snapshot(dbtoaster::data_t& data) {
    struct timeval tp;
    gettimeofday(&tp, nullptr);
    std::cout << data.tN << " tuples processed at "
              << tp.tv_sec * 1000 + tp.tv_usec / 1000
              << " ms" << std::endl;
}

void Application::on_begin_processing(dbtoaster::data_t& data) {

}

void Application::on_end_processing(dbtoaster::data_t& data, bool print_result) {
    if (print_result) {
        data.serialize(std::cout, 0);
    }
}

#endif /* APPLICATION_RETAILER_HPP */