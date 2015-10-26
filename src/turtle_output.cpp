#include <turtle_output.hpp>

namespace duraark_rdf {

turtle_output::turtle_output(const std::string& output_file) {
    world_ = raptor_new_world();
    rs_ = raptor_new_serializer(world_, "turtle");
    raptor_serializer_start_to_filename(rs_, output_file.c_str());
}

turtle_output::~turtle_output() {
    raptor_serializer_serialize_end(rs_);
    raptor_free_serializer(rs_);
    raptor_free_world(world_);
}

void
turtle_output::print_sentence(prefix&& prefix) {
    auto p = prefix.value();
    auto u = prefix.uri(world_);
    raptor_serializer_set_namespace(rs_, u, (const unsigned char*)p.c_str());
}

void
turtle_output::print_sentence(statement&& statement) {
    auto s = statement(world_);
    raptor_serializer_serialize_statement(rs_, s);
    raptor_free_statement(s);
}

}  // duraark_rdf
