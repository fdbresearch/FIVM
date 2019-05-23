#ifndef APPLICATION_TPCH_HPP
#define APPLICATION_TPCH_HPP

#include "application_tpch_base.hpp"

void Application::on_snapshot(dbtoaster::data_t& data) {
    on_end_processing(data, false);
}

void Application::on_end_processing(dbtoaster::data_t& data, bool print_result) {
    if (print_result) {
        data.serialize(std::cout, 0);
    }
}

#endif /* APPLICATION_TPCH_HPP */