pybind11_add_module( Scripting-Python STATIC
    src/Python/scripting_python.cpp
    include/Embed-Pipeline/Python/scripting_python.h
    scripts/example.py
)

add_linktoexe( Scripting-Python )

library_post_installbuild( Embed-Systems ${PYTHON_LIBRARIES} ${PROJECT_SOURCE_DIR}/.Build/Lib )
library_post_installbuild( Embed-Systems ${pyroot}/python37.dll ${PROJECT_SOURCE_DIR}/.Build/Bin )
library_post_installbuild( Embed-Systems ${pyroot}/Lib ${PROJECT_SOURCE_DIR}/.Build/Bin/Python/Lib )