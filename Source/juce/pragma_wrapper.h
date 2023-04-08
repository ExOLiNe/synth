#include <juce_core/system/juce_CompilerWarnings.h>
#define PRAGMA_IGNORE_DIAGNOSTIC_BEGIN \
    JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE("-Wundef");
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Weverything\"")
#define PRAGMA_IGNORE_DIAGNOSTIC_END \
    _Pragma("clang diagnostic pop")    \
    JUCE_END_IGNORE_WARNINGS_GCC_LIKE; \
//PRAGMA_IGNORE_DIAGNOSTIC_BEGIN
//PRAGMA_IGNORE_DIAGNOSTIC_END
