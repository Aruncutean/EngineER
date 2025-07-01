//
// Created by arunc on 01/07/2025.
//

#ifndef COMPONENTBASE_HPP
#define COMPONENTBASE_HPP

namespace Component {

    class ComponentBase
    {
    public:
        virtual std::string GetTypeName() const = 0;
        virtual boost::property_tree::ptree ToPtree() const = 0;
        virtual void FromPtree(const boost::property_tree::ptree& pt) = 0;
    };


}
#endif //COMPONENTBASE_HPP
