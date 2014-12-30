ADD_EXECUTABLE(mymymysql main.cpp)
TARGET_LINK_LIBRARIES(mymymysql Parser TypeDB Stmt BTree TypeDB Context Exception PageDB)
