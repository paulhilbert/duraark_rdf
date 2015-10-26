#include <wrappers.hpp>

#include <iostream>

namespace duraark_rdf {

term::term(std::string value) : value_(value) {}

term::~term() {}

raptor_term* term::operator()(raptor_world* world) { return nullptr; }

uri::uri(std::string value) : term(value) {}

uri::~uri() {}

raptor_term* uri::operator()(raptor_world* world) {
    return raptor_new_term_from_uri_string(
        world, (const unsigned char*)value_.c_str());
}

literal_type::literal_type(std::string uri) : uri_string_(uri), uri_(nullptr) {}

literal_type::~literal_type() {
    if (uri_) raptor_free_uri(uri_);
}

raptor_uri* literal_type::operator()(raptor_world* world) {
    if (!uri_)
        uri_ = raptor_new_uri(world, (const unsigned char*)uri_string_.c_str());
    return uri_;
}

literal::literal(std::string value, literal_type::sptr_t type)
    : term(value), type_(type) {}

literal::~literal() {}

raptor_term* literal::operator()(raptor_world* world) {
    return raptor_new_term_from_literal(
        world, (const unsigned char*)value_.c_str(), (*type_)(world), nullptr);
}

statement::statement(uri&& subject, uri&& predicate, uri&& object)
    : statement_(nullptr) {
    subject_ = new uri(std::move(subject));
    predicate_ = new uri(std::move(predicate));
    object_ = new uri(std::move(object));
}

statement::statement(uri&& subject, uri&& predicate, literal&& object)
    : statement_(nullptr) {
    subject_ = new uri(std::move(subject));
    predicate_ = new uri(std::move(predicate));
    object_ = new literal(std::move(object));
}

statement::~statement() {
    if (statement_) raptor_free_statement(statement_);
}

raptor_statement* statement::operator()(raptor_world* world) {
    if (!statement_) {
        statement_ = raptor_new_statement(world);
        statement_->subject = (*subject_)(world);
        statement_->predicate = (*predicate_)(world);
        statement_->object = (*object_)(world);
    }
    return statement_;
}

prefix::prefix(std::string prefix, std::string uri)
    : prefix_(prefix), uri_string_(uri), uri_(nullptr) {}

prefix::~prefix() {
    if (uri_) raptor_free_uri(uri_);
}

std::string
prefix::value() {
    return prefix_;
}

raptor_uri*
prefix::uri(raptor_world* world) {
    if (!uri_)
        uri_ = raptor_new_uri(world, (const unsigned char*)uri_string_.c_str());
    return uri_;
}

}  // duraark_rdf
