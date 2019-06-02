#ifndef APPLICATION_TPCH_FQ3_LISTING_JOIN_HPP
#define APPLICATION_TPCH_FQ3_LISTING_JOIN_HPP

#include "application_tpch_base.hpp"

void Application::on_snapshot(dbtoaster::data_t& data) {
    on_end_processing(data, false);
}

void Application::on_begin_processing(dbtoaster::data_t& data) {

}

void Application::on_end_processing(dbtoaster::data_t& data, bool print_result) {

    cout << endl << "Enumerating listing join result... " << endl;

    const auto& v_orderkey_LPSCO1 = data.get_V_orderkey_LPSCO1();

    size_t output_size = v_orderkey_LPSCO1.store.size();
    size_t total_multiplicity = 0;

    for (auto &t : v_orderkey_LPSCO1.store) {
        if (print_result) cout << t.first << " -> " << t.second << endl;
        total_multiplicity += t.second;
    }

    cout << "Number of output tuples: " << output_size << endl;
    cout << "Total multiplicity: " << total_multiplicity << endl;
}

#endif /* APPLICATION_TPCH_FQ3_LISTING_JOIN_HPP */