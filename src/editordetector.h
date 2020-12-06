#ifndef EDITORDETECTOR_H
#define EDITORDETECTOR_H

#include "editor.h"

class EditorDetector
{
public:
    EditorDetector();

    static QList<Editor> detect();

};

#endif // EDITORDETECTOR_H
