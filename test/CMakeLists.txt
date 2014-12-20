ADD_EXECUTABLE(testPageDB testPageDB.cpp)
TARGET_LINK_LIBRARIES(testPageDB PageDB)

ADD_EXECUTABLE(testBTree testBTree.cpp)
TARGET_LINK_LIBRARIES(testBTree BTree PageDB)

ADD_EXECUTABLE(testContext testContext.cpp)
TARGET_LINK_LIBRARIES(testContext BTree PageDB TypeDB Context Exception)

ADD_EXECUTABLE(testContext testParser.cpp)
TARGET_LINK_LIBRARIES(testContext Parser)
