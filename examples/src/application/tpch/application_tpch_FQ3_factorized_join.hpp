#ifndef APPLICATION_TPCH_FQ3_FACTORIZED_JOIN_HPP
#define APPLICATION_TPCH_FQ3_FACTORIZED_JOIN_HPP

#include "../application.hpp"

void Application::on_snapshot(dbtoaster::data_t& data) {
    on_end_processing(data, false);
}

void Application::on_begin_processing(dbtoaster::data_t& data) {

}

void Application::on_end_processing(dbtoaster::data_t& data, bool print_result) {

    cout << endl << "Enumerating factorized join result... " << endl;

    // cout << "  sizeof(V_orderkey_LPSCO1) = " << data.get_V_orderkey_LPSCO1().store.size() << endl;
    // cout << "  sizeof(V_partkey_LPS1) = " << data.get_V_partkey_LPS1().count() << endl;
    // cout << "  sizeof(V_suppkey_LPS1) = " << data.get_V_suppkey_LPS1().count() << endl;
    // cout << "  sizeof(V_l_linenumber_L1) = " << data.get_V_l_linenumber_L1().count() << endl;
    // cout << "  sizeof(V_ps_availqty_P1) = " << data.get_V_ps_availqty_P1().count() << endl;
    // cout << "  sizeof(V_s_name_S1) = " << data.get_V_s_name_S1().count() << endl;
    // cout << "  sizeof(V_custkey_CO1) = " << data.get_V_custkey_CO1().count() << endl;
    // cout << "  sizeof(V_c_name_C1) = " << data.get_V_c_name_C1().count() << endl;
    // cout << "  sizeof(V_o_orderstatus_O1) = " << data.get_V_o_orderstatus_O1().count() << endl;

    size_t output_size = 0; 
    size_t total_multiplicity = 0;

    const auto& v_orderkey_LPSCO1 = data.get_V_orderkey_LPSCO1();

    // For each v_orderkey_LPSCO1(orderkey)
    for (auto &t1 : v_orderkey_LPSCO1.store) {
        auto &key1 = t1.first;
        auto orderkey = std::get<0>(key1);

        { // For each V_partkey_LPS1(orderkey) -> partkey
            V_partkey_LPS1_entry e2;
            const auto& rel2 = data.get_V_partkey_LPS1().getValueOrDefault(e2.modify(orderkey));
            for (auto &t2 : rel2.store) {
                auto &key2 = t2.first;
                auto partkey = std::get<0>(key2);

                // For each V_suppkey_LPS1(orderkey, partkey) -> suppkey
                V_suppkey_LPS1_entry e3;
                const auto& rel3 = data.get_V_suppkey_LPS1().getValueOrDefault(e3.modify(orderkey, partkey));
                for (auto &t3 : rel3.store) {
                    auto &key3 = t3.first;
                    auto suppkey = std::get<0>(key3);

                    // For each V_l_linenumber_L1(orderkey, partkey, suppkey) -> (l_linenumber, l_quantity, l_extendedprice, l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate, l_receiptdate, l_shipinstruct, l_shipmode, l_comment)
                    V_l_linenumber_L1_entry e4;
                    const auto& rel4 = data.get_V_l_linenumber_L1().getValueOrDefault(e4.modify(orderkey, partkey, suppkey));
                    for (auto &t4 : rel4.store) {
                        auto &key4 = t4.first;
                        long value4 = t4.second;

                        // For each V_ps_availqty_P1(partkey, suppkey) -> (ps_availqty, ps_supplycost, ps_comment)
                        V_ps_availqty_P1_entry e5;
                        const auto& rel5 = data.get_V_ps_availqty_P1().getValueOrDefault(e5.modify(partkey, suppkey));
                        for (auto &t5 : rel5.store) {
                            auto &key5 = t5.first;
                            long value5 = t5.second;
                            long value45 = value4 * value5;

                            // For each V_s_name_S1(suppkey) -> (s_name, s_address, s_nationkey, s_phone, s_acctbal, s_comment)
                            V_s_name_S1_entry e6;
                            const auto& rel6 = data.get_V_s_name_S1().getValueOrDefault(e6.modify(suppkey));
                            for (auto &t6 : rel6.store) {
                                auto &key6 = t6.first;
                                long value6 = t6.second;
                                long value456 = value45 * value6;

                                // For each V_custkey_CO1(orderkey) -> custkey
                                V_custkey_CO1_entry e7;
                                const auto& rel7 = data.get_V_custkey_CO1().getValueOrDefault(e7.modify(orderkey));
                                for (auto &t7 : rel7.store) {
                                    auto &key7 = t7.first;
                                    auto custkey = std::get<0>(key7);

                                    // For each V_c_name_C1(custkey) -> (c_name, c_address, c_nationkey, c_phone, c_acctbal, c_mktsegment, c_comment)
                                    V_c_name_C1_entry e8;
                                    const auto& rel8 = data.get_V_c_name_C1().getValueOrDefault(e8.modify(custkey));
                                    for (auto &t8 : rel8.store) {
                                        auto &key8 = t8.first;
                                        long value8 = t8.second;
                                        long value4568 = value456 * value8;

                                        // For each V_o_orderstatus_O1(orderkey, custkey) -> (o_orderstatus, o_totalprice, o_orderdate, o_orderpriority, o_clerk, o_shippriority, o_comment)
                                        V_o_orderstatus_O1_entry e9;
                                        const auto& rel9 = data.get_V_o_orderstatus_O1().getValueOrDefault(e9.modify(orderkey, custkey));
                                        for (auto &t9 : rel9.store) {
                                            auto &key9 = t9.first;
                                            long value9 = t9.second;

                                            auto key = std::tuple_cat(key1, key2, key3, key4, key5, key6, key7, key8, key9);
                                            long value = value4568 * value9;

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
    }

    cout << "Number of output tuples: " << output_size << endl;
    cout << "Total multiplicity: " << total_multiplicity << endl;
}

#endif /* APPLICATION_TPCH_FQ3_FACTORIZED_JOIN_HPP */