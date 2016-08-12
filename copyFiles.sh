FOLDER=F:\\playground\\assassin-playground\\ctsvetkov\\minh_nguyen\\Allocators\\Allocators

HEADER_FOLDER=.\\include
SRC_FOLDER=.\\src

cp $FOLDER\\*.h $HEADER_FOLDER
cp $FOLDER\\*.cpp $SRC_FOLDER

'
cp \
$FOLDER\\ImplicitFreeListAllocator.h \
$FOLDER\\ImplicitFreeListAllocator.cpp \
$FOLDER\\ExplicitFreeListAllocator.h \
$FOLDER\\ExplicitFreeListAllocator.cpp \
$FOLDER\\FrameAllocator.h \
$FOLDER\\FrameAllocator.cpp \
$FOLDER\\IntrusiveLinkedList.h \
$FOLDER\\IntrusiveLinkedList.cpp \
$FOLDER\\SegregatedListAllocator.h \
$FOLDER\\SegregatedListAllocator.cpp .
'
