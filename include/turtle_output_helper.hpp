#ifndef _DURAARK_RDF_TURTLE_OUTPUT_HELPER_HPP_
#define _DURAARK_RDF_TURTLE_OUTPUT_HELPER_HPP_

#include <Eigen/Dense>

#include "turtle_output.hpp"
#include "entity.hpp"

namespace duraark_rdf {

void write_prologue(turtle_output& output);

void write_entity(turtle_output& output, const entity& e);

void write_subset_entity(turtle_output& output, const entity& e,
                         uint32_t possible_hits);

void write_registration(turtle_output& output, const entity& src,
                        const entity& tgt, const Eigen::Matrix4f& t);

}  // duraark_rdf

#endif /* _DURAARK_RDF_TURTLE_OUTPUT_HELPER_HPP_ */
