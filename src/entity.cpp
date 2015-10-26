#include <entity.hpp>

#include <iostream>
#include <stdexcept>

#include <regex>

namespace duraark_rdf {

template <typename T>
T
parse_builtin_(const std::string& str) {
    std::stringstream sstr;
    T result;
    sstr << str;
    sstr >> result;
    return result;
}

std::string type_prefix_(entity::type_t type) {
    switch (type) {
        case entity::type_t::IFC:
            return "IFC_";
        case entity::type_t::PC:
            return "PC_";
        case entity::type_t::IFC_MESH:
            return "IFC_MESH_";
        case entity::type_t::IFC_OBJECT:
            return "IFC_OBJECT_";
        case entity::type_t::SUBSET:
            return "SBS_";
    };
    return "";
}

entity::entity(type_t type, const std::string& name, const std::string& guid)
    : type_(type), guid_(guid), rep_of_("") {
    std::string prefix = type_prefix_(type);
    name_ = prefix + name;
    //guid_ = prefix + "GUID_" + guid;
}

entity::entity(const_sptr_t parent, const indices_t& indices)
    : parent_(parent), indices_(indices), rep_of_("") {
    if (parent->type() != type_t::PC)
        throw std::runtime_error(
            "Subset entities must have point clouds as parent type.");

    process_indices_();

    type_ = type_t::SUBSET;
    std::string hashed = std::to_string(
        std::hash<std::string>()(parent_->name() + indices_str_));
    std::string prefix = type_prefix_(type_);
    name_ = prefix + parent_->name() + "_" + hashed;
    guid_ = prefix + parent_->guid() + "_" + hashed;
}

entity::entity(const_sptr_t parent, const indices_t& indices,
               const std::string& representation_of)
    : parent_(parent), indices_(indices), rep_of_(representation_of) {
    if (parent->type() != type_t::PC)
        throw std::runtime_error(
            "Subset entities must have point clouds as parent type.");

    process_indices_();

    type_ = type_t::SUBSET;
    std::string hashed = std::to_string(
        std::hash<std::string>()(parent_->name() + indices_str_));
    std::string prefix = type_prefix_(type_);
    name_ = prefix + parent_->name() + "_" + hashed;
    guid_ = prefix + parent_->guid() + "_" + hashed;
}

entity::~entity() {}

//entity::ptr
//entity::from_predicates(std::string subject,
                        //const std::map<std::string, std::string>& predicates) {
    //ptr result(new entity());
    //result->m_name = subject;
    //// determine type
    //std::regex re("^obj:([a-zA-Z]*?)_.*");
    //std::smatch m;
    //if (std::regex_match(subject, m, re)) {
        //if (m[1] == "PC") {
            //result->m_type = PC;
            //result->process_pc_predicates_(predicates);
        //} else if (m[1] == "MESH") {
            //result->m_type = LEGACY;
        //} else if (m[1] == "SBS") {
            //result->m_type = SUBSET;
            //result->process_subset_predicates_(predicates);
        //} else {
            //return result;
        //}
    //} else {
        //return result;
    //}

    //return result;
//}

const entity::type_t&
entity::type() const {
    return type_;
}

const std::string&
entity::name() const {
    return name_;
}

const std::string&
entity::guid() const {
    return guid_;
}

entity::const_sptr_t
entity::parent() const {
    return parent_;
}

void
entity::set_parent(const_sptr_t parent) {
    parent_ = parent;
}

const std::string&
entity::parent_name() const {
    return parent_->name();
}

const entity::indices_t&
entity::indices() const {
    return indices_;
}

const std::string&
entity::indices_string() const {
    return indices_str_;
}

const std::string&
entity::representation_of() const {
    return rep_of_;
}

entity::entity() {}

void
entity::process_indices_() {
    // sanitize indices to string
    std::vector<std::vector<int>> cons;
    for (auto idx : indices_) {
        if (!cons.size() || cons.back().back() != (idx - 1)) {
            cons.emplace_back(1, idx);
        } else {
            cons.back().push_back(idx);
        }
    }
    bool not_first = false;
    indices_str_.clear();
    for (const auto& c : cons) {
        if (!c.size()) continue;
        if (not_first++) indices_str_ += " ";
        indices_str_ += std::to_string(c[0]);
        if (c.size() > 2) indices_str_ += "-" + std::to_string(c.back());
    }
}

//void
//entity::parse_indices_() {
    //std::istringstream iss(m_indices_str);
    //std::vector<std::string> tokens(std::istream_iterator<std::string>{iss},
                                    //std::istream_iterator<std::string>{});

    //m_indices.clear();
    //std::regex re("(\\d+)-(\\d+)");
    //std::smatch m;
    //for (const auto& token : tokens) {
        //if (std::regex_match(token, m, re)) {
            //int begin = parse_builtin_<int>(m[1]);
            //int range = parse_builtin_<int>(m[2]) - begin + 1;
            //std::vector<int> subset(range);
            //std::iota(subset.begin(), subset.end(), begin);
            //m_indices.insert(m_indices.end(), subset.begin(), subset.end());
        //} else {
            //m_indices.push_back(parse_builtin_<int>(token));
        //}
    //}
//}

//void
//entity::process_pc_predicates_(
    //const std::map<std::string, std::string>& predicates) {
    //for (const auto& pred : predicates) {
        //if (pred.first == "rel:globalPointCloudId") {
            //m_guid = pred.second;
        //}
    //}
//}

//void
//entity::process_subset_predicates_(
    //const std::map<std::string, std::string>& predicates) {
    //for (const auto& pred : predicates) {
        //if (pred.first == "rel:globalPointSubsetId") {
            //m_guid = pred.second;
        //} else if (pred.first == "rel:pointSubsetContains") {
            //m_indices_str = pred.second;
            //parse_indices_();
        //} else if (pred.first == "rel:pointSubsetOf") {
            //m_parent_name = pred.second;
        //} else if (pred.first == "rel:subsetRepOf") {
            //m_rep_of = pred.second;
        //}
    //}
//}

}  // duraark_rdf
