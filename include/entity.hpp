#ifndef DURAARK_RDF_ENTITY_HPP_
#define DURAARK_RDF_ENTITY_HPP_

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "osdef.hpp"

namespace duraark_rdf {

class EXPORT_DECL entity {
public:
    typedef std::unique_ptr<entity> uptr_t;
    typedef std::shared_ptr<entity> sptr_t;
    typedef std::weak_ptr<entity> wptr_t;
    typedef std::shared_ptr<const entity> const_sptr_t;
    typedef std::weak_ptr<const entity> const_wptr_t;

    typedef enum class type_ : int { PC, IFC, IFC_MESH, IFC_OBJECT, SUBSET } type_t;
    typedef std::vector<int> indices_t;

public:
    entity(type_t type, const std::string& name, const std::string& guid);
    entity(const_sptr_t parent, const indices_t& indices);
    entity(const_sptr_t parent, uint32_t scan_index);
    entity(const_sptr_t parent, const std::string& guid);
    entity(const_sptr_t parent, const indices_t& indices,
           const std::string& representation_of);
    virtual ~entity();

    //static sptr_t
    //from_predicates(std::string subject,
                    //const std::map<std::string, std::string>& predicates);

    const type_t& type() const;

    const std::string& name() const;

    const std::string& guid() const;

    const_sptr_t parent() const;

    void set_parent(const_sptr_t parent);

    const std::string& parent_name() const;

    const indices_t& indices() const;

    const std::string& indices_string() const;

    const std::string& representation_of() const;

protected:
    entity();
    void process_indices_();
    //void parse_indices_();
    //void process_pc_predicates_(
        //const std::map<std::string, std::string>& predicates);
    //void process_subset_predicates_(
        //const std::map<std::string, std::string>& predicates);

protected:
    type_t type_;
    std::string name_;
    std::string guid_;
    const_sptr_t parent_;
    indices_t indices_;
    std::string indices_str_;
    std::string rep_of_;
};

}  // duraark_rdf

#endif /* DURAARK_RDF_ENTITY_HPP_ */
