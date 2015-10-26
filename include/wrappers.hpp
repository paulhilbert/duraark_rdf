#ifndef DURAARK_RDF_WRAPPERS_HPP_
#define DURAARK_RDF_WRAPPERS_HPP_

#include <memory>
#include <string>

#include <raptor2/raptor2.h>

namespace duraark_rdf {

class term {
public:
    term(std::string value);
    virtual ~term();

    virtual raptor_term* operator()(raptor_world* world);

protected:
    std::string value_;
};

class uri : public term {
public:
    uri(std::string value);
    virtual ~uri();

    virtual raptor_term* operator()(raptor_world* world);
};

class literal_type {
public:
    typedef std::unique_ptr<literal_type> uptr_t;
    typedef std::shared_ptr<literal_type> sptr_t;
    typedef std::weak_ptr<literal_type> wptr_t;
    typedef std::shared_ptr<const literal_type> const_sptr_t;
    typedef std::weak_ptr<const literal_type> const_wptr_t;

public:
    literal_type(std::string uri);
    virtual ~literal_type();

    raptor_uri* operator()(raptor_world* world);

protected:
    std::string uri_string_;
    raptor_uri* uri_;
};

class literal : public term {
public:
    literal(std::string value, literal_type::sptr_t type);
    virtual ~literal();

    virtual raptor_term* operator()(raptor_world* world);

protected:
    literal_type::sptr_t type_;
};

class statement {
public:
    typedef std::unique_ptr<statement> uptr_t;
    typedef std::shared_ptr<statement> sptr_t;
    typedef std::weak_ptr<statement> wptr_t;
    typedef std::shared_ptr<const statement> const_sptr_t;
    typedef std::weak_ptr<const statement> const_wptr_t;

public:
    statement(uri&& subject, uri&& predicate, uri&& object);
    statement(uri&& subject, uri&& predicate, literal&& object);
    virtual ~statement();

    raptor_statement* operator()(raptor_world* world);

protected:
    term* subject_;
    term* predicate_;
    term* object_;
    raptor_statement* statement_;
};

class prefix {
public:
    typedef std::unique_ptr<prefix> uptr_t;
    typedef std::shared_ptr<prefix> sptr_t;
    typedef std::weak_ptr<prefix> wptr_t;
    typedef std::shared_ptr<const prefix> const_sptr_t;
    typedef std::weak_ptr<const prefix> const_wptr_t;

public:
    prefix(std::string prefix, std::string uri);
    virtual ~prefix();

    std::string value();
    raptor_uri* uri(raptor_world* world);

protected:
    std::string prefix_;
    std::string uri_string_;
    raptor_uri* uri_;
};

}  // duraark_rdf

#endif /* DURAARK_RDF_WRAPPERS_HPP_ */
