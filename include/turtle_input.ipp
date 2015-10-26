namespace duraark_rdf {

template <typename F0, typename F1>
inline turtle_input::triples_t
turtle_input::filter_triples(F0&& subject_filter, F1&& predicate_filter) {
    triples_t filtered;
    for (const auto& subject : triples_) {
        if (!subject_filter(subject.first)) continue;
        for (const auto& pred : subject.second) {
            if (!predicate_filter(pred.first)) continue;
            if (filtered.find(subject.first) == filtered.end()) {
                filtered[subject.first] = predicates_t();
            }
            filtered[subject.first][pred.first] = pred.second;
        }
    }

    return filtered;
}

template <typename F>
turtle_input::disjunctive_filter<turtle_input::filter, F> turtle_input::filter::operator||(F other) const {
    return disjunctive_filter<filter, F>(*this, other);
}

template <typename F>
turtle_input::conjunctive_filter<turtle_input::filter, F> turtle_input::filter::operator&&(F other) const {
    return conjunctive_filter<filter, F>(*this, other);
}

template <typename F0, typename F1>
inline turtle_input::disjunctive_filter<F0, F1>::disjunctive_filter(F0 f0, F1 f1) : f0_(std::move(f0)), f1_(std::move(f1)) {
}

template <typename F0, typename F1>
inline turtle_input::disjunctive_filter<F0, F1>::~disjunctive_filter() {
}

template <typename F0, typename F1>
inline bool turtle_input::disjunctive_filter<F0, F1>::operator()(const std::string& str) const {
    return f0_(str) || f1_(str);
}

template <typename F0, typename F1>
inline turtle_input::conjunctive_filter<F0, F1>::conjunctive_filter(F0 f0, F1 f1) : f0_(std::move(f0)), f1_(std::move(f1)) {
}

template <typename F0, typename F1>
inline turtle_input::conjunctive_filter<F0, F1>::~conjunctive_filter() {
}

template <typename F0, typename F1>
inline bool turtle_input::conjunctive_filter<F0, F1>::operator()(const std::string& str) const {
    return f0_(str) && f1_(str);
}

}  // duraark_rdf
