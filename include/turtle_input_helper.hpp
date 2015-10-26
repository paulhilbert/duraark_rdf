#ifndef _DURAARK_RDF_TURTLE_INPUT_HELPER_HPP_
#define _DURAARK_RDF_TURTLE_INPUT_HELPER_HPP_

#include <set>
#include <Eigen/Dense>
#include "turtle_input.hpp"

namespace duraark_rdf {

typedef struct subset_association_ {
    std::string ifc_object_guid;
    std::string pointcloud_guid;
    std::set<uint32_t> point_indices;
} subset_association_t;

std::pair<std::string, bool>
parse_transformation_string(turtle_input& input, const std::string& guid_0,
                            const std::string& guid_1);

Eigen::Matrix4f
parse_registration(turtle_input& input, const std::string& guid_0,
                   const std::string& guid_1);

std::vector<subset_association_t> parse_subset_associations(
    turtle_input& input, std::map<std::string, uint32_t>& possible_hits,
    std::set<std::string>* const pointcloud_entities = nullptr);

}  // duraark_rdf

#endif /* _DURAARK_RDF_TURTLE_INPUT_HELPER_HPP_ */
