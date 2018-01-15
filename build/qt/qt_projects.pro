###lib 库
###example 例子
###src 工程

###注意 注意 注意###
###1、此工程环境在文件： environment.pri 中
###2、此工程依赖公共库：CCXX，因此要CheckOut到目录 $$PWD/../ygct_ics_cpp/ccxx中。如果目录有变，要
###   在 environment.pri中修改变量 CCXX_PATH = $$PWD/../../../../ygct_ics_cpp/ccxx
###3、先Build库文件(子工程项目lib)、再Build工具(子工程项目tool)、其他的就没有顺序了


TEMPLATE = subdirs

SUBDIRS = \
    $$PWD/dll \
    $$PWD/sample \
#    $$PWD/tools \
#    $$PWD/test \

