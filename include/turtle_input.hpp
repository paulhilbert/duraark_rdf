#ifndef DURAARK_RDF_RAPTOR_INPUT_HPP_
#define DURAARK_RDF_RAPTOR_INPUT_HPP_

#include <memory>
#include <map>
#include <regex>
#include <tuple>

namespace duraark_rdf {

class turtle_input {
public:
    typedef std::unique_ptr<turtle_input> uptr_t;
    typedef std::shared_ptr<turtle_input> sptr_t;
    typedef std::weak_ptr<turtle_input> wptr_t;
    typedef std::shared_ptr<const turtle_input> const_sptr_t;
    typedef std::weak_ptr<const turtle_input> const_wptr_t;
    typedef std::map<std::string, std::string> predicates_t;
    typedef std::map<std::string, predicates_t> triples_t;

    class filter;
    template <typename F0, typename F1>
    class disjunctive_filter;
    template <typename F0, typename F1>
    class conjunctive_filter;

public:
    turtle_input(const std::string& path);
    virtual ~turtle_input();

    triples_t& triples();

    const triples_t& triples() const;

    template <typename F0, typename F1>
    triples_t filter_triples(F0&& subject_filter, F1&& predicate_filter);

protected:
    triples_t triples_;
};

class turtle_input::filter {
public:
    filter();

    filter(const std::string& regex);

    virtual ~filter();

    static filter begins_with(const std::string& str);

    static filter ends_with(const std::string& str);

    static filter equals(const std::string& str);

    static filter contains(const std::string& str);

    bool operator()(const std::string& str) const;

    template <typename F>
    disjunctive_filter<filter, F> operator||(F other) const;

    template <typename F>
    conjunctive_filter<filter, F> operator&&(F other) const;

protected:
    bool always_true_;
    std::regex regex_;
};

template <typename F0, typename F1>
class turtle_input::disjunctive_filter {
public:
    disjunctive_filter(F0 f0, F1 f1);

    virtual ~disjunctive_filter();

    bool operator()(const std::string& str) const;

protected:
    F0 f0_;
    F1 f1_;
};

template <typename F0, typename F1>
class turtle_input::conjunctive_filter {
public:
    conjunctive_filter(F0 f0, F1 f1);

    virtual ~conjunctive_filter();

    bool operator()(const std::string& str) const;

protected:
    F0 f0_;
    F1 f1_;
};

}  // duraark_rdf

#include "turtle_input.ipp"

#endif /* DURAARK_RDF_RAPTOR_INPUT_HPP_ */
