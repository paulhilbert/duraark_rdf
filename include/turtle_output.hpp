#ifndef DURAARK_RDF_TURTLE_OUTPUT_HPP_
#define DURAARK_RDF_TURTLE_OUTPUT_HPP_

#include <string>
#include <memory>

#include <raptor2/raptor2.h>

#include "wrappers.hpp"

namespace duraark_rdf {

class turtle_output {
public:
    typedef std::shared_ptr<turtle_output> Ptr;
    typedef std::weak_ptr<turtle_output> WPtr;
    typedef std::shared_ptr<const turtle_output> ConstPtr;
    typedef std::weak_ptr<const turtle_output> ConstWPtr;

public:
    turtle_output(const std::string& output_file);
    virtual ~turtle_output();

    void print_sentence(prefix&& prefix);
    void print_sentence(statement&& statement);

protected:
    raptor_world* world_ = nullptr;
    raptor_serializer* rs_ = nullptr;
};

}  // duraark_rdf

#endif /* DURAARK_RDF_TURTLE_OUTPUT_HPP_ */
