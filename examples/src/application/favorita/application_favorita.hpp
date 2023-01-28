#ifndef APPLICATION_FAVORITA_HPP
#define APPLICATION_FAVORITA_HPP

#include <sys/time.h>
#include "application_favorita_base.hpp"

void Application::on_snapshot(dbtoaster::data_t& data) {
    struct timeval tp;
    gettimeofday(&tp, nullptr);
    std::cout << data.tN << " tuples processed at "
              << tp.tv_sec * 1000 + tp.tv_usec / 1000
              << " ms" << std::endl;
    DUMP_HEAP_PROFILE
}

void Application::on_begin_processing(dbtoaster::data_t& data) {
    START_HEAP_PROFILE
}

void Application::on_end_processing(dbtoaster::data_t& data, bool print_result) {
    STOP_HEAP_PROFILE

    if (print_result) {
        data.serialize(std::cout, 0);
    }
}

#endif /* APPLICATION_FAVORITA_HPP */