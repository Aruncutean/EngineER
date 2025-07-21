//
// Created by arunc on 04/07/2025.
//

#ifndef EDITORVIEW_HPP
#define EDITORVIEW_HPP
#include "component/View.hpp"


class EditorView : public View {
public:
    EditorView(): View("EditorView") {
        noCollapse = true;


    }
};


#endif //EDITORVIEW_HPP
