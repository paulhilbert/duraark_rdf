#include <turtle_output_helper.hpp>

namespace duraark_rdf {

void
write_prologue(turtle_output& output) {
    output.print_sentence(prefix("xsd", "http://www.w3.org/2001/XMLSchema#"));
    output.print_sentence(prefix("rel", "http://duraark.eu/rdf/relations#"));
    output.print_sentence(prefix("obj", "http://duraark.eu/rdf/objects#"));
    output.print_sentence(prefix("types", "http://duraark.eu/rdf/types#"));
}

void
write_entity(turtle_output& output, const entity& e) {
    output.print_sentence(statement(
        uri(e.name()), uri("rel:globalUniqueId"),
        literal(e.guid(), std::make_shared<literal_type>("xsd:string"))));
}

void
write_subset_entity(turtle_output& output, const entity& e, uint32_t possible_hits) {
    if (e.type() != entity::type_t::SUBSET) return;

    auto string_type = std::make_shared<literal_type>("xsd:string");
    auto integer_type = std::make_shared<literal_type>("xsd:nonNegativeInteger");
    auto indices_type = std::make_shared<literal_type>("types:indexSet");

    write_entity(output, e);
    output.print_sentence(statement(uri(e.name()), uri("rel:pointSubsetOf"),
                                    uri(e.parent()->name())));
    output.print_sentence(statement(uri(e.name()),
                                    uri("rel:pointSubsetContains"),
                                    literal(e.indices_string(), indices_type)));
    output.print_sentence(
        statement(uri(e.name()), uri("rel:subsetRepOf"),
                  literal(e.representation_of(), string_type)));

    output.print_sentence(
        statement(uri(e.name()), uri("rel:possibleScanHits"),
                  literal(std::to_string(possible_hits), integer_type)));
}

void
write_registration(turtle_output& output, const entity& src, const entity& tgt,
                   const Eigen::Matrix4f& t) {
    auto string_type = std::make_shared<literal_type>("xsd:string");

    std::string tname = "obj:" + src.name() + "_to_" + tgt.name();
    std::string aname = "obj:affine_" + src.name() + "_to_" + tgt.name();
    output.print_sentence(
        statement(uri(tname), uri("rel:corrSource"), uri(src.name())));
    output.print_sentence(
        statement(uri(tname), uri("rel:corrTarget"), uri(tgt.name())));
    output.print_sentence(statement(
        uri(tname), uri("rel:correspondenceTransformation"), uri(aname)));

    bool second = false;
    std::string matrix;
    for (unsigned int i = 0; i < 16; ++i) {
        if (second++) matrix += " ";
        matrix += std::to_string(t(i / 4, i % 4));
    }
    output.print_sentence(statement(uri(tname), uri("rel:transformType"),
                                    uri("obj:affineTransform")));
    output.print_sentence(statement(uri(tname), uri("rel:transformParam"),
                                    literal(matrix, string_type)));
}

}  // duraark_rdf
