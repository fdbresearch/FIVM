#ifndef APPLICATION_RETAILER_FACTORIZED_JOIN_HPP
#define APPLICATION_RETAILER_FACTORIZED_JOIN_HPP

#include "../application.hpp"

void Application::on_snapshot(dbtoaster::data_t& data) {
    on_end_processing(data, false);
}

void Application::on_begin_processing(dbtoaster::data_t& data) {

}

void Application::on_end_processing(dbtoaster::data_t& data, bool print_result) {

    cout << endl << "Enumerating factorized join result... " << endl;

    // cout << "  sizeof(V_locn_IIWLC1) = " << data.get_V_locn_IIWLC1().store.size() << endl;
    // cout << "  sizeof(V_dateid_IIW1) = " << data.get_V_dateid_IIW1().count() << endl;
    // cout << "  sizeof(V_ksn_II1) = " << data.get_V_ksn_II1().count() << endl;
    // cout << "  sizeof(V_subcategory_I1) = " << data.get_V_subcategory_I1().count() << endl;
    // cout << "  sizeof(V_rain_W1) = " << data.get_V_rain_W1().count() << endl;
    // cout << "  sizeof(V_zip_LC1) = " << data.get_V_zip_LC1().count() << endl;
    // cout << "  sizeof(V_rgn_cd_L1) = " << data.get_V_rgn_cd_L1().count() << endl;
    // cout << "  sizeof(V_population_C1) = " << data.get_V_population_C1().count() << endl;

    size_t output_size = 0; 
    size_t total_multiplicity = 0;

    const auto& viewIIWLC = data.get_V_locn_IIWLC1();

    // For each viewIIWLC(locn)
    for (auto &t1 : viewIIWLC.store) {
        auto &key1 = t1.first;
        auto locn = std::get<0>(key1);

        { // For each V_dateid_IIW1(locn) -> dateid
            V_dateid_IIW1_entry e2;
            const auto& rel2 = data.get_V_dateid_IIW1().getValueOrDefault(e2.modify(locn));
            for (auto &t2 : rel2.store) {
                auto &key2 = t2.first;
                auto dateid = std::get<0>(key2);

                // For each V_ksn_II1(locn, dateid) -> (ksn, inventoryunits)
                V_ksn_II1_entry e3;
                const auto& rel3 = data.get_V_ksn_II1().getValueOrDefault(e3.modify(locn, dateid));
                for (auto &t3 : rel3.store) {
                    auto &key3 = t3.first;
                    long value3 = t3.second;
                    auto ksn = std::get<0>(key3); 

                    // For each V_subcategory_I1(ksn) -> (subcategory, category, categoryCluster, prize)
                    V_subcategory_I1_entry e4;
                    const auto& rel4 = data.get_V_subcategory_I1().getValueOrDefault(e4.modify(ksn));
                    for (auto &t4 : rel4.store) {
                        auto &key4 = t4.first;
                        long value4 = t4.second;
                        long value34 = value3 * value4;

                        // For each V_rain_W1(locn, dateid) -> (rain, snow, maxtemp, mintemp, meanwind, thunder)
                        V_rain_W1_entry e5;
                        const auto& rel5 = data.get_V_rain_W1().getValueOrDefault(e5.modify(locn, dateid));
                        for (auto &t5 : rel5.store) {
                            auto &key5 = t5.first;
                            long value5 = t5.second;
                            long value345 = value34 * value5;

                            // For each V_zip_LC1(lcon) -> zip
                            V_zip_LC1_entry e6;
                            const auto& rel6 = data.get_V_zip_LC1().getValueOrDefault(e6.modify(locn));
                            for (auto &t6 : rel6.store) {
                                auto &key6 = t6.first;
                                auto zip = std::get<0>(key6);

                                // For each V_rgn_cd_L1(locn, zip) -> (rgn_cd, clim_zn_nbr, tot_area_sq_ft, sell_area_sq_ft, avghhi, supertargetdistance, supertargetdrivetime, targetdistance, targetdrivetime, walmartdistance, walmartdrivetime, walmartsupercenterdistance, walmartsupercenterdrivetime)
                                V_rgn_cd_L1_entry e7;
                                const auto& rel7 = data.get_V_rgn_cd_L1().getValueOrDefault(e7.modify(locn, zip));
                                for (auto &t7 : rel7.store) {
                                    auto &key7 = t7.first;
                                    long value7 = t7.second;
                                    long value3457 = value345 * value7;

                                    // For each V_population_C1(zip) -> (population, white, asian, pacific, blackafrican, medianage, occupiedhouseunits, houseunits, families, households, husbwife, males, females, householdschildren, hispanic)
                                    V_population_C1_entry e8;
                                    const auto& rel8 = data.get_V_population_C1().getValueOrDefault(e8.modify(zip));
                                    for (auto &t8 : rel8.store) {
                                        auto &key8 = t8.first;
                                        long value8 = t8.second;

                                        auto key = std::tuple_cat(key1, key2, key3, key4, key5, key6, key7, key8);
                                        long value = value3457 * value8;

                                        output_size++;
                                        total_multiplicity += value;

                                        if (print_result) cout << key << " -> " << value << endl;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    cout << "Number of output tuples: " << output_size << endl;
    cout << "Total multiplicity: " << total_multiplicity << endl;
}

#endif /* APPLICATION_RETAILER_FACTORIZED_JOIN_HPP */