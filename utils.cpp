#include "utils.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

void export_to_csv(const std::string& filename, 
                   const std::vector<double>& s_grid, 
                   const std::vector<double>& res_cn, 
                   const std::vector<double>& res_imp) {
    
    std::ofstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Erreur d'ouverture du fichier " << filename << std::endl;
        return;
    }

    file << "S;CN_Price;Imp_Price\n";

    for (size_t i = 0; i < s_grid.size(); ++i) {
        file << std::fixed << std::setprecision(6) 
             << s_grid[i] << ";" 
             << res_cn[i] << ";" 
             << res_imp[i] << "\n";
    }

    file.close();
    std::cout << "Fichier " << filename << " généré (données numériques uniquement)." << std::endl;
}