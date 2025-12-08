#ifndef APPLICATION_RETAILER_LISTING_JOIN_HPP
#define APPLICATION_RETAILER_LISTING_JOIN_HPP

#include "../application.hpp"

void Application::on_snapshot(dbtoaster::data_t& data) {
    on_end_processing(data, false);
}

void Application::on_begin_processing(dbtoaster::data_t& data) {

}

void Application::on_end_processing(dbtoaster::data_t& data, bool print_result) {

    cout << endl << "Enumerating listing join result... " << endl;

    const auto& viewIIWLC = data.get_V_locn_IIWLC1();

    size_t output_size = viewIIWLC.store.size();
    size_t total_multiplicity = 0;

    for (auto &t : viewIIWLC.store) {
        if (print_result) cout << t.first << " -> " << t.second << endl;
        total_multiplicity += t.second;
    }

    cout << "Number of output tuples: " << output_size << endl;
    cout << "Total multiplicity: " << total_multiplicity << endl;
}

#endif /* APPLICATION_RETAILER_LISTING_JOIN_HPP */