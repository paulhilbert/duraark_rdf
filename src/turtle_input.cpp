#include <turtle_input.hpp>

#include <sstream>
#include <raptor2/raptor2.h>

namespace duraark_rdf {

void statement_handler(void* user_data, raptor_statement* statement);

turtle_input::turtle_input(const std::string& path) {
    raptor_world* world = raptor_new_world();
    raptor_parser* parser = raptor_new_parser(world, "turtle");

    unsigned char* uri_string;
    raptor_uri *uri, *base_uri;
    uri_string = raptor_uri_filename_to_uri_string(path.c_str());
    uri = raptor_new_uri(world, uri_string);
    base_uri = raptor_uri_copy(uri);

    std::pair<triples_t*, raptor_uri*> user_data(&triples_, base_uri);
    raptor_parser_set_statement_handler(parser, (void*)&user_data,
                                        &statement_handler);

    triples_.clear();
    raptor_parser_parse_file(parser, uri, base_uri);

    raptor_free_uri(base_uri);
    raptor_free_uri(uri);
    raptor_free_memory(uri_string);

    raptor_free_parser(parser);
    raptor_free_world(world);
}

turtle_input::~turtle_input() {}

turtle_input::triples_t&
turtle_input::triples() {
    return triples_;
}

const turtle_input::triples_t&
turtle_input::triples() const {
    return triples_;
}

void
statement_handler(void* user_data, raptor_statement* statement) {
    typedef std::pair<turtle_input::triples_t*, raptor_uri*> user_data_t;
    user_data_t* u_data = (user_data_t*)user_data;
    turtle_input::triples_t* triples = u_data->first;
    raptor_uri* base_uri = u_data->second;
    raptor_term* subject = statement->subject;
    raptor_term* predicate = statement->predicate;
    raptor_term* object = statement->object;
    unsigned char *subject_name_w, *object_name_w, *predicate_name_w;
    switch (subject->type) {
        case RAPTOR_TERM_TYPE_URI:
            subject_name_w = raptor_uri_to_relative_uri_string(base_uri, subject->value.uri);
            break;
        case RAPTOR_TERM_TYPE_LITERAL:
            subject_name_w = subject->value.literal.string;
            break;
        case RAPTOR_TERM_TYPE_BLANK:
            subject_name_w = subject->value.blank.string;
            break;
        default:
            break;
    }
    switch (object->type) {
        case RAPTOR_TERM_TYPE_URI:
            object_name_w = raptor_uri_to_relative_uri_string(base_uri, object->value.uri);
            break;
        case RAPTOR_TERM_TYPE_LITERAL:
            object_name_w = object->value.literal.string;
            break;
        case RAPTOR_TERM_TYPE_BLANK:
            object_name_w = object->value.blank.string;
            break;
        default:
            break;
    }
    switch (predicate->type) {
        case RAPTOR_TERM_TYPE_URI:
            predicate_name_w = raptor_uri_to_relative_uri_string(base_uri, predicate->value.uri);
            break;
        case RAPTOR_TERM_TYPE_LITERAL:
            predicate_name_w = predicate->value.literal.string;
            break;
        case RAPTOR_TERM_TYPE_BLANK:
            predicate_name_w = predicate->value.blank.string;
            break;
        default:
            break;
    }
    std::stringstream subject_sstr, object_sstr, predicate_sstr;
    subject_sstr << subject_name_w;
    object_sstr << object_name_w;
    predicate_sstr << predicate_name_w;
    std::string subject_name = subject_sstr.str();
    std::string object_name = object_sstr.str();
    std::string predicate_name = predicate_sstr.str();
    if (triples->find(subject_name) == triples->end())
        (*triples)[subject_name] = turtle_input::predicates_t();
    (*triples)[subject_name][predicate_name] = object_name;
}

turtle_input::filter::filter() : always_true_(true), regex_(".*") {}

turtle_input::filter::filter(const std::string& regex) : always_true_(false), regex_(std::regex(regex)) {}

turtle_input::filter::~filter() {}

turtle_input::filter turtle_input::filter::begins_with(const std::string& str) {
    return filter("^" + str + ".*");
}

turtle_input::filter turtle_input::filter::ends_with(const std::string& str) {
    return filter(".*" + str + "$");
}

turtle_input::filter turtle_input::filter::equals(const std::string& str) {
    return filter("^" + str + "$");
}

turtle_input::filter turtle_input::filter::contains(const std::string& str) {
    return filter(".*" + str + ".*");
}

bool turtle_input::filter::operator()(const std::string& str) const {
    return always_true_ || std::regex_match(str, regex_);
}

}  // duraark_rdf
