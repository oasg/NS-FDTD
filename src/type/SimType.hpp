//
// Created by gouru on 11/21/2023.
//

#ifndef NS_FDTD_SIMTYPE_HPP
#define NS_FDTD_SIMTYPE_HPP
#include <vector>
#include <string>
namespace TYPE{
    enum sim_type{
        FTDT_TE,
        FTDT_TM,
        NsFTDT_TE,
        NsFTDT_TM,
    };

    const std::vector<std::string> sim_type_name = {"FTDT_TE","FTDT_TM","NsFTDT_TE","NsFTDT_TM"};
}

#endif //NS_FDTD_SIMTYPE_HPP
