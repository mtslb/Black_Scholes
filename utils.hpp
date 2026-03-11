#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>

/**
 * @brief Exporte uniquement les résultats numériques des solveurs en CSV
 */
void export_to_csv(const std::string& filename, 
                   const std::vector<double>& s_grid, 
                   const std::vector<double>& res_cn, 
                   const std::vector<double>& res_imp);

#endif