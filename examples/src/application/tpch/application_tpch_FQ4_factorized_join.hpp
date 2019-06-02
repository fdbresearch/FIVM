#ifndef APPLICATION_TPCH_FQ4_FACTORIZED_JOIN_HPP
#define APPLICATION_TPCH_FQ4_FACTORIZED_JOIN_HPP

#include "application_tpch_base.hpp"

void Application::on_snapshot(dbtoaster::data_t& data) {
    on_end_processing(data, false);
}

void Application::on_begin_processing(dbtoaster::data_t& data) {

}

void Application::on_end_processing(dbtoaster::data_t& data, bool print_result) {

    cout << endl << "Enumerating factorized join result... " << endl;

    cout << "  sizeof(V_suppkey_LPS1) = " << data.get_V_suppkey_LPS1().store.size() << endl;
    cout << "  sizeof(V_partkey_LP1) = " << data.get_V_partkey_LP1().count() << endl;
    cout << "  sizeof(V_l_orderkey_L1) = " << data.get_V_l_orderkey_L1().count() << endl;
    cout << "  sizeof(V_ps_availqty_P1) = " << data.get_V_ps_availqty_P1().count() << endl;
    cout << "  sizeof(V_s_name_S1) = " << data.get_V_s_name_S1().count() << endl;

    size_t output_size = 0; 
    size_t total_multiplicity = 0;

    const auto& v_suppkey_LPS1 = data.get_V_suppkey_LPS1();

    // For each v_suppkey_LPS1(suppkey)
    for (auto &t1 : v_suppkey_LPS1.store) {
        auto &key1 = t1.first;
        auto suppkey = std::get<0>(key1);

        { // For each V_partkey_LP1(suppkey) -> partkey
            V_partkey_LP1_entry e2;
            const auto& rel2 = data.get_V_partkey_LP1().getValueOrDefault(e2.modify(suppkey));
            for (auto &t2 : rel2.store) {
                auto &key2 = t2.first;
                auto partkey = std::get<0>(key2);

                // For each V_l_orderkey_L1(partkey, suppkey) -> (l_orderkey, l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment)
                V_l_orderkey_L1_entry e3;
                const auto& rel3 = data.get_V_l_orderkey_L1().getValueOrDefault(e3.modify(partkey, suppkey));
                for (auto &t3 : rel3.store) {
                    auto &key3 = t3.first;
                    long value3 = t3.second;

                    // For each V_ps_availqty_P1(partkey, suppkey) -> (ps_availqty, ps_supplycost, ps_comment)
                    V_ps_availqty_P1_entry e4;
                    const auto& rel4 = data.get_V_ps_availqty_P1().getValueOrDefault(e4.modify(partkey, suppkey));
                    for (auto &t4 : rel4.store) {
                        auto &key4 = t4.first;
                        long value4 = t4.second;
                        long value34 = value3 * value4;

                        // For each V_s_name_S1(suppkey) -> (s_name, s_address, s_nationkey, s_phone, s_acctbal, s_comment)
                        V_s_name_S1_entry e5;
                        const auto& rel5 = data.get_V_s_name_S1().getValueOrDefault(e5.modify(suppkey));
                        for (auto &t5 : rel5.store) {
                            auto &key5 = t5.first;
                            long value5 = t5.second;
                            
                            auto key = std::tuple_cat(key1, key2, key3, key4, key5);
                            long value = value34 * value5;

                            output_size++;
                            total_multiplicity += value;

                            if (print_result) cout << key << " -> " << value << endl;
                        }
                    }
                }
            }
        }
    }

    cout << "Number of output tuples: " << output_size << endl;
    cout << "Total multiplicity: " << total_multiplicity << endl;
}

#endif /* APPLICATION_TPCH_FQ4_FACTORIZED_JOIN_HPP */