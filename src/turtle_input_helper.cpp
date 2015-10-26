#include <turtle_input_helper.hpp>
#include <iostream>

#include <boost/spirit/include/qi.hpp>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

namespace duraark_rdf {

std::set<uint32_t> parse_index_list_(const std::string& str) {
    using boost::spirit::qi::uint_;
    using boost::spirit::qi::char_;
    using boost::spirit::qi::phrase_parse;
    using boost::spirit::ascii::space;

    auto first = str.begin();
    auto last = str.end();
    uint32_t index_begin = 0, index_end = 0;
    std::set<uint32_t> indices;
    auto match_first = [&] (uint32_t idx) { index_begin = index_end = idx; };
    auto match_second = [&] (uint32_t idx) { index_end = idx; };
    auto match_after = [&] () { for (uint32_t i=index_begin; i<=index_end; ++i) { indices.insert(i); }; };
    bool r = phrase_parse(first, last, *((uint_[match_first] >> -(char_('-') >> uint_[match_second]))[match_after]), space);
    if (!r || first != last) {
        return std::set<uint32_t>();
    }
    return indices;
}

std::pair<std::string, bool>
parse_transformation_string(turtle_input& input, const std::string& guid_0,
                     const std::string& guid_1) {
    auto entity_triples = input.filter_triples(
        turtle_input::filter(),
        turtle_input::filter::equals("rel:globalUniqueId"));
    std::string name_0 = "", name_1 = "";
    for (auto& entity : entity_triples) {
        std::string crt_guid = entity.second["rel:globalUniqueId"];
        if (crt_guid == guid_0) {
            name_0 = fs::path(entity.first).filename().stem().string();
        }
        if (crt_guid == guid_1) {
            name_1 = fs::path(entity.first).filename().stem().string();
        }
    }

    if (name_0 == "") {
        throw std::runtime_error("Unable to find object for GUID \"" + guid_0 +
                                 "\"");
    }
    if (name_1 == "") {
        throw std::runtime_error("Unable to find object for GUID \"" + guid_1 +
                                 "\"");
    }

    bool inverse = false;
    std::string tr_name = "obj:" + name_0 + "_to_" + name_1;
    auto tr_obj = input.filter_triples(
        turtle_input::filter::equals(tr_name),
        turtle_input::filter::equals("rel:transformParam"));
    if (!tr_obj.size()) {
        tr_name = name_1 + "_to_" + name_0;
        tr_obj = input.filter_triples(turtle_input::filter::equals(tr_name),
                                      turtle_input::filter());
        if (!tr_obj.size()) {
            throw std::runtime_error(
                "No transformation found for given objects");
        }
        inverse = true;
    }

    auto& pred = tr_obj[tr_name];

    auto pred_found = pred.find("rel:transformParam");

    if (pred_found == pred.end()) {
        throw std::runtime_error(
            "Transform object has no \"rel:transformParam\" attribute");
    }

    return {pred_found->second, inverse};
}

Eigen::Matrix4f
parse_registration(turtle_input& input, const std::string& guid_0,
                     const std::string& guid_1) {
    std::pair<std::string, bool> t = parse_transformation_string(input, guid_0, guid_1);
    std::istringstream iss(t.first);
    std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
    assert(tokens.size() == 16);

    Eigen::Matrix4f m;
    for (uint32_t i = 0; i < 4; ++i) {
        for (uint32_t j = 0; j < 4; ++j) {
            double value;
            std::stringstream convert_str(tokens[i*4+j]);
            convert_str >> value;
            convert_str.str("");
            m(i, j) = value;
        }
    }

    if (t.second) {
        Eigen::Matrix4f inv = m.inverse();
        m = inv;
    }

    return m;
}

std::vector<subset_association_t> parse_subset_associations(turtle_input& input, std::map<std::string, uint32_t>& possible_hits, std::set<std::string>* const pointcloud_entities) {
    // parse point-cloud entities
    std::map<std::string, std::string> pc_guids;
    if (pointcloud_entities) pointcloud_entities->clear();
    auto entity_triples = input.filter_triples(
        turtle_input::filter::begins_with("PC_"),
        turtle_input::filter::equals("rel:globalUniqueId"));
    for (const auto& entity : entity_triples) {
        std::string pc_guid = entity.second.at("rel:globalUniqueId");
        pc_guids[entity.first] = pc_guid;
        if (pointcloud_entities) pointcloud_entities->insert(pc_guid);
    }

    std::vector<subset_association_t> assocs;
    entity_triples = input.filter_triples(turtle_input::filter::begins_with("SBS_"), turtle_input::filter());
    for (const auto& entity : entity_triples) {
        auto& m = entity.second;
        auto sub_it = m.find("rel:pointSubsetOf");
        auto rep_it = m.find("rel:subsetRepOf");
        auto idx_it = m.find("rel:pointSubsetContains");
        auto poss_it = m.find("rel:possibleScanHits");
        if (sub_it != m.end() && rep_it != m.end() && idx_it != m.end()) {
            subset_association_t assoc;
            assoc.ifc_object_guid = rep_it->second;
            assoc.pointcloud_guid = pc_guids[sub_it->second];
            assoc.point_indices = parse_index_list_(idx_it->second);
            std::stringstream sstr_conv;
            sstr_conv << poss_it->second;
            uint32_t poss_hits = 0;
            sstr_conv >> poss_hits;
            possible_hits[rep_it->second] = poss_hits;
            assocs.push_back(assoc);
        }
    }

    return assocs;
}

}  // duraark_rdf
